int duty_cycle = 100;
int period = 1000;

typedef enum {HIGH_LEVEL, LOW_LEVEL} PWM_STATE_T;

typedef struct {
  uint8_t   previous_value;
  int       pin_out;
  uint32_t  previous_time;
  uint32_t  time_HI, time_LO;
  PWM_STATE_T state; 
} ANALOG_WRITE_T;


ANALOG_WRITE_T led[0];

void setup() {
   MyAnalogWriteSetup(&led[0], 0, 0);
   MyAnalogWriteSetup(&led[1], 64, 1);
   MyAnalogWriteSetup(&led[2], 128, 2);
   MyAnalogWriteSetup(&led[3], 255, 3);  
}

void loop() {
  MyAnalogWrite(&led[0],0);
  MyAnalogWrite(&led[1],64);
  MyAnalogWrite(&led[2],128);
  MyAnalogWrite(&led[3],255);
}

void MyAnalogWriteSetup(ANALOG_WRITE_T *fsm, uint8_t value, int pin){
  fsm->previous_value = value;
  fsm->pin_out = pin;
  fsm->previous_time = micros();
  fsm->time_HI = ((uint32_t)value <<3);
  fsm->time_LO = 2040 - fsm ->time_HI;
  fsm->state = HIGH_LEVEL;
  pinMode(pin , HIGH);
} /* MyAnalogWriteSetup */

void MyAnalogWrite(ANALOG_WRITE_T *fsm, uint8_t value){
  switch (fsm -> state) {
    case HIGH_LEVEL:
      if(micros() - fsm->previous_time >= fsm->time_HI ){
        digitalWrite(fsm -> pin_out, LOW);
        fsm ->previous_time = micros();
        fsm->state = LOW_LEVEL;
      }
    break;

    case LOW_LEVEL:
      if(micros() - fsm->previous_time >= fsm->time_LO ){
        digitalWrite(fsm -> pin_out, HIGH);
        fsm ->previous_time = micros();
        if(value != fsm -> previous_value){
          fsm -> time_HI = ((uint32_t)value <<3);
          fsm -> time_LO = 2040 - fsm ->time_HI;
          fsm -> previous_value = value;  
        }
        fsm->state = HIGH_LEVEL;
      }
    break;
  }
}
