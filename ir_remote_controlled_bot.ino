// LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13, A0, A1, A2, A3);
// NeoPixel LED
#include <Adafruit_NeoPixel.h>
#define PIN A5
#define NUMPIXELS 4
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// IR Remote
#include <IRremote.h>
const int irSensor = 7;
IRrecv irrecv(irSensor);
decode_results results;
const int halt_5 = 43095;
const int forward_2 = 34935;
const int left_4 = 10455;
const int right_6 = 26775;
const int backward_8 = 39015;
// Ultrasonic Sensor Front
int echoPin = 11;
int trigPin = 11;
// MOTOR A
const int motor1Pin = 2;
const int motor2Pin = 3;
const int enablePin12 = 9;
// MOTOR B
const int motor3Pin = 4;
const int motor4Pin = 5;
const int enablePin34 = 8;
// Buzzer
const int buzzer = 10;
// Potentiometer
const int speedPot = A4;
// DIP Switch
const int dipManOver = 6;

void setup()
{
  // MOTOR PINS
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor3Pin, OUTPUT);
  pinMode(motor4Pin, OUTPUT);

  // LCD DECLARATIONS
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("    LUCAS bot");
  lcd.setCursor(0, 1);
  lcd.print("Group 3 CPE41S3");
  delay(2000);
  lcd.clear();

  // SERIAL COMMUNICATION
  Serial.begin(9600);

  // IR SIGNAL
  Serial.println("Enabling IRin");
  irrecv.enableIRIn();
  Serial.println("Enabled IRin");
}

void loop()
{
  int front_distance = measureDistanceFront();
  int manual_override = digitalRead(dipManOver);
  int speed = analogRead(speedPot) / 4;

  Serial.print("Front Distance: ");
  Serial.println(front_distance);
  Serial.print("Manual Override: ");
  Serial.println(manual_override);

  obstacleDetection(front_distance, manual_override, speed);
}

// MOTOR MOVEMENTS
void halt(int speed)
{
  analogWrite(enablePin12, speed);
  analogWrite(enablePin34, speed);
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, LOW);
  digitalWrite(motor3Pin, LOW);
  digitalWrite(motor4Pin, LOW);

  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
  }

  lcd.setCursor(0, 1);
  lcd.print("Not Moving......");
}

void forward(int speed)
{
  analogWrite(enablePin12, speed);
  analogWrite(enablePin34, speed);
  digitalWrite(motor1Pin, HIGH);
  digitalWrite(motor2Pin, LOW);
  digitalWrite(motor3Pin, HIGH);
  digitalWrite(motor4Pin, LOW);

  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    pixels.show();
  }

  lcd.setCursor(0, 1);
  lcd.print("Moving FORWARD..");
}

void backward(int speed)
{
  analogWrite(enablePin12, speed);
  analogWrite(enablePin34, speed);
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, HIGH);
  digitalWrite(motor3Pin, LOW);
  digitalWrite(motor4Pin, HIGH);

  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, 102, 0));
    pixels.show();
  }

  lcd.setCursor(0, 1);
  lcd.print("Moving BACKWARD.");
}

void left(int speed)
{
  analogWrite(enablePin12, speed);
  analogWrite(enablePin34, speed);
  digitalWrite(motor1Pin, HIGH);
  digitalWrite(motor2Pin, LOW);
  digitalWrite(motor3Pin, LOW);
  digitalWrite(motor4Pin, HIGH);

  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(140, 0, 255));
    pixels.show();
  }

  lcd.setCursor(0, 1);
  lcd.print("Turning LEFT....");
}

void right(int speed)
{
  analogWrite(enablePin12, speed);
  analogWrite(enablePin34, speed);
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, HIGH);
  digitalWrite(motor3Pin, HIGH);
  digitalWrite(motor4Pin, LOW);

  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(255, 0, 212));
    pixels.show();
  }

  lcd.setCursor(0, 1);
  lcd.print("Turning RIGHT...");
}

// ULTRASONIC DISTANCE MEASUREMENT
int measureDistanceFront()
{
  long duration, inches;

  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  return inches;
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

void obstacleDetection(int front_distance, int manual_override, int speed)
{
  if (front_distance < 10 && manual_override == HIGH)
  {
    lcd.setCursor(0, 0);
    lcd.print("OBSTACLEDETECTED");
    halt(speed);
    delay(1000);
    backward(speed);
    delay(1000);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Normal Operation");
    remoteControl(speed);
  }
}

// IR REMOTE CONTROL
void remoteControl(int speed)
{
  if (irrecv.decode(&results))
  {
    unsigned int value = results.value;
    Serial.println(value);
    switch (value)
    {
    case forward_2:
      forward(speed);
      break;
    case backward_8:
      backward(speed);
      break;
    case left_4:
      left(speed);
      break;
    case right_6:
      right(speed);
      break;
    case halt_5:
      halt(speed);
      break;
    }
    irrecv.resume();
  }
}