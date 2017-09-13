// Incluse I2C lib
#include <Wire.h>

// Define the I2C to listen for the pi on
#define SLAVE_ADDRESS 0x04

// The pin to listen for light data on
int photocellPin = 3;
// The pin to listen for the temperature data on
int sensorPin = 2;
// The pin to activate the motor on
int motorPin = 7;

// Whether or not we're currently crying
int activate = 0;
// The last X readings from the light sendor
int photoHistory[] = {0, 0, 0, 0, 0, 0};
float temperatureHistory[] = {0, 0, 0, 0, 0, 0};

int photoLength = sizeof(photoHistory) / 2;
int temperatureLength = sizeof(temperatureHistory) / 4;


void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(motorPin, OUTPUT); // set motorPin as output
  Serial.begin(9600); // We'll send debugging information via the Serial monitor

  Wire.begin(SLAVE_ADDRESS);

  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Gestart");
}

void loop(void) {
  if (activate == 1) {
    delay(12000);
    digitalWrite(motorPin, HIGH);
    delay(5000);
  }

  int photocellReading = analogRead(photocellPin);  //Reading light
  int reading = analogRead(sensorPin);              //Reading voltage
  float voltage = reading * 5.0;                    //Converting reading to voltage
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 ;  //converting
  for (int i = 0; i < photoLength; i++) {
    if (i == photoLength - 1) {
      photoHistory[i] = photocellReading;
    }
    else {
      photoHistory[i] = photoHistory[i + 1];
    }
  }

  for (int i = 0; i < temperatureLength; i++) {
    if (i == temperatureLength - 1) {
      temperatureHistory[i] = temperatureC;
    }
    else {
      temperatureHistory[i] = temperatureHistory[i + 1];
    }
  }

  if (temperatureHistory[1] == 0 && photoHistory[1] == 0) {
    return;
  }

  int deltaPhoto = photoHistory[5] - photoHistory[0];
  float deltaTemperature = temperatureHistory[0] - temperatureHistory[5];
  Serial.print("Temp = ");
  Serial.print(deltaTemperature);
  Serial.print(" - ");
  Serial.print("Light = ");
  Serial.println(deltaPhoto);

  if (deltaTemperature < 2 && deltaPhoto > 30) {
    Serial.println("VUUUUUR");
    digitalWrite(LED_BUILTIN, HIGH);
    activate = 1;
  }
  else  {
    activate = 0;
    digitalWrite(motorPin, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(500);
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    Wire.read();
  }
}

// callback for sending data
void sendData() {
  Serial.print("I2C van pi = ");
  Serial.println(activate);

  if (activate == 1) {
    Wire.write(2);
  }
  else {
    Wire.write(1);
  }
}
