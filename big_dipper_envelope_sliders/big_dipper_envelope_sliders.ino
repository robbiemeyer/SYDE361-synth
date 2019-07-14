////////////THINGS TO CHANGE FOR SOFTWARE INTEGRATION//////////////////////
// controllerBase variable might be conflict with other midi controller signals we are using with other inputs
// I used pin15 for my input signal - will need to use more pins / a different pin number when everything is integrated probably


const int controllerBase = 80;
const int channel = 1;
int value = 0;
int final_value = 0;
float last_value;


void setup() {
  Serial.begin(9600);
  pinMode(15, INPUT);
}

void loop() {

  value = analogRead(15) / 8;
  sendControl(15, value);

  delay(100);
}

void sendControl(int pin, int value) {
  if (last_value != value) {
    usbMIDI.sendControlChange(controllerBase + pin, value, channel);
    Serial.println(value);
    Serial.println();
    last_value = value;
  }
}
