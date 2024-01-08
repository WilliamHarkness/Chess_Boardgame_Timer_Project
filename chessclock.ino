
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

//  Serial.begin(115200);
//  Serial.println("Starting");

//  setDisplay(LEFT_CHAR_0, DISPLAY_DASH);
//  Serial.println("Set DASH");
}

void loop() {  
  while(1){
    processState();
    processInputs();
    updateDisplay();
  }
}
