#include <Arduino.h>

//Variaveis da logica PID
float Kp = 0;
float Ki = 0;
float Kd = 0;
float erro = 0;

float integral = 0;
float prop = 0;
float derivativo = 0;
float U_erro = 0;
float PID = 0;

float velesq = 0;
float veldir = 0;

float avgSpeedESQ = 0;
float avgSpeedDIR = 0;
//-----------------------

//Variavel entrada para os motores------
int IN1 = 4;
int IN2 = 5;

int PWM_A = 8; //controle velocidade PWM
int PWM_B = 9; //controle velocidade PWM
//--------------------------------------

//Array para os sensores-------------------
int sensorPin [7] = {33,35,37,39,41,43,45};
int sensor [7];
//-----------------------------------------

//Declaracao de funcoes
void calcErro();
void calcPID();
//---------------------

void setup(){
  Serial.begin(9600);
  for(int i=0; i <= 6; i++){ //configuracao dos sensores como INPUT
    pinMode(sensorPin[i], INPUT);
    Serial.println("Sensor definido na porta:" + String(sensorPin[i])); //printa no serial o pino correspondente do sensor
  }
  pinMode (IN1, OUTPUT); //config. do pino do motor como OUTPUT
  pinMode (IN2, OUTPUT); //config. do pino do motor como OUTPUT
  pinMode (PWM_A, OUTPUT); //config. do pino pwm do motor como OUTPUT
  pinMode (PWM_B, OUTPUT); //config. do pino pwm do motor como OUTPUT
}

void loop(){
  contProc(); //declaracao da funcao main do codigo
  delay(200);
}

void calcErro(){ //Funcao que calcula o erro da relacao de quais sensores estao ativos
  for(int i = 0; i <= 6; i++){ //estrutura para ler os valores de cada sensor
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
    erro = -1;
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

void calculaPID(){ //funcao que calcula o PID para controlar a velocidade do motor
  if(erro==0){ // se o erro for igual a zero
    integral = 0;
    prop = erro;
    integral = integral + erro;
  }
  if(integral > 255){ // se o valor da variavel integral for maior que 255
    integral = 255;
  }else if(integral < -255){ // se o valor da variavel integral for menor que -255
    integral = -255;
    derivativo = erro - U_erro;
    PID = ((Kp * prop) + (Ki * integral) + (Kd * derivativo));
    U_erro = erro;
  }
}

void contProc(){ //funcao princiapal que faz a logica de acionamento de cada motor
  calcErro();
  calcPID();

  if(PID>=0){ // se o PID for maior ou igual a zero
    velesq = avgSpeedESQ;
    veldir = avgSpeedDIR - PID;
  }else{
    velesq = avgSpeedESQ + PID;
    veldir = avgSpeedDIR;
  }
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  analogWrite(PWM_A, velesq);
  analogWrite(PWM_B, veldir);
}