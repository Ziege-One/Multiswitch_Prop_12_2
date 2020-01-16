/*
   Futaba_Multi-Switch_Prop_12_2
   Ziege-One
   v1.0
   www.Ziege-One.de
 
 Arduino pro Mini 5v/16mHz w/ Atmega 328

 
 /////Pin Belegung////
 D0: 
 D1: 
 D2:  Einstellung Kanal8  Memory Funktion (0=an)
 D3:  Eingang von Empfänger
 D4:  
 D5:  Ausgang Kanal 6 A
 D6:  Ausgang Kanal 6 B 
 D7:  Ausgang Kanal 5 A
 D8:  Ausgang Kanal 5 B
 D9:  Ausgang Kanal 7 Servo
 D10: Ausgang Kanal 8 Servo
 D11: Ausgang Kanal 4 A (Taster)
 D12: Ausgang Kanal 4 B (Taster)
 D13: LED
 
 A0: Ausgang Kanal 3 A
 A1: Ausgang Kanal 3 B
 A2: Ausgang Kanal 2 A
 A3: Ausgang Kanal 2 B
 A4: Ausgang Kanal 1 A (Taster)
 A5: Ausgang Kanal 1 B

 /////Info /////
  Mittel Stellung 1520
  MIN Stellung    1111 unten 
  MAX Stellung    1925 oben
  Drehkanäle Urzeigersinn 1925 
 */
 
// ======== Futaba_Multi-Switch_Prop_12_2  =======================================
#include <VarSpeedServo.h>

//Pin Belegung
#define kanal1_A 8
#define kanal1_B 7
#define kanal2_A 6
#define kanal2_B 5
#define kanal3_A 11
#define kanal3_B 12
#define kanal4_A 13
#define kanal4_B A0
#define kanal5_A A1
#define kanal5_B A2
#define kanal6_A A3
#define kanal6_B A4

#define kanaele 8       //Anzahl der Kanäle
int value1[kanaele];    //Speicher 1
int value2[kanaele];    //Speicher 2
int Servo1 = 1500;
int Servo2 = 1500;
bool memory_on;         //Einstellung Kanal 1A / 4AB  Memory Funktion (1=an)
bool memory_1A;         //Flanken Merker 1A
bool memory_4A;         //Flanken Merker 4A
bool memory_4B;         //Flanken Merker 4B
bool memorylast_1A;     //Speicher für Kanal 1A
bool memorylast_4A;     //Speicher für Kanal 4A
bool memorylast_4B;     //Speicher für Kanal 4B

VarSpeedServo kanal7;
VarSpeedServo kanal8;

//#define Debug // Debuging Ein Aus

// ======== Setup & Initialisierung =======================================
void setup()
{
  #ifdef Debug
    Serial.begin(9600);
  #endif
    
  pinMode(2, INPUT); //Pin als Einstellung
  pinMode(3, INPUT); //Pin 3 als input
  
  pinMode(kanal1_A, OUTPUT);
  pinMode(kanal1_B, OUTPUT);
  pinMode(kanal2_A, OUTPUT);
  pinMode(kanal2_B, OUTPUT);
  pinMode(kanal3_A, OUTPUT);
  pinMode(kanal3_B, OUTPUT);
  pinMode(kanal4_A, OUTPUT);
  pinMode(kanal4_B, OUTPUT);
  pinMode(kanal5_A, OUTPUT);
  pinMode(kanal5_B, OUTPUT);
  pinMode(kanal6_A, OUTPUT);
  pinMode(kanal6_B, OUTPUT);        

  digitalWrite(2, HIGH);

// Servo Ausgänge einstellen
  kanal7.attach(9);
  kanal8.attach(10);
}

// ======== Haupt Schleife  =======================================
void loop()
{
  // Schleife 1 
  while(pulseIn(3, HIGH) > 1000){} //Wait for the beginning of the frame
  for(int x=0; x<=kanaele-1; x++)//Loop to store all the channel position
  {
    value1[x]=pulseIn(3, HIGH);
  }

  // Schleife 2 
  while(pulseIn(3, HIGH) > 1000){} //Wait for the beginning of the frame
  for(int y=0; y<=kanaele-1; y++)//Loop to store all the channel position
  {
    value2[y]=pulseIn(3, HIGH);
  }

  #ifdef Debug
    Serial.print(value1[0]);  //Kanal 8
    Serial.print(" ");
    Serial.print(value1[1]);  //Kanal 7
    Serial.print(" ");
    Serial.print(value1[2]);  //Kanal 6
    Serial.print(" ");
    Serial.print(value1[3]);  //Kanal 5
    Serial.print(" ");
    Serial.print(value1[4]);  //Kanal 4
    Serial.print(" ");
    Serial.print(value1[5]);  //Kanal 3
    Serial.print(" ");
    Serial.print(value1[6]);  //Kanal 2
    Serial.print(" ");
    Serial.println(value1[7]); //Kanal 1
  #endif
  
  value2[0] = (value1[0] + value2[0]) / 2;
  value2[1] = (value1[1] + value2[1]) / 2;

  Servo2 = ((Servo1 * 0) + value2[0]) / 1;
  Servo1 = ((Servo2 * 0) + value2[1]) / 1;

  kanal7.write(Servo1,30,true);  // Servo Kanal 7 30 = Speed (0-255)
  kanal8.write(Servo2,30,true);  // Servo Kanal 8 30 = Speed (0-255)
  
   
  memory_on =  not digitalRead(2);

// Kanal 6

  if ((value1[2] < 1200) && (value1[2] > 1000) && (value2[2] < 1200) && (value2[2] > 1000)) // Kanal A
    {
      digitalWrite(kanal6_A, HIGH);
      digitalWrite(kanal6_B, LOW);
    }
  else if ((value1[2] > 1800) && (value1[2] < 2000) && (value2[2] > 1800) && (value2[2] < 2000)) // Kanal B
    {
      digitalWrite(kanal6_A, LOW);
      digitalWrite(kanal6_B, HIGH);
    }
  else if ((value1[2] > 1200) && (value1[2] < 1800) && (value2[2] > 1200) && (value2[2] < 1800)) // Kanal A/B
    {
      digitalWrite(kanal6_A, LOW);
      digitalWrite(kanal6_B, LOW);
    }

// Kanal 5

  if ((value1[3] < 1200) && (value1[3] > 1000) && (value2[3] < 1200) && (value2[3] > 1000)) // Kanal A
    {
      digitalWrite(kanal5_A, HIGH);
      digitalWrite(kanal5_B, LOW);
    }
  else if ((value1[3] > 1800) && (value1[3] < 2000) && (value2[3] > 1800) && (value2[3] < 2000)) // Kanal B
    {
      digitalWrite(kanal5_A, LOW);
      digitalWrite(kanal5_B, HIGH);
    }
  else if ((value1[3] > 1200) && (value1[3] < 1800) && (value2[3] > 1200) && (value2[3] < 1800)) // Kanal A/B
    {
      digitalWrite(kanal5_A, LOW);
      digitalWrite(kanal5_B, LOW);
    }

// Kanal 4

  if ((value1[4] < 1200) && (value1[4] > 1000) && (value2[4] < 1200) && (value2[4] > 1000)) // Kanal A
    {
      if (memory_on) // keine Memory Funktion
      {
        if (not memory_4A) //Flanke von 0 nach 1
        { 
            if (memorylast_4A == LOW) //War Ausgang 8 = 0 dann Einschalten
            {
              digitalWrite(kanal4_A, HIGH);; //Ausgang schreiben
              memorylast_4A = true;
            }
            else                   //War Ausgang 8 = 1 dann Ausschalten
            {
              digitalWrite(kanal4_A, LOW); //Ausgang schreiben
              memorylast_4A = false;
            }
        } 
        memory_4A = true;  //Flanken Merker = 1
      }
      else
      {
        digitalWrite(kanal4_A, HIGH);
        digitalWrite(kanal4_B, LOW);
      }
    }
  else if ((value1[4] > 1800) && (value1[4] < 2000) && (value2[4] > 1800) && (value2[4] < 2000)) // Kanal B
    {
      if (memory_on) // keine Memory Funktion
      {
        if (not memory_4B) //Flanke von 0 nach 1
        { 
            if (memorylast_4B == LOW) //War Ausgang 8 = 0 dann Einschalten
            {
              digitalWrite(kanal4_B, HIGH);; //Ausgang schreiben
              memorylast_4B = true;
            }
            else                   //War Ausgang 8 = 1 dann Ausschalten
            {
              digitalWrite(kanal4_B, LOW); //Ausgang schreiben
              memorylast_4B = false;
            }
        } 
        memory_4B = true;  //Flanken Merker = 1  
      }
      else
      {      
      digitalWrite(kanal4_A, LOW);
      digitalWrite(kanal4_B, HIGH);
      }
    }
  else if ((value1[4] > 1200) && (value1[4] < 1800) && (value2[4] > 1200) && (value2[4] < 1800)) // Kanal A/B
    {
      if (not memory_on) // keine Memory Funktion
      {
      digitalWrite(kanal4_A, LOW);
      digitalWrite(kanal4_B, LOW);
      }
      else
      {
        memory_4A = false;  //Flanken Merker = 0
        memory_4B = false;  //Flanken Merker = 0
      }
    }

// Kanal 3

  if ((value1[5] < 1200) && (value1[5] > 1000) && (value2[5] < 1200) && (value2[5] > 1000)) // Kanal A
    {
      digitalWrite(kanal3_A, HIGH);
      digitalWrite(kanal3_B, LOW);
    }
  else if ((value1[5] > 1800) && (value1[5] < 2000) && (value2[5] > 1800) && (value2[5] < 2000)) // Kanal B
    {
      digitalWrite(kanal3_A, LOW);
      digitalWrite(kanal3_B, HIGH);
    }
  else if ((value1[5] > 1200) && (value1[5] < 1800) && (value2[5] > 1200) && (value2[5] < 1800)) // Kanal A/B
    {
      digitalWrite(kanal3_A, LOW);
      digitalWrite(kanal3_B, LOW);
    }

// Kanal 2

  if ((value1[6] < 1200) && (value1[6] > 1000) && (value2[6] < 1200) && (value2[6] > 1000)) // Kanal A
    {
      digitalWrite(kanal2_A, HIGH);
      digitalWrite(kanal2_B, LOW);
    }
  else if ((value1[6] > 1800) && (value1[6] < 2000) && (value2[6] > 1800) && (value2[6] < 2000)) // Kanal B
    {
      digitalWrite(kanal2_A, LOW);
      digitalWrite(kanal2_B, HIGH);
    }
  else if ((value1[6] > 1200) && (value1[6] < 1800) && (value2[6] > 1200) && (value2[6] < 1800)) // Kanal A/B
    {
      digitalWrite(kanal2_A, LOW);
      digitalWrite(kanal2_B, LOW);
    }

// Kanal 1

  if ((value1[7] < 1200) && (value1[7] > 1000) && (value2[7] < 1200) && (value2[7] > 1000)) // Kanal A
    {
      if (memory_on) // keine Memory Funktion
      {
        if (not memory_1A) //Flanke von 0 nach 1
        { 
            if (memorylast_1A == LOW) //War Ausgang 8 = 0 dann Einschalten
            {
              digitalWrite(kanal1_A, HIGH);; //Ausgang schreiben
              memorylast_1A = true;
            }
            else                   //War Ausgang 8 = 1 dann Ausschalten
            {
              digitalWrite(kanal1_A, LOW); //Ausgang schreiben
              memorylast_1A = false;
            }
        } 
        memory_1A = true;  //Flanken Merker = 1
      }
      else
      {
        digitalWrite(kanal1_A, HIGH);
      }
      digitalWrite(kanal1_B, LOW);
    }
  else if ((value1[7] > 1800) && (value1[7] < 2000) && (value2[7] > 1800) && (value2[7] < 2000)) // Kanal B
    {
      if (not memory_on) // keine Memory Funktion
      {
        digitalWrite(kanal1_A, LOW);
      }  
      digitalWrite(kanal1_B, HIGH);
    }
  else if ((value1[7] > 1200) && (value1[7] < 1800) && (value2[7] > 1200) && (value2[7] < 1800)) // Kanal A/B
    {
      if (not memory_on) // keine Memory Funktion
      {
        digitalWrite(kanal1_A, LOW);
      }
      else
      {
        memory_1A = false;  //Flanken Merker = 0
      }  
      digitalWrite(kanal1_B, LOW);
    }
}
