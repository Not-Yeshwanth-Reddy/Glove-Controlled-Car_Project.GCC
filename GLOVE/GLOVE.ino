#include <Wire.h>
#include <MPU6050.h>

#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

MPU6050 mpu;

int finger1_tip = 2,
    finger1_mid = 3,
    finger2_tip = 4,
    finger2_mid = 5;
int temp= -1 , msg;
bool index_tip, index_mid, middle_tip, middle_mid;

void setup() {
  Serial.begin(9600);
  pinMode(finger1_tip, INPUT_PULLUP);
  pinMode(finger1_mid, INPUT_PULLUP);
  pinMode(finger2_tip, INPUT_PULLUP);
  pinMode(finger2_mid, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  Mirf.cePin = 9;
  Mirf.csnPin = 10;
  Mirf.spi = &MirfHardwareSpi;    //NRF-module is controlled by SPI
  Mirf.init();                    //init NRF-module
  Mirf.setRADDR((byte *)"serv1"); //set name of this NRF-module as "serv1" (max 5 bytes)
  Mirf.payload = sizeof(int);     //data transferred between modules are int-s
  Mirf.channel = 78;              //channel is 78 (by default the channel is 1)
  Mirf.config();                  //apply settings specified above to NRF-module
  Serial.println("Started...");

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }
}

int gyro() {
  Vector normAccel = mpu.readNormalizeAccel();

  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
  if (pitch > 30)
    return 4;
  if (pitch <= 30 && pitch >= -30)
    return 0;
  if (pitch < -30)
    return 3;
}

void transmit(int msg) {
  if (Mirf.isSending()) {         //wait if NRF-module is sending data
  }
  Mirf.setTADDR((byte *)"clie1");

  Mirf.send((byte*)&msg);     //send int values by address of the field

  //optional
  Serial.print("Sent:");          //indicate what values was sent
  Serial.println(msg);
}

void loop() {
  index_tip = digitalRead(finger1_tip);
  index_mid = digitalRead(finger1_mid);
  middle_tip = digitalRead(finger2_tip);
  middle_mid = digitalRead(finger2_mid);

  if (index_tip == 0) {
    if (gyro() == 0)
      msg = 1;
    if (gyro() == 3)
      msg = 13;
    if (gyro() == 4)
      msg = 14;
  }

  if (index_mid == 0) {
    if (gyro() == 0)
      msg = 10;
    if (gyro() == 3)
      msg = 130;
    if (gyro() == 4)
      msg = 140;
  }

  if (middle_tip == 0) {
    if (gyro() == 0)
      msg = 2;
    if (gyro() == 3)
      msg = 23;
    if (gyro() == 4)
      msg = 24;
  }

  if (middle_mid == 0) {
    if (gyro() == 0)
      msg = 20;
    if (gyro() == 3)
      msg = 230;
    if (gyro() == 4)
      msg = 240;
  }

  if (index_tip == 1 && index_mid == 1 && middle_tip == 1 && middle_mid == 1) {
    msg = 0;
  }

  if (msg != temp) {
    transmit(msg);
  }
  temp = msg;
  delay(100);
}
