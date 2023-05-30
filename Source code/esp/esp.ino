#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>

#define CONVEYOR_MOTOR_PIN D1
#define IR_SENSOR_PIN D2
#define SERVO_PIN D4
#define LRED D5
#define LGREEN D6
#define LYELLOW D7

Servo servo;

const char* ssid = "GARCLA";
const char* password = "11111111";
IPAddress local_IP(192,168,0,105);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

void setup() {
pinMode(CONVEYOR_MOTOR_PIN, OUTPUT);
pinMode(IR_SENSOR_PIN, INPUT_PULLUP);
pinMode(LRED, OUTPUT);
pinMode(LGREEN, OUTPUT);
pinMode(LYELLOW, OUTPUT);

servo.attach(SERVO_PIN);
servo.write(90);

WiFi.softAPConfig(local_IP, gateway, subnet);
WiFi.softAP(ssid, password);

Serial.begin(57600);
//Serial.println("Connecting...");
//Serial.println("Connected");
}

void loop() {
digitalWrite(CONVEYOR_MOTOR_PIN, HIGH); // start conveyor

if (digitalRead(IR_SENSOR_PIN) == LOW) {
//Serial.println("Sensor HIGH");
digitalWrite(CONVEYOR_MOTOR_PIN, LOW); // stop conveyor
//Serial.println("Conveyor stop");
//Serial.println("Wait 2 sec");

Serial.println("Connecting...");
digitalWrite(LYELLOW, HIGH);
delay(2000);

WiFiClient client;
const int httpPort = 8080;
if (!client.connect("192.168.0.106", httpPort)) {
  Serial.println("Connection failed");
  return;
}

client.print(String("GET / HTTP/1.1\r\n") +
             "Host: 192.168.0.106\r\n" +
             "Connection: close\r\n\r\n");

while (!client.available()) {
  delay(10);
}

Serial.println("Connected");
digitalWrite(LYELLOW, LOW);

String line = "";
//tring line = client.readStringUntil('\n');
//char response = line.charAt(0);

bool quoteEncountered = false;
char response;
while (client.available()) {
    char ch = client.read();
    Serial.print(ch);
    if (ch == '\'' && !quoteEncountered) {
    quoteEncountered = true;
    } else if (quoteEncountered) {
    response = ch;
    //Serial.println(ch);
    break;
}
}

client.stop();

int unk = 0;
Serial.println("received response:");
if (response == '0') {
  Serial.println("Bio");
  digitalWrite(LGREEN, HIGH);
  servo.write(0);
} else if (response == '1') {
  Serial.println("Non-Bio");
  digitalWrite(LRED, HIGH);
  servo.write(180);
} else if (response == '2') {
  Serial.println("Empty");
  unk = 1;
} else {
  Serial.println("Unknown response:");
  Serial.println(response);
  unk = 1;
}



//Serial.println("Conveyor start (2)");
digitalWrite(CONVEYOR_MOTOR_PIN, HIGH); // start conveyor
//Serial.println("Let the object move (2 sec)");
if(unk==0){
  delay(5000);
}
digitalWrite(LGREEN, LOW);
digitalWrite(LRED, LOW);
} else {
//Serial.println("Sensor LOW");
}
}
