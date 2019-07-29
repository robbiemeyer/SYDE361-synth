#include <avr/io.h>
#include <avr/interrupt.h>


#define arp1 0
#define arp2 1
#define arp3 2
#define arp4 3
#define arpf A22
#define pitch A9
#define att A8
#define dec A7
#define sus A6
#define rel A5
#define noise A1
#define vol A2
#define sub A0
#define joyY A4
#define joyX A3

const int controllerBase = 80;
const int channel = 1;
const int numInputs = 15;

//int values [numInputs] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
//int last_values [numInputs] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

int next_noise_val = 0;
int noise_val = 0;
int noise_ctrl = 0;

int next_sub_val = 0;
int sub_val = 0;
int sub_ctrl = 1;

int next_vol_val = 0;
int vol_val = 0;
int vol_ctrl = 2;

int next_att_val = 0;
int att_val = 0;
int att_ctrl = 3;

int next_dec_val = 0;
int dec_val = 0;
int dec_ctrl = 4;

int next_sus_val = 0;
int sus_val = 0;
int sus_ctrl = 5;

int next_rel_val = 0;
int rel_val = 0;
int rel_ctrl = 6;

int arp_ctrl = 7;

int next_arpf_val = 0;
int arpf_val = 0;
int arpf_ctrl = 8;

int next_pitch_val = 0;
int pitch_val = 0;

const float max_stick = 21.4;
const float min_stick = 1.3;

float last_valueX = -1;
float last_valueY = -1;

bool arp1_changed = false;
bool arp2_changed = false;
bool arp3_changed = false;
bool arp4_changed = false;

void setup() {
  Serial.begin(9600);

  pinMode(arp1, INPUT_PULLUP);
  pinMode(arp2, INPUT_PULLUP);
  pinMode(arp3, INPUT_PULLUP);
  pinMode(arp4, INPUT_PULLUP);
//  pinMode(arpf, INPUT);
//  pinMode(joyX, INPUT);
//  pinMode(joyY, INPUT);
//  pinMode(noise, INPUT);
//  pinMode(vol, INPUT);

  attachInterrupt(digitalPinToInterrupt(arp1), arprupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(arp2), arprupt2, FALLING);
  attachInterrupt(digitalPinToInterrupt(arp3), arprupt3, FALLING);
  attachInterrupt(digitalPinToInterrupt(arp4), arprupt4, FALLING);
}

void loop() {
  next_noise_val = 1023*normalize(1024 - smoothRead(noise, 200), 0, 1024);
  next_sub_val = 1023*normalize(1024 - smoothRead(sub, 200), 0, 1024);
  next_pitch_val = 1023*normalize(smoothRead(pitch, 200), 0, 1024);
  next_vol_val = 1023*normalize(1024 - smoothRead(vol, 10), 0, 1024);
  next_att_val = 1023*normalize(smoothRead(att, 10), 0, 1024);
  next_dec_val = 1023*normalize(smoothRead(dec, 10), 0, 1024);
  next_sus_val = 1023*normalize(smoothRead(sus, 10), 0, 1024);
  next_rel_val = 1023*normalize(smoothRead(rel, 10), 0, 1024);
  next_arpf_val = 1023*normalize(1024 - smoothRead(arpf, 1), 100, 1024);

  if ( get_delta(next_pitch_val, pitch_val) >= 5 ) {
    pitch_val = next_pitch_val;
    Serial.println(pitch_val);

    usbMIDI.sendPitchBend(10000*normalize(pitch_val, 0, 900) - 5000, channel);
  }
  if( get_delta(next_arpf_val, arpf_val) >= 5 ) {
    arpf_val = next_arpf_val;
    Serial.print("arpf");
    Serial.println(arpf_val/8);

    usbMIDI.sendControlChange(arpf_ctrl, arpf_val/8, channel);
  }

  if ( get_delta(next_att_val, att_val ) >= 2) {
    att_val = next_att_val;
    usbMIDI.sendControlChange(att_ctrl, att_val/8, channel);
    Serial.print("att");
    Serial.println(att_val);
  }
  if ( get_delta(next_dec_val, dec_val ) >= 2 ) {
    dec_val = next_dec_val;
    usbMIDI.sendControlChange(dec_ctrl, dec_val/8, channel);
    Serial.print("dec");
    Serial.println(dec_val);
  }
  else if ( get_delta(next_sus_val, sus_val ) >= 2 ) {
    sus_val = next_sus_val;
    usbMIDI.sendControlChange(sus_ctrl, sus_val/8, channel);
    Serial.print("sus");
    Serial.println(sus_val);
  }
  if ( get_delta(next_rel_val, rel_val ) >= 16 ) {
    rel_val = next_rel_val;
    usbMIDI.sendControlChange(rel_ctrl, rel_val/8, channel);\
    Serial.print("rel");
    Serial.println(rel_val);
  }

  if ( get_delta(next_vol_val, vol_val) >= 2.5){
    vol_val = next_vol_val;
    usbMIDI.sendControlChange(vol_ctrl, vol_val/8, channel);
    Serial.print("vol");
    Serial.println(vol_val);
  }
  
 if ( get_delta(next_noise_val, noise_val) >= 5 ) {
    noise_val = next_noise_val;
    usbMIDI.sendControlChange(noise_ctrl, noise_val/8 , channel);
    Serial.print("noise");
    Serial.println(noise_val);
  }
  if ( get_delta(next_sub_val, sub_val) >= 5 ) {
    sub_val = next_sub_val;
    usbMIDI.sendControlChange(sub_ctrl, sub_val/8, channel);
    Serial.print("sub");
    Serial.println(sub_val);
  }

  

  if (arp1_changed){
    usbMIDI.sendControlChange(arp_ctrl, 0, channel);
    Serial.println("arp1");
    arp1_changed = false;
  } else if (arp2_changed){
    usbMIDI.sendControlChange(arp_ctrl, 127*(3.0/8.0), channel);
    Serial.println("apr2");
    arp2_changed = false;
  } else if (arp3_changed){
    usbMIDI.sendControlChange(arp_ctrl, 127.0*(5.0/8.0), channel);
    Serial.println("arp3");
    arp3_changed = false;
  } else if (arp4_changed){
    usbMIDI.sendControlChange(arp_ctrl, 127, channel);
    Serial.println("arp4");
    arp4_changed = false;
  }


  float valueX = readJoystick(joyX);
  valueX = 1 - normalize(valueX, min_stick, max_stick);
  if (abs(valueX - last_valueX) > 0.05) {
    usbMIDI.sendControlChange(90, 127*valueX, channel);
    Serial.print("joyX");
    Serial.println(127*valueX);
    last_valueX = valueX;
  }

  //Serial.println(valueX);
  
  float valueY = readJoystick(joyY);
  valueY = 1 - normalize(valueY, min_stick, max_stick);
  if (abs(valueY - last_valueY) > 0.05) {
    usbMIDI.sendControlChange(92, 127*valueY, channel);
    Serial.print("joyY");
    Serial.println(127*valueY);
    last_valueY = valueY;
  }

  //Serial.println(valueY);
  
  //Serial.println();
  delay(200);

}

float normalize (float value, int min_value, int max_value) {
  value = value >= max_value ? max_value : value;
  value = value <= min_value ? min_value : value;
  return ((value - min_value))/(max_value - min_value);
}

float readJoystick(uint8_t pin) {
  float value = smoothRead(pin, 100);
  if (value < 400)
    value = 400;
  return exp((value - 400)/200);
}

int smoothRead(uint8_t pin, int iter){
  int sum = 0;
  int num_iters = iter;
  
  for (int i = 0; i < num_iters; i++) {    
    sum += analogRead(pin);
  }
  
  return sum/num_iters;
}

float get_delta(float next_val, float cur_val){
  return abs(cur_val - next_val);
}

void arprupt1(){
  arp1_changed = true;
}
void arprupt2(){
  arp2_changed = true;
}
void arprupt3(){
  arp3_changed = true;
}
void arprupt4(){
  arp4_changed = true;
}
