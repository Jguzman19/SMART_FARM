int sensorPin = 35; //the analog pin the TMP36's Vout (sense)
int val = 0; //value for storing moisture value
int soilPin = 36;//Declare a variable for the soil moisture sensor
int soilPower = 5;// Soil moisture Power

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include<Servo.h>

BlynkTimer timer;
char auth[] = "300270fd8093438fb664de47d1764762"; // tokebn from blybk app
char ssid[] = "*********";// wifi username
char pass[] = "";// wifi password

Servo waterF;

void setup()
{
  Blynk.begin(auth, ssid, pass);
  Serial.begin(115200);  //Start the serial connection with the computer
  pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor
}
//to view the result open the serial monitor


void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000); // V5, V6, V10 virtual pins
  Blynk.virtualWrite(V6, millis() / 1000);// send data every one millisecond
  Blynk.virtualWrite(V10, millis() / 1000);
}


void loop()                     // loop
{

  Blynk.run();
  timer.run();
  // the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);

  // converting that reading to voltage to temperature
  float voltage = reading * 1.08;
  voltage /= 1024.0;

  // print out the voltage
  Serial.print(voltage); Serial.println(" volts");

  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100;  //converting from 10 mv per degree wit 500 mV offset
  //to degrees ((voltage - 500mV) times 100)
  Serial.print(temperatureC); Serial.println(" degrees C");

  // now convert to Fahrenheit
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  Serial.print(temperatureF); Serial.println(" degrees F");

  delay(1000); //waiting a second

  Serial.print("Soil Moisture = ");
  //get soil moisture value from the function below and print it
  Serial.println(readSoil());


  Blynk.virtualWrite(V6, temperatureC); // write data to blynk virtual pins
  Blynk.virtualWrite(V5, temperatureF);//
  Blynk.virtualWrite(V10, readSoil());//
  delay(1000);

}

//This is a function used to get the soil moisture content
int readSoil()
{

  digitalWrite(soilPower, HIGH);//turn  "On"
  delay(10);//wait 10 milliseconds
  val = analogRead(soilPin);//Read the SIG value form sensor
  digitalWrite(soilPower, LOW);//turn  "Off"
  return val;//send current moisture value

}


