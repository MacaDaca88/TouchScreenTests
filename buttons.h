#include "Adafruit_GFX.h"
#include <FastLED_NeoPixel.h>  // libary needed to run x number of leds
#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>
#include <TouchScreen.h>

#include "buttons.h"
//#include "snake.h"

void drawRGBButton(const char* label, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t radius) {
  tft.fillRoundRect(x, y, width / 3 * 1, height, radius, RED);
  tft.fillRoundRect(x, y, width / 3 * 2, height, radius, GREEN);
  tft.fillRoundRect(x, y, width / 3, height, radius, BLUE);

  tft.setTextColor(WHITE);
  tft.setTextSize(1);

  uint16_t labelWidth = strlen(label) * 6;  // 6 pixels per character
  uint16_t labelX = x + (width - labelWidth) / 2;
  uint16_t labelY = y + (height - 12) / 3;  // Assuming font height is 12 pixels

  tft.setCursor(labelX, labelY);
  tft.print(label);
}


void drawBlueButton(const char* label, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t radius) {
  tft.fillRoundRect(x, y, width, height, radius, BLUE);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);

  uint16_t labelWidth = strlen(label) * 6;  // 6 pixels per character
  uint16_t labelX = x + (width - labelWidth) / 2;
  uint16_t labelY = y + (height - 12) / 3;  // Assuming font height is 12 pixels

  tft.setCursor(labelX, labelY);
  tft.print(label);
}

void drawOffButton(const char* label, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t radius) {
  tft.fillRoundRect(x, y, width, height, radius, RED);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);

  uint16_t labelWidth = strlen(label) * 6;  // 6 pixels per character
  uint16_t labelX = x + (width - labelWidth) / 2;
  uint16_t labelY = y + (height - 12) / 3;  // Assuming font height is 12 pixels

  tft.setCursor(labelX, labelY);
  tft.print(label);
}

void drawBackButton(const char* label, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t radius) {
  tft.fillRoundRect(x, y, width, height, radius, RED);
  tft.setTextColor(GREEN);
  tft.setTextSize(1);

  uint16_t labelWidth = strlen(label) * 6;  // 6 pixels per character
  uint16_t labelX = x + (width - labelWidth) / 2;
  uint16_t labelY = y + (height - 12) / 3;  // Assuming font height is 12 pixels

  tft.setCursor(labelX, labelY);
  tft.print(label);
}


void drawButton(const char* label, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  tft.fillRect(x, y, width, height, GREEN);
  tft.setTextColor(RED);
  tft.setTextSize(1);

  uint16_t labelWidth = strlen(label) * 6;  // 6 pixels per character
  uint16_t labelX = x + (width - labelWidth) / 2;
  uint16_t labelY = y + (height - 12) / 2;  // Assuming font height is 12 pixels

  tft.setCursor(labelX, labelY);
  tft.print(label);
}
