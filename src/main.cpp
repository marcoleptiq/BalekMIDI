 #include <SoftwareSerial.h>
 #include <Arduino.h> // For PlatformIO to work
/*
  Twenty-four clock tick bytes are sent per quarter note when the host sequencer is running.
  Every time the sequencer is stopped, the stop byte is sent. Every time the sequencer is 
  started after being stopped, the start byte is sent. Every time the sequencer is started 
  after being paused, the continue byte is sent.
  
  Using this information, it is possible to write a simple Arduino sketch to handle MIDI 
  clock bytes and "do something" for every clock tick. The trick is to work out what you 
  want to do with every clock tick, and then simply adjust the code below to add this 
  desired functionality.
*/

#define LED1_PIN 3
#define LED2_PIN 5
#define LED3_PIN 6
#define LED4_PIN 9

#define BRIGHTNESS_MIN 8
#define BRIGHTNESS_MED 64
#define BRIGHTNESS_MAX 255

char led1_state = 255;
char led2_state = 255;
char led3_state = 255;
char led4_state = 255;
int midi_start = 250;
int midi_stop = 252;
int midi_clock = 248;
int midi_continue = 251;
int play_flag = 0;
byte data;

const byte rxPin = 12;
const byte txPin = 10; // not used for the moment

int ppqn = 0;       // 96 ticks / mesure
int stepBeat = 0;   // 24 ticks <-> each quarter mesure
int mesure = 0;     // 4*96 ticks <-> 4*4 stepBeats
int ledState = HIGH;

/*
 * VERIFY what is SoftwareSerial || pin12 as RX ?
 */
// SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin); //viewed on website
/*
 * Supported baud rates are 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 
 * 38400, 57600, and 115200.
 */
SoftwareSerial mySerial(rxPin, txPin);


void Sync() {
  ppqn++;
  if (ppqn == 96) {
    ppqn = 0;
    stepBeat = 0;
    mesure = (mesure + 1) % 4 ;
  }

  if ( ppqn == 0 ) {
    stepBeat = 0 ;
  }
  else if ( ppqn == 24 || ppqn == 48 || ppqn == 72 ) {

    stepBeat += 1 % 4;
  }
  
  if (stepBeat==0){
    led1_state = BRIGHTNESS_MAX;
    led2_state = BRIGHTNESS_MIN;
    led3_state = BRIGHTNESS_MIN;
    led4_state = BRIGHTNESS_MIN;
  }

  if (stepBeat==1){
    led1_state = BRIGHTNESS_MIN;
    led2_state = BRIGHTNESS_MAX;
    led3_state = BRIGHTNESS_MIN;
    led4_state = BRIGHTNESS_MIN;
  }
  if (stepBeat==2){
    led1_state = BRIGHTNESS_MIN;
    led2_state = BRIGHTNESS_MIN;
    led3_state = BRIGHTNESS_MAX;
    led4_state = BRIGHTNESS_MIN;
  }
  if (stepBeat==3){
    led1_state = BRIGHTNESS_MIN;
    led2_state = BRIGHTNESS_MIN;
    led3_state = BRIGHTNESS_MIN;
    led4_state = BRIGHTNESS_MAX;
  }
  switch(mesure){
    case 0:
      if (stepBeat != 0){
        led1_state = BRIGHTNESS_MED;
      }
    break;
      
    case 1:
      if (stepBeat != 1){
        led2_state = BRIGHTNESS_MED;
      }
    break;
    
    case 2:
      if (stepBeat != 2){
        led3_state = BRIGHTNESS_MED;
      }
    break;
    
    case 3:
      if (stepBeat != 3){
        led4_state = BRIGHTNESS_MED;
      }
    break;
  }
}

void Frame () {
  analogWrite(LED1_PIN,led1_state);
  analogWrite(LED2_PIN,led2_state);
  analogWrite(LED3_PIN,led3_state);
  analogWrite(LED4_PIN,led4_state);
}


void setup() {

  pinMode( rxPin, INPUT );
  pinMode( txPin, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  mySerial.begin( 31250 );
  Serial.begin(9600);
  analogWrite(LED1_PIN,LOW);
  digitalWrite(LED2_PIN,HIGH);
  analogWrite(LED3_PIN,LOW);
  analogWrite(LED4_PIN,BRIGHTNESS_MED);
}



void loop() {
  
  if ( mySerial.available() > 0) {
    data = mySerial.read();
    if (data == midi_start) {
      play_flag = 1;
    }
    else if (data == midi_continue) {
      play_flag = 1;
    }
    else if (data == midi_stop) {
      play_flag = 0;
    }
    else if ((data == midi_clock) && (play_flag == 1)) {
      Sync();
      Frame();
    }
  }
}