#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 13, A0, A1, A2, A3);

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
// Infrared
const int irSensor = 7;
// Potentiometer
const int speedPot = A4;
// DIP Switch
const int dipManOver = 6;

void setup()
{
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor3Pin, OUTPUT);
  pinMode(motor4Pin, OUTPUT);
  digitalWrite(enablePin12, HIGH);
  digitalWrite(enablePin34, HIGH);

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
  Serial.begin(9600);
}

void loop()
{
  int front_distance = measureDistanceFront();
  Serial.println(front_distance);
  forward();

  if (front_distance < 10)
  {
    lcd.setCursor(0, 0);
    lcd.print("OBSTACLEDETECTED");
    halt();
    delay(500);
    backward();
    delay(1000);
    left();
    delay(500);
    right();
    delay(500);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Normal Operation");
  }
}

void halt()
{
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, LOW);
  digitalWrite(motor3Pin, LOW);
  digitalWrite(motor4Pin, LOW);

  lcd.setCursor(0, 1);
  lcd.print("Not Moving......");
}

void forward()
{
  digitalWrite(motor1Pin, HIGH);
  digitalWrite(motor2Pin, LOW);
  digitalWrite(motor3Pin, HIGH);
  digitalWrite(motor4Pin, LOW);

  lcd.setCursor(0, 1);
  lcd.print("Moving FORWARD..");
}

void backward()
{
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, HIGH);
  digitalWrite(motor3Pin, LOW);
  digitalWrite(motor4Pin, HIGH);

  lcd.setCursor(0, 1);
  lcd.print("Moving BACKWARD.");
}

void left()
{
  digitalWrite(motor1Pin, HIGH);
  digitalWrite(motor2Pin, LOW);
  digitalWrite(motor3Pin, LOW);
  digitalWrite(motor4Pin, HIGH);

  lcd.setCursor(0, 1);
  lcd.print("Turning LEFT....");
}

void right()
{
  digitalWrite(motor1Pin, LOW);
  digitalWrite(motor2Pin, HIGH);
  digitalWrite(motor3Pin, HIGH);
  digitalWrite(motor4Pin, LOW);

  lcd.setCursor(0, 1);
  lcd.print("Turning RIGHT...");
}

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