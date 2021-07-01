    #include <Adafruit_Sensor.h>

    #include <ArduinoJson.h>
    
    #include <DHT.h>
    #include <DHT_U.h>
    
    #define DHTPIN 7
    #define DHTTYPE DHT22 

    DHT dht(DHTPIN, DHTTYPE);
    
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


    
    
    void setup() {
      Serial.begin(9600);
      dht.begin();
      radio.begin();
      radio.openWritingPipe(address);
      radio.setPALevel(RF24_PA_MIN);
      radio.stopListening();
    }
    void loop() {
      readSensor();
      radio.write(&data, sizeof(data));
      Serial.print(data.temperatureOUT);
      Serial.print("  ");
      Serial.print(data.humidityOUT,1);
      Serial.print("  ");
      delay(5000);
    }


void readSensor()
{
 data.humidityOUT = dht.readHumidity();
 data.temperatureOUT = dht.readTemperature();
}
