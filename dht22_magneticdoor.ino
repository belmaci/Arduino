
//Libraries
#include <DHT.h>;
#include <ArduinoJson.h>



//Constants
#define DHTPIN 7     // D7
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define MagneticDoor 4  //D4
//Variables
int newDoor,oldDoor = 0, DoorDrm = 0;
int state = 0;
int hum;  //Stores humidity value
int temp; //Stores temperature value

DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

void setup()
{
  pinMode(MagneticDoor, INPUT_PULLUP);
  dht.begin();
  Serial.begin(115200);
}

void loop()
{
  state = digitalRead(MagneticDoor);    //magnetic door sensorun degeri
  newDoor = state;                      //durumun degisecegini gozlemlemek icin kullanilan degisken
  hum = dht.readHumidity();             //nem degeri
  temp= dht.readTemperature();          //sicaklik degeri
  DynamicJsonDocument  doc(1024);       //gonderilecek json mesaji
  doc["hmd"] = hum;                     //json ile gonderilen nem degeri
  doc["tmp"] = temp;                    //json ile gonderilen sicaklik degeri
  doc["dgr"] = state;                   //json ile gonderilen kapinin son durum degeri

  /**************************************
   * Manyetik kapi sensorunun degisiminin
   * olup olmadiginin bakildigi kisim
   * degisim var ise seriden son durum
   * mesaji gonderilir.
   *************************************/
  if(newDoor != oldDoor)
  {
    DoorDrm = 1;
    oldDoor = newDoor;
  }
  else
  {
    DoorDrm = 0;
  }
  if (DoorDrm == 1)
  {
    serializeJson(doc,Serial);
    Serial.println();
  }
  /***********************************
   * Sorgu mesaji geldiginde degeri
   * 1 ise son durum mesaji seri ile 
   * gonderilir.
   **********************************/
  if (Serial.available() > 0) 
  {
    DynamicJsonDocument doc1(200);
    String incomingByte = Serial.readString();
    DeserializationError error = deserializeJson(doc1, incomingByte );
      // Test if parsing succeeds.
      if (error) 
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      int req = doc1["req"];
      if(req == 1)
      {
        serializeJson(doc,Serial);
        Serial.println();
      }
  }
}


   
