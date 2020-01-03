#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <BLEPeripheral.h>

const int SW_SENSE_PIN = 6;
const int SE_SENSE_PIN = 7;
const int NW_SENSE_PIN = 8;
const int NE_SENSE_PIN = 9;

const int XSHUT_CENTER = 16;
const int XSHUT_RIGHT = 13;
const int XSHUT_LEFT = 10;

const int OUT_OF_RANGE = 4;
const bool NO_DEBUG = false;

const int LEFT_INPUT1 = 0;
const int LEFT_INPUT2 = 1;
const int RIGHT_INPUT1 = 2;
const int RIGHT_INPUT2 = 3;
const int LEFT_SPEED = 4;
const int RIGHT_SPEED = 4; 

Adafruit_VL53L0X lox_center = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_left = Adafruit_VL53L0X();
Adafruit_VL53L0X lox_right = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t center_measure;
VL53L0X_RangingMeasurementData_t left_measure;
VL53L0X_RangingMeasurementData_t right_measure;

enum RANGINING_SENSOR_ID 
{
  LEFT,
  CENTER,
  RIGHT
};

 char msg[128];


void resetRangingSensorArray() 
{
  digitalWrite(XSHUT_CENTER, LOW);
  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
  delay(10);

  digitalWrite(XSHUT_CENTER, HIGH);
  digitalWrite(XSHUT_LEFT, HIGH);
  digitalWrite(XSHUT_RIGHT, HIGH);
  delay(10);

  digitalWrite(XSHUT_CENTER, LOW);
  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
}



void activateRangingSensor(RANGINING_SENSOR_ID id, Adafruit_VL53L0X & sensor, int i2c_address) 
{
  sprintf(msg, "Attempting to boot VL53L0X at address :0x%02X", i2c_address);
  Serial.println(msg);

  switch (id) {
    case LEFT:    digitalWrite(XSHUT_LEFT, HIGH);
                  break;
    case CENTER:  digitalWrite(XSHUT_CENTER, HIGH);
                  break;
    case RIGHT:   digitalWrite(XSHUT_RIGHT, HIGH);
                  break;
  }
  delay(10);
 
  while (!sensor.begin(i2c_address)) {
    sprintf(msg, "Failed to boot VL53L0X at address :0x%02X", i2c_address);
    Serial.println(msg);
    delay(100);
  }
  sprintf(msg, "VL53L0X booted at address :0x%02X", i2c_address);
  Serial.println(msg);
}

void initMotors()
{
  pinMode(LEFT_INPUT1, OUTPUT);
  pinMode(LEFT_INPUT2, OUTPUT);
  pinMode(RIGHT_INPUT1, OUTPUT);
  pinMode(RIGHT_INPUT2, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT); 

  digitalWrite(LEFT_INPUT1, 0);
  digitalWrite(LEFT_INPUT2, 0);
  digitalWrite(RIGHT_INPUT1, 0);
  digitalWrite(RIGHT_INPUT2, 0);
  analogWrite(LEFT_SPEED, 0);
  analogWrite(RIGHT_SPEED, 0);
}

void initSensorArray()
{
  // We're overriding the default micro:bit GPIO settings here. Don't expect
  // the LED matrix or buttons to function correctly in this version
  // Line sensors
  pinMode(SW_SENSE_PIN, INPUT);
  pinMode(SE_SENSE_PIN, INPUT);
  pinMode(NW_SENSE_PIN, INPUT);
  pinMode(NE_SENSE_PIN, INPUT);

  // VL53L0X pins
  pinMode(XSHUT_CENTER, OUTPUT);
  pinMode(XSHUT_LEFT, OUTPUT);
  pinMode(XSHUT_RIGHT, OUTPUT);

  resetRangingSensorArray();
  activateRangingSensor(LEFT, lox_left, 0x70);
  activateRangingSensor(RIGHT, lox_right, 0x71);
  activateRangingSensor(CENTER, lox_center, 0x72);
}

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  initMotors();
  initSensorArray();

  Serial.println("Sumo robot demo is ready!");
}

void TestRangingSensors() {
  Serial.println("TestRangingSensors");
  lox_center.rangingTest(&center_measure, NO_DEBUG); 
  lox_left.rangingTest(&left_measure, NO_DEBUG);
  lox_right.rangingTest(&right_measure, NO_DEBUG);

  // print center sensorreading
  Serial.print("C: ");
  if (center_measure.RangeStatus != OUT_OF_RANGE) {
    Serial.print(center_measure.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  Serial.print("\n");

  // print left sensorreading
  Serial.print("L: ");
  if (left_measure.RangeStatus != OUT_OF_RANGE) {
    Serial.print(left_measure.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  Serial.print("\n");

  // print right sensorreading
  Serial.print("R: ");
  if (right_measure.RangeStatus != OUT_OF_RANGE) {
    Serial.print(right_measure.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  Serial.print("\n");
}


void TestLineSensors() 
{
  for (;;) {
    char buffer[100];

    int sw, se, nw, ne;

    sw = digitalRead(SW_SENSE_PIN);
    se = digitalRead(SE_SENSE_PIN);
    nw = digitalRead(NW_SENSE_PIN);
    ne = digitalRead(NE_SENSE_PIN);

    sprintf(buffer, "%d %d %d %d", sw, se, nw, ne);
    Serial.println(buffer);
  }
}

void Forward()
{
  digitalWrite(LEFT_INPUT1, 1);
  digitalWrite(LEFT_INPUT2, 0);
  digitalWrite(RIGHT_INPUT1, 0);
  digitalWrite(RIGHT_INPUT2, 1);
}

void Reverse() 
{
  digitalWrite(LEFT_INPUT1, 0);
  digitalWrite(LEFT_INPUT2, 1);
  digitalWrite(RIGHT_INPUT1, 1);
  digitalWrite(RIGHT_INPUT2, 0);
}

void TurnLeft() 
{
  digitalWrite(LEFT_INPUT1, 0);
  digitalWrite(LEFT_INPUT2, 1);
  digitalWrite(RIGHT_INPUT1, 0);
  digitalWrite(RIGHT_INPUT2, 1);
}

void TurnRight()
{
  digitalWrite(LEFT_INPUT1, 1);
  digitalWrite(LEFT_INPUT2, 0);
  digitalWrite(RIGHT_INPUT1, 1);
  digitalWrite(RIGHT_INPUT2, 0);
}

void SetSpeed(int speed) 
{
  analogWrite(LEFT_SPEED, speed);
  analogWrite(RIGHT_SPEED, speed);
}

void loop() 
{
  // Simple demo. 
  //  Full speed ahead. 
  //  If an object is determined to be closer than 15 cm by the center sensor then
  //    Reverse for half a second 
  //    Then turn left for 100ms. 
  //  Repeat
  SetSpeed(255);
  Forward();
  lox_center.rangingTest(&center_measure, NO_DEBUG);
  if (center_measure.RangeStatus != OUT_OF_RANGE) {  
    Serial.println(center_measure.RangeMilliMeter);
    if (center_measure.RangeMilliMeter < 150) {
      Serial.println("   Reversing");
      Reverse();
      delay(500);
      TurnLeft();
      delay(100);
    }
  } 
}