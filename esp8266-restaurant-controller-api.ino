#include "SerialMessager.hpp"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// WiFi bilgilerini güncelleyin
const char *ssid = "megas";
const char *password = "megas.online";
IPAddress staticIP(192, 168, 73, 100);  // Statik IP adresini burada ayarlayın
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);



// ESP8266WebServer nesnesi oluşturun
ESP8266WebServer server(5951);
SerialMessager messager;
void setup() {
  Serial.begin(115200);

  // Statik IP ayarları
  WiFi.config(staticIP, gateway, subnet);

  // WiFi bağlantısını başlatın
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  messager = SerialMessager();
  Serial.println("Connected to WiFi");

  // HTTP POST isteği geldiğinde çağrılacak fonksiyonu belirtin
  server.on("/color_change", HTTP_POST, handleColor);
  server.on("/test", handleTest);  // get
  server.on("/call_waiter", HTTP_POST, handleWaiter);
  server.on("/attic", HTTP_POST, handleAttic);

  // Sunucuyu başlatın
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  // Sunucu taleplerini işleyin
  server.handleClient();
  handleLog();
}
void handleColor() {
  String message = server.arg("color");
  String res = messager.color + message;
  Serial.println(messager.send(res));
  server.send(200, "text/plain", "color: " + message);
}

void handleWaiter() {
  String message = server.arg("desk_id");
  String res = messager.waiter + message;
  Serial.println(messager.send(res));
  server.send(200, "text/plain", "Waiter called by: " + message);
}

void handleAttic() {
  String rfid = server.arg("desk_rfid");
  String attic = server.arg("attic");
  String automatic_attic = server.arg("automatic_attic");
  String res = messager.attic + attic + " " + automatic_attic;
  Serial.println(messager.send(res));
  server.send(200, "text/plain", "attic: " + attic + " desk: " + rfid + " automatic: " + automatic_attic);
}

String result;
void handleLog() {
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    result = messager.readLog(line);
    if (result.length() > 0)
      postLog(result);
  }
}

void postLog(String log) {
  // Create an HTTPClient object
  HTTPClient http;
  WiFiClient client;
  http.begin(client, "http://192.168.73.227:5000/log");
  // Set content type to application/json
  http.addHeader("Content-Type", "application/json");
  // Make the POST request
  int httpCode = http.POST(log);
  // Check for successful response
  if (httpCode > 0) {
    Serial.printf("HTTP response code: %d\n", httpCode);
    String payload = http.getString();
    Serial.println("Response payload: " + payload);
  } else {
    Serial.printf("HTTP request failed with error: %s\n", http.errorToString(httpCode).c_str());
  }

  // Close connection
  http.end();
}


void handleTest() {
  Serial.println("Received POST request with message: ");
  // Gelen mesaja cevap verin
  server.send(200, "text/plain", "Received your message: ");
}
