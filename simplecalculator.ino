#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
5+void show(String line1, String line2) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(line1);
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println(line2);
  display.display();
}

void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {   // try 0x3D if blank
    Serial.println("OLED not found");
    while (true);
  }
  show("Type value", "");
  Serial.println("Calculator ready. Type like 12+5, 9*3, 7/2");
}

void loop() {
  if (!Serial.available()) return;

  String s = Serial.readStringUntil('\n');
  s.trim();
  s.replace(" ", "");
  if (s.length() == 0) return;

  // find operator
  int op = -1;
  for (int i = 1; i < (int)s.length(); i++) {
    char c = s[i];
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == 'x') { op = i; break; }
  }
  if (op == -1) { show(s, "Error"); Serial.println("Error"); return; }

  float a = s.substring(0, op).toFloat();
  float b = s.substring(op + 1).toFloat();
  char o = s[op];
  float r = 0;

  if (o == '+') r = a + b;
  else if (o == '-') r = a - b;
  else if (o == '*' || o == 'x') r = a * b;
  else if (o == '/') {
    if (b == 0) { show(s, "Div by 0"); Serial.println("Div by 0"); return; }
    r = a / b;
  }

  String out = (r == (long)r) ? String((long)r) : String(r, 4);
  show(s + " =", out);
  Serial.println(s + " = " + out);
}
