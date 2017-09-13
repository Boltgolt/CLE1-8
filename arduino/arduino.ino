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
// Will disallow activation when above 0
int cooldown = 3;
// The last X readings from the light sendor
int photoHistory[] = {0, 0, 0, 0, 0, 0};
float temperatureHistory[] = {0, 0, 0, 0, 0, 0};

// Dynamic array lengths
int photoLength = sizeof(photoHistory) / 2;
int temperatureLength = sizeof(temperatureHistory) / 4;


void setup(void) {
  // Get the LED, motor and serial console ready
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);

  // Start listening for I2C comm
  Wire.begin(SLAVE_ADDRESS);

  // Define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Gestart");
}

void loop(void) {
  // When activated, wait 12s for the sound to play and active the motor after that
  if (activate == 1) {
    delay(12000);
    digitalWrite(motorPin, HIGH);
    delay(5000);
    digitalWrite(motorPin, LOW);

    // Reset activation and set the cooldown to 6 loops to ingore invalid data after wakeup
    activate = 0;
    cooldown = 6;
    return;
  }

  // Read raw sansor data
  int photocellReading = analogRead(photocellPin);
  int reading = analogRead(sensorPin);

  // Convert temperature voltage to celcius
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100 ;

  // Move the light sensor data points by 1 and add the last reading
  for (int i = 0; i < photoLength; i++) {
    if (i == photoLength - 1) {
      photoHistory[i] = photocellReading;
    }
    else {
      photoHistory[i] = photoHistory[i + 1];
    }
  }

  // Move the temperature sensor data points by 1 and add the last reading
  for (int i = 0; i < temperatureLength; i++) {
    if (i == temperatureLength - 1) {
      temperatureHistory[i] = temperatureC;
    }
    else {
      temperatureHistory[i] = temperatureHistory[i + 1];
    }
  }

  // If both are 0, the data hasn't been filled yet
  if (temperatureHistory[1] == 0 && photoHistory[1] == 0) {
    return;
  }

  // Get the diff of old and new temperature and light data
  int deltaPhoto = photoHistory[5] - photoHistory[0];
  float deltaTemperature = temperatureHistory[0] - temperatureHistory[5];

  // Print the last data to the console
  Serial.print("Temp = ");
  Serial.print(deltaTemperature);
  Serial.print(" - Light = ");
  Serial.print(deltaPhoto);
  Serial.print(" - Cooldown = ");
  Serial.println(cooldown);

  // If the temperature and light diff is high and we're not on cooldown, activate
  if (deltaTemperature < 2 && deltaPhoto > 15 && cooldown == 0) {
    Serial.println("VUUUUUR");
    digitalWrite(LED_BUILTIN, HIGH);
    activate = 1;
  }
  // Otherwise, make sure we're not activated
  else  {
    activate = 0;
    digitalWrite(motorPin, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Lower the cooldown by 1 if we're currently on cooldown
  if (cooldown > 0) {
    cooldown--;
  }

  delay(500);
}

// Receives data from the pi but discards it as we don't need it
// We need to keep receiveing the data because the I2C connection will close otherwise
void receiveData(int byteCount) {
  while (Wire.available()) {
    Wire.read();
  }
}

// When we're allowed to send data
void sendData() {
  // Print what we're doing
  Serial.print("I2C naar pi = ");
  Serial.println(activate);

  // Let the pi know it should play sound if we're activated
  if (activate == 1) {
    Wire.write(2);
  }
  else {
    Wire.write(1);
  }
}
