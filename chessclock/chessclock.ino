
#include "pinout.h"
#include "display.h"
#include "states.h"
#include "inputs.h"
#include "buzzer.h"

void setup() {
  initState();
  initDisplay();
  initInputs();
  initBuzzer();
}

void loop() {  
  while(1){
    processState();
    processInputs();
    updateDisplay();
    updateBuzzer();
  }
}
