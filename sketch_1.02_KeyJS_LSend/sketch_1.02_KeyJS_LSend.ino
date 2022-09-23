#include <SPI.h>
#include <LoRa.h>
#include <Arduino_JSON.h>

#define ss 15
#define rst 16
#define dio0 2

int counter = 0;
String jsonString;
String jsonString2;

int i, n;
JSONVar JSArray;

//keys__
 byte DataKey1[]    = {0, 0, 0, 0};
int key1size = 4;


char key[26];


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);

  JSONCreation();
  
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1){
       Serial.print(".");
       delay(1000);
      };
  }
}

void loop() {
  Serial.print("JSON packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  //LoRa.print("JSON");
  LoRa.print(jsonString2);
  LoRa.endPacket();
  counter++;
  delay(5000);
}


void JSONCreation() {
  Serial.println("========");


DataKey1[0] = 0xFE;
DataKey1[1] = 0x96;
DataKey1[2] = 0x0E;
DataKey1[3] = 0xA9;

for (byte j = 0; j < key1size; j++) 
  {  
    JSArray[j] = DataKey1[j];
  }

  // JSArray.length();

  // JSON.stringify(myVar) can be used to convert the json var to a String
  jsonString = JSON.stringify(JSArray);
  Serial.println(jsonString);

JSONmix();

}

void JSONmix() {

 String sedit = jsonString;
 char key1[50];
 String key     = "8372140965KLMNOPQRSTUVWXYZ";
 String keyprot = "0123456789KLMNOPQRSTUVWXYZ";//"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
 
for (int i = 0, n = jsonString.length(); i < n; i++)
    {
      key1[i] = jsonString[i];
      //Serial.println(jsonString[i]);

        for (int j = 0; j < 26; j++)
        {
            if (key1[i] == keyprot[j])
            {
                sedit [i] = key[j];
            }
        }   
    }
  jsonString2 = sedit;
     Serial.println(jsonString2);
     //JSONdemix();
}


/*
void JSONdemix() {

 String sedit3 = jsonString2;
 char key3[50];
 String keyf3    = "0123456789KLMNOPQRSTUVWXYZ";//"0123456789KLMNOPQRSTUVWXYZ"
 String keyprot3 = "8372140965KLMNOPQRSTUVWXYZ";//"ABCDEFGHIJKLMNOPQRSTUVWXYZ""8302147965KLMNOPQRSTUVWXYZ"
 
for (int i = 0, n = jsonString2.length(); i < n; i++)
    {
      key3[i] = jsonString2[i];
      //Serial.println(jsonString[i]);

        for (int j = 0; j < 26; j++)
        {
            if (key3[i] == keyprot3[j])
            {
                sedit3 [i] = keyf3[j];
            }
        }   
    }
  jsonString3 = sedit3;
     Serial.println(jsonString3);
}
*/
