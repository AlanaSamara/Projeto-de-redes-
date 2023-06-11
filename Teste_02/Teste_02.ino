
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>  //https://github.com/bbx10/WebServer_tng


WebServer server ( 80 );

const char* ssid     = "wifi";
const char* password = "senha";


void setup()
{

  Serial.begin(9600);

  connectToWifi();

  beginServer();


}

void loop() {

  server.handleClient();

  delay(1000);

}

void connectToWifi()
{
  WiFi.enableSTA(true);

  delay(2000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void handleRoot() {
  if ( server.hasArg("NOME")) {
    handleSubmit();
  } else {
    server.send ( 200, "text/html", getPage() );
  }
}

void handleSubmit() {

  nomeAluno = server.arg("NOME");


  server.send ( 200, "text/html", getPage() );

 
}

String getPage() {
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  page += "<title>ESP32 WebServer RFID</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }</style>";
  page += "</head><body><h1>ESP32 WebServer RFID tests</h1>";
  page += "<h3>Cadastrar Aluno:</h3>";
  page += "<ul><li>Aluno Cadastrado: ";
  page += nomeAluno;

  page += "<h3>CADASTRO</h3>";
  page += "<form action='/' method='POST'>";
 
  page += "";
  page += "<label for='NOMEA'>Nome:</label>";
  page += "<INPUT type='text' name='NOME' id='NOMEA' value='' placehoulder='Nome do aluno'>";

  page += "<INPUT type='submit' value='Submit'>";

  page += "</form>";
  page += "</body></html>";
  return page;
}
