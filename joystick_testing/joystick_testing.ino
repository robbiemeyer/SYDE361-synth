

const float max_stick = 21.4;
const float min_stick = 1.3;

const int channel = 1;

float last_valueX = -1;
float last_valueY = -1;

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  float valueX = readJoytick(A8);
  valueX = normalize(valueX, min_stick, max_stick);
  if (abs(valueX - last_valueX) > 0.01) {
    usbMIDI.sendControlChange(90, 125*valueX, channel);
    last_valueX = valueX;
  }
  
  Serial.println(valueX);
  
  float valueY = readJoytick(A7);
  valueY = normalize(valueY, min_stick, max_stick);
  if (abs(valueY - last_valueY) > 0.01) {
    usbMIDI.sendControlChange(92, 125*valueY, channel);
    last_valueY = valueY;
  }

  Serial.println(valueY);
  
  Serial.println();
  delay(100);
}

float normalize (float value, int min_value, int max_value) {
  value = value >= max_value ? max_value : value;
  value = value <= min_value ? min_value : value;
  return ((value - min_value))/(max_value - min_value);
}

float readJoytick(uint8_t pin) {
  
  float value = smoothRead(pin);
  if (value < 400)
    value = 400;
  return exp((value - 400)/200);
}

int smoothRead(uint8_t pin){
  int sum = 0;
  int num_iters = 10;
  
  for (int i = 0; i < num_iters; i++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    pinMode(pin, INPUT);
    delay(5);
    
    sum += analogRead(pin);
    delay(1);
  }
  
  return sum/num_iters;
}
