#include <Wire.h>

#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;

#include <Adafruit_Sensor.h>

#include <ArduinoJson.h>

#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7
#define DHTTYPE DHT22 

float temperature = 0.0f;
float humidity = 0.0f;
float pressure = 1024.5f;


DHT dht(DHTPIN, DHTTYPE);

DS3231 Clock;

String TIME;

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

struct package
{
  float temperatureOUT ;
  float humidityOUT ;
  float pressureOUT ;
};

typedef struct package Package;
Package data;

void setup() 
{
      Serial.begin(9600);
      dht.begin();
      clock.begin();
      clock.setDateTime(__DATE__, __TIME__);
      radio.begin();
      radio.openReadingPipe(0, address);
      radio.setPALevel(RF24_PA_MIN);
      radio.startListening();
}

void loop() {
  dt = clock.getDateTime();
  readSensor();
  
  checkForWirelessData();
     
  sendINHumidityToNextion();
  sendINTemperatureToNextion();
  sendINPressureToNextion();
  sendTimeToNextion();
  delay(1000);
}


void readSensor()
{
 humidity = dht.readHumidity();
 temperature = dht.readTemperature();
 TIME = clock.dateFormat("d-m-Y H:i:s", dt);
}


void sendINHumidityToNextion()
{
  String command = "INhumidity.txt=\""+String(humidity,1)+"\"";
  Serial.print(command);
  endNextionCommand();
}
void sendINTemperatureToNextion(){
  String command = "INtemperature.txt=\""+String(temperature)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendINPressureToNextion(){
  String command ="INpressure.txt=\""+String(pressure)+"\"";
  Serial.print(command);
  endNextionCommand();
}

void sendTimeToNextion(){
  String command ="TIME.txt=\""+String(TIME)+"\"";
  Serial.print(command);
  endNextionCommand(); 
}

void sendOUTHumidityToNextionFromSlave()
{
  String command1 = "OUThumidity.txt=\""+String(data.humidityOUT,1)+"\"";
  Serial.print(command1);
  endNextionCommand();
}
void sendOUTTemperatureToNextionFromSlave(){
  String command2 = "OUTtemperature.txt=\""+String(data.temperatureOUT)+"\"";
  Serial.print(command2);
  endNextionCommand();
}
void sendOUTPressureToNextionFromSlave(){
  String command3 ="OUTpressure.txt=\""+String(data.pressureOUT)+"\"";
  Serial.print(command3);
  endNextionCommand();
}


void endNextionCommand()
{
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void checkForWirelessData()
{
    if ( radio.available()) 
  {
    while ( radio.available())
    {
      radio.read( &data, sizeof(data) );
    }
    sendOUTHumidityToNextionFromSlave();
    sendOUTTemperatureToNextionFromSlave();
    sendOUTPressureToNextionFromSlave();
  } 
}

