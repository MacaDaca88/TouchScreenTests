#include "Adafruit_GFX.h"
#include <FastLED_NeoPixel.h>  // libary needed to run x number of leds
#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>
#include <TouchScreen.h>

//#include "snake.h"

#define led A11
#define DATA_PIN A15  // data line for led
#define NUM_LEDS 8    // leds used on this line


#define BLACK 0x0000
#define BLUE 0x001F
#define GREEN 0x07E0
#define RED 0xF800
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define YELLOW 0xFFE0

Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB);

MCUFRIEND_kbv tft;  //(A3, A2, A1, A0, A4)

// Calibration values obtained from TouchScreen_Calibr_native(file->examples->MCUFRIEND_kbv->TouchScreen_Calibr_native.ino)
const int TS_LEFT = 889, TS_RT = 149, TS_TOP = 97, TS_BOT = 893;
const int XP = 8, XM = A2, YP = A3, YM = 9;  //240x320 ID=0x9595


// PORTRAIT  CALIBRATION     240 x 320
// x = map(p.x, LEFT = 889, RT = 149, 0, 240)
// y = map(p.y, TOP = 97, BOT = 893, 0, 320)

// LANDSCAPE CALIBRATION     320 x 240
// x = map(p.y, LEFT = 97, RT = 893, 0, 320)
// y = map(p.x, TOP = 149, BOT = 889, 0, 240)

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

TSPoint tp;

// Menu states
enum MenuState {
  START,
  MAIN_MENU,
  GAMES,
  OPTION_MENU,
  SUBMENU_1,
  SUBMENU_2,
  RGB_STRIP
};

MenuState currentMenu = START;

bool checkButtonPressed(uint16_t touch_x, uint16_t touch_y, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  if (touch_x >= x && touch_x <= x + width && touch_y >= y && touch_y <= y + height) {
    return true;
  }
  return false;
}
int red = 255;
int green = 255;
int blue = 255;
int red_position = 50;
int green_position = 50;
int blue_position = 50;
int red_old_pos = 0;
int green_old_pos = 0;
int blue_old_pos = 0;
int fill_color = tft.color565(red, green, blue);
int brightness =70 ;
int brightness_old=0;
int brightness_max = 255;
bool ledstate = true;
int a;  //LED strip


void setup() {
  tft.reset();
  Serial.begin(9600);
  strip.begin();             // initialize strip (required!)
  strip.setBrightness(255);  // set default ledstrip brightness
  pinMode(led, OUTPUT);
  analogWrite(led, 0);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.print("Reality 2.0\n");
  delay(100);

  tft.setCursor(10, 296);
  tft.setTextSize(1);
  tft.print("Created By 'MacaDaca88'      2023\n");
  delay(100);

  Start();
}


void loop() {



  tft.setRotation(0);

  uint16_t touch_x, touch_y;
  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  digitalWrite(XM, HIGH);
  digitalWrite(YP, HIGH);

  // Check if the touchscreen is pressed
  if (tp.z > 0) {
    touch_x = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    touch_y = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

    Serial.print("Touch X: ");
    Serial.print(touch_x);
    Serial.print("\tTouch Y: ");
    Serial.println(touch_y);

    switch (currentMenu) {

      case START:
        if (checkButtonPressed(touch_x, touch_y, 120, 160, 20, 20)) {
          drawMainMenu();
          currentMenu = MAIN_MENU;
        }
        break;


      case MAIN_MENU:
        if (checkButtonPressed(touch_x, touch_y, 140, 10, 200, 20)) {
          // Option Menu button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 60, 200, 40)) {
          // Game 1 button pressed,
          case GAMES:
            game1();
        } else if (checkButtonPressed(touch_x, touch_y, 20, 110, 200, 40)) {
          // Game 2 button pressed,
          game2();
        } else if (checkButtonPressed(touch_x, touch_y, 20, 160, 200, 40)) {
          // Game 3 button pressed,
          game3();
        } else if (checkButtonPressed(touch_x, touch_y, 20, 210, 200, 40)) {
          // Game 4 button pressed,
          game4();
        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        }
        break;


      case OPTION_MENU:
        if (checkButtonPressed(touch_x, touch_y, 20, 20, 100, 40)) {
          // Main Menu button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 60, 200, 40)) {
          // Option 1 button pressed,
          tft.fillScreen(BLUE);
          delay(100);
          drawSubMenu("Submenu 1");
          currentMenu = SUBMENU_1;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 110, 200, 40)) {
          // Option 2 button pressed,
          tft.fillScreen(YELLOW);
          delay(500);
          drawSubMenu("Submenu 2");
          currentMenu = SUBMENU_2;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 160, 200, 40)) {
          // Option 3 button pressed,
        } else if (checkButtonPressed(touch_x, touch_y, 20, 210, 200, 40)) {
          // Option 4 button pressed,
        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        }
        break;


      case SUBMENU_1:
        // Submenu 1 button pressed
        if (checkButtonPressed(touch_x, touch_y, 10, 10, 100, 20)) {
          // Main Menu button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 140, 10, 200, 20)) {
          // Option Menu button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 10, 60, 200, 40)) {
          // led on button pressed,
          ledon();
        } else if (checkButtonPressed(touch_x, touch_y, 10, 110, 200, 40)) {
          // led off button pressed,
          ledoff();
        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        }
        break;


      case SUBMENU_2:
        // Submenu 2 button pressed
        if (checkButtonPressed(touch_x, touch_y, 10, 10, 100, 20)) {
          // Main Menu button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 140, 10, 200, 20)) {
          // Option Menu button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 10, 60, 200, 40)) {
          // ledstrip on button pressed,
          drawRGB();
          currentMenu = RGB_STRIP;
        } else if (checkButtonPressed(touch_x, touch_y, 10, 110, 200, 40)) {
          // ledstrip off button pressed,
          for (int a = 0; a < 8; a++) {                    // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(0, 0, 0));  // set STRIP to green
          }
          strip.show();  // needed to show changes
          currentMenu = OPTION_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        }
        break;


      case RGB_STRIP:
        if (checkButtonPressed(touch_x, touch_y, 30, 50, 30, 180)) {
          for (int a = 0; a < 8; a++) {                      // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
          }
          strip.show();  // needed to show changes
        } else if (checkButtonPressed(touch_x, touch_y, 100, 50, 30, 180)) {
          for (int a = 0; a < 8; a++) {                      // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(0, 255, 0));  // set STRIP to green
          }
          strip.show();  // needed to show changes
        } else if (checkButtonPressed(touch_x, touch_y, 170, 50, 30, 180)) {
          for (int a = 0; a < 8; a++) {                             // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(red, green, blue));  // set STRIP to blue
          }
          strip.show();  // needed to show changes
        } else if (checkButtonPressed(touch_x, touch_y, 70, 10, 225, 30)) {
          // brightness button pressed,

          if (touch_x > 70 && touch_x < 225) {
            if (touch_y > 10 && touch_y < 30) {
              brightness = map(touch_x, 70, 225, 0, 255);

              brightness_old = brightness;
              brightness = touch_x;
            }
            strip.setBrightness(brightness);  // set default ledstrip brightness
            strip.show();
            //////////////// Debuging Switch on or off ////////////
            Serial.print("brightness =  ");
            Serial.println(brightness);
            Serial.print("brightness_old =  ");
            Serial.println(brightness_old);
            delay(2000);
            //////////////////////////////////////////////////////
          }
          
  tft.drawFastHLine(70, 20, 150, BLACK);
  tft.fillRoundRect(brightness_old, 15, 30, 10, 10, BLACK);
  tft.drawFastHLine(70, 20, 150, WHITE);
  tft.drawPixel(brightness, 15, WHITE);

        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          currentMenu = SUBMENU_2;
        }
        break;
    }
  }

  if (ledstate) {
    fill_color = tft.color565(red, green, blue);
  } else {
    fill_color = BLACK;
  }
}


void Start() {
  tft.setRotation(0);
  tft.setTextColor(RED);
  tft.setCursor(96, 154);
  tft.setTextSize(2);
  tft.fillCircle(120, 160, 40, GREEN);
  tft.print("Start\n");
}


void drawMainMenu() {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setTextColor(YELLOW);
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.print("Main Menu");

  tft.setTextColor(RED);
  tft.setCursor(140, 10);
  tft.setTextSize(1);
  tft.print("Options");

  drawButton("Game 1", 20, 60, 200, 40);
  drawButton("Game 2", 20, 110, 200, 40);
  drawButton("Game 3", 20, 160, 200, 40);
  drawButton("Game 4", 20, 210, 200, 40);
}


void drawOptionMenu() {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(10, 10);
  tft.setTextSize(1);
  tft.print("Main Menu");

  tft.setTextColor(YELLOW);
  tft.setCursor(140, 10);
  tft.setTextSize(2);
  tft.print("Options");

  drawBlueButton("Led", 20, 60, 200, 40, 0);
  drawRGBButton("RGB Srip", 20, 110, 200, 40, 0);
  drawButton("Option 3", 20, 160, 200, 40);
  drawButton("Option 4", 20, 210, 200, 40);
  drawBackButton("Back", 60, 300, 60, 20, 10);
}


void drawSubMenu(const char* label) {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(10, 10);
  tft.setTextSize(1);
  tft.print("Main Menu");

  tft.setTextColor(RED);
  tft.setCursor(80, 10);
  tft.setTextSize(1);
  tft.print("Options");

  tft.setTextColor(YELLOW);
  tft.setCursor(10, 30);
  tft.setTextSize(2);
  tft.print(label);

  drawBlueButton("Led On", 20, 60, 200, 40, 20);
  drawBlueButton("Led Off", 20, 110, 200, 40, 20);
  drawBackButton("Back", 60, 300, 60, 20, 10);
}


void drawRGB() {
  tft.fillScreen(BLACK);
  tft.fillRect(10, 10, 50, 20, fill_color);
  tft.drawRect(10, 10, 50, 20, WHITE);
  tft.fillRoundRect(30, 50, 30, 180, 10, RED);
  tft.fillRoundRect(100, 50, 30, 180, 10, GREEN);
  tft.fillRoundRect(170, 50, 30, 180, 10, BLUE);

  tft.drawFastVLine(45, 60, 150, WHITE);
  tft.drawFastVLine(115, 60, 150, WHITE);
  tft.drawFastVLine(185, 60, 150, WHITE);

  tft.drawFastHLine(70, 20, 150, WHITE);
  tft.fillRoundRect(brightness, 15, 30, 10, 10, WHITE);


  drawBackButton("Back", 60, 300, 60, 20, 10);
}


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


void ledon() {
  analogWrite(led, 255);
}


void ledoff() {
  analogWrite(led, 0);
}


void game1() {

  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(20, 20);
  tft.setTextSize(3);
  tft.print("Game 1");

  drawBackButton("Back", 60, 300, 60, 20, 10);
}


void game2() {
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(20, 20);
  tft.setTextSize(3);
  tft.print("Game 2");

  drawBackButton("Back", 60, 300, 60, 20, 10);
}


void game3() {
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(20, 20);
  tft.setTextSize(3);
  tft.print("Game 3");

  drawBackButton("Back", 60, 300, 60, 20, 10);
}


void game4() {
  tft.fillScreen(BLACK);
  tft.setTextColor(RED);
  tft.setCursor(20, 20);
  tft.setTextSize(3);
  tft.print("Game 4");

  drawBackButton("Back", 60, 300, 60, 20, 10);
}
