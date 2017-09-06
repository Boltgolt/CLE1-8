//Pin Variables
int heatsensorPin = 0; //the analog pin the sensor pin is connected to
int lightsensorPin = 0;     // the cell and 10K pulldown are connected to a0
int lightsensorReading;     // the analog reading from the sensor divider

void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);
  
}
 
void loop(void) {
  int lightsensorReading = analogRead(lightsensorPin);  
  //Reading voltage
 int reading = analogRead(sensorPin); 
  //Converting reading to voltage
 float voltage = reading * 5.0;
 voltage /= 1024.0; 

 if (lightsensorReading > 100 ) {
  Serial.print("Analog reading = ");
  Serial.println(lightsensorReading); 
  
   //Print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting
 Serial.print(temperatureC); Serial.println(" degrees C");
   
 }
 else {
  Serial.println("Te donker");
 
   }
  delay(500);
}
