#define Buton1 43
#define Buton2 41
#define Buton3 38
#define Buton4 39
#define Buton5 37
#define Buton6 47
#define Buton7 42
#define Buton8 40

int b1,b2,b3,b4,b5,b6,b7,b8;
void setup() {
  // put your setup code here, to run once:
  pinMode(Buton1, INPUT);
  pinMode(Buton2, INPUT);
  pinMode(Buton3, INPUT);
  pinMode(Buton4, INPUT);
  pinMode(Buton5, INPUT);
  pinMode(Buton6, INPUT);
  pinMode(Buton7, INPUT);
  pinMode(Buton8, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  b1=digitalRead(Buton1);
  b2=digitalRead(Buton2);
  b3=digitalRead(Buton3);
  b4=digitalRead(Buton4);
  b5=digitalRead(Buton5);
  b6=digitalRead(Buton6);
  b7=digitalRead(Buton7);
  b8=digitalRead(Buton8);
  if(b1==1)
  {
    Serial.println("1.Buton");
  }
  else if(b2==1)
  {
    Serial.println("2.Buton");
  }
  else if(b3==1)
  {
    Serial.println("3.Buton");
  }
  else if(b4==1)
  {
    Serial.println("4.Buton");
  }
  else if(b5==1)
  {
    Serial.println("5.Buton");
  }
  else if(b6==1)
  {
    Serial.println("6.Buton");
  }
  else if(b7==1)
  {
    Serial.println("7.Buton");
  }
  else if(b8==1)
  {
    Serial.println("8.Buton");
  }
  //Serial.println(b);
}
