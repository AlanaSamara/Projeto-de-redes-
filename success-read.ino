/*
****************
Linuxhint.com
****************
Linuxhint.com
****************
*/
#include <SPI.h>
#include <MFRC522.h> 
#define SS_PIN  21  /*Slave Select Pin*/
#define RST_PIN 22  /*Reset Pin for RC522*/
#define LED_G   2   /*Pin 8 for LED*/
MFRC522 mfrc522(SS_PIN, RST_PIN);   /*Create MFRC522 initialized*/
void setup()
{
  Serial.begin(9600);   /*Serial Communication begin*/
  SPI.begin();          /*SPI communication initialized*/
  mfrc522.PCD_Init();   /*RFID sensor initialized*/
  pinMode(LED_G, OUTPUT);  /*LED Pin set as output*/
  Serial.println("Put your card to the reader...");
  Serial.println();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  
}
void loop()
{
  /*Look for the RFID Card*/
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  /*Select Card*/
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  /*Show UID for Card/Tag on serial monitor*/
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "02 DC B4 C3") /*UID for the Card/Tag we want to give access Replace with your card UID*/
  {
    Serial.println("Authorized access");  /*Print message if UID match with the database*/
    Serial.println();
    delay(3000);
    digitalWrite(LED_G, HIGH);  /*Led Turn ON*/
    delay(2500);
    digitalWrite(LED_G, LOW);
  }
 else   {
    Serial.println(" Access denied"); /*If UID do not match print message*/
    delay(3000);
  }
}