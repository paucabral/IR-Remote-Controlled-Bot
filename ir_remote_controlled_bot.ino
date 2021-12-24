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
int speed;
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

  // BUZZER
  pinMode(buzzer, OUTPUT);

  // LCD DECLARATIONS
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   IR RC 'Bot   ");
  lcd.setCursor(0, 1);
  lcd.print("GROUP#3  CPE41S3");
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
  speed = analogRead(speedPot) / 4;

  lcd.setCursor(0, 0);
  lcd.print("MO:");
  if (manual_override == HIGH)
  {
    lcd.print("ON ");
  }
  else
  {
    lcd.print("OFF");
  }

  Serial.print("Front Distance: ");
  Serial.println(front_distance);
  Serial.print("Manual Override: ");
  Serial.println(manual_override);

  obstacleDetection(front_distance, manual_override, speed);
}

// MOTOR MOVEMENTS
void halt()
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
  lcd.print("HALT     ");
  lcd.print("SPD:");
  lcd.print("000");
}

void forward()
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
  lcd.print("FORWARD  ");
  lcd.setCursor(9, 1);
  lcd.print("SPD:");
  lcd.print("000");
  lcd.setCursor(13, 1);
  lcd.print(speed);
}

void backward()
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
  lcd.print("BACKWARD ");
  lcd.setCursor(9, 1);
  lcd.print("SPD:");
  lcd.print("000");
  lcd.setCursor(13, 1);
  lcd.print(speed);
}

void left()
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
  lcd.print("LEFT     ");
  lcd.setCursor(9, 1);
  lcd.print("SPD:");
  lcd.print("000");
  lcd.setCursor(13, 1);
  lcd.print(speed);
}

void right()
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
  lcd.print("RIGHT    ");
  lcd.setCursor(9, 1);
  lcd.print("SPD:");
  lcd.print("000");
  lcd.setCursor(13, 1);
  lcd.print(speed);
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
  if (front_distance < 10 && manual_override == LOW)
  {
    lcd.setCursor(8, 0);
    lcd.print("OBSTCL:1");
    digitalWrite(buzzer, HIGH);
    halt();
    delay(500);
    backward();
    delay(1000);
    halt();
    delay(500);
  }
  else
  {
    if (front_distance < 10)
    {
      lcd.setCursor(8, 0);
      lcd.print("OBSTCL:1");
      digitalWrite(buzzer, LOW);
      remoteControl(speed);
    }
    else
    {
      lcd.setCursor(8, 0);
      lcd.print("OBSTCL:0");
      digitalWrite(buzzer, LOW);
      remoteControl(speed);
    }
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
      forward();
      break;
    case backward_8:
      backward();
      break;
    case left_4:
      left();
      break;
    case right_6:
      right();
      break;
    case halt_5:
      halt();
      break;
    }
    irrecv.resume();
  }
}