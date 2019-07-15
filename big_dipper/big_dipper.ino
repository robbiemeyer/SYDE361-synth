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

void setup() {
  Serial.begin(9600);

  pinMode(arp1, INPUT);
  pinMode(arp2, INPUT);
  pinMode(arp3, INPUT);
  pinMode(arp4, INPUT);
}

void loop() {
//  Serial.println(smoothRead(vol));
//
//  Serial.println(digitalRead(arp1));
//  Serial.println(digitalRead(arp2));
//  Serial.println(digitalRead(arp3));
//  Serial.println(digitalRead(arp4));
  Serial.println(readJoystick(joyX));
  Serial.println(readJoystick(joyY));
//  Serial.println(smoothRead(noise));
//  Serial.println(smoothRead(vol));
  Serial.println();
  
  delay(100);

}

float normalize (float value, int min_value, int max_value) {
  value = value >= max_value ? max_value : value;
  value = value <= min_value ? min_value : value;
  return ((value - min_value))/(max_value - min_value);
}

float readJoystick(uint8_t pin) {
  float value = smoothRead(pin);
  if (value < 400)
    value = 400;
  return exp((value - 400)/200);
}

int smoothRead(uint8_t pin){
  int sum = 0;
  int num_iters = 100;
  
  for (int i = 0; i < num_iters; i++) {    
    sum += analogRead(pin);
  }
  
  return sum/num_iters;
}
