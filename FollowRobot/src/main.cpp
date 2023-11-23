#include <Arduino.h>

float Kp = 0;
float Ki = 0;
float Kd = 0;
float erro = 0;

float integral = 0;
float prop = 0;
float derivativo = 0;
float U_erro = 0;
float PID = 0;


int sensorPin [7] = {33,35,37,39,41,43,45};
int sensor [7];

void calcErro();
void calcPID();

void setup(){
  Serial.begin(9600);
  for(int i=0; i <= 6; i++){
    pinMode(sensorPin[i], INPUT);
    Serial.println("Sensor definido na porta:" + String(sensorPin[i]));
  }
}

void loop(){
  calcErro();
  calcPID();
  delay(200);
}

void calcErro(){
  for(int i = 0; i <= 6; i++){
    sensor[i] = digitalRead(sensorPin[i]);
    Serial.println("Sensor " + String(i) + " valor: " + String(sensor[i]));
  }

  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==HIGH && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = 0;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==HIGH && sensor[4]==HIGH && sensor[5]==LOW && sensor[6]==LOW){
    erro = 1;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==HIGH && sensor[3]==HIGH && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = 1;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==HIGH && sensor[5]==LOW && sensor[6]==LOW){
    erro = 2;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==HIGH && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = -2;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==HIGH && sensor[5]==HIGH && sensor[6]==LOW){
    erro = 3;
  }
  if(sensor[0]==LOW && sensor[1]==HIGH && sensor[2]==HIGH && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = -3;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==HIGH && sensor[6]==LOW){
    erro = 4;
  }
  if(sensor[0]==LOW && sensor[1]==HIGH && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = -4;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==HIGH && sensor[6]==HIGH){
    erro = 5;
  }
  if(sensor[0]==HIGH && sensor[1]==HIGH && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = -5;
  }
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==HIGH){
    erro = 6;
  }
  if(sensor[0]==HIGH && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW && sensor[5]==LOW && sensor[6]==LOW){
    erro = -6;
  }
}

void calculaPID(){
  if(erro==0){
    integral = 0;
    prop = erro;
    integral = integral + erro;
  }
  if(integral > 255){
    integral = 255;
  }else if(integral < -255){
    integral = -255;
    derivativo = erro - U_erro;
    PID = ((Kp * prop) + (Ki * integral) + (Kd * derivativo));
    U_erro = erro;
  }
}