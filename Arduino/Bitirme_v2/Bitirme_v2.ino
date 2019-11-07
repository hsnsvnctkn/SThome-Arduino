#include <MFRC522.h>
//#include <ThingSpeak.h>
#include<SPI.h>
#include <Servo.h>
#include <SoftwareSerial.h>   
//#include <NeoSWSerial.h>
//#include <ArduinoJson.h>
#include <dht11.h>

#define DhtPin 7
#define LDRisik 36
#define GAZled 32
#define HARsensor 34
#define HARled 3
#define RfidRed 30
#define RfidGreen 28
#define RfidBuzzer 26
#define ParkServo 4 //6
#define NemSensor A2
#define SuMotor 45
#define sokakLamba1 31
#define sokakLamba2 33
#define klimaLed 35

#define BtnLamba 43
#define BtnSokakLamba 41
#define BtnPerde 38
#define BtnKlima 39
#define BtnOtopark 37
#define BtnHirsiz 47
#define BtnBahce 42
#define BtnReset 40
boolean bLamba,bSLamba,bPerde,bKlima,bOtopark,bHirsiz,bBahce,bReset,manuel=0;

#define RST_pin 5
#define SS_pin 53 
#define STEP1 8
#define STEP2 9
#define STEP3 10
#define STEP4 11

#define rx 12
#define tx 13
#define HARDWARE_RESET 3
#define led 36

//Zamanlamalar
long writeTimingSeconds = 17;
long readTimingSeconds = 10;
long startWriteTiming = 0;
long elapsedWriteTime = 0;
long startReadTiming = 0;
long elapsedReadTime = 0;

int spare=0;
boolean error ,perdekntrl ,otoLamba = 1 ,otoPerde = 0 ,hirsiz = 0 ,otoBahce = 1 ,lamba ,sokakLamba = 1 ,klima = 0 ,otoSokakLamba = 1;
boolean perde;
String canalPerde = "775305",canalEvKontrol="667526";
String tsip="184.106.153.149";
int hareket , isik , perdekonum , gaz , nem ,sicaklik;
byte ID[4]={122,198,144,99};
String tsId = "666730";
String agAdi="Hasan ST";//Kablonet Netmaster-B2F8-G
String agSifresi="12011201";//aeh.1453
String ip="184.106.153.149";

dht11 dhtSensor;
//SoftwareSerial Serial(12,13);// RX = 12 , TX = 13
Servo otoparkServo;
MFRC522 rfid(SS_pin,RST_pin);

void setup() {    
  //otoparkServo.write(90);
  Serial.begin(9600);
  pinMode(HARDWARE_RESET,OUTPUT);
  digitalWrite(HARDWARE_RESET, HIGH);

  //Serial.begin(9600);
  //EspHardwareReset(); //Esp Sıfırlama
  startWriteTiming = millis();
  
  perdekonum = 8;
  perdekntrl = 0;

  pinMode(LDRisik,OUTPUT);
  pinMode(GAZled,OUTPUT);
  pinMode(HARsensor,INPUT);
  pinMode(HARled,OUTPUT);
  pinMode(RfidRed,OUTPUT);
  pinMode(RfidGreen,OUTPUT);
  pinMode(RfidBuzzer,OUTPUT);
  pinMode(SuMotor,OUTPUT);
  pinMode(sokakLamba1,OUTPUT);
  pinMode(sokakLamba2,OUTPUT);
  pinMode(klimaLed,OUTPUT);

  pinMode(BtnLamba, INPUT);
  pinMode(BtnSokakLamba, INPUT);
  pinMode(BtnPerde, INPUT);
  pinMode(BtnKlima, INPUT);
  pinMode(BtnOtopark, INPUT);
  pinMode(BtnHirsiz, INPUT);
  pinMode(BtnBahce, INPUT);
  pinMode(BtnReset, INPUT);

  pinMode(STEP1,OUTPUT);
  pinMode(STEP2,OUTPUT);
  pinMode(STEP3,OUTPUT);
  pinMode(STEP4,OUTPUT);

  otoparkServo.attach(ParkServo);
  otoparkServo.write(0);
  digitalWrite(LDRisik,HIGH);
  SPI.begin();
  rfid.PCD_Init();

  digitalWrite(SuMotor,HIGH);
  veriCek();
  espBaslangic();
}

void loop() {
   readSensors();
   btnKontrol();
   //Serial.println(gaz);
   espGonder();
   bilgiGonder();
   
   

   //--------------- KONTROLLER -------------------//otoLamba=true,otoPerde=true,hirsiz=true,otoBahce=true;
   klimaCalistir();
    if(otoLamba==1 && manuel==0)
   {
    otoIsik();
   }
   else if(otoLamba==0 && manuel==0)
   {
    digitalWrite(LDRisik,LOW);
   }
   if(otoLamba==0 && lamba==1 && manuel==0)
   {
    digitalWrite(LDRisik,HIGH);
   }
   else if(otoLamba==0 && lamba==0 && manuel==0)
   {
    digitalWrite(LDRisik,LOW);
   }


   if(otoPerde==1)
   {
    moveStep();
   }
 
   if(hirsiz==1)
   {
    hirsizAlarm();
   }
   else
   {
    digitalWrite(HARled,LOW);
   }

   if(otoBahce == 1 && manuel == 0)
   {
    bahceSula();
   }
   else if(otoBahce == 0 && manuel == 0)
   {
    digitalWrite(SuMotor,HIGH);
   }
   
   //--------------- KONTROLLER -------------------//*/
    gazKontrol();
    rfidOtopark();


}
void servoCalistir()
{
  otoparkServo.write(90);
  delay(4000);
  otoparkServo.write(0);
}
void bahceSulaManuel()
{
  digitalWrite(SuMotor,LOW);
    delay(3000);
    digitalWrite(SuMotor,HIGH);
    delay(1000);
}
void espBaslangic()
{                                   
  Serial.println("AT");                                        
  while(!Serial.find("OK")){                                
    Serial.println("AT");
  }
  delay(1000);  
  Serial.println("AT+RST");                                     
  delay(5000);
  //while(!Serial.find("ready"));                             
  delay(1000);
  Serial.println("AT+CWMODE=1");                              
  while(!Serial.find("OK"));                                   
  Serial.println("AT+CWJAP=\""+agAdi+"\",\""+agSifresi+"\"");   
  while(!Serial.find("OK"));
  delay(1000);                                    
  Serial.println("AT+CIFSR\r\n");  
  delay(1000);                                
  Serial.println(espOkuma(2000));                                 
  serialTemizle(2000);                                         
  Serial.println("AT+CIPMUX=1\r\n");                             
  serialTemizle(2000);
  Serial.println("AT+CIPSERVER=1,80\r\n");    
  while(!Serial.find("OK")){                                
    Serial.println("cipserver");
  }                   
  serialTemizle(2000);
}
/*
#define BtnLamba 43
#define BtnSokakLamba 41
#define BtnPerde 38
#define BtnKlima 39
#define BtnOtopark 37
#define BtnHirsiz 47
#define BtnBahce 46
#define BtnReset 40                 boolean bLamba,bSLamba,bPerde,bKlima,bOtopark,bHirsiz,bBahce,bReset;
 * */
 // boolean error,perdekntrl,otoLamba=1,otoPerde=1,hirsiz=0,otoBahce=1,lamba,sokakLamba=1,klima=0;
void btnKontrol()
{
  bLamba = digitalRead(BtnLamba);
  bSLamba = digitalRead(BtnSokakLamba);
  bPerde = digitalRead(BtnPerde);
  bKlima = digitalRead(BtnKlima);
  bOtopark = digitalRead(BtnOtopark);
  bHirsiz = digitalRead(BtnHirsiz);
  bBahce = digitalRead(BtnBahce);
  bReset = digitalRead(BtnReset);

  if(bLamba == 1)  // -----------------------------------Lamba
  {
    manuel = 1;
    otoLamba = 0;
    if(digitalRead(LDRisik) == LOW)
    {
      digitalWrite(LDRisik,HIGH);
    }
    else
    {
      digitalWrite(LDRisik,LOW);
    }
    delay(500);
  }
  else if(bSLamba == 1)  // -------------------------Sokak Lambası
  {
    manuel = 1;
    otoSokakLamba = 0;
    if(digitalRead(sokakLamba1) == LOW)
    {
      sokakLambaYak();
    }
    else
    {
      sokakLambaSondur();
    }
    delay(500);
  }
  else if(bPerde == 1)  // ---------------------------- Perde
  {
    manuel = 1;
    otoPerde = 0;
    if(perdekntrl == 0)
    {
      perdeAc();
    }
    else
    {
      perdeKapat();
    }
    delay(500);
  }
  else if(bKlima == 1)
  {
    if(digitalRead(klimaLed) == LOW)
    {
      klima = 1;
    }
    else
    {
      klima = 0;
    }
    delay(500);
  }
  else if(bOtopark==1)
  {
    servoCalistir();
  }
  else if(bHirsiz==1)
  {
    if(hirsiz == 0)
    {
      hirsiz = 1;
    }
    else
    {
      hirsiz = 0;
    }
    delay(500);
  }
  else if(bBahce==1)
  {
    Serial.println("aa");
    otoBahce = 0;
    manuel = 1;
    bahceSulaManuel();
  }
  else if(bReset == 1)
  {
    otoBahce = 1;
    manuel = 0;
    hirsiz = 0;
    klima = 0;
    otoPerde = 0;
    otoSokakLamba = 1;
    otoLamba = 1;
    delay(500);
  }
}
void klimaCalistir()
{
  if(klima == 1)
  {
    digitalWrite(klimaLed,HIGH);
  }
  else
  {
    digitalWrite(klimaLed,LOW);
  }
}
void sokakLambaYak()
{
  digitalWrite(sokakLamba1,HIGH);
  digitalWrite(sokakLamba2,HIGH);
}
void sokakLambaSondur()
{
  digitalWrite(sokakLamba1,LOW);
  digitalWrite(sokakLamba2,LOW);
}
void bahceSula()
{
  if(nem>800)
  {
    digitalWrite(SuMotor,LOW);
    delay(3000);
    digitalWrite(SuMotor,HIGH);
    delay(1000);
  }
}
void kartNoYazdir()
{
  Serial.println("ID numarasi :");
  for(int sayac=0; sayac<4;sayac++)
  {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" , ");
  }
  Serial.println(" ");
}
void gazKontrol()
{
  

  if(gaz>400)
  {
    digitalWrite(GAZled,HIGH);
    delay(300);
    digitalWrite(GAZled,LOW);
    delay(30);
  }
  else
  {
    digitalWrite(GAZled,LOW);
  }
}
void otoIsik()
{  
   if(isik<200)
    {
      digitalWrite(LDRisik,HIGH);
      if(otoSokakLamba==1)
      {
        sokakLambaYak();
      }
    }
   else
    {
      digitalWrite(LDRisik,LOW);
      if(otoSokakLamba==1)
      {
        sokakLambaSondur();
      }
    }
}
void hirsizAlarm()
{
    digitalWrite(HARled,hareket);
}
void rfidOtopark()
{
    if( ! rfid.PICC_IsNewCardPresent())
    {
       return;      
    }
    if( ! rfid.PICC_ReadCardSerial())
    {
      return;
    }
    if( rfid.uid.uidByte[0] == ID[0] && 
    rfid.uid.uidByte[1]==ID[1] && 
    rfid.uid.uidByte[2]==ID[2] && 
    rfid.uid.uidByte[3]==ID[3])
    {
      Serial.println("Kapı AÇıldı");
      kartNoYazdir();
      digitalWrite(RfidGreen,HIGH);
      digitalWrite(RfidBuzzer,HIGH);
      delay(150);
      digitalWrite(RfidGreen,LOW);
      digitalWrite(RfidBuzzer,LOW);
      servoCalistir();
    }
    else
    {
      Serial.println("Yetkisiz Kart!");
      kartNoYazdir();
      digitalWrite(RfidRed,HIGH);
      digitalWrite(RfidBuzzer,HIGH);
      delay(100);
      digitalWrite(RfidRed,LOW);
      digitalWrite(RfidBuzzer,LOW);
      delay(100);
      digitalWrite(RfidRed,HIGH);
      digitalWrite(RfidBuzzer,HIGH);
      delay(100);
      digitalWrite(RfidRed,LOW);
      digitalWrite(RfidBuzzer,LOW);
    }
    
     rfid.PICC_HaltA();
}
void perdeAc()
{
  if(perdekntrl!=1){
    for(int i = 0; i < 3200; i++)
    {
      digitalWrite(perdekonum,HIGH);
      delay(2);
      digitalWrite(perdekonum,LOW);
      konumAzalt();
    }
    perdekntrl = 1;
    writePerde();
  }
}
void perdeKapat()
{
  if(perdekntrl!=0){
    for(int i = 0; i < 3200; i++)
    {
      digitalWrite(perdekonum,HIGH);
      delay(2);
      digitalWrite(perdekonum,LOW);
      konumArttir();
    }
    perdekntrl = 0;
    writePerde();
  }
}
void konumAzalt()
{
  perdekonum--;
  if(perdekonum == 7) perdekonum = 11;
}
void konumArttir()
{
  perdekonum++;
  if(perdekonum == 12) perdekonum = 8;
}
void moveStep()
{
    if(isik > 200 && perdekntrl == 1)
    {
      perdeKapat();
      perdekntrl = 0;
    }
   else if(isik < 200 && perdekntrl == 0)
    {
      perdeAc();
      perdekntrl = 1;
    }
}

String espOkuma(long int zamanAsimi){                        
  long int baslangic = millis();  
  String gelen;
  while(millis() - baslangic < zamanAsimi){                   
    if(Serial.available()>0){
        char c = Serial.read(); 
        gelen += c;
    } 
  }
  gelen.replace("AT+","");
  return gelen;
}
void serialTemizle(long int zamanAsimi){                
  long int baslangic = millis();
  while(millis() - baslangic < zamanAsimi){
    if(Serial.available()>0){
        Serial.read(); 
    } 
  }
}
void espGonder()
{
    if(Serial.available()){                                   
    if(Serial.find("+IPD,")){                                 
      delay(200);
      int connectionId = Serial.read() - 48;                  
      String komut;
      komut= espOkuma(1000);                           
      if(komut.indexOf("gelen=lambaetkin") != -1){                 
          otoLamba=1;
          otoPerde=1;
        }
      else if(komut.indexOf("gelen=lambapasif") != -1){            
          otoLamba=0;
          otoPerde=0;
        }

      if(komut.indexOf("gelen=hirsizetkin") != -1){               
          hirsiz=1;
        }
      else if(komut.indexOf("gelen=hirsizpasif") != -1){            
        hirsiz=0;
        }

      if(komut.indexOf("gelen=bahceetkin") != -1){               
          otoBahce=1;
        }
      else if(komut.indexOf("gelen=bahcepasif") != -1){       
          otoBahce=0;
        }

      /*if(komut.indexOf("gelen=perdeetkin") != -1){                
          otoPerde=1;
        }
      else if(komut.indexOf("gelen=perdepasif") != -1){            
          otoPerde=0;
        }*/
        if(komut.indexOf("gelen=lambaac") != -1){                 
          otoLamba=0;
          lamba=1;
        }
      else if(komut.indexOf("gelen=lambakapat") != -1){             
          otoLamba=0;
          lamba=0;
        }
        if(komut.indexOf("gelen=perdeac") != -1){             
          otoPerde=0;
          perdeAc();
        }
      else if(komut.indexOf("gelen=perdekapat") != -1){               
          otoPerde=0;
          perdeKapat();
        }
        if(komut.indexOf("gelen=sokaklambaac") != -1){             
          otoSokakLamba = 0;
          sokakLambaYak();
        }
      else if(komut.indexOf("gelen=sokaklambakapat") != -1){               
          otoSokakLamba = 0;
          sokakLambaSondur();
        }
        if(komut.indexOf("gelen=klimaac") != -1){             
          klima = 1;
        }
        
      else if(komut.indexOf("gelen=klimakapat") != -1){               
          klima = 0;
        }
        if(komut.indexOf("gelen=otopark") != -1){             
          servoCalistir();
        }
        
        String otoLambaDurum,lambaDurum,hirsizDurum,otoBahceDurum,otoPerdeDurum,perdeDurum,sokakLambaDurum,klimaDurum;
        
        if(otoLamba==1){
          otoLambaDurum="Aktif";
        }
        else{
          otoLambaDurum="Pasif";
        }
        if(digitalRead(LDRisik) == HIGH){
          lambaDurum="Acik";
        }
        else if(digitalRead(LDRisik) == LOW){ //perdekntrl=0,otoLamba=1,otoPerde=1,hirsiz=0,otoBahce=1,lamba;
          lambaDurum="Kapali";
        }
        if(otoPerde==0 && perdekntrl==1){
          perdeDurum="Acik";
        }
        else if(otoPerde==0 && perdekntrl==0){
          perdeDurum="Kapali";
        }
        if(hirsiz==1){
          hirsizDurum="Aktif";
        }
        else{
          hirsizDurum="Pasif";
        }
        if(otoBahce==1){
          otoBahceDurum="Aktif";
        }
        else{
          otoBahceDurum="Pasif";
        }
        if(otoPerde==1){
          otoPerdeDurum="Aktif";
        }
        else{
          otoPerdeDurum="Pasif";
        }
        if(digitalRead(sokakLamba1) == LOW){
          sokakLambaDurum = "Kapali";
        }
        else{
          sokakLambaDurum = "Acik";
        }
        if(klima == 1){
          klimaDurum = "Acik";
        }
        else{
          klimaDurum = "Kapali";
        }
      String sayfa = "<div align=\"center\"><b>Otomatik Perde-Lamba<br><i>"+otoLambaDurum+"</i><br>";        //<tr><td><hr size=4></td></tr>
      sayfa+="<a href=\"?gelen=lambaetkin\"><button style=\"width:70;height:30;font-size:10px\">Aktif</button></a>";
      sayfa+="<a href=\"?gelen=lambapasif\"><button style=\"width:70;height:30;font-size:10px\">Pasif</button></a><br><br>";
      
      
      sayfa+="<b>Hirsiz Alarm<br><i>"+hirsizDurum+"</i><br>";
      sayfa+="<a href=\"?gelen=hirsizetkin\"><button style=\"width:70;height:30;font-size:10px\">Aktif</button></a>";
      sayfa+="<a href=\"?gelen=hirsizpasif\"><button style=\"width:70;height:30;font-size:10px\">Pasif</button></a><br><br></b>";

      sayfa+="<b>Otomatik Bahce Sulama<br><i>"+otoBahceDurum+"</i><br>";
      sayfa+="<a href=\"?gelen=bahceetkin\"><button style=\"width:70;height:30;font-size:10px\">Aktif</button></a>";
      sayfa+="<a href=\"?gelen=bahcepasif\"><button style=\"width:70;height:30;font-size:10px\">Pasif</button></a><br><hr width=\"20%\"></b>";

      /*sayfa+="<b>Otomatik Perde<br><i>"+otoPerdeDurum+"</i><br>";
      sayfa+="<a href=\"?gelen=perdeetkin\"><button style=\"width:70;height:30;font-size:10px\">Aktif</button></a>";
      sayfa+="<a href=\"?gelen=perdepasif\"><button style=\"width:70;height:30;font-size:10px\">Pasif</button></a><br>";*/
      
      sayfa+="Perde Ac-Kapat<br><i>"+perdeDurum+"</i><br><a href=\"?gelen=perdeac\"><button style=\"width:70;height:30;font-size:10px\">Ac</button></a>";
      sayfa+="<a href=\"?gelen=perdekapat\"><button style=\"width:70;height:30;font-size:10px\">Kapat</button></a><br><br>";

      sayfa+="Klima Ac-Kapat<br><i>"+klimaDurum+"</i><br><a href=\"?gelen=klimaac\"><button style=\"width:70;height:30;font-size:10px\">Ac</button></a>";
      sayfa+="<a href=\"?gelen=klimakapat\"><button style=\"width:70;height:30;font-size:10px\">Kapat</button></a><br><br>";

      sayfa+="Lamba Ac-Kapat<br><i>"+lambaDurum+"</i><br><a href=\"?gelen=lambaac\"><button style=\"width:70;height:30;font-size:10px\">Ac</button></a>";
      sayfa+="<a href=\"?gelen=lambakapat\"><button style=\"width:70;height:30;font-size:10px\">Kapat</button></a><br><br>";
      
      sayfa+="Sokak Lambasi Ac-Kapat<br><i>"+sokakLambaDurum+"</i><br><a href=\"?gelen=sokaklambaac\"><button style=\"width:70;height:30;font-size:10px\">Ac</button></a>";
      sayfa+="<a href=\"?gelen=sokaklambakapat\"><button style=\"width:70;height:30;font-size:10px\">Kapat</button></a><br><br>";
      
      sayfa+="Otopark<br><a href=\"?gelen=otopark\"><button style=\"width:70;height:30;font-size:10px\">Ac</button></a></b></div>";
      
      
      komut = "AT+CIPSEND=";                                 
      komut += connectionId;
      komut += ",";
      komut +=sayfa.length();
      komut +="\r\n";
      Serial.print(komut);                                     
      delay(1000);
      Serial.print(sayfa);            
      delay(1000);
      komut = "AT+CIPCLOSE=";  
      komut+=connectionId;
      komut+="\r\n";
      Serial.print(komut);
    }
  }
}  
//AT+CIPSTART="TCP","184.106.153.149",80
//GET https://api.thingspeak.com/update?api_key=1BOPKMFBE9FMAP1E&field2=200
void bilgiGonder()
{
  start:
  error=0;

  elapsedWriteTime = millis()-startWriteTiming;

  if (elapsedWriteTime > (writeTimingSeconds*1000)) 
  {
    readSensors();
    writeThingSpeak();
    startWriteTiming = millis();   
  }
  
  if (error==1) 
  {       
    Serial.println(" <<<< ERROR >>>>");
    delay (2000);  
    goto start; 
  }
}

void readSensors()
{
   hareket=digitalRead(HARsensor);
   isik = analogRead(A0);
   gaz = analogRead(A1);
   nem = analogRead(NemSensor);  
   int chk = dhtSensor.read(DhtPin); // Sensör konrol ediliyor. 
   sicaklik = dhtSensor.temperature;
}
void writePerde(void)
{
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=0LYLXCV0JFYZX32O";
  getStr +="&field1=";
  getStr += String(perdekntrl);
  getStr += "\r\n\r\n";
  sendThingSpeakGetCmd(getStr);
}
void writeThingSpeak(void)
{

  startThingSpeakCmd();
  String getStr = "GET /update?api_key=1BOPKMFBE9FMAP1E";
  getStr +="&field1=";
  getStr += String(sicaklik);
  getStr +="&field2=";
  getStr += String(nem);
  getStr +="&field3=";
  getStr += String(gaz);
  getStr +="&field4=";
  if(hirsiz == 1)
  {
    getStr += String(hareket);
  }
  else
  {
    getStr += "0";
  }
  getStr += "\r\n\r\n";

  sendThingSpeakGetCmd(getStr); 
}

void EspHardwareReset(void)
{
  Serial.println("Reseting......."); 
  digitalWrite(HARDWARE_RESET, LOW); 
  delay(500);
  digitalWrite(HARDWARE_RESET, HIGH);
  delay(8000);
  Serial.println("RESET"); 
}

void startThingSpeakCmd(void)
{
  Serial.flush();
  String cmd = "AT+CIPSTART=1,\"TCP\",\"";
  cmd += "184.106.153.149"; //  api.thingspeak.com
  cmd += "\",80";
  Serial.println(cmd);
  Serial.print("Baglanti Basarili : ");
  Serial.println(cmd);

  if(Serial.find("Error"))
  {
    Serial.println("AT+CIPSTART Hatası");
    return;
  }
}

String sendThingSpeakGetCmd(String getStr)
{
  String cmd = "AT+CIPSEND=1,";
  cmd += String(getStr.length());
  Serial.println(cmd);
  Serial.print("Veri Uzunlugu : ");
  Serial.println(cmd);

  if(Serial.find(">"))
  {
    Serial.print(getStr);
    Serial.print("Veri Gönderildi : ");
    Serial.println(getStr);
    delay(500);

    String messageBody = "";
    while (Serial.available()) 
    {
      String line = Serial.readStringUntil('\n');
      if (line.length() == 1) 
      {
        messageBody = Serial.readStringUntil('\n');
      }
    }
    Serial.print("Gelen Mesaj : ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    Serial.println("AT+CIPCLOSE=1");    
    Serial.println("ESP8266 CIPSEND Hatası : Tekrar Gönderiliyor .."); //Resend...
    spare = spare + 1;
    error=1;
    return "error";
  } 
}

void veriCek()
{
  start: 
  error=0;
  
  elapsedReadTime = millis()-startReadTiming; 

  if (elapsedReadTime > (readTimingSeconds*1000)) 
  {
    int command = readThingSpeak(canalPerde,"1"); 
    if (command != 9) perdekntrl = command; 
    delay (5000);
    startReadTiming = millis();   
    /*
    int command = readThingSpeak(canalID1); 
    if (command != 9) pump = command; 
    delay (5000);
    command = readThingSpeak(canalID2); 
    if (command != 9) lamp = command; 
    takeActions();
    startReadTiming = millis();  
     */
  }
  if (error==1)
  {       
    Serial.println(" <<<< ERROR >>>>");
    delay (2000);  
    goto start; 
  }
}

int readThingSpeak(String channelID,String i)
{
  startThingSpeakCmd();
  int command;
  // preparacao da string GET
  String getStr = "GET /channels/";
  getStr += channelID;
  getStr +="/fields/"+i+"/last";
  getStr += "\r\n";

  String messageDown = sendThingSpeakGetCmd(getStr);
  if (messageDown[5] == 49)
  {
    command = messageDown[7]-48; 
    Serial.print("Gelen Veri : ");
    Serial.println(command);
  }
  else command = 9;
  return command;
}
