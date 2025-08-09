
void handleRGBGeneratorMenu() {
  const char* options[] = { "Red", "Green", "Blue", "Brightness", "Apply" };
  const int totalOptions = sizeof(options) / sizeof(options[0]);

  static int redValue = 100;
  static int greenValue = 100;
  static int blueValue = 100;
  static int brightness = 80;
  static int menuIndex = 0;

  static bool applied = false;

  // === After Apply Screen ===
  if (applied) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(20, 20, "Color Applied!");

    // Show preview box
    u8g2.drawFrame(50, 30, 30, 20);
    u8g2.drawBox(51, 31, 28, 18);  // Simulated color box

    // Back button
    u8g2.drawBox(40, 55, 48, 10);
    u8g2.setDrawColor(0);
    u8g2.drawStr(52, 63, "Back");
    u8g2.setDrawColor(1);
    u8g2.sendBuffer();

    if (digitalRead(BTN_SELECT) == LOW) {
      delay(150);
      applied = false;
    }
    return;
  }

  // === Handle Navigation ===
  if (digitalRead(BTN_UP) == LOW) {
    menuIndex = (menuIndex - 1 + totalOptions) % totalOptions;
    delay(150);
  }

  if (digitalRead(BTN_DOWN) == LOW) {
    menuIndex = (menuIndex + 1) % totalOptions;
    delay(150);
  }

  // === Adjust Values ===
  int colorStep = 10;
  int brightStep = 5;

  if (digitalRead(BTN_LEFT) == LOW || digitalRead(BTN_RIGHT) == LOW) {
    bool inc = digitalRead(BTN_RIGHT) == LOW;
    switch (menuIndex) {
      case 0: redValue = constrain(redValue + (inc ? colorStep : -colorStep), 0, 255); break;
      case 1: greenValue = constrain(greenValue + (inc ? colorStep : -colorStep), 0, 255); break;
      case 2: blueValue = constrain(blueValue + (inc ? colorStep : -colorStep), 0, 255); break;
      case 3: brightness = constrain(brightness + (inc ? brightStep : -brightStep), 0, 100); break;
    }
    delay(80);
  }

  // === Apply Color ===
  if (digitalRead(BTN_SELECT) == LOW && menuIndex == 4) {
    pixel.setBrightness(map(brightness, 0, 100, 0, 255));
    pixel.setPixelColor(0, pixel.Color(redValue, greenValue, blueValue));
    pixel.show();

    applied = true;
    delay(150);
    return;
  }

  // === Draw UI ===
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.drawStr(0, 10, "RGB LED Controller");

  for (int i = 0; i < totalOptions; i++) {
    int y = 20 + i * 10;

    // Highlight selected item
    if (i == menuIndex) {
      u8g2.drawBox(0, y - 8, 128, 10);
      u8g2.setDrawColor(0);
    }

    // Labels
    u8g2.drawStr(5, y, options[i]);

    // Sliders
    if (i == 0) {
      u8g2.drawFrame(75, y - 7, 48, 8);
      u8g2.drawBox(76, y - 6, map(redValue, 0, 255, 1, 46), 6);
    }
    if (i == 1) {
      u8g2.drawFrame(75, y - 7, 48, 8);
      u8g2.drawBox(76, y - 6, map(greenValue, 0, 255, 1, 46), 6);
    }
    if (i == 2) {
      u8g2.drawFrame(75, y - 7, 48, 8);
      u8g2.drawBox(76, y - 6, map(blueValue, 0, 255, 1, 46), 6);
    }
    if (i == 3) {
      u8g2.drawFrame(75, y - 7, 48, 8);
      u8g2.drawBox(76, y - 6, map(brightness, 0, 100, 1, 46), 6);
    }

    u8g2.setDrawColor(1);  // Reset draw color
  }

  // Live color preview box (top-right)
  u8g2.drawFrame(105, 0, 22, 12);
  u8g2.drawBox(106, 1, 20, 10);  // Could use dither pattern to simulate brightness

  u8g2.sendBuffer();
}
