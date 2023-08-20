#include <SoftwareSerial.h>

SoftwareSerial BT(10,11);    // Definimos los pines RX y TX del Arduino conectados al Bluetooth
const int motor1Pin = 3; // Pin for Motor 1
const int motor2Pin = 5; // Pin for Motor 2
const int motor3Pin = 6; // Pin for Motor 3
const int motor4Pin = 9;

const int rutina1[] = {100, 200, 100, 200, 100};
const int rutina2[] = {300, 100, 300, 100, 300};
const int rutina3[] = {200, 50, 200, 50, 200};

const int numSteps = 5;

int motorNum = 0;
unsigned long motorStartTime = 0;
unsigned long motorEndTime = 0;
int rutinaIndex = 0;
bool motorRunning = false;
char CharIN = ' ';     

void setup() {
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(motor3Pin, OUTPUT);
  pinMode(motor4Pin, OUTPUT);
  BT.begin(9600);
  Serial.begin(9600);
  Serial.println("Starting motors spinning ...");
}

void loop() {
  
  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    //Serial.write(BT.read());
    //char c = BT.read();
    Serial.println("llego por aire");
  }
 
  if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
    //Serial.print(Serial.available());
     //BT.write(Serial.read());
     //char c = Serial.read();
     Serial.println("llego por tierra");
  }
  Serial.println(BT.available());
  delay(100);
  if (BT.available() > 0) {
    String command = BT.readStringUntil('\n');
    if (command.startsWith("ON")) {
      int motor = command.substring(2, 3).toInt();
      int timing = command.substring(4, command.indexOf(' ', 4)).toInt();
      int rutina = command.substring(command.indexOf(' ', 4) + 1).toInt();
      executeRutina(motor, timing, rutina);
    } else if (command.startsWith("OFF")) {
      int motor = command.substring(4).toInt();
      turnOffMotor(motor);
    }
  }


  if (motorRunning) {
    unsigned long currentMillis = millis();
    if (currentMillis >= motorEndTime) {
      digitalWrite(getMotorPin(motorNum), LOW); // Turn off motor after timing
      motorRunning = false;
    } else if (currentMillis - motorStartTime >= rutina1[rutinaIndex]) {
      digitalWrite(getMotorPin(motorNum), !digitalRead(getMotorPin(motorNum))); // Toggle motor state
      rutinaIndex++;
      if (rutinaIndex >= numSteps) {
        rutinaIndex = 0; // Restart routine if it's finished
      }
      motorStartTime = currentMillis;
    }
  }
}

void executeRutina(int motor, int timing, int rutinaNum) {
  motorNum = motor;
  motorRunning = true;
  rutinaIndex = 0;
  motorStartTime = millis();
  motorEndTime = motorStartTime + timing;
  digitalWrite(getMotorPin(motorNum), HIGH);
}

void turnOffMotor(int motor) {
  digitalWrite(getMotorPin(motor), LOW);
  motorRunning = false;
}


 void OnOffMotors (void)
{
  if(CharIN == 'ON1 '){digitalWrite(motor1Pin, HIGH);}
  if(CharIN == 'OFF1'){digitalWrite(motor1Pin, LOW);}

  if(CharIN == 'ON2'){digitalWrite(motor2Pin, HIGH);}
  if(CharIN == 'OFF2'){digitalWrite(motor2Pin, LOW);}

  if(CharIN == 'ON3'){digitalWrite(motor3Pin, HIGH);}
  if(CharIN == 'OFF3'){digitalWrite(motor3Pin, LOW);}

  if(CharIN == 'ON4'){digitalWrite(motor4Pin, HIGH);}
  if(CharIN == 'OFF4'){digitalWrite(motor4Pin, LOW);}

} 
  

int getMotorPin(int motor) {
  if (motor == 1) {
    return motor1Pin;
  } else if (motor == 2) {
    return motor2Pin;
  } else if (motor == 3) {
    return motor3Pin;
  }else if (motor == 4) {
    return motor4Pin;
  }
  return -1; // Invalid motor number
}