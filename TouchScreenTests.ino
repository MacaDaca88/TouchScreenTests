#include "Adafruit_GFX.h"
#include <FastLED_NeoPixel.h>  // libary needed to run x number of leds
#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>
#include <TouchScreen.h>

//#include "snake.h"

// led pins (RGB,WS218b,Standard etc....)
#define led A11
#define DATA_PIN A15  // data line for led
#define NUM_LEDS 8    // leds used on this line
#define red_pin 46
#define green_pin 44
#define blue_pin 45

// Sensors pins
#define Sound A12
#define MIQ A13

int DB = 0;
int drunk = 0;

// color choices add more when needed
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

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 100);

TSPoint tp;

// Menu states
enum MenuState {
  START,
  MAIN_MENU,
  GAMES,
  OPTION_MENU,
  SOUNDS,
  SUBMENU_1,
  SUBMENU_2,
  RGB_STRIP,
  DRAW,
  PEN_RECT,
  PEN_CIRCLE,
  PEN_RED,
  PEN_RECT_RED,
  PEN_BLUE,
  PEN_GREEN,
  PEN_YELLOW,
  PEN_ORANGE,
  PEN_RECT_BLUE,
  PEN_CIRCLE_BLUE,
  PEN_HLINE_BLUE,
  PEN_RECT_GREEN,
  PEN_CIRCLE_GREEN,
  PEN_RECT_YELLOW,
  PEN_CIRCLE_YELLOW,
  PEN_RECT_ORANGE,
  PEN_CIRCLE_ORANGE
};

MenuState currentMenu = START;
#define MINPRESS 10
#define MAXPRESS 1000

bool Sensor1 = false;
bool checkButtonPressed(uint16_t touch_x, uint16_t touch_y, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  if (touch_x >= x && touch_x <= x + width && touch_y >= y && touch_y <= y + height) {
    return true;
  }
  return false;
}
int red = 255;
int green = 255;
int blue = 255;
int red_position = 220;
int green_position = 220;
int blue_position = 220;
int red_old_pos = 0;
int green_old_pos = 0;
int blue_old_pos = 0;
int fill_color = tft.color565(red, green, blue);
int brightness = 120;
int brightness_old = 0;
int RGBbrightness = 255;
bool ledstate = true;
int a;  //LED strip

int pen = tft.color565(red, green, blue);
int pen_new = 0;

bool sensor1 = false;
/////////////////////////////////////////////////////////////////////////Setup///////////////////////////////////////////
void setup() {
  tft.reset();
  Serial.begin(9600);
  strip.begin();                    // initialize strip (required!)
  strip.setBrightness(brightness);  // set default ledstrip brightness
  // Pin Input/Ouput Declerations
  pinMode(led, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  // sets all lights on  during startup
  digitalWrite(led, 1);
  delay(100);
  analogWrite(red_pin, 0);
  delay(100);
  analogWrite(green_pin, 0);
  delay(100);
  analogWrite(blue_pin, 0);
  delay(200);
  digitalWrite(led, 0);
  delay(100);
  analogWrite(red_pin, 255);
  delay(100);
  analogWrite(green_pin, 255);
  delay(100);
  analogWrite(blue_pin, 255);
  delay(100);

  // begin touch screen intiate
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  // start splash screen
  Start();
}
/////////////////////////////////////////////////////////////////////loooooooooooop///////////////////////////////////////////
void loop() {
  int DB = analogRead(Sound);
  int drunk = analogRead(MIQ);

  uint16_t touch_x, touch_y;
  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  digitalWrite(XM, HIGH);
  digitalWrite(YP, HIGH);
  tft.setRotation(0);

  // Check if the touchscreen is pressed
  if (tp.z >= MINPRESS && tp.z <= MAXPRESS) {
    touch_x = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    touch_y = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
    //////////////// Debuging Switch on or off ////////////
    Serial.print("Touch X: ");
    Serial.print(touch_x);
    Serial.print("\tTouch Y: ");
    Serial.println(touch_y);
    Serial.print("CurrentMenu  = ");
    Serial.println(currentMenu);
    //////////////////////////////////////////////////////
    if (sensor1 == true) {
      Serial.print("test");
    }
    switch (currentMenu) {

      case START:
        if (checkButtonPressed(touch_x, touch_y, 80, 120, 80, 80)) {
          drawMainMenu();
          currentMenu = MAIN_MENU;
        }
        break;

      case GAMES:

        if (checkButtonPressed(touch_x, touch_y, 80, 120, 80, 80)) {
          // Start Button
        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          Start();
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
          game1();
          currentMenu = GAMES;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 110, 200, 40)) {
          // Game 2 button pressed,
          game2();
          currentMenu = GAMES;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 160, 200, 40)) {
          // Game 3 button pressed,
          game3();
          currentMenu = GAMES;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 210, 200, 40)) {
          // Game 4 button pressed,
          game4();
          currentMenu = GAMES;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          Start();
          currentMenu = START;
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
          delay(100);
          drawSubMenu("Submenu 2");
          currentMenu = SUBMENU_2;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 160, 200, 40)) {
          // Option 3 button pressed,
          Sounds();
          currentMenu = SOUNDS;
        } else if (checkButtonPressed(touch_x, touch_y, 30, 210, 200, 40)) {
          // Option 4 button pressed,else if (checkButtonPressed(touch_x, touch_y, 60, 300, 60, 20)) {
          // Back button pressed,
          Drawings();
          currentMenu = DRAW;

        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
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
        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
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
          analogWrite(red_pin, 255);
          analogWrite(green_pin, 255);
          analogWrite(blue_pin, 255);
          currentMenu = OPTION_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
          // Back button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        }
        break;


      case RGB_STRIP:

        if (checkButtonPressed(touch_x, touch_y, 30, 50, 30, 170)) {
          // red slider
          for (int a = 0; a < 8; a++) {                      // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(255, 0, 0));  // set STRIP to red
          }
          strip.show();  // needed to show changes

          red = map(touch_y, 50, 220, 255, 0);
          red_old_pos = red_position;
          red_position = touch_y;

          tft.fillRoundRect(30, 50, 30, 180, 10, RED);
          tft.fillRoundRect(30, red_position, 30, 10, 10, YELLOW);

          analogWrite(red_pin, red_position);
          tft.setTextColor(RED);
          tft.setCursor(30, 250);
          tft.setTextSize(2);
          tft.fillRect(30, 250, 40, 20, WHITE);
          tft.print(red);

          fill_color = tft.color565(red_position, green_position, blue_position);
          tft.fillRect(10, 10, 50, 20, fill_color);
          //////////////// Debuging Switch on or off ////////////
          Serial.print("red = ");
          Serial.println(red);
          //////////////////////////////////////////////////////
        } else if (checkButtonPressed(touch_x, touch_y, 100, 50, 30, 170)) {
          // green slider
          for (int a = 0; a < 8; a++) {                      // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(0, 255, 0));  // set STRIP to green
          }
          strip.show();  // needed to show changes

          green = map(touch_y, 50, 220, 255, 0);
          green_old_pos = green_position;
          green_position = touch_y;

          tft.fillRoundRect(100, 50, 30, 180, 10, GREEN);
          tft.fillRoundRect(100, green_position, 30, 10, 10, YELLOW);

          analogWrite(green_pin, green_position);
          tft.setTextColor(GREEN);
          tft.setCursor(100, 250);
          tft.setTextSize(2);
          tft.fillRect(100, 250, 40, 20, WHITE);
          tft.print(green);

          fill_color = tft.color565(red_position, green_position, blue_position);
          tft.fillRect(10, 10, 50, 20, fill_color);
          //////////////// Debuging Switch on or off ////////////
          Serial.print("green = ");
          Serial.println(green);
          //////////////////////////////////////////////////////
        } else if (checkButtonPressed(touch_x, touch_y, 170, 50, 30, 170)) {
          // blue slider
          for (int a = 0; a < 8; a++) {                       // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(0, 0, blue));  // set STRIP to blue
          }
          strip.show();  // needed to show changes
          blue = map(touch_y, 50, 220, 255, 0);
          blue_old_pos = blue_position;
          blue_position = touch_y;

          tft.fillRoundRect(170, 50, 30, 180, 10, BLUE);
          tft.fillRoundRect(170, blue_position, 30, 10, 10, YELLOW);

          analogWrite(blue_pin, blue_position);
          tft.setTextColor(BLUE);
          tft.setCursor(170, 250);
          tft.setTextSize(2);
          tft.fillRect(170, 250, 40, 20, WHITE);
          tft.print(blue);

          fill_color = tft.color565(red_position, green_position, blue_position);
          tft.fillRect(10, 10, 50, 20, fill_color);

          //////////////// Debuging Switch on or off ////////////
          Serial.print("blue = ");
          Serial.println(blue);
          //////////////////////////////////////////////////////
        } else if (checkButtonPressed(touch_x, touch_y, 125, 10, 210, 30)) {
          // brightness button pressed,
          RGBbrightness = brightness;

          if (touch_x > 70 && touch_x < 210) {
            if (touch_y > 10 && touch_y < 30) {
              brightness = map(touch_x, 125, 210, 0, 255);
              RGBbrightness = map(brightness, 255, 0, 0, 255);

              brightness_old = brightness;
              brightness = touch_x;
            }
            tft.setTextColor(BLACK);
            tft.setCursor(60, 20);
            tft.setTextSize(2);
            tft.fillRect(50, 20, 50, 25, WHITE);
            tft.print(brightness_old);
            //////////////// Debuging Switch on or off ////////////
            Serial.print("brightness =  ");
            Serial.println(brightness);
            Serial.print("brightness_old =  ");
            Serial.println(brightness_old);
            Serial.print("RGBbrightness =  ");
            Serial.println(RGBbrightness);
            //////////////////////////////////////////////////////
            tft.fillRoundRect(120, 15, 100, 15, 10, BLACK);
            tft.fillRoundRect(brightness, 15, 10, 15, 10, YELLOW);
          }
        } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
          // ledstrip off button pressed,
          for (int a = 0; a < 8; a++) {                    // data count for how many leds inline (change to suit how many leds you are using)
            strip.setPixelColor(a, strip.Color(0, 0, 0));  // set STRIP to green
          }
          strip.show();  // needed to show changes
          analogWrite(red_pin, 255);
          analogWrite(green_pin, 255);
          analogWrite(blue_pin, 255);
          tft.fillRect(10, 10, 50, 20, WHITE);
          tft.fillRoundRect(30, 50, 30, 180, 10, RED);
          tft.setTextColor(RED);
          tft.setCursor(30, 250);
          tft.setTextSize(2);
          tft.fillRect(30, 250, 40, 20, WHITE);
          tft.print("0");
          tft.fillRoundRect(30, 220, 30, 10, 10, YELLOW);
          tft.fillRoundRect(100, 50, 30, 180, 10, GREEN);
          tft.setTextColor(GREEN);
          tft.setCursor(100, 250);
          tft.setTextSize(2);
          tft.fillRect(100, 250, 40, 20, WHITE);
          tft.print("0");
          tft.fillRoundRect(100, 220, 30, 10, 10, YELLOW);
          tft.fillRoundRect(170, 50, 30, 180, 10, BLUE);
          tft.setTextColor(BLUE);
          tft.setCursor(175, 250);
          tft.setTextSize(2);
          tft.fillRect(170, 250, 40, 20, WHITE);
          tft.print("0");
          tft.fillRoundRect(170, 220, 30, 10, 10, YELLOW);

        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
          // Back button pressed,
          currentMenu = SUBMENU_2;
        }
        break;

      case SOUNDS:
        //////////////// Debuging Switch on or off ////////////
        Serial.print("Sound Meter Reading  = ");
        Serial.println(DB);
        Serial.print("GAS Meter Reading  = ");
        Serial.println(drunk);
        ////////////////////////////////////////////////////////
        Sensor1 = true;
        tft.fillRoundRect(148, 225, 37, 25, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(10, 230);
        tft.setTextSize(1);
        tft.print("GAS Meter Reading  = ");
        tft.setTextColor(WHITE);
        tft.setCursor(150, 230);
        tft.setTextSize(2);
        tft.print(drunk);

        tft.fillRoundRect(148, 275, 37, 25, 10, BLUE);
        tft.setTextColor(BLACK);
        tft.setCursor(10, 280);
        tft.setTextSize(1);
        tft.print("Sound Meter Reading  = ");
        tft.setTextColor(WHITE);
        tft.setCursor(150, 280);
        tft.setTextSize(2);
        tft.print(DB);
        delay(50);
        if (DB > 650) {
          delay(1000);
          analogWrite(red_pin, 255);
        }
        if (checkButtonPressed(touch_x, touch_y, 10, 10, 100, 20)) {
          // Main Menu button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 140, 10, 200, 20)) {
          // Option Menu button pressed,
          drawOptionMenu();
          currentMenu = OPTION_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          Start();
          currentMenu = START;
        } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
          // Back button pressed,
          currentMenu = SUBMENU_2;
        }
        break;


      case DRAW:
        if (touch_y > 15 && touch_y < 270 && touch_x > 15 && touch_x < 230) {
          pen = BLACK;
          pen_new = pen;
          tft.drawPixel(touch_x, touch_y, pen_new);
          //////////////// Debuging Switch on or off ////////////
          Serial.print("pen = ");
          Serial.println(pen);
          Serial.print("pen_new  = ");
          Serial.print(pen_new);
          ////////////////////////////////////////////////////////
        } else if (checkButtonPressed(touch_x, touch_y, 20, 270, 20, 20)) {
          // red button pressed,
          currentMenu = PEN_RED;
          tft.drawFastHLine(110, 275, 10, RED);
          tft.drawFastVLine(125, 270, 10, RED);
          tft.drawRect(135, 270, 10, 10, RED);
          tft.drawCircle(155, 275, 5, RED);
          //////////////// Debuging Switch on or off ////////////
          Serial.print("pen = ");
          Serial.println(pen);
          Serial.print("pen_new  = ");
          Serial.println(pen_new);
            ////////////////////////////////////////////////////////

          case PEN_RED:
            if (touch_y > 15 && touch_y < 270 && touch_x > 15 && touch_x < 230) {
              tft.drawPixel(touch_x, touch_y, RED);
            } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
              // exit button pressed,
              Start();
              currentMenu = START;
            } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
              // Back button pressed,
              currentMenu = DRAW;
            } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
              // RESET button pressed,
              tft.fillRoundRect(10, 10, 225, 250, 10, BLACK);
              delay(500);
              tft.fillRoundRect(10, 10, 225, 250, 10, WHITE);
              tft.drawRect(15, 15, 215, 270, BLACK);
            }
            if (checkButtonPressed(touch_x, touch_y, 150, 270, 10, 10)) {
              // red circle pressed
              currentMenu = PEN_CIRCLE;
              tft.fillCircle(155, 275, 3, RED);

              case PEN_CIRCLE:
                if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
                  tft.fillCircle(touch_x, touch_y, 5, RED);
                  currentMenu = PEN_CIRCLE;
                } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
                  // exit button pressed,
                  Start();
                  currentMenu = START;
                } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
                  // Back button pressed to return to color selection
                  currentMenu = PEN_RED;
                } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
                  // RESET button pressed,
                  tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
                  delay(500);
                  tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
                  tft.drawRect(15, 15, 215, 270, BLACK);
                }
                break;

            } else if (checkButtonPressed(touch_x, touch_y, 130, 270, 10, 10)) {
              // Rect button pressed,
              currentMenu = PEN_RECT_RED;
              tft.fillRect(130, 270, 10, 10, RED);

              case PEN_RECT_RED:
                if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
                  tft.fillRect(touch_x, touch_y, 10, 10, RED);
                } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
                  // exit button pressed,
                  Start();
                  currentMenu = START;
                } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
                  // Back button pressed to return to color selection
                  currentMenu = PEN_RED;
                } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
                  // RESET button pressed,
                  tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
                  delay(500);
                  tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
                  tft.drawRect(15, 15, 215, 270, BLACK);
                }
                break;
            }
            break;

        } else if (checkButtonPressed(touch_x, touch_y, 35, 265, 20, 20)) {
          //Green button pressed
          tft.drawFastHLine(110, 275, 10, GREEN);
          tft.drawFastVLine(125, 270, 10, GREEN);
          tft.drawRect(135, 270, 10, 10, GREEN);
          tft.drawCircle(155, 275, 5, GREEN);
          currentMenu = PEN_GREEN;

          case PEN_GREEN:
            if (touch_y > 15 && touch_y < 270 && touch_x > 15 && touch_x < 230) {
              tft.drawPixel(touch_x, touch_y, GREEN);
            } else if (checkButtonPressed(touch_x, touch_y, 35, 300, 60, 20)) {
              // exit button pressed,
              Start();
              currentMenu = START;
            } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
              // Back button pressed,
              currentMenu = DRAW;
            } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
              // RESET button pressed,
              tft.fillRoundRect(10, 10, 225, 250, 10, BLACK);
              delay(500);
              tft.fillRoundRect(10, 10, 225, 250, 10, WHITE);
              tft.drawRect(15, 15, 215, 270, BLACK);
            }
            if (checkButtonPressed(touch_x, touch_y, 160, 275, 10, 10)) {
              // Green circle pressed
              currentMenu = PEN_CIRCLE_GREEN;
              tft.fillCircle(160, 275, 3, GREEN);

              case PEN_CIRCLE_GREEN:
                if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
                  tft.fillCircle(touch_x, touch_y, 5, GREEN);
                  currentMenu = PEN_CIRCLE_GREEN;
                } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
                  // exit button pressed,
                  Start();
                  currentMenu = START;
                } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
                  // Back button pressed to return to color selection
                  currentMenu = PEN_GREEN;
                } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
                  // RESET button pressed,
                  tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
                  delay(500);
                  tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
                  tft.drawRect(15, 15, 215, 270, BLACK);
                }
                break;

            } else if (checkButtonPressed(touch_x, touch_y, 135, 270, 10, 10)) {
              // Green Rect button pressed,
              currentMenu = PEN_RECT_GREEN;
              tft.fillRect(135, 270, 10, 10, GREEN);

              case PEN_RECT_GREEN:
                if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
                  tft.fillRect(touch_x, touch_y, 10, 10, GREEN);
                } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
                  // exit button pressed,
                  Start();
                  currentMenu = START;
                } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
                  // Back button pressed to return to color selection
                  currentMenu = DRAW;
                } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
                  // RESET button pressed,
                  tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
                  delay(500);
                  tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
                  tft.drawRect(15, 15, 215, 270, BLACK);
                }
                break;
            }
            break;

        } else if (checkButtonPressed(touch_x, touch_y, 50, 270, 10, 10)) {
          //Blue button pressed
          tft.drawFastHLine(110, 275, 10, BLUE);
          tft.drawFastVLine(125, 270, 10, BLUE);
          tft.drawRect(135, 270, 10, 10, BLUE);
          tft.drawCircle(155, 275, 5, BLUE);
          currentMenu = PEN_BLUE;

          case PEN_BLUE:
            if (touch_y > 15 && touch_y < 270 && touch_x > 15 && touch_x < 230) {
              tft.drawPixel(touch_x, touch_y, BLUE);
            } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
              // exit button pressed,
              Start();
              currentMenu = START;
            } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
              // Back button pressed,
              currentMenu = DRAW;
            } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
              // RESET button pressed,
              tft.fillRoundRect(10, 10, 225, 250, 10, BLACK);
              delay(500);
              tft.fillRoundRect(10, 10, 225, 250, 10, WHITE);
              tft.drawRect(15, 15, 215, 270, BLACK);
            }
            if (checkButtonPressed(touch_x, touch_y, 160, 275, 10, 10)) {
              // blue circle pressed
              currentMenu = PEN_CIRCLE_BLUE;
              tft.fillCircle(160, 275, 3, BLUE);

              case PEN_CIRCLE_BLUE:
                if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
                  tft.fillCircle(touch_x, touch_y, 5, BLUE);
                  currentMenu = PEN_CIRCLE_BLUE;
                } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
                  // exit button pressed,
                  Start();
                  currentMenu = START;
                } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
                  // Back button pressed to return to color selection
                  currentMenu = PEN_BLUE;
                } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
                  // RESET button pressed,
                  tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
                  delay(500);
                  tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
                  tft.drawRect(15, 15, 215, 270, BLACK);
                }
                break;

            } else if (checkButtonPressed(touch_x, touch_y, 135, 270, 10, 10)) {
              // blue Rect button pressed,
              currentMenu = PEN_RECT_BLUE;
              tft.fillRect(135, 270, 10, 10, BLUE);

              case PEN_RECT_BLUE:
                if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
                  tft.fillRect(touch_x, touch_y, 10, 10, BLUE);
                } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
                  // exit button pressed,
                  Start();
                  currentMenu = START;
                } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
                  // Back button pressed to return to color selection
                  currentMenu = DRAW;
                } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
                  // RESET button pressed,
                  tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
                  delay(500);
                  tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
                  tft.drawRect(15, 15, 215, 270, BLACK);
                }
                break;
            }
            break;

        } else if (checkButtonPressed(touch_x, touch_y, 135, 270, 10, 10)) {
          // Rect button pressed,
          currentMenu = PEN_RECT;
          tft.fillRect(135, 270, 10, 10, BLACK);

          case PEN_RECT:
            if (touch_y > 15 && touch_y < 260 && touch_x > 15 && touch_x < 230) {
              tft.fillRect(touch_x, touch_y, 10, 10, BLACK);
            } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
              // exit button pressed,
              Start();
              currentMenu = START;
            } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
              // Back button pressed,
              currentMenu = DRAW;
            } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
              // RESET button pressed,
              tft.fillRoundRect(10, 10, 225, 260, 10, BLACK);
              delay(500);
              tft.fillRoundRect(10, 10, 225, 260, 10, WHITE);
              tft.drawRect(15, 15, 215, 270, BLACK);
            }
            break;
        } else if (checkButtonPressed(touch_x, touch_y, 20, 300, 60, 20)) {
          // exit button pressed,
          drawMainMenu();
          currentMenu = MAIN_MENU;
        } else if (checkButtonPressed(touch_x, touch_y, 90, 300, 60, 20)) {
          // Back button pressed,
          currentMenu = SUBMENU_2;
        } else if (checkButtonPressed(touch_x, touch_y, 160, 300, 60, 20)) {
          // RESET button pressed,
          tft.fillRoundRect(10, 10, 225, 250, 10, BLACK);
          delay(500);
          tft.fillRoundRect(10, 10, 225, 250, 10, WHITE);
          tft.drawRect(15, 15, 215, 270, BLACK);
        }
        break;
    }
  }
}
void Start() {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);

  tft.setTextColor(BLACK);
  tft.setCursor(60, 20);
  tft.setTextSize(2);
  tft.print("Reality 2.0\n");

  tft.setCursor(20, 296);
  tft.setTextSize(1);
  tft.print("Created By 'MacaDaca88'      2023\n");
  tft.drawFastHLine(20, 306, 200, RED);
  delay(500);

  tft.setTextColor(RED);
  tft.setCursor(93, 153);
  tft.setTextSize(2);
  tft.fillCircle(120, 160, 40, GREEN);
  delay(200);
  tft.drawCircle(120, 160, 40, RED);
  delay(200);
  tft.print("Start\n");
}
void drawMainMenu() {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);
  tft.setTextColor(BLUE);
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.print("Main Menu");

  tft.setTextColor(RED);
  tft.setCursor(140, 10);
  tft.setTextSize(2);
  tft.print("Options");

  drawButton("Game 1", 20, 60, 200, 40);
  drawButton("Game 2", 20, 110, 200, 40);
  drawButton("Game 3", 20, 160, 200, 40);
  drawButton("Game 4", 20, 210, 200, 40);

  drawBackButton("Exit", 20, 300, 60, 20, 10);
}
void drawOptionMenu() {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);
  tft.setTextColor(RED);
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.print("Main Menu");

  tft.setTextColor(BLUE);
  tft.setCursor(140, 10);
  tft.setTextSize(2);
  tft.print("Options");

  drawBlueButton("Led", 20, 60, 200, 40, 0);
  drawRGBButton("RGB Srip", 20, 110, 200, 40, 0);
  drawButton("Sensors", 20, 160, 200, 40);
  drawButton("Drawing", 20, 210, 200, 40);

  drawBackButton("Exit", 20, 300, 60, 20, 10);
  drawBackButton("Back", 90, 300, 60, 20, 10);
}
void drawSubMenu(const char* label) {
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);
  tft.setTextColor(RED);
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.print("Main Menu");

  tft.setTextColor(RED);
  tft.setCursor(140, 10);
  tft.setTextSize(2);
  tft.print("Options");

  tft.setTextColor(BLUE);
  tft.setCursor(10, 30);
  tft.setTextSize(2);
  tft.print(label);

  drawBlueButton("Led On", 20, 60, 200, 40, 20);
  drawBlueButton("Led Off", 20, 110, 200, 40, 20);

  drawBackButton("Exit", 20, 288, 60, 20, 10);
  drawBackButton("Back", 90, 288, 60, 20, 10);
}
void drawRGB() {
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);
  tft.fillRect(10, 10, 50, 20, fill_color);
  tft.setTextColor(BLACK);
  tft.setCursor(60, 20);
  tft.setTextSize(2);
  tft.print(brightness_old);
  tft.fillRoundRect(30, 50, 30, 180, 10, RED);
  tft.fillRoundRect(30, red_position, 30, 10, 10, YELLOW);
  tft.setTextColor(RED);
  tft.setCursor(35, 250);
  tft.setTextSize(2);
  tft.print(0);
  tft.fillRoundRect(100, 50, 30, 180, 10, GREEN);
  tft.fillRoundRect(100, green_position, 30, 10, 10, YELLOW);
  tft.setTextColor(GREEN);
  tft.setCursor(105, 250);
  tft.setTextSize(2);
  tft.print(0);
  tft.fillRoundRect(170, 50, 30, 180, 10, BLUE);
  tft.fillRoundRect(170, blue_position, 30, 10, 10, YELLOW);
  tft.setTextColor(BLUE);
  tft.setCursor(170, 250);
  tft.setTextSize(2);
  tft.print(0);

  tft.fillRoundRect(120, 15, 100, 15, 10, BLACK);
  tft.fillRoundRect(brightness, 15, 10, 20, 10, YELLOW);

  drawBackButton("Exit", 20, 288, 60, 20, 10);
  drawBackButton("Back", 90, 288, 60, 20, 10);
  drawOffButton("Off", 160, 288, 60, 20, 10);
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
void ledon() {
  analogWrite(led, 255);
}
void ledoff() {
  analogWrite(led, 0);
}
void Sounds() {
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);
  tft.setTextColor(RED);
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.print("Main Menu");

  tft.setTextColor(RED);
  tft.setCursor(145, 10);
  tft.setTextSize(2);
  tft.print("Options");

  tft.setTextColor(BLACK);
  tft.setCursor(10, 280);
  tft.setTextSize(1);
  tft.print("GAS Meter Reading  = ");
  tft.fillRect(150, 230, 35, 25, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(150, 230);
  tft.setTextSize(2);
  tft.print(drunk);

  tft.setTextColor(BLACK);
  tft.setCursor(10, 230);
  tft.setTextSize(1);
  tft.print("Sound Meter Reading  = ");
  tft.fillRect(150, 280, 35, 25, WHITE);
  tft.setTextColor(BLACK);
  tft.setCursor(150, 280);
  tft.setTextSize(2);
  tft.print(DB);

  drawBackButton("Exit", 20, 300, 60, 20, 10);
  drawBackButton("Back", 90, 300, 60, 20, 10);
}
void Drawings() {

  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 225, 300, 10, WHITE);
  tft.drawRect(15, 15, 215, 270, BLACK);
  tft.drawFastHLine(15, 265, 215, BLACK);

  ///////////////////////COLORS  && SHAPES//////////////////
  // tft.fillRect(10, 270, 10, 10, BLACK);
  tft.fillRect(20, 270, 10, 10, RED);
  tft.fillRect(35, 270, 10, 10, GREEN);
  tft.fillRect(50, 270, 10, 10, BLUE);
  tft.fillRect(65, 270, 10, 10, YELLOW);
  tft.fillRect(80, 270, 10, 10, ORANGE);


  tft.drawFastHLine(110, 275, 10, BLACK);
  tft.drawFastVLine(125, 270, 10, BLACK);
  tft.drawRect(135, 270, 10, 10, BLACK);
  tft.drawCircle(155, 275, 5, BLACK);
  //////////////////////////////////////////////////////

  drawBackButton("Exit", 20, 300, 60, 20, 10);
  drawBackButton("Back", 90, 300, 60, 20, 10);
  drawBackButton("RESET", 160, 300, 60, 20, 10);
}
void game1() {

  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 240, 320, RED);
  tft.fillRoundRect(10, 10, 220, 300, 10, WHITE);
  tft.setTextColor(RED);
  tft.setCursor(50, 80);
  tft.setTextSize(3);
  tft.print("Game 1");
  tft.setTextColor(RED);
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.print("Main Menu");

  tft.setTextColor(RED);
  tft.setCursor(145, 10);
  tft.setTextSize(2);
  tft.print("Options");

  tft.setTextColor(RED);
  tft.setCursor(96, 154);
  tft.setTextSize(2);
  tft.fillCircle(120, 160, 40, GREEN);
  tft.drawCircle(120, 160, 40, RED);
  tft.print("Start\n");

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
