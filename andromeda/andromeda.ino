#define pot A9
#define clear_pot A8


const int numPins = 7;
const int controllerBase = 80;
const int channel = 1;

int value = 0;
int last_value;
int digitalPins [numPins] = {10, 11, 12, 14, 15, 16, 17};


void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numPins; i++)
    pinMode(digitalPins[i], INPUT);
}

void loop() { 
  last_value = value;
  value = getPotValue();
  
  for (int i = 0; i < numPins; i++) {
    //Serial.println(digitalRead(digitalPins[i]));
    sendControl(digitalPins[i], value, last_value);
  }
  //Serial.println();
  delay(100);


}

int getPotValue() {
  pinMode(clear_pot, OUTPUT);
  digitalWrite(clear_pot, LOW);
  pinMode(clear_pot, INPUT);
  int value = analogRead(pot)/8;
  return value <= 10 ? 0 : value;
}

void sendControl(int pin, int value, int last_value) {
  if (value > 0 && digitalRead(pin) && last_value != value)
    usbMIDI.sendControlChange(controllerBase + pin, value, channel);
}
