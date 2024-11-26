const int NUM_SLIDERS = 1;
const int analogInputs[NUM_SLIDERS] = {A0};

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
     analogSliderValues[i] = analogRead(analogInputs[i]);
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

// Pretty much just linear to exponential
int logarithmicToLinear(int rawValue) {
  int minVal = 0;
  int maxVal = 1023;

  // Normalize the raw value to a range between 0 and 1
  float normalizedValue = (float)rawValue / (maxVal - minVal);

  // Calculate the exponential value to convert from linear to exponential scale
  float expValue = exp(normalizedValue);

  // Define the range of the desired linear output
  float linearRange = maxVal - minVal;

  // Scale the exponential value to the linear range
  float scaledValue = ((expValue - 1) / (exp(1) - 1)) * linearRange;

  // Round the scaled value to an integer within the desired range
  return minVal + (int)round(scaledValue);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(logarithmicToLinear(analogSliderValues[i])) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}