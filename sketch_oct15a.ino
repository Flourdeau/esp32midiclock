#include <MIDI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>

// Define the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, SCREEN_ADDRESS);

// Define the MIDI interface using Serial1 (UART1)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
const int midiOutputChannel = 1;

// Define the encoder pins
const int ENC_A_PIN = 2;
const int ENC_B_PIN = 3;
Encoder encoder(ENC_A_PIN, ENC_B_PIN);

// Variables for BPM and timer
int bpm = 120;
unsigned long previousMillis = 0;
const long interval = 60000 / 120;

void setup() {
  if (!display.begin(SCREEN_ADDRESS, OLED_RESET)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  MIDI.begin(midiOutputChannel);
  // Configure Serial1 for MIDI
  Serial1.begin(31250);  // MIDI operates at a baud rate of 31250
}

void loop() {
  MIDI.read();
  int16_t encoderValue = encoder.read();

  if (encoderValue != 0) {
    bpm += encoderValue;
    if (bpm < 30) {
      bpm = 30;
    }
    if (bpm > 240) {
      bpm = 240;
    }
    updateDisplay();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Send MIDI Clock message (0xF8) via UART
    Serial1.write(0xF8);
    previousMillis = currentMillis;
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("BPM: ");
  display.print(bpm);
  display.display();
}
