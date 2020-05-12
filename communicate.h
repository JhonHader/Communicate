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

#ifndef COMMUNICATE_H
#define COMMUNICATE_H "communicate.h"
#include <Wire.h>
#define address_mem 0x54
int Incoming_Byte = 0; 

/*----------------------------------------------------------------------------*/

void Communicate();
void ack_pooling();
int Write_Data_Memory(uint8_t , uint8_t , uint8_t, uint8_t);
int Read_Data_Memory(uint8_t , uint8_t);
uint8_t CRC(uint8_t *, uint8_t);

/*----------------------------------------------------------------------------*/

void Communicate(){

static int Data_Read = 0;
static int8_t index = 0;
static int8_t flag_Write = 0;
static int8_t flag_Read = 0;
static uint8_t data_temp = 0;
static int8_t My_Data[6];
static int8_t My_Address[4];
static int8_t Add_LSB = 0, Add_MSB = 0;
static int8_t Data_LSB = 0, Data_MSB = 0;
static int8_t length_CRC = 0;
static int8_t *ptr_CRC = 0;

typedef enum { 
    INIT_ST,        /* state 0 */
    COMMAND_ST,     /* state 1 */
    WRITE_ALL_ST,   /* state 2 */
    READ_ALL_ST,    /* state 3 */
    READ_ONE_ST,    /* state 4 */
    CRC_ST,         /* state 5 */
} STATE_T;

typedef struct {
    char INIT_WORD;
    char WR_ALL_COM;
    char RE_ALL_COM;
    char RE_ONE_COM;
} WORDS;

static WORDS command = { 
   .INIT_WORD = '#', 
   .WR_ALL_COM = '1',
   .RE_ALL_COM = '2',
   .RE_ONE_COM = '3'
};

static STATE_T state = INIT_ST;

if( 1 ){
    switch( state ){
        case INIT_ST:
            if( Incoming_Byte == command.INIT_WORD ){
                My_Data[0] = Incoming_Byte;
                My_Address[0] = Incoming_Byte; 
                Serial.write(Incoming_Byte);
                state = COMMAND_ST;
            } else {
                state = INIT_ST;
            }
        break;
/*----------------------------------------------------------------------------*/
        case COMMAND_ST:
        
/*                  =======================================                   */
/*                  | Command |         Function          |                   */
/*                  =======================================                   */
/*                  |   '1'   | Write 1000 data in memory |                   */
/*                  =======================================                   */
/*                  |   '2'   | Read 1000 data in memory  |                   */
/*                  =======================================                   */
/*                  |   '3'   |  Read one data in memory  |                   */
/*                  =======================================                   */

                        
            if( Incoming_Byte == command.WR_ALL_COM ){          
                My_Data[1] = Incoming_Byte;
                Serial.write(Incoming_Byte);
                state = WRITE_ALL_ST;
            } 
            else if( Incoming_Byte == command.RE_ALL_COM ){
                My_Address[1] = Incoming_Byte;
                Serial.write(Incoming_Byte);
                state = READ_ALL_ST;
            }
            else if( Incoming_Byte == command.RE_ONE_COM ){
                My_Address[1] = Incoming_Byte;
                Serial.write(Incoming_Byte);
                state = READ_ONE_ST;
            } else {
                state = INIT_ST;
            }
        break;
/*----------------------------------------------------------------------------*/
        case WRITE_ALL_ST:
        
            /* Read input data from serial port: There are 4 words */
            
/*  =======================================================================   */
/*  | '#' | '1' | Addres(MSB) | Addres(LSB) | Data(MSB) | Data(LSB) | CRC |   */
/*  =======================================================================   */         
            
            data_temp = Incoming_Byte;
            if( index == 0 ){
                Add_MSB = Incoming_Byte;
                My_Data[2] = Add_MSB;
                Serial.write(Incoming_Byte);
                index++;
            }
            else if( index == 1 ){
                Add_LSB = Incoming_Byte;
                My_Data[3] = Add_LSB;
                Serial.write(Incoming_Byte);
                index++;
            }
            else if( index == 2 ){
                Data_MSB = Incoming_Byte;
                My_Data[4] = Data_MSB;
                Serial.write(Incoming_Byte);
                index++;
            } else {
                Data_LSB = Incoming_Byte;
                My_Data[5] = Data_LSB;
                Serial.write(Incoming_Byte);
                index = 0;
                flag_Write = 1;
            } /* if */
          
            /* Write data in 24LC256 memory */
            if( flag_Write == 1 ){
                ptr_CRC = &My_Data[0];
                length_CRC = (sizeof(My_Data)/sizeof(My_Data[0]));
                index = 0;
                flag_Write = 0;
                state = CRC_ST;
            }
        break;
/*----------------------------------------------------------------------------*/
        case READ_ALL_ST:
        
            /* Read input data from serial port: There are 2 words */           
            
/*             ===============================================                */
/*             | '#' | '2' | Addres(MSB) | Addres(LSB) | CRC |                */
/*             ===============================================                */         

           if( index == 0 ){
                Add_MSB = Incoming_Byte;
                My_Address[2] = Add_MSB;
                Serial.write(Incoming_Byte);
                index++;
            } else {
                Add_LSB = Incoming_Byte;
                My_Address[3] = Add_LSB;
                Serial.write(Incoming_Byte);
                index = 0;
                flag_Read = 1;
            }
          
            /* Read data in 24LC256 memory */
            if( flag_Read == 1 ){ 
                ptr_CRC = &My_Address[0];
                length_CRC = (sizeof(My_Address)/sizeof(My_Address[0]));
                flag_Read = 0;
                state = CRC_ST;
            }
            break;
/*----------------------------------------------------------------------------*/
        case READ_ONE_ST:
        
            /* Read input data from serial port: There are 2 words */
            
/*             ===============================================                */
/*             | '#' | '3' | Addres(MSB) | Addres(LSB) | CRC |                */
/*             ===============================================                */         

            if( index == 0 ){
                Add_MSB = Incoming_Byte;
                My_Address[2] = Add_MSB;
                Serial.write(Incoming_Byte);
                index++;
            } else {
                Add_LSB = Incoming_Byte;
                My_Address[3] = Add_LSB;
                Serial.write(Incoming_Byte);
                index = 0;
                flag_Read = 1;
            }
          
            /* Read data in 24LC256 memory */
            if( flag_Read == 1 ){ 
                ptr_CRC = &My_Address[0];
                length_CRC = (sizeof(My_Address)/sizeof(My_Address[0]));
                flag_Read = 0;
                state = CRC_ST;
            }
        break;
/*----------------------------------------------------------------------------*/
        case CRC_ST:
            data_temp = CRC(ptr_CRC, length_CRC);
            Serial.write(Incoming_Byte);
            if ( Incoming_Byte == data_temp ){
              if ( My_Data[1] == '1' ) {
                Write_Data_Memory(My_Data[2], My_Data[3], My_Data[4], My_Data[5]);
              }
              if ( My_Address[1] == '2' ){
                Data_Read = Read_Data_Memory(My_Address[2], My_Address[3]);
              }
              if ( My_Address[1] == '3' ){
                Data_Read = Read_Data_Memory(My_Address[2], My_Address[3]);      
              }               
            } else {
                Serial.println("\nError 404 not found: Transmission error");
            }
            My_Data[1]=0;
            My_Address[1]=0;
            state = INIT_ST;
        break;
        
    } /* switch */
} /* if */
} /* Communicate */

/*----------------------------------------------------------------------------*/
int Write_Data_Memory(uint8_t Add_MSB, uint8_t Add_LSB, uint8_t Data_MSB, uint8_t Data_LSB){

  uint16_t Add16_MSB = 0;
  uint16_t Add16_LSB = 0;
  Add16_MSB = ((Add_MSB << 8) | Add_LSB)*2;
  Add16_LSB = Add16_MSB + 1;
  ack_pooling();
  Wire.beginTransmission(address_mem);
  Wire.write((byte)(Add16_MSB >> 8));
  Wire.write((byte)(Add16_MSB & 0xFF));
  Wire.write(Data_MSB);       //Write 
  Wire.endTransmission();
  ack_pooling();
  Wire.beginTransmission(address_mem);
  Wire.write((byte)(Add16_LSB >> 8));
  Wire.write((byte)(Add16_LSB & 0xFF));
  Wire.write(Data_LSB);       //Write 
  Wire.endTransmission();
}

/*----------------------------------------------------------------------------*/
int Read_Data_Memory(uint8_t Add_MSB, uint8_t Add_LSB){

  byte c;
  uint16_t Add16_MSB = 0;
  uint16_t Add16_LSB = 0;
  Add16_MSB = ((Add_MSB << 8) | Add_LSB)*2;
  Add16_LSB = Add16_MSB + 1;
  
  ack_pooling();
  Wire.beginTransmission(address_mem);
  Wire.write((byte)(Add16_MSB >> 8));
  Wire.write((byte)(Add16_MSB & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(address_mem,1);
  Wire.available();
  c = Wire.read();
  Serial.write(c);
  
  ack_pooling();
  Wire.beginTransmission(address_mem);
  Wire.write((byte)(Add16_LSB >> 8));
  Wire.write((byte)(Add16_LSB & 0xFF));
  Wire.endTransmission();
  Wire.requestFrom(address_mem,1);
  Wire.available();
  c = Wire.read();
  Serial.write(c);
}

/*----------------------------------------------------------------------------*/
void ack_pooling(){
  byte code = -1;
  do{
    Wire.beginTransmission(address_mem);
    Wire.write((byte) 0);
    code = Wire.endTransmission();
  }
  while(code != 0);
}

/*----------------------------------------------------------------------------*/
uint8_t CRC(uint8_t *byte_s, uint8_t len){
    
uint8_t generator = 0x07;
uint8_t currByte = 0;
uint8_t crc = 0; /* start with 0 so first byte can be 'xored' in */

//Serial.print(len);

for (int index = 0; index < len; index++){

    currByte = byte_s[index];
    crc ^= currByte; 

    for (int i = 0; i < 8; i++){
        if ((crc & 0x80) != 0){
            crc = (uint8_t)((crc << 1) ^ generator);
        } else {
            crc <<= 1;
        } 
    } /* for */
} /* for */
    return crc;
} /* CRC */

/*
LINKS CRC:
    - http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
    - http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
*/
#endif
