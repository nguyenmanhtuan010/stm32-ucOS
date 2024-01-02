#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

const int LED1 = 12;
const int LED2 = 13;

const char* ssid = "Tuan";
const char* password = "123456789";
const char* mqtt_server = "192.168.43.132";

WiFiClient espClient;
PubSubClient client(espClient);
SoftwareSerial mySerial(22, 23); //RX, TX

char  *temp[15];
unsigned int  Split_count = 0;

#define sub "temp1"
#define sub1 "humi1"
#define sub2 "temp2"
#define sub3 "humi2"
#define sub4 "light1"
#define sub5 "light2"

//kết nối wifi
void setup_Wifi(){
    WiFi.begin(ssid,password);
    while(WiFi.status()!= WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("đã kết nối wifi");
    Serial.println("ip address: ");
    Serial.println("WiFi.localIP()");
}
void reconnect(){
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "ESP32Client-01";
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        client.subscribe("sub");
        client.subscribe("sub1");
        client.subscribe("sub2");
        client.subscribe("sub3");
        client.subscribe("sub4");
        client.subscribe("sub5");
        client.subscribe("batden1");
        client.subscribe("batden2");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
   }
}
void callBack(char* topic, byte* payload, unsigned int lenght){
    Serial.print("tin nhan [");
    Serial.print(topic);
    Serial.print("]");
    for(int i = 0 ; i <lenght; i++){
      Serial.print((char)payload[i]);
    }
    Serial.println();
    if(strcmp(topic, "batden2")==0)
    {
      if((char)payload[0] == '1') //on
        digitalWrite(LED1,HIGH);
      else if((char)payload[0] == '0') //off
        digitalWrite(LED1, LOW);
    }
     if(strcmp(topic, "batden1")==0)
    {
      if((char)payload[0] == '1') //on
        digitalWrite(LED2,HIGH);
      else if((char)payload[0] == '0') //off
        digitalWrite(LED2, LOW);
    }
}
void setup(){
    Serial.begin(9600);
    setup_Wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callBack);
    mySerial.begin(9600);
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
 }
void loop(){
    if(!client.connected()){
      reconnect();
    }
    client.loop();
    String input = mySerial.readString();
    int inputLength = input.length() + 1;
    char charArray[inputLength];
    input.toCharArray(charArray, inputLength);
    Split_count = 0;
    temp[0] = strtok(charArray, " ");
    while (temp[Split_count] != NULL)
    {
        Split_count++;
        temp[Split_count] = strtok(NULL, " ");
    }

    Serial.println(String(temp[0])); 
    Serial.println(String(temp[1]));
    Serial.println(String(temp[2])); 
    Serial.println(String(temp[3]));
    Serial.println(String(temp[4]));
    Serial.println(String(temp[5])); 
    delay(200);
 
    client.publish(sub, temp[0]);
    client.publish(sub1, temp[1]);
    client.publish(sub2, temp[2]);
    client.publish(sub3, temp[3]);
    client.publish(sub4, temp[4]);
    client.publish(sub5, temp[5]);
}
