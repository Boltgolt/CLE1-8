//Pin Variables
int sensorPin = 0; //the analog pin the sensor pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
 
//setup
void setup()
{
  Serial.begin(9600);  //Starting serial monitor
}

//loop
void loop()
{
 //Reading voltage
 int reading = analogRead(sensorPin);  
 
 //Converting reading to voltage
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 //Print out the voltage
 Serial.print(voltage); Serial.println(" volts");
 
 //Print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting
 Serial.print(temperatureC); Serial.println(" degrees C");
 
 
 delay(1000);                                     //delay
}
