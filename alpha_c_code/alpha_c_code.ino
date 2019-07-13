int noise_pot = 33;
float next_noise_val = 0;
float noise_val = 0;
float noise_ctrl = 0;

int channel = 1;

void setup() {
// put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(noise_pot, INPUT);

}

void loop() {
  next_noise_val = de_sketchify(noise_pot, 200);

  if ( get_delta(next_noise_val, noise_val) >= 5 ) {
    noise_val = next_noise_val;
    usbMIDI.sendControlChange(noise_ctrl, 123*(noise_val/684), channel);
    Serial.println(noise_val);
  }


  //delay(1000);
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

