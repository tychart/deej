#include "MultiMap.h"
enum PotentiometerType {
  LINEAR,
  LOGARITHMIC
};

const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3};
const PotentiometerType POTENTIOMETER_TYPE = LOGARITHMIC; //LINEAR OR LOGARITHMIC

int analogSliderValues[NUM_SLIDERS];

void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    if(POTENTIOMETER_TYPE == 1) {
     analogSliderValues[i] = logarithmicToLinearValue(analogRead(analogInputs[i]));
    } else {
      analogSliderValues[i] = analogRead(analogInputs[i]);
    }
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

int inputMap[]  = {63, 259, 456, 587, 600, 623, 668, 711, 762, 816, 864, 915, 964, 1011};
int outputMap[] = {0, 51, 102, 133, 153, 205, 307, 409, 512, 614, 716, 818, 921, 1023};

int logarithmicToLinearValue(int logarithmicValue) {
  int linearValue = multiMap<int>(logarithmicValue, inputMap, outputMap, 14);
  return linearValue;
}
