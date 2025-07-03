#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

// Sensor Pins
#define ONE_WIRE_BUS 21     // DS18B20 on GPIO21
#define PULSE_INPUT A0      // Pulse Sensor on A0 (Analog Input)
#define PULSE_BLINK 2       // Optional LED blink pin for heartbeat visualization

// Sensor Objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
PulseSensorPlayground pulseSensor;

// Variables
unsigned long previousMillis = 0;
const unsigned long interval = 2000;  // Update every 2 seconds

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial connection

  // Initialize temperature sensor
  tempSensor.begin();
  
  // Configure pulse sensor
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);  // Optional LED visual feedback
  pulseSensor.setThreshold(550);          // Adjust based on your sensor

  // Initialize pulse sensor
  if (!pulseSensor.begin()) {
    Serial.println("Pulse sensor failed to initialize!");
    while (1); // Halt if pulse sensor fails
  }

  Serial.println("Starting real-time monitoring...");
  Serial.println("----------------------------------");
  Serial.println("Time(s)\tBPM\tTemp(°C)");
  Serial.println("----------------------------------");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Read temperature
    tempSensor.requestTemperatures();
    float tempC = tempSensor.getTempCByIndex(0);
    
    // Get heart rate (this updates continuously in background)
    int bpm = pulseSensor.getBeatsPerMinute();

    // Validate and display readings
    if (!isnan(tempC) && bpm > 30) {  // Filter invalid readings
      Serial.print(currentMillis/1000);
      Serial.print("\t");
      Serial.print(bpm);
      Serial.print("\t");
      Serial.println(tempC, 1);
    } else {
      Serial.println("Waiting for valid readings...");
    }
  }
}
