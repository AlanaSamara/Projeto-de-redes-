#include <Arduino.h>
#define LED_PIN 2  // Pin number for the ESP32 built-in LED
void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Por favor aproxime seu cartao no sensor: "); // Prompt for UID input
}
void loop()
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n'); // Read the input from the terminal until a newline character is encountered

    if (input.length() > 0)
    {
      Serial.print("UID tag: ");
      Serial.println(input);

      input.replace(" ", ""); // Remove any spaces from the input string
      input.toUpperCase();

      if (input.length() == 8) // Check if the input string has the correct length
      {
        // Convert the input string to bytes
        byte uidBytes[4];
        for (byte i = 0; i < 4; i++)
        {
          String byteStr = input.substring(i * 2, i * 2 + 2);
          uidBytes[i] = strtol(byteStr.c_str(), nullptr, 16);
        }

        // Compare the UID bytes with the authorized UID
        if (uidBytes[0] == 0x70 && uidBytes[1] == 0xEA && uidBytes[2] == 0xAE && uidBytes[3] == 0x1B)
        {
          delay(3000);
          Serial.println("Identificado: Direção da escola. \nAcesso mestre autorizado");
          // Feedback do LED acesso autorizado
          digitalWrite(LED_PIN, HIGH);
          delay(1000);
          digitalWrite(LED_PIN, LOW);

          Serial.println("Escolha uma opção: \n1. Cadastrar nova credencial \n2. Editar credenciais \n3. Acessar relatorios \n4. Entrar em contato com pais");
        }
        else if (uidBytes[0] == 0xB3 && uidBytes[1] == 0xFD && uidBytes[2] == 0xCD && uidBytes[3] == 0x9A)
        {
          delay(3000);
          Serial.println("Identificado: Ariane Simões (aluna). \nAcesso autorizado");
          // Feedback do LED acesso autorizado
          digitalWrite(LED_PIN, HIGH);
          delay(1000);
          digitalWrite(LED_PIN, LOW);
        }
      }
      else
      {
        delay(3000);
        Serial.println("Acesso negado.");
        // Feedback LED acesso negado
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
      }

      Serial.println("Por favor aproxime seu cartao no sensor: "); // Prompt for the next UID input
    }
  }
}