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
    forward(speed);
  }
}