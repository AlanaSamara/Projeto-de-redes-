/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h> 
#define SS_PIN  21  /*Slave Select Pin*/
#define RST_PIN 22  /*Reset Pin for RC522*/
MFRC522 mfrc522(SS_PIN, RST_PIN);   /*Create MFRC522 initialized*/

const char* ssid     = "yourssid"; //Nome da rede Wifi
const char* password = "yourpasswd"; //Senha da rede Wifi

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

    SPI.begin();          /*SPI communication initialized*/
    mfrc522.PCD_Init();   /*RFID sensor initialized*/
    Serial.println("Put your card to the reader...");
    Serial.println();
    mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);

}

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">aqui</a> para ler o cartão<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
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
          /*Show UID for Card/Tag*/
          client.print("UID tag :");
          String content= "";
          byte letter;
          for (byte i = 0; i < mfrc522.uid.size; i++)
          {
            client.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            client.print(mfrc522.uid.uidByte[i], HEX);
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
            content.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          client.println();
          client.print("Message : ");
          content.toUpperCase();
          if (content.substring(1) == "02 DC B4 C3") /*UID for the Card/Tag we want to give access Replace with your card UID*/
          {
            client.println("Authorized access");  /*Print message if UID match with the database*/
            client.println();
          }              
          else   {
            Client.println(" Access denied"); /*If UID do not match print message*/
            delay(3000);
          }
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
