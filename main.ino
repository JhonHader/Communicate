/*
                    
      /$$$$$$   /$$$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$   /$$      
     /$$__  $$ | $$_____/ | $$__  $$ |_  $$_/  /$$__  $$ | $$      
    | $$  \__/ | $$       | $$  \ $$   | $$   | $$  \ $$ | $$      
    |  $$$$$$  | $$$$$    | $$$$$$$/   | $$   | $$$$$$$$ | $$      
     \____  $$ | $$__/    | $$__  $$   | $$   | $$__  $$ | $$      
     /$$  \ $$ | $$       | $$  \ $$   | $$   | $$  | $$ | $$      
    |  $$$$$$/ | $$$$$$$$ | $$  | $$  /$$$$$$ | $$  | $$ | $$$$$$$$
     \______/  |________/ |__/  |__/ |______/ |__/  |__/ |________/
    
                     /$$$$$$   /$$$$$$    /$$$$$$  
                    |_  $$_/  /$$__  $$  /$$__  $$ 
                      | $$   |__/  \ $$ | $$  \__/ 
                      | $$     /$$$$$$/ | $$       
                      | $$    /$$____/  | $$       
                      | $$   | $$       | $$    $$ 
                     /$$$$$$ | $$$$$$$$ |  $$$$$$/ 
                    |______/ |________/  \______/  
                    
                    
    Developed by: - Jhon Hader Fernández 
                     jhon_fernandez@javeriana.edu.co
                     
                  - Laura Katherine Henao
                     laurahenao@javeriana.edu.co
                     
                  - Juan Sebastián Palacios
                     palacios_juan@javeriana.edu.co
*/

extern "C"{
  #include "communicate.h"
}
#include <Wire.h>
#define address_mem 0x54

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 115200 bits per second:
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  Wire.begin();
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {
digitalWrite(2, LOW);
digitalWrite(3, LOW);
digitalWrite(4, HIGH);

  if(Serial.available()){
    Incoming_Byte = Serial.read();
    Communicate();
   } 
}
