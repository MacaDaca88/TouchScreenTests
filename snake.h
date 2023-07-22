/**
SNAKE GAME using Arduino Uno and TFT display
by Shubham Santosh
**/
#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
#include <EEPROM.h>
#include <TouchScreen.h> // only when you want to use touch screen

#include "food_bitmap.h"

#define BLACK 0x0000
#define BLUE 0x001F
#define GREEN 0x07E0
#define RED 0xF800
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define YELLOW 0xFFE0

#define FOOD_AREA 20
MCUFRIEND_kbv tft; //(A3,A2,A1,A0,A4);

bool bait_caught = false;

// Calibration values obtained from TouchScreen_Calibr_native(file->examples->MCUFRIEND_kbv->TouchScreen_Calibr_native.ino)
const int TS_LEFT = 904, TS_RT = 187, TS_TOP = 876, TS_BOT = 161;
const int XP = 7, XM = A1, YP = A2, YM = 6; //240x320 ID=0x9341
uint8_t points = 0;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

struct Node
{
  uint16_t x, y;
  struct Node *next;
};
struct Node *start = new Node;

void snake_init_pos(struct Node *head)
{
  struct Node *last_node = new Node;
  for (int i = 0; i <= 30; i++)
  {
    head->x = 30;
    head->y = 30;
    last_node = head;
    head->next = new Node;
    head = head->next;
  }
  last_node->next = NULL;
  delete head;
}

void clear_tail(struct Node *h, MCUFRIEND_kbv tft)
{
  struct Node *last_node = new Node;
  while (h->next != NULL)
  {
    last_node = h;
    h = h->next;
  }
  tft.fillCircle(h->x, h->y, 1.5, YELLOW);
  last_node->next = NULL;
  delete h;
}

void increase_length(struct Node *h)
{
  struct Node *last_node = new Node;
  struct Node *new_node = new Node;
  while (h != NULL)
  {
    last_node = h;
    h = h->next;
  }
  new_node->x = last_node->x;
  new_node->y = last_node->y;
  last_node->next = new_node;
  new_node->next = NULL;
}

bool body_bite(struct Node *h)
{
  int head_x = h->x;
  int head_y = h->y;
  h = h->next;
  while (h != NULL)
  {
    if (h->x == head_x && h->y == head_y)
    {
      return true;
    }
    h = h->next;
  }
  return false;
}

void position_display(struct Node *h, MCUFRIEND_kbv disp)
{
  while (h != NULL)
  {
    disp.fillCircle(h->x, h->y, (h == start ? 3 : 1.5), BLUE); // Increase the size of the head
    h = h->next;
  }
}

void insert_head(struct Node **h, uint16_t x, uint16_t y)
{
  struct Node *new_head = new Node;
  new_head->x = x;
  new_head->y = y;
  new_head->next = *h;
  *h = new_head;
}

void game_over(MCUFRIEND_kbv disp)
{
  tft.fillScreen(BLACK);
  uint8_t high_score = EEPROM.read(0);
  tft.setTextColor(WHITE);
  tft.setCursor(50, 10);
  tft.setTextSize(3);
  tft.print("GAME OVER\n\n\n");

  if (points > high_score)
  {
    EEPROM.write(0, points);
    tft.setTextSize(2);
    tft.print("\n\nConratulations!");
    tft.setTextSize(1);
    tft.print("\n\n You set a new high score!!!");
    tft.setTextSize(2);
    tft.print("\n\nNew high score: ");
    tft.print(points);
  }
  else
  {
    tft.setTextSize(2);
    tft.print("YOUR POINTS: ");
    tft.print(points);
    tft.print("\nHIGH SCORE: ");
    tft.print(high_score);
  }
  tft.setTextSize(1);
  tft.print("\n\n\nPress reboot to restart the game");
}

uint16_t x = 41, y = 30, bait_x = 100, bait_y = 100;
bool up = false, down = false, left = false, right = true;
uint8_t delay_time = 15; // Increase the delay time to make the snake move faster

void setup()
{
  tft.reset();
  Serial.begin(9600);
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);
  tft.fillScreen(YELLOW);
  tft.setTextColor(RED);
  tft.setCursor(20, 50);
  tft.setTextSize(3);
  tft.print("SNAKE GAME\n");
  tft.setCursor(40, 200);
  tft.setTextSize(3);
  tft.print("LOADING\n");
  tft.drawRGBBitmap(82, 110, front_page, 48, 64);
  delay(2000);
  tft.fillScreen(YELLOW);
  tft.setTextColor(0x0000);
  tft.setCursor(50, 300);
  tft.setTextSize(1);
  tft.print("points: " + String(points));
  snake_init_pos(start);
}

void loop()
{
  uint16_t touch_x, touch_y;
  tp = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  position_display(start, tft);
  if (bait_caught == true)
  {
    bait_caught = false;
    tft.fillScreen(YELLOW);
    bait_x = random(229);
    bait_y = random(290);
    points++;
    delay_time--;
    tft.setTextColor(0x0000);
    tft.setCursor(50, 300);
    tft.setTextSize(1);
    tft.print("POINTS: ");
    tft.print(points);
    delay(200);
  }

  tft.drawRGBBitmap(bait_x, bait_y, frog1, FOOD_AREA, FOOD_AREA);

  if (((start->x) >= bait_x && (start->x) <= bait_x + FOOD_AREA) && (((start->y) >= bait_y) && (start->y) <= bait_y + FOOD_AREA))
  {
    bait_caught = true;
  }
  if (tp.z > 200 && tp.z < 1000)
  {
    touch_x = map(tp.x, TS_LEFT, TS_RT, 0, 239);
    touch_y = map(tp.y, TS_TOP, TS_BOT, 0, 319);
    if ((touch_x > 110) && (touch_x < 130) && (touch_y > 10) && (touch_y < 30))
    {
      if (up == false)
      {
        up = true;
        down = false;
        left = false;
        right = false;
      }
      else
      {
        up = false;
        down = false;
        left = false;
        right = true;
      }
    }
    else if ((touch_x > 110) && (touch_x < 130) && (touch_y > 40) && (touch_y < 60))
    {
      if (down == false)
      {
        up = false;
        down = true;
        left = false;
        right = false;
      }
      else
      {
        up = false;
        down = false;
        left = false;
        right = true;
      }
    }
    else if ((touch_x > 90) && (touch_x < 110) && (touch_y > 20) && (touch_y < 40))
    {
      if (left == false)
      {
        up = false;
        down = false;
        left = true;
        right = false;
      }
      else
      {
        up = false;
        down = false;
        left = false;
        right = true;
      }
    }
    else if ((touch_x > 130) && (touch_x < 150) && (touch_y > 20) && (touch_y < 40))
    {
      if (right == false)
      {
        up = false;
        down = false;
        left = false;
        right = true;
      }
      else
      {
        up = false;
        down = false;
        left = false;
        right = true;
      }
    }
  }
  else
  {
    if (up == true)
    {
      if (y > 0)
      {
        y -= 4;
      }
      else
      {
        game_over(tft);
        while (1)
          ;
      }
      if (body_bite(start) == true)
      {
        game_over(tft);
        while (1)
          ;
      }
      insert_head(&start, x, y);
      clear_tail(start, tft);
    }
    if (down == true)
    {
      if (y < 299)
      {
        y += 4;
      }
      else
      {
        game_over(tft);
        while (1)
          ;
      }
      if (body_bite(start) == true)
      {
        game_over(tft);
        while (1)
          ;
      }
      insert_head(&start, x, y);
      clear_tail(start, tft);
    }
    if (left == true)
    {
      if (x > 0)
      {
        x -= 4;
      }
      else
      {
        game_over(tft);
        while (1)
          ;
      }
      if (body_bite(start) == true)
      {
        game_over(tft);
        while (1)
          ;
      }
      insert_head(&start, x, y);
      clear_tail(start, tft);
    }
    if (right == true)
    {
      if (x < 229)
      {
        x += 4;
      }
      else
      {
        game_over(tft);
        while (1)
          ;
      }
      if (body_bite(start) == true)
      {
        game_over(tft);
        while (1)
          ;
      }
      insert_head(&start, x, y);
      clear_tail(start, tft);
    }
  }
  delay(delay_time);
}
