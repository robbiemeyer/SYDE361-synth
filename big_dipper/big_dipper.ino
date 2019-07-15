#define arp3 0
#define arp2 1
#define arp1 2
#define arp4 3
#define arpf A9
#define pitch A8
#define att A7
#define dec A6
#define sus A5
#define rel A4
#define noise A2
#define vol A1
#define sub A3
#define joyX A0
#define joyY A22

const int controllerBase = 80;
const int channel = 1;
const int numInputs = 15;

int values [numInputs] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int last_values [numInputs] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

float next_noise_val = 0;
float noise_val = 0;
int noise_ctrl = 0;

float next_sub_val = 0;
float sub_val = 0;
int sub_ctrl = 1;

float next_vol_val = 0;
float vol_val = 0;
int vol_ctrl = 2;

float next_att_val = 0;
float att_val = 0;
int att_ctrl = 3;

float next_dec_val = 0;
float dec_val = 0;
int dec_ctrl = 4;

float next_sus_val = 0;
float sus_val = 0;
int sus_ctrl = 5;

float next_rel_val = 0;
float rel_val = 0;
int rel_ctrl = 6;

int arp_ctrl = 7;

float next_arpf_val = 0;
float arpf_val = 0;
int arpf_ctrl = 8;

float next_pitch_val = 0;
float pitch_val = 0;



void setup() {
  Serial.begin(9600);

  pinMode(arp1, INPUT);
  pinMode(arp2, INPUT);
  pinMode(arp3, INPUT);
  pinMode(arp4, INPUT);
  pinMode(arpf, INPUT);
  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(noise, INPUT);
  pinMode(vol, INPUT);
}

void loop() {
  next_noise_val = smoothRead(noise, 200);
  next_sub_val = smoothRead(sub, 200);
  next_pitch_val = smoothRead(pitch, 200);
  next_vol_val = smoothRead(vol, 10);
  next_att_val = smoothRead(att, 10);
  next_dec_val = smoothRead(dec, 10);
  next_sus_val = smoothRead(sus, 10);
  next_rel_val = smoothRead(rel, 10);
  next_arpf_val = smoothRead(arpf, 1);

  if ( get_delta(next_pitch_val, pitch_val) >= 5 ) {
    pitch_val = next_pitch_val;
    Serial.println(pitch_val);
  }
  if( get_delta(next_arpf_val, arpf_val) >= 2 ) {
    arpf_val = next_arpf_val;
    Serial.println(arpf_val);
  }

  if ( get_delta(next_att_val, att_val ) >= 2 ) {
    att_val = next_att_val;
    Serial.println(att_val);
  }
  if ( get_delta(next_dec_val, dec_val ) >= 2 ) {
    dec_val = next_dec_val;
    Serial.println(dec_val);
  }
  if ( get_delta(next_sus_val, sus_val ) >= 2 ) {
    sus_val = next_sus_val;
    Serial.println(sus_val);
  }
  if ( get_delta(next_rel_val, rel_val ) >= 2 ) {
    rel_val = next_rel_val;
    Serial.println(rel_val);
  }

  
  if ( get_delta(next_noise_val, noise_val) >= 5 ) {
    noise_val = next_noise_val;
    usbMIDI.sendControlChange(noise_ctrl, 123*(noise_val/710), channel);
    Serial.print("noise");
    Serial.println(noise_val);
  }
  if ( get_delta(next_sub_val, sub_val) >= 5 ) {
    sub_val = next_sub_val;
    usbMIDI.sendControlChange(sub_ctrl, 123*(sub_val/710), channel);
    Serial.print("sub");
    Serial.println(sub_val);
  }

  if ( get_delta(next_vol_val, vol_val) >= 2.5){
    vol_val = next_vol_val;
    //usbMIDI.sendControlChange(vol_ctrl, 123*(vol_val/838), channel);
    usbMIDI.sendPitchBend(12300*(vol_val/838), channel);
    Serial.print("vol");
    Serial.println(vol_val);
  }

  if (digitalRead(arp1) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -10, channel);
    Serial.println("39");
  } else if (digitalRead(arp2) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -20, channel);
    Serial.println("30");
  } else if (digitalRead(arp3) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -30, channel);
    Serial.println("31");
  } else if (digitalRead(arp4) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -50, channel);
    Serial.println("32");
  }

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
