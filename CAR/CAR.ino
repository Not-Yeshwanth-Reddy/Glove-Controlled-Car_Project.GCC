 #include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

int data = 0, temp = -1;   //  for receiving data (NRF

int frontEnable = 5;  // for L293D
int backEnable = 6;
int frontMotorWire1 = 2;
int frontMotorWire2 = 3;
int backMotorWire1 = 7;
int backMotorWire2 = 8;
int chipPower = 4;  //  chip power (can also be taken from vcc pin)

void setup() {// put your setup code here, to run once:

  Mirf.cePin = 9;
  Mirf.csnPin = 10;
  Mirf.spi = &MirfHardwareSpi;    //NRF-module is controlled by SPI
  Mirf.init();                    //init NRF-module
  Mirf.setRADDR((byte *)"clie1"); //set name of this NRF-module as "clie1" (max 5 bytes)
  Mirf.payload = sizeof(int);     //data transferred between modules are int-s
  Mirf.channel = 78;              //channel is 78 (by default the channel is 1)
  Mirf.config();                  //apply settings specified above to NRF-module

  pinMode(frontEnable, OUTPUT);      // int frontEnable = 5;
  pinMode(backEnable, OUTPUT);     //  int backEnable = 6;
  pinMode(frontMotorWire1, OUTPUT);  //  int frontMotorWire1 = 2;
  pinMode(frontMotorWire2, OUTPUT);  //  int frontMotorWire2 = 3;
  pinMode(backMotorWire1, OUTPUT); //  int backMotorWire1 = 7;
  pinMode(backMotorWire2, OUTPUT); //  int backMotorWire2 = 8;
  
  pinMode(chipPower, OUTPUT);       //  int chipPower = 4;
  
  digitalWrite(chipPower, HIGH);    // should always be HIGH.
  digitalWrite(frontEnable, HIGH);
  digitalWrite(backEnable, HIGH);
  
  Serial.begin(9600);             //monitor state over Serial Monitor
  Serial.println("Listening...!"); //indicate that the program is ready to receive data

}

void loop() {  // put your main code here, to run repeatedly:

  if (Mirf.dataReady()) {         //check if data received by NRF-module

    while (!Mirf.rxFifoEmpty()) //repeat data reading while the queue (buffer) is not emptied
    { //in cycle
      Mirf.getData((byte *) &data);//get data by the address of "data" field

      //optional
      Serial.print("Received:");
      Serial.println(data);

    }
  }
  if (data != temp) {
    switch (data) {
      case 1 :
        //Accelerate
          digitalWrite(backEnable, HIGH);
          
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, LOW);
          digitalWrite(backMotorWire2, HIGH);
        //Serial.println("Running...");
        break;
      case 2 :
        //Reverse
          digitalWrite(backEnable, HIGH);
          
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, HIGH);
          digitalWrite(backMotorWire2, LOW);
        //Serial.println("Reversing...");
        break;
      case 13 :
        //AccelerateLeft
          digitalWrite(backEnable, HIGH);
          
          digitalWrite(frontMotorWire1, HIGH);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, LOW);
          digitalWrite(backMotorWire2, HIGH);
        //Serial.println("Running Left...");
        break;
      case 14 :
        //AccelerateRight
          digitalWrite(backEnable, HIGH);
          
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, HIGH);
          digitalWrite(backMotorWire1, LOW);
          digitalWrite(backMotorWire2, HIGH);
        //Serial.println("Running Right...");
        break;
      case 23 :
        //ReverseLeft
        digitalWrite(backEnable, HIGH);
        
        digitalWrite(frontMotorWire1, HIGH);
        digitalWrite(frontMotorWire2, LOW);
        digitalWrite(backMotorWire1, HIGH);
        digitalWrite(backMotorWire2, LOW);
        //Serial.println("Reversing Left...");
        break;
      case 24 :
        //ReverseRight
        digitalWrite(backEnable, HIGH);
        
        digitalWrite(frontMotorWire1, LOW);
        digitalWrite(frontMotorWire2, HIGH);
        digitalWrite(backMotorWire1, HIGH);
        digitalWrite(backMotorWire2, LOW);
        //Serial.println("Reversing Right...");
        break;
      case 10 :
        //SlowAccelerate  
          analogWrite(backEnable, 140);
                  
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, LOW);
          digitalWrite(backMotorWire2, HIGH);
        //Serial.println("Walking...");
        break;
      case 20 :
        //SlowReverse();
          analogWrite(backEnable, 140);
        
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, HIGH);
          digitalWrite(backMotorWire2, LOW);
        //Serial.println("Slowly Reversing...");
        break;
      case 130 :
        //SlowAccelerateLeft
          analogWrite(backEnable, 140);
        
          digitalWrite(frontMotorWire1, HIGH);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, LOW);
          digitalWrite(backMotorWire2, HIGH);
        //Serial.println("Walking Left...");
        break;
      case 140 :
        //SlowAccelerateRight
          analogWrite(backEnable, 140);
        
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, HIGH);
          digitalWrite(backMotorWire1, LOW);
          digitalWrite(backMotorWire2, HIGH);
        //Serial.println("Walking Right...");
        break;
      case 230 :
        //SlowReverseLeft
          analogWrite(backEnable, 140);
        
          digitalWrite(frontMotorWire1, HIGH);
          digitalWrite(frontMotorWire2, LOW);
          digitalWrite(backMotorWire1, HIGH);
          digitalWrite(backMotorWire2, LOW);
        //Serial.println("Slowly Reversing Left...");
        break;
      case 240 :
        //SlowReverseRight
          analogWrite(backEnable, 140);
        
          digitalWrite(frontMotorWire1, LOW);
          digitalWrite(frontMotorWire2, HIGH);
          digitalWrite(backMotorWire1, HIGH);
          digitalWrite(backMotorWire2, LOW);
        //Serial.println("Slowly Reversing Right...");
        break;
      case 0 :
        //Stop();
        digitalWrite(backEnable, LOW);
        //Serial.println("DAFAQ...?");
        break;
      default:
        Serial.println("What the F*** was that...?");
    }
    temp = data;
  }


}
