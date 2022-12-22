#include<ESP8266WiFi.h>
#include<ThingSpeak.h>
#include<DHT.h>
#include "MQ135.h"

DHT dht(D3, DHT11);
//DHT objectname(pin,versi-on/DHT11);
float h, t;
int r;
char* ssid = "OPPO A5s";    //(hotspot ssid or name)
char* pswd = "ishanarola";  //(hotspot password)

WiFiClient client;

char ip[] = "184.106.153.149";   //ip address of thingspeak
long id = 1841453; //my channel id
char* api = "TYJJ6RZ1QDWD5GZP"; //my write api key

int gled = D7;  //green led connected to pin 6
int rled =D6;//red led
int wled=D5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Serial.println("Connecting to WiFi");
  pinMode (gled,OUTPUT); 
  pinMode(rled,OUTPUT);
  pinMode(wled,OUTPUT);
  WiFi.begin(ssid, pswd);
//  while(WiFi.status()!= WL_CONNECTED){
//    Serial.print(".");
//    delay(500);
//  }
  //Serial.println("WiFi connected");
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (client.connect(ip, 80)) {
    h = dht.readHumidity();
    t = dht.readTemperature();
     //Rain sensor
    r = analogRead(A0);
    r = map(r, 0, 1024, 0, 100);
    MQ135 gasSensor = MQ135(A0);
    float air_quality = gasSensor.getPPM();
    float AQI=air_quality;
    Serial.print(h);
    Serial.print(",");
    Serial.print(t);
    Serial.print(",");
    Serial.print(r);
    Serial.print(",");
    Serial.println(AQI);
    ThingSpeak.setField(1, t);
    ThingSpeak.setField(2, h);
    ThingSpeak.setField(3, r);
    ThingSpeak.setField(4,AQI );
   
    ThingSpeak.writeFields(id, api);

    if(AQI >= 200 && AQI<=5000)
  {
    digitalWrite(gled,HIGH); 
    digitalWrite(rled,LOW);
    digitalWrite(wled,LOW);
    Serial.println("AQ Level Normal");
  }
  else if (AQI > 5000 && AQI < 7000)
  {
    digitalWrite(gled,LOW);   //jump here if ppm is not greater than threshold and turn off gled
    digitalWrite(rled,LOW);
    digitalWrite(wled,HIGH);
    Serial.println("AQ Level Medium");
  }
  else
  {
    Serial.println("AQ Level Danger!");     
    digitalWrite(rled,HIGH);
    digitalWrite(gled,LOW);
    digitalWrite(wled,LOW);
    
  }

  }
  delay(1500);
}
