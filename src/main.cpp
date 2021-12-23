#include <Arduino.h>

volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int ENA = 14; //enable
int DIR = 16; //direction
int PUL = 17; //step
bool position = false;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}
 
void setup() {
  pinMode(PUL, OUTPUT); 
  pinMode(DIR, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true); // номер таймера, prescaler value, true - прямой отсчёт
  // 80 MHz / 80 = 1 MHz
  timerAttachInterrupt(timer, &onTimer, true); // timer, function, edge/level interruption
  timerAlarmWrite(timer, 500, true); 
  timerAlarmEnable(timer);

  digitalWrite(DIR,LOW);
  digitalWrite(ENA,HIGH);
  digitalWrite(PUL,HIGH);
}

void loop() {
  
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;
    Serial.print("An interrupt: ");
    Serial.println(totalInterruptCounter);

    digitalWrite(DIR,LOW);
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL, !digitalRead(PUL) );
  }
}