/*
   Hockey++ by George Bougakov

   (c) George Bougakov, 2019

   https://bygeorgenet.me

   ### PINOUT ###
   #GOALS#
   D2 - Right Goal Light Sensor (Digital out)
   D3 - Left Goal Light Sensor (Digital out)
   D5 - Right Goal LED data pin
   D6 - Left Goal LED data pin

   #LED MATRIX#
   D11 - DIN/CIN
   D9  - CS
   D13 - CLK

   #START BUTTON OR TOUCH SENSOR#
   D7 - signal pin
*/
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <FastLED.h>

#define RIGHT_GOAL_SENSOR 2
#define LEFT_GOAL_SENSOR  3
#define START_BUTTON 7

#define LEDMX_BRIGHTNESS 10 // Use a value between 0 and 15
#define LEDMX_ROTATION 1    // Replace 1 with 0 if matrixes display incorrectly

#define LEDIND_LEFT 6
#define LEDIND_RIGHT 5

#define GOAL_COOLDOWN_DELAY 2000
#define GAME_LENGTH 120000 // Two minutes 

int pinCS = 9; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 4;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);
CRGB leds[2];

void setup() {
  Serial.begin(9600);
  pinMode(RIGHT_GOAL_SENSOR, INPUT);
  pinMode(LEFT_GOAL_SENSOR, INPUT);
  pinMode(START_BUTTON, INPUT);

  FastLED.addLeds<WS2812B, LEDIND_LEFT, GRB>(leds, 0, 1);
  FastLED.addLeds<WS2812B, LEDIND_RIGHT, GRB>(leds, 1, 1);

  leds[0] = CRGB::White;
  leds[1] = CRGB::White;
  FastLED.show();

  matrix.setRotation(0, LEDMX_ROTATION);
  matrix.setRotation(1, LEDMX_ROTATION);
  matrix.setRotation(2, LEDMX_ROTATION);
  matrix.setRotation(3, LEDMX_ROTATION);

  matrix.setIntensity(LEDMX_BRIGHTNESS);
}

int leftGoalPreviousState = LOW;
int rightGoalPreviousState = LOW;

int leftScore = 0;
int rightScore = 0;

long startTime = 0;
bool isGameRunning = false;

void drawScore() {
  matrix.fillScreen(LOW);

  if (leftScore > 9) {
    matrix.drawChar(4, 0, String(leftScore)[0], HIGH, LOW, 1);
    matrix.drawChar(9, 0, String(leftScore)[1], HIGH, LOW, 1);
  } else {
    matrix.drawChar(9, 0, String(leftScore)[0], HIGH, LOW, 1);
  }

  matrix.drawChar(14, 0, ':', HIGH, LOW, 1);

  if (rightScore > 9) {
    matrix.drawChar(19, 0, String(rightScore)[0], HIGH, LOW, 1);
    matrix.drawChar(24, 0, String(rightScore)[1], HIGH, LOW, 1);
  } else {
    matrix.drawChar(19, 0, String(rightScore)[0], HIGH, LOW, 1);
  }

  matrix.write();
}

void animateGoal(int ledNum) {
  matrix.fillScreen(LOW);
  matrix.drawChar(4, 0, 'G', HIGH, LOW, 1);
  matrix.drawChar(10, 0, 'O', HIGH, LOW, 1);
  matrix.drawChar(16, 0, 'A', HIGH, LOW, 1);
  matrix.drawChar(22, 0, 'L', HIGH, LOW, 1);
  matrix.write();

  leds[ledNum] = CRGB::Red;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Orange;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Yellow;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Green;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Cyan;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Blue;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Purple;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Red;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Orange;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Yellow;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Green;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Cyan;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Blue;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Purple;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Red;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Orange;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Yellow;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Green;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Cyan;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::Blue;
  FastLED.show();
  delay(100);
  leds[ledNum] = CRGB::White;
  FastLED.show();
  delay(100);

  drawScore();
}

void loop() {
  if (isGameRunning && millis() - startTime < GAME_LENGTH) {
    drawScore();

    int leftGoalState = digitalRead(LEFT_GOAL_SENSOR);
    int rightGoalState = digitalRead(RIGHT_GOAL_SENSOR);

    Serial.println(digitalRead(START_BUTTON));

    if (leftGoalState == HIGH) {
      if (leftGoalPreviousState == LOW) {
        leftGoalPreviousState = HIGH;

        leftScore++;

        Serial.println("LEFT GOAL!");
        Serial.println(String(leftScore) + ":" + String(rightScore));

        animateGoal(0);
      }
    } else {
      if (leftGoalPreviousState == HIGH) {
        delay(GOAL_COOLDOWN_DELAY); // Wait to not accidentally score any additional goals
      }
      leftGoalPreviousState = LOW;
    }

    if (rightGoalState == HIGH) {
      if (rightGoalPreviousState == LOW) {
        rightGoalPreviousState = HIGH;

        rightScore++;

        Serial.println("RIGHT GOAL!");
        Serial.println(String(leftScore) + ":" + String(rightScore));

        animateGoal(1);
      }
    } else {
      if (rightGoalPreviousState == HIGH) {
        delay(GOAL_COOLDOWN_DELAY); // Wait to not accidentally score any additional goals
      }

      rightGoalPreviousState = LOW;
    }

    int progress = map(millis() - startTime, 0, GAME_LENGTH, 1, 32);
    Serial.println(millis() - startTime);
    Serial.println(progress);

    for (int i = 0; i <= progress; i++) {
      matrix.drawPixel(i, 7, HIGH);
      matrix.write();
    }
  } else if (isGameRunning && millis() - startTime > GAME_LENGTH) {
    Serial.println("STOP GAME");
    Serial.println(millis() - startTime);
    Serial.println(millis());
    Serial.println(startTime);
    isGameRunning = false;
  } else if (!isGameRunning) {
    matrix.fillScreen(LOW);
    matrix.drawChar(4, 0, 'O', HIGH, LOW, 1);
    matrix.drawChar(10, 0, 'V', HIGH, LOW, 1);
    matrix.drawChar(16, 0, 'E', HIGH, LOW, 1);
    matrix.drawChar(22, 0, 'R', HIGH, LOW, 1);
    matrix.write();

    if (digitalRead(START_BUTTON) == HIGH) {
      startTime = millis();
      isGameRunning = true;

      leftScore = 0;
      rightScore = 0;
      
      //Serial.println("TAP");
      //Serial.println(millis());
    }
  }
}
