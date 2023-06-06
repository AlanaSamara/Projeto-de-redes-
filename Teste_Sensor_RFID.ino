#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 2 // Pino de reset do leitor RC522
#define SS_PIN 5  // Pino de seleção do leitor RC522

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Cria uma instância do leitor RC522

void setup() {
  Serial.begin(9600);   // Inicia a comunicação serial
  SPI.begin();          // Inicia a biblioteca SPI
  mfrc522.PCD_Init();   // Inicializa o leitor RC522
  Serial.println("Aproxime um cartao RFID...");
  Serial.println();
}

void loop() {
  // Verifica se há um novo cartão RFID presente
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Lê o UID do cartão RFID
    Serial.print("UID do cartao: ");
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      uid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println(uid);
    Serial.println();

    // Finaliza a leitura do cartão RFID
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}