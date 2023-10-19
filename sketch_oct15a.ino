#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Encoder.h>
#include <Ticker.h>  // Bibliothèque Ticker pour gérer les interruptions de temps

#define CLK 13 // Broche CLK de l'encodeur
#define DT 15  // Broche DT de l'encodeur
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C  // Adresse I2C de l'écran OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ESP32Encoder encoder;

int bpm = 120;  // BPM par défaut
int encoderPos = 0;
int lastEncoderPos = 0;

Ticker midiClock;  // Ticker pour envoyer les signaux MIDI Clock

void sendMidiClock() {
  Serial.write(0xF8);  // MIDI Clock
}

void setup() {
  encoder.attachHalfQuad(DT, CLK);
  encoder.setCount(200);
  Serial.begin(31250);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // Configuration du Ticker pour envoyer des signaux MIDI Clock à intervalles réguliers
  int interval = 60000 / (bpm * 24);  // Interval en millisecondes
  midiClock.attach_ms(interval, sendMidiClock);
}

void loop() {
  // Lire la position de l'encodeur
  long newPosition = encoder.getCount() / 2;
  bpm = map(newPosition, 0, 100, 60, 240) / 2;  // Mappage du BPM en fonction de la position de l'encodeur

  // Mise à jour de l'affichage sur l'écran OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.printf("BPM: %d\n", bpm);
  display.display();

}
