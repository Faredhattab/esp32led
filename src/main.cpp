#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN         4   // Pin for LED strip
#define NUM_LEDS        262  // Number of LEDs
#define SENSOR_UP_PIN   21  // Pin for "sensor_up"
#define SENSOR_DOWN_PIN 5   // Pin for "sensor_down"

CRGB leds[NUM_LEDS];
const int segmentLengths[] = {26, 24, 22, 22, 21, 17, 19, 18, 17, 16, 21, 26, 26};
bool sequenceInProgress = false;

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_UP_PIN, INPUT);
  pinMode(SENSOR_DOWN_PIN, INPUT);
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
}

void setSegmentColor(int start, int length, CRGB color) {
  fill_solid(&leds[start], length, color);
  FastLED.show();
}

void randomColor(bool reverseOrder) {
  sequenceInProgress = true;
  CRGB randomColor = CRGB(random(256), random(256), random(256));
  int start = reverseOrder ? sizeof(segmentLengths) / sizeof(segmentLengths[0]) - 1 : 0;
  int end = reverseOrder ? -1 : sizeof(segmentLengths) / sizeof(segmentLengths[0]);
  int increment = reverseOrder ? -1 : 1;

  for (int i = start; i != end; i += increment) {
    setSegmentColor(i * segmentLengths[0], segmentLengths[i], randomColor);
    delay(200);
    Serial.println(i);
  }

  Serial.println("LEDs lit sequentially in random color");
  sequenceInProgress = false;
}

void turnOff() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  Serial.println("LEDs turned off");
}

void loop() {
  if (!sequenceInProgress) {
    // Check if the light is off before processing sensor signals
    if (leds[0] == CRGB::Black) {
      if (digitalRead(SENSOR_UP_PIN) == HIGH) {
        randomColor(true);
        Serial.println("Sensor Up Activated");
        delay(20000);
      }

      if (digitalRead(SENSOR_DOWN_PIN) == HIGH) {
        randomColor(false);
        Serial.println("Sensor Down Activated");
        delay(20000);
      }
    }
  }

  if (!sequenceInProgress && digitalRead(SENSOR_UP_PIN) == LOW && digitalRead(SENSOR_DOWN_PIN) == LOW) {
    turnOff();
    Serial.println("Sensors Inactive");
    delay(1500);
  }
}
