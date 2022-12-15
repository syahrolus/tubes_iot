#include <SoftwareSerial.h>
#include <DHT.h>       //Driver DHT
#define DHTPIN D3      //DHT Dipasang pada Pin D3
#define sensorHujan D5
#define ledKipas D6    //Deklarasi LED hijau pada pin D6 NodeMcu
#define ledMotor D7    //Deklarasi LED merah pada pin D7 NodeMcu
#define DHTTYPE DHT11  //Type DHTT11
#define buzzer D8      //Deklarasi buzzer alarm pada pin 8

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "<REDACTED>"
#define BLYNK_DEVICE_NAME "<REDACTED>"
#define BLYNK_AUTH_TOKEN "<REDACTED>"

// Comment this out to disable prints and save space
// #define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "<REDACTED>";
char pass[] = "<REDACTED>";

BlynkTimer timer;

DHT dht(DHTPIN, DHTTYPE);

// const int sensorHujan = D5;
const int sensorLDR = D0;
bool kondisiAtap = false;
int kondisiLangit = 0;

float nilaiSensorHujan = 0;
float nilaiSensorLDR = 0;
float h = 0;
float t = 0;


// This function sends Arduino's uptime every second to Virtual Pin 2.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  pinMode(ledKipas, OUTPUT);
  pinMode(ledMotor, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sensorHujan, INPUT);
  //pinMode(sensorLDR, INPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();

    
}

void sendLEDdata(int ledMotor, int ledKipas, int buzzer){
  if(ledMotor == HIGH){
    Blynk.virtualWrite(V1, 1);
  }
  else{
    Blynk.virtualWrite(V1, 0);  
  }

  if(ledKipas == HIGH){
    Blynk.virtualWrite(V2, 1);
  }
  else{
    Blynk.virtualWrite(V2, 0);
  }

  if(buzzer == HIGH){
    Blynk.virtualWrite(V0, 1);
  }
  else{
    Blynk.virtualWrite(V0, 0);
  }
}

void loop() {  
  // put your main code here, to run repeatedly:
  nilaiSensorHujan = analogRead(sensorHujan);
  nilaiSensorLDR = analogRead(A0);
  h = dht.readHumidity();
  t = dht.readTemperature();

  Serial.print("Sensor Hujan: ");
  Serial.println(nilaiSensorHujan);
  Serial.print("Sensor LDR: ");
  Serial.println(nilaiSensorLDR);
  Blynk.virtualWrite(V14, nilaiSensorLDR);
  Blynk.virtualWrite(V5, t);
  
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  } 
  else {
    Serial.print("Kelembapan: ");  //kelembaban
    Serial.print(h);
    Serial.print(" %t");
    Serial.print(" Suhu: ");
    Serial.print(t);
    Serial.println(" *C");
  }
  Serial.println("");
  
  if (nilaiSensorHujan <= 800 && nilaiSensorLDR >= 800) {  //HUJAN
    digitalWrite(ledMotor, HIGH);
    digitalWrite(buzzer, HIGH);
    digitalWrite(ledKipas, HIGH);
    kondisiAtap = false;
    kondisiLangit = 1;
    Serial.println("SEDANG HUJAN !!!!");
    Serial.println(kondisiAtap);
    Serial.println("=====================");
    Serial.println("");
    sendLEDdata(HIGH, HIGH, HIGH);
    delay(2000);
  } 
  else if (nilaiSensorHujan <= 800 && nilaiSensorLDR <= 200) {
    digitalWrite(ledMotor, HIGH);
    digitalWrite(ledKipas, LOW);
    digitalWrite(buzzer, HIGH);
    kondisiAtap = false;
    kondisiLangit = 2;
    Serial.println("SEDANG HUJAN, TAPI TERANG !!!!");
    Serial.println(kondisiAtap);
    Serial.println("=====================");
    Serial.println("");
    sendLEDdata(HIGH, LOW, HIGH);
    delay(2000);
  } 
  else if (nilaiSensorHujan >= 801 && nilaiSensorLDR > 200) {
    if (t >= 28) {
      digitalWrite(ledMotor, LOW);
      digitalWrite(ledKipas, LOW);
      digitalWrite(buzzer, LOW);
      kondisiAtap = true;
      kondisiLangit = 3;
      Serial.println("MENDUNG SUHU PANAS !!!!");
      Serial.println(kondisiAtap);
      Serial.println("=====================");
      Serial.println("");
      sendLEDdata(LOW, LOW, LOW);
      delay(2000);
      loop();
    } 
    else {
      digitalWrite(ledMotor, HIGH);
      digitalWrite(ledKipas, HIGH);
      digitalWrite(buzzer, LOW);
      kondisiAtap = false;
      kondisiLangit = 4;
      Serial.println("MENDUNG SUHU DINGIN!!!!");
      Serial.println(kondisiAtap);
      Serial.println("=====================");
      Serial.println("");
      sendLEDdata(HIGH, HIGH, LOW);
      delay(2000);
      loop();
   }
  } 
  else {
  digitalWrite(ledMotor, LOW);
  digitalWrite(ledKipas, LOW);
  digitalWrite(buzzer, LOW);

  kondisiAtap = true;
  kondisiLangit = 5;
  Serial.println("TERANG!!!!");
  Serial.println(kondisiAtap);
  Serial.println("=====================");
  Serial.println("");
  sendLEDdata(LOW, LOW, LOW);
  delay(2000);
  }


}
