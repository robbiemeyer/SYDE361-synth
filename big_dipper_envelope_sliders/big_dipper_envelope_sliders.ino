////////////THINGS TO CHANGE FOR SOFTWARE INTEGRATION//////////////////////
// controllerBase variable might be conflict with other midi controller signals we are using with other inputs
// I used pin15 for my input signal - will need to use more pins / a different pin number when everything is integrated probably


const int controllerBase = 80;
const int channel = 1;
float value = 0;
float normalized_value = 0;
float final_value = 0;
float last_value;


void setup() {
  Serial.begin(9600);
  pinMode(15, INPUT);
}

void loop() {
  last_value = value;
  value = analogRead(15);
  normalized_value = normalize(value, 1.0, 270.0);
  final_value = 125*normalized_value;
  sendControl(15, final_value, last_value);
  Serial.println(final_value);
}

float normalize (float value, int min_value, int max_value) {
  value = value >= max_value ? max_value : value;
  value = value <= min_value ? min_value : value;
  return ((value - min_value))/(max_value - min_value);
}

void sendControl(int pin, int value, int last_value) {
  if (value > 0 && last_value != value)
    usbMIDI.sendControlChange(controllerBase + pin, value, channel);
}
