#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER "https://discord.com/api/webhooks/955519218823098489/sQQWWHmfz7-h8-g3DF1tsMi1FoBEitLiyfXl5gtzcpzuC7QP-xKLCabJ6nzmlQgjqPVV"//discord Webhook adresse
#define FINGERPRINT "7F 8C 3C 11 CB B1 87 28 AA 1B BF 7B E0 D0 49 64 A6 D3 DA 3A"
#ifndef STASSID
#define STASSID "Nida"                                                             //SSID des WLANs
#define STAPSK  "Di1WSP!!"                                                          //Passwort des WLANs
String message = "||@everyone|| :rotating_light: !!!ALARM!!! :rotating_light:  ||@everyone||";  //Nachricht
const int trigPin = D6;
const int echoPin = D7;

float duration, distance, normdistance;
#endif
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  Serial.print("Connecting");

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  while (normdistance < 10) {             //Herrausfinden der kalibrierungsentfernung
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    normdistance = (duration * .0343) / 2;
    delay(100);
  }
  Serial.print("STARTDISTANCE: ");
  Serial.println(normdistance);
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClientSecure client;
    HTTPClient http;
    client.setInsecure();
    //client.setFingerprint(FINGERPRINT);

    Serial.print("[HTTP] begin...\n");
    http.begin(client, SERVER); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] Initializing...\n");
    const String& payload = "{\"content\":\"```\\nAlarm initialized!\\nNormal distance: " + String(normdistance) + "cm\\n```\",\"tts\":\"true\"}"; //Nachricht zum anschalten
    int httpCode = http.POST(payload);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.println("[HTTP] Initialized!");
      const String& payload = http.getString();
      if (payload != "") {
        Serial.println("[HTTP] payload:  ");
        Serial.println(payload);
      }

    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;  //ermitteln der aktuellen entfernung

  if (distance < normdistance * 0.8) {  //bei 80% des Anfangsabstandes geht es los
    if ((WiFi.status() == WL_CONNECTED)) {

      WiFiClientSecure client;
      HTTPClient http;
      client.setInsecure();
      //client.setFingerprint(FINGERPRINT);

      Serial.print("[HTTP] begin...\n");
      http.begin(client, SERVER); //HTTP
      http.addHeader("Content-Type", "application/json");

      Serial.print("[HTTP] Posting...\n");
      //const String& payload = "{\"content\":\"||@nobody|| :rotating_light: !!!ALARM!!! :rotating_light:  ||@nobody||\",\"tts\":\"true\"}";
      const String& payload = "{\"content\":\"" + message + "\",\"tts\":\"false\"}";
      int httpCode = http.POST(payload);

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.println("[HTTP] Postet content:" + message);
        Serial.printf("[HTTP] Posted code: %d\n", httpCode);
        Serial.print("[HTTP] HTTP_CODE_OK: ");
        Serial.println(HTTP_CODE_OK);
        const String& payload = http.getString();
        if (payload != "") {
          Serial.println("[HTTP] payload:  ");  //falls es eine rückmeldung gibt(ERROR) wird es angezeigt
          Serial.println(payload);
        }

      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      setup();
    }
    Serial.println("Distance: "+String(distance));
    delay(0);
    Serial.println("Timeout over!");
  }
  delay(100);
}
