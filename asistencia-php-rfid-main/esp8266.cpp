#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D3
#define SS_PIN D4

MFRC522 reader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;


const char *ssid = "Loreto";
const char *password = "manuelmoya0120";
const String SERVER_ADDRESS = "http://192.168.0.11/asistencia-php-rfid";
void setup()
{

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  SPI.begin();

  reader.PCD_Init();
  
  delay(4);

  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF; 
  }
}

void loop()
{
  
  if (WiFi.status() != WL_CONNECTED)
  {
    return;
  }
  

 
  if (!reader.PICC_IsNewCardPresent())
  {
    return;
  }

  
  if (!reader.PICC_ReadCardSerial())
  {
    return;
  }


  String serial = "";
  for (int x = 0; x < reader.uid.size; x++)
  {
  
    if (reader.uid.uidByte[x] < 0x10)
    {
      serial += "0";
    }
    
    serial += String(reader.uid.uidByte[x], HEX);
  
    if (x + 1 != reader.uid.size)
    {
      serial += "-";
    }
  }

  serial.toUpperCase();

  
  reader.PICC_HaltA();
 
  reader.PCD_StopCrypto1();

  HTTPClient http;

  
  const String full_url = SERVER_ADDRESS + "/rfid_register.php?serial=" + serial;
  http.begin(full_url);

 
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {

      
    }
    else
    {
    }
  }
  else
  {
  }

  http.end(); 
}
