int noise_pot = 33;
float next_noise_val = 0;
float noise_val = 0;
float noise_ctrl = 0;

int sub_pot = 34;
float next_sub_val = 0;
float sub_val = 0;
float sub_ctrl = 1;

int vol_pot = 35;
float next_vol_val = 0;
float vol_val = 0;
float vol_ctrl = 2;

int button_4 = 29;
int button_3 = 30;
int button_2 = 31;
int button_1 = 32;
int arp_ctrl = 3;


int channel = 1;

void setup() {
// put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(noise_pot, INPUT);
  pinMode(sub_pot, INPUT);
  pinMode(vol_pot, INPUT);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_3, INPUT);
  pinMode(button_4, INPUT);

}

void loop() {
  next_noise_val = de_sketchify(noise_pot, 200);
  next_sub_val = de_sketchify(sub_pot, 200);
  next_vol_val = de_sketchify(vol_pot, 10);

  if ( get_delta(next_noise_val, noise_val) >= 5 ) {
    noise_val = next_noise_val;
    usbMIDI.sendControlChange(noise_ctrl, 123*(noise_val/710), channel);
    Serial.println(noise_val);
  }
  if ( get_delta(next_sub_val, sub_val) >= 5 ) {
    sub_val = next_sub_val;
    usbMIDI.sendControlChange(sub_ctrl, 123*(sub_val/710), channel);
    Serial.println(sub_val);
  }

  if ( get_delta(next_vol_val, vol_val) >= 2.5){
    vol_val = next_vol_val;
    //usbMIDI.sendControlChange(vol_ctrl, 123*(vol_val/838), channel);
    usbMIDI.sendPitchBend(12300*(vol_val/838), channel);
    Serial.println(vol_val);
  }

  if (digitalRead(29) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -10, channel);
  } else if (digitalRead(30) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -20, channel);
  } else if (digitalRead(31) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -30, channel);
  } else if (digitalRead(32) == LOW){
    usbMIDI.sendControlChange(arp_ctrl, -50, channel);
  }

}

float de_sketchify(int port, int iter){
  float avg_reading = 0;
  
  for (int i = 0; i < iter; i++){
    avg_reading += analogRead(port);
  }
  avg_reading = avg_reading / iter;
  return avg_reading;

}

float get_delta(float next_val, float cur_val){
  return abs(cur_val - next_val);
}

