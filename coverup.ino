/*
Alert system for when your undergarments are showing.
 
 */

int LDR_Pin = A0;
//Pin Definitions
int data = 2; 
int clock = 3;
int latch = 4;
//Used for single LED manipulation
int ledState = 0;
const int ON = HIGH;
const int OFF = LOW;
//LED Pin Variables
int ledPins[] = {5,6,7,8};
//Imp pin
int impPin = 10;
int impState = 0;

void setup(){
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT); 
  
  //Set each pin connected to an LED to output mode (pulling high (on) or low (off)
  for(int i = 0; i < 8; i++){
      pinMode(ledPins[i],OUTPUT); //we use this to set each LED pin to output
  }  
}

void loop(){
  int LDRReading = analogRead(LDR_Pin); 
  Serial.println(LDRReading);
  delay(250);
  if( LDRReading > 500) {
    Serial.println("Oh no! Cover up");
      digitalWrite(impPin, 1); //signal Electric Imp to alert state
    beep(200, LDRReading); 
    int delayTime = 20; //the number of milliseconds to delay between LED updates
   for (int i=0; i < 8; i++) {
     changeLED(i,ON);
     delay(delayTime);
   }
   for (int i=0; i < 4; i++) {
     digitalWrite(ledPins[i], HIGH);
     delay(delayTime);
   }
   for (int i=0; i < 8; i++) {
     changeLED(i,OFF);
     delay(delayTime);
   }
   for (int i=0; i < 4; i++) {
     digitalWrite(ledPins[i], LOW);
     delay(delayTime);
   }
  }
    else{
      Serial.println("All clear!");
      analogWrite(9, 0); 
      digitalWrite(impPin,0); 
      
    }  

}

void beep(unsigned char delayms, unsigned char amntLight){
  analogWrite(9, 20);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
                           
 // tone(9, amntLight / 4);
  delay(delayms);          // wait for a delayms ms
  analogWrite(9, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  

/*
 * updateLEDs() - sends the LED states set in ledStates to the 74HC595
 * sequence
 */
void updateLEDs(int value){
  digitalWrite(latch, LOW);     //Pulls the chips latch low
  shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
  digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}



//These are used in the bitwise math that we use to change individual LEDs
//For more details http://en.wikipedia.org/wiki/Bitwise_operation
int bits[] = {B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000};
int masks[] = {B11111110, B11111101, B11111011, B11110111, B11101111, B11011111, B10111111, B01111111};
/*
 * changeLED(int led, int state) - changes an individual LED 
 * LEDs are 0 to 7 and state is either 0 - OFF or 1 - ON
 */
 void changeLED(int led, int state){
   ledState = ledState & masks[led];  //clears ledState of the bit we are addressing
   if(state == ON){ledState = ledState | bits[led];} //if the bit is on we will add it to ledState
   updateLEDs(ledState);              //send the new LED state to the shift register
 }
