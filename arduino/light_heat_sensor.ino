int photocellPin = 0;     // the analog pin the light sensor pin is connected to
int photocellReading;     // the analog reading from the sensor divider
int sensorPin = 2;        // the analog pin the heat sensor pin is connected to
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
     
}
 
void loop(void) {
  int photocellReading = analogRead(photocellPin);  //Reading light
  int reading = analogRead(sensorPin);              //Reading voltage
  float voltage = reading * 5.0;                    //Converting reading to voltage
 voltage /= 1024.0; 

 
 if (photocellReading > 100 ) {
  Serial.print("Analog reading = ");
  Serial.println(photocellReading);                   //Print out the light reading
 // Serial.print(voltage); Serial.println(" volts");  //Print out the voltage reading
   
  float temperatureC = (voltage - 0.5) * 100 ;  //converting
  Serial.print(temperatureC); Serial.println(" degrees C");
 
 }
 else {
  //Serial.println("Te donker");
  float temperatureC = (voltage - 0.5) * 100 ;  //converting
  Serial.print(temperatureC); Serial.println(" degrees C");
  Serial.print("Analog reading = ");  
  Serial.println(photocellReading);                 //Print out the light reading
 
   }
  delay(1000);
}
