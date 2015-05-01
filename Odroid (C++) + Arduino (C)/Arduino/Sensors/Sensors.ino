

#include <Servo.h>
Servo esc;
Servo servo;




#include <SonarSRF.h> //library from open source
#include <Wire.h>
#define ultrasonicAddress (0xE4 >> 1) //sensor's adress (right) 
#define ultrasonicAddress1 (0x70) //(right)

#define IS_1 7 //pin for front right 
#define IS_2 0 //pin for rear right IR
#define IS_3 5 // pin for the back IR           

#define GAIN 0x09 // Setup Analogue Gain
#define LOCATION 0x8C // Setup Range Location

int IS_frontRight, IS_rearRight, IS_back, val1,val2, val3;
SonarSRF MainSonar, secondSonar;
char unit = 'c'; // 'i' for inches, 'c' for centimeters, 'm' for micro-seconds
float sensorReading = 0;
float sensorReading2 = 0;
const int encoderPinA = 19; 
const int encoderPinB = 25;
volatile long encoderTicks = 0; 


int throttle = 1500; //
int angle = 90;

  

void setup() {
   Serial.begin(9600);
    esc.attach(9);
    servo.attach(10);
   
    MainSonar.connect(ultrasonicAddress,GAIN, LOCATION);
    secondSonar.connect(ultrasonicAddress1, GAIN, LOCATION);
    
    pinMode(encoderPinA, INPUT);    
    pinMode(encoderPinB, INPUT);     
    attachInterrupt(4, WheelEncoderInterrupt, CHANGE);
    
}

void loop() {
  
    esc.writeMicroseconds(throttle); 
   servo.write(angle);
    sensorReading = MainSonar.getRange(unit, true);
    sensorReading2 = secondSonar.getRange(unit, true);
    
    
    val1 = analogRead(IS_1);  // reads the value of the sharp sensor
    val2 = analogRead(IS_2);
    val3 = analogRead(IS_3);
    
    IS_frontRight = ((2914/(val1 + 5) - 1));
    IS_rearRight =  ((2914/(val2 + 5) - 1)); //converts to cm
    IS_back = ((2914/(val3 + 5) - 1)); 
   
    Serial.print("0xE4: ");
    Serial.println(sensorReading);
    Serial.print("0x70: ");
    Serial.println(sensorReading2);
    
    
    Serial.print("IS_frontRight: ");
    Serial.println(IS_frontRight);
    Serial.print("IS_rearRight: ");
    Serial.println(IS_rearRight);
    Serial.print("Back: ");
    Serial.println(IS_back); // prints the value of the sensor to the serial monitor
    
    
    
    Serial.print("Wheel encoder: ");
    Serial.println(encoderTicks);
    delay(100);               // wait for this much time before printing next value
   
}

// Interrupt service routines for the wheel encoder

void WheelEncoderInterrupt(){ //if pinA is high and pinB is low ticks ++ else ticks
                              //if pinA is low and pinB is low ticks -- else ticks ++
   if(digitalRead(encoderPinA)){ 
   !(digitalRead(encoderPinB)) ? encoderTicks -- : encoderTicks ++;
   }
  else{
   !(digitalRead(encoderPinB)) ? encoderTicks ++ :encoderTicks --; 
   } 
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    switch(inChar){
      case 'f': throttle= 1600; break;
      case 'b': throttle= 1200;break;
      case 'n': throttle= 1500;break;
      case 'r': angle = 65;break;
      case 'l': angle = 115;break;
      case 'm': angle = 90;break;
      case 's': throttle= 1500;angle = 90;break;
    } 
  }   
}


 
