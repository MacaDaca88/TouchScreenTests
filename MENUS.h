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