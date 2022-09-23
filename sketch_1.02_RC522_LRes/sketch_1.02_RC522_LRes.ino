#include <SPI.h>
#include <LoRa.h>
#include <Arduino_JSON.h>
#include <MFRC522.h>

//RC522
#define RST_PIN      D1  // 5//D1          
#define SS_PIN       D3  // 0//D3          

//Lora
#define ss 15
#define rst 16
#define dio0 2

//Door relay
#define Dor D4

int i = 0; //j
int LenghtInput = 63;
int LenghtData= LenghtInput-4;
int temp = 0;
int type = 0;
int truekey = 0;


//sys chek
int dataload = 0;
int cycle = 0;

JSONVar JsObject;
JSONVar JsObject1;
String jsonString;
String jsonStringmix;
String jsonString3;

//const char* cj = "J";
//const char* aj = "S";
//const char* sj = "O";
//const char* ej = "N"; //JSON

char input[63];

byte dataBlock[]    = {0, 0, 0, 0, 0, 0, 0, 0,};
 byte block;
 byte len;
 byte BlockSize;
 // Create MFRC522 instance
 MFRC522::StatusCode status;
 MFRC522::MIFARE_Key key;
 MFRC522 mfrc522(SS_PIN, RST_PIN);  

void setup() {
  Serial.begin(9600);
  //while (!Serial);

  
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1) {
      Serial.print(".");
      delay(1000);
    };
  }
  
  Serial.println("LoRa Receiver");
  pinMode(LED_BUILTIN, OUTPUT);

   //digital door relay
   pinMode(Dor, OUTPUT);
   digitalWrite(Dor, LOW);
}

void loop() {

// ###########################################
  if(Serial.available())                  // #
  {serial_input();}                       // #
// ###########################################


   switch (dataload) {
      case 0:
   
 //   switch_SPI_lora();  
    LoraUsed();
    
        break;
      case 1: 
      
  //   switch_SPI_RC522();
     mfrc522.PCD_Init();
     MFRC522doing();
        break;
   }
}

void LoraUsed(void) {  
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("\nReceived packet: ");

    // read packet
    if (LoRa.available()) {
      LoraPacketType();
    }

    switch (type) {
      case 0:
        // statements
    
        break;
      case 1:
        JsonPacket();
   //  DataRead();

     
        break;
        /*
      case 2:
      Serial.print("JSON Resived\n");
        LoraJSONDecode();
        JSONRead();
        break;
      //  default:
     //    break;
     */
    }

  }
}

void LoraPacketType(void) {
 type = 1;
 i = 0;
  while (LoRa.available()&& i <= LenghtInput) {
       input[i] = LoRa.read();
       i++;
    } 
}



void JsonPacket(void) {
JSONVar JsObjectmix = JSON.parse(input);
jsonString = JSON.stringify(JsObjectmix);

  Serial.println(input);
  JSONdemix();

 JsObject = JSON.parse(jsonString3);

if (JSON.typeof(JsObjectmix) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }
  Serial.print("JsObject decode: ");
  Serial.println(JsObject);


 dataload=1;
     }

  
// ###########################################
//serial_input()                          // #
void serial_input(void) {                 // #
    char temp = Serial.read();            // #
    if(temp == 'L' || temp == '1')        // #
      { dataload=0;  }                    // #
    else if(temp == 'R' || temp == '2')   // #
     {dataload=1;}                        // #
    else if(temp == '-' || temp == 'z')   // #
     {last_card_show();}                  // #
    else if(temp == 'O' || temp == 'k')   // #
     {key_chek();}                  // #
     
  }                                       // #
// ###########################################

void key_chek(void){
 
 
 Serial.print("\n Card UID:");
if(BlockSize==0){
  Serial.print(" *No Data*");
  }

for (byte j = 0; j < BlockSize; j++) { 
 truekey = 0;
 Serial.print("\n RC522 =  ");
 JSONVar JsObject1 = dataBlock[j];// 0;
 Serial.print(dataBlock[j]);  
 Serial.print(" LoRa =  ");
 Serial.print(JsObject[j]);  

if(JsObject1 == JsObject[j]){truekey = 1;}
  else{return;}
 Serial.print(" TK =  "); 
 Serial.print(truekey);

 
    }
    open_Door();    
  }


void JSONdemix() {

 String sedit3 = jsonString;
 char key3[50];
 String keyf3    = "0123456789KLMNOPQRSTUVWXYZ";
 String keyprot3 = "8372140965KLMNOPQRSTUVWXYZ";
 
for (int i = 0, n = jsonString.length(); i < n; i++)
    {
      key3[i] = jsonString[i];
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
 //Serial.println(jsonString3);
}


  
// ################################################
void open_Door(void) {
  digitalWrite(Dor, HIGH);
   for (int Dtime = 0; Dtime < 15; Dtime++){
  delay(1000);
  }
  digitalWrite(Dor, LOW);

}
// ################################################

// ################################################
void MFRC522doing(void) {
  
  MFRC522::MIFARE_Key key;
  for (byte j = 0; j < 6; j++) key.keyByte[j] = 0xFF;
//some variables we need
//  byte block;
//  byte len;
  MFRC522::StatusCode status;
 if ( mfrc522.PICC_IsNewCardPresent()) {
  if (  mfrc522.PICC_ReadCardSerial()) {
    card_readv2();
      }}
}

void card_readv2(void) {
    Serial.print("\n\n**Card Detected:**");
    Serial.print("\nCard UID:");
    BlockSize = 0;
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.print("\nPICC type: ");
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.print(mfrc522.PICC_GetTypeName(piccType));
    Serial.print("\n**End Reading**\n");
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  key_chek();
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    BlockSize = bufferSize;
    for (byte j = 0; j < bufferSize; j++) {  
        dataBlock[j] = buffer[j];
        Serial.print(buffer[j] < 0x10 ? " 0" : " ");
        Serial.print(buffer[j], HEX);   
    }
}
// ################################################


// #################################################
void last_card_show(void) {                     // #
Serial.print("\n Card UID:");
if(BlockSize==0){
  Serial.print(" *No Data*");
  }

for (byte j = 0; j < BlockSize; j++) { 
        Serial.print(dataBlock[j] < 0x10 ? " 0" : " ");
        Serial.print(dataBlock[j], HEX);   
    }
}                                               // #
// #################################################
