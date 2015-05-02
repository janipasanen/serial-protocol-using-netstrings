/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 * (http://www.parallax.com/detail.asp?product_id=555-28027)
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

/////////////////////////////
// Variables for PIR Motion sensor
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
//int calibrationTime = 30;
int calibrationTime = 3;


//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 8;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;


//////////////////////////////
// Ultra sonic variable
int inputPin=2; //ECHO pin 
int outputPin=3; //TRIG pin 

/////////////////////////////
// Netstrings
char netstring;
String netstringEncodeIntTransfer();
String netstringEncodeStringTransfer();

// Serial.read
String data = "";


/////////////////////////////
//SETUP

void setup(){
  Serial.begin(9600);

// Pir motion sensor
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  
 
  // Ultra sonic
  pinMode(inputPin, INPUT); 
  pinMode(outputPin, OUTPUT); 
 
 // Netstrings & serial communication
 
 
}
////////////////////////////
//LOOP
void loop(){

   
// Pir motion sensor
  if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         //Serial.println("---");
         Serial.print(netstringEncodeStringTransfer("motion detected at "));
         Serial.print(netstringEncodeIntTransfer((millis()/1000)));
         Serial.print(netstringEncodeStringTransfer(" sec")); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print(netstringEncodeStringTransfer("motion ended at "));      //output
           Serial.print(netstringEncodeIntTransfer((millis() - pause)/1000));
           Serial.print(netstringEncodeStringTransfer(" sec"));
           delay(50);
           }
       }
       
       
       // Ultra sonic
       
     digitalWrite(outputPin, HIGH); //Trigger ultrasonic detection 
     delayMicroseconds(10); 
     digitalWrite(outputPin, LOW); 
     int distance = pulseIn(inputPin, HIGH); //Read ultrasonic reflection
     distance= distance/58; //Calculate distance 
     Serial.print(netstringEncodeIntTransfer(distance)); //Print distance 
     delay(100);        
       
       
     // Read from serial port if it is not locked by Serial.print
     char incomingData;
     if (Serial.available() > 0) {
     // read the incoming byte:
     incomingData = Serial.read();
   
     Serial.print("I received: ");
     Serial.println(incomingData);

     data = data + incomingData;

     delay(100);   
       
     }
       
  }
  
  
  // Netstrings encoding & serial TX communication
  
  String netstringEncodeIntTransfer(int value) {
    
    String s = String(value); 
    int len = s.length();
    if (!len) return "ERROR:_EMPTY_STRING";
    String netstring = len + String(":" + s +",");
    
    return netstring;
 
  }
  
  String netstringEncodeStringTransfer(String value) {
    
    int len = value.length();
    if (!len) return "ERROR:_EMPTY_STRING";
    String netstring = len + String(":" + value +",");
    
    return netstring;
 
  }
  
  
    // Netstrings decoding & serial FX communication
 
