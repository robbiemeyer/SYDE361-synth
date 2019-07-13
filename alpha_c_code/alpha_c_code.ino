int pot1 = 32;
int pot2 = 31;
int sldr = 33;
int but_g = 28;
int but_val = 0;
int but_r = 27;
int switch1 = 26; //ARPG
int switch2 = 24;

int pot_sldr1 = 23;
int pot_sldr2 = 22;

float pot1_val = 0;
float pot2_val = 0;
float sldr_val = 0;

int pot1_cont = 1;
int pot2_cont = 2;
int sldr_cont = 3;
int pot_sldr1_cont = 4;
int pot_sldr2_cont = 5;
int but_cont = 6;
int sw1_cont = 7;

int channel = 1;

int sw1_state = 0;
float pot_sldr1_val = 0;

void setup() {
// put your setup code here, to run once:
  Serial.begin(38400);
  pinMode(pot1, INPUT);
  //pinMode(pot2, INPUT);
  pinMode(but_g, INPUT);
  pinMode(but_r, INPUT);
  pinMode(switch1,INPUT);
  //pinMode(switch2, INPUT);
  //pinMode(pot_sldr1, INPUT);
  //pinMode(pot_sldr2, INPUT);
  sw1_state = digitalRead(switch1);
}

void loop() {
  float temp_pot_val = analogRead(pot_sldr1);
//  if ( temp_pot_val/8 < pot_sldr1_val -  7 || temp_pot_val/8 > pot_sldr1_val + 7){  
//    pot_sldr1_val = analogRead(pot_sldr2)/8;
//    Serial.println(strcat("slder pot 1: ", static_cast<char>(pot_sldr1_val)));
//    usbMIDI.sendControlChange(pot_sldr1_cont, pot_sldr1_val, channel);
//  }

//  if (analogRead(pot_sldr2)/8 <= 100){
//    int value = analogRead(pot_sldr2)/8;
//    Serial.println(value <= 10 ? 0 : value);
//    usbMIDI.sendControlChange(pot_sldr2_cont, value, channel);
//  }

  if ( analogRead(pot1) < pot1_val -  5  || analogRead(pot1) > pot1_val + 5){
    ///WORKS PUT POT1 CODE HERE
    pot1_val = analogRead(pot1);
    float value = ( pot1_val / 851 ) * 123;
    Serial.println(pot1_val);
    usbMIDI.sendControlChange(pot1_cont, value, channel);
  }

  if ( digitalRead(but_g) == LOW ){
    //Serial.println("but g");
    //WORKS PUT CODE FOR GREEN BUTTON HERE
    but_val += 10;
    usbMIDI.sendControlChange(but_cont, but_val, channel);
    delay(150);

  }
  if ( digitalRead(but_r) == LOW ){
    //Serial.println("but r");
    but_val -= 10;
    usbMIDI.sendControlChange(but_cont, but_val, channel);
    //WORKS PUT CODE FOR RED BUTTON HERE
    delay(150);
  }
  if (digitalRead(switch1) != sw1_state){
    sw1_state = digitalRead(switch1);
    Serial.println(digitalRead(switch1));
    usbMIDI.sendControlChange(sw1_cont, sw1_state, channel);
  }
  
  float curr_sldr = analogRead(sldr);
  if ( curr_sldr < sldr_val -  5|| curr_sldr > sldr_val + 5){
    sldr_val = curr_sldr;
    float value = ( sldr_val / 90 ) * 250;
    Serial.println(value);
    usbMIDI.sendControlChange(sldr_cont, value, channel);
  }

}
