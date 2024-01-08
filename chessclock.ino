
#include "pinout.h"
#include "display.h"
#include "states.h"
#include "inputs.h"

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, 0);

  initState();
  initDisplay();
  initInputs();
  initProcess();
}

void loop() {  
  while(1){
    processState();
    processInputs();
    updateDisplay();
  }
}
