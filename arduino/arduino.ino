int photocellPin = 0;     // the analog pin the light sensor pin is connected to
int photocellReading;     // the analog reading from the sensor divider
int sensorPin = 2;        // the analog pin the heat sensor pin is connected to

int photoHistory[] = {0, 0, 0, 0, 0, 0};
float temperatureHistory[] = {0, 0, 0, 0, 0, 0};

int photoLength = sizeof(photoHistory) / 2;
int temperatureLength = sizeof(temperatureHistory) / 4;

void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
     
}
 
void loop(void) {
  int photocellReading = analogRead(photocellPin);  //Reading light
  int reading = analogRead(sensorPin);              //Reading voltage
  float voltage = reading * 5.0;                    //Converting reading to voltage
  voltage /= 1024.0; 
  float temperatureC = (voltage - 0.5) * 100 ;  //converting

  for (int i=0; i < photoLength; i++) {
      if (i == photoLength - 1) {
        photoHistory[i] = photocellReading;
      }
      else {
        photoHistory[i] = photoHistory[i+1];
      }
  }

  for (int i=0; i < temperatureLength; i++) {
    if (i == temperatureLength - 1) {
      temperatureHistory[i] = temperatureC;
    }
    else {
      temperatureHistory[i] = temperatureHistory[i+1];
    }
  }

  if (temperatureHistory[1] == 0 && photoHistory[1] == 0) {
    return;
  }
  
  int deltaPhoto = photoHistory[5] - photoHistory[0];
  float deltaTemperature = temperatureHistory[0] - temperatureHistory[5];
  
    Serial.print(deltaTemperature);
  Serial.print(" - ");
  
  Serial.println(deltaPhoto);

  if (deltaTemperature > 2 && deltaPhoto > 30) {
     Serial.println("VUUUUUR");
  }
  

 
// if (photocellReading > 100 ) {
//  Serial.print("Analog reading = ");
//  Serial.println(photocellReading);                   //Print out the light reading
// // Serial.print(voltage); Serial.println(" volts");  //Print out the voltage reading
//   
//  Serial.print(temperatureC); Serial.println(" degrees C");
// 
// }
// else {
//  //Serial.println("Te donker");
//  float temperatureC = (voltage - 0.5) * 100 ;  //converting
//  Serial.print(temperatureC); Serial.println(" degrees C");
//  Serial.print("Analog reading = ");  
//  Serial.println(photocellReading);                 //Print out the light reading
// 
//   }
  delay(500);
}
