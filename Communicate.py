'''
                    
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
                                                               
'''

import warnings
import serial
import serial.tools.list_ports
import time
import math
import numpy as np
import csv
################################################################################
n = list();
sine = list();
Data_MSB = list();
Data_LSB = list();
Data_Read = list();
Address_LSB = list();
Address_MSB = list();
CRC_s = 0
j = 0

################################################################################
def abrirPuertoArduino(timeout=2,defaultport=0,baudrate=115200,sleep=2):
    arduino_ports = [ 
    p.device 
    for p in serial.tools.list_ports.comports() 
     if 'Arduino' in str(p.description)+str(p.manufacturer) 
     ]# si no entiende esta lineas pitonica ver: https://docs.python.org/3/tutorial/datastructures.html#list-comprehensions 
    if not arduino_ports:
        raise IOError("No se encontraron arduinos")
        exit()#algo extremo, pero funciona al cerrar la aplicacion
    if len(arduino_ports) > 1:
        warnings.warn('Se encontraron multiples arduinos Usando el '+str(defaultport))
    print(arduino_ports[defaultport])
    ser = serial.Serial(port=arduino_ports[defaultport],baudrate=baudrate,timeout=timeout)# Ver https://pyserial.readthedocs.io/en/latest/pyserial_api.html
    time.sleep (sleep)#esperando por el serial
    return ser

################################################################################
def enviarYrecibirbytes(ser,datos):
    if isinstance(datos, list):
        print("Enviando lista")
        byteList=bytearray(datos)
        for bits in byteList:
            
            ser.write(bytes([bits])) #es necesario hacer de nuevo bytes([bits]) o se puede usar solo bits
            rx=ser.read(1) #tiene echo activado por lo que espera el dato que se envia
            print("enviando "+str(bits)+" Recibiendo: ",rx)
    else:
        print("No soportado")

################################################################################
def cerrarPuerto(ser):
    if ser.isOpen():
        ser.close()
    else:
        warnings.warn('Ya estaba cerrado el puerto',ser)

################################################################################
def datosEnColaRx(ser):
    return ser.in_waiting()


################################################################################
def CRC(byte_s, length):
    
    generator = 0x07;
    currByte = 0;
    crc = 0;
    
    for index in range(length):
        currByte = ord(byte_s[index]);
        crc ^= currByte;
        for i in range(8):
            if ((crc & 0x80) != 0):
                crc = np.uint8((crc << 1) ^ generator);
            else:
                crc <<= 1;
            
    return crc;

################################################################################
def Sine_Wave():

    PI = math.pi;
    A = 512;
    off = 512;
    fs = 1000;
    f = 1;
    m = 1000;

    for i in range(m):
        n.append(i);
        sine.append(np.int16((A*math.sin(2*PI*f*i*(1/fs)))+off));
        Address_LSB.append(n[i] & ~(0xFF00));
        Address_MSB.append(n[i] >> 8);
        Data_LSB.append(sine[i] & ~(0xFF00));
        Data_MSB.append(sine[i] >> 8);

Sine_Wave()

################################################################################
def generarNumeros(protocolo,direccion=0):
  
    l = list()
    global j
    
    if (j < 1000):
        l.append('#')
        if(protocolo == 1):
            l.append('1')
            l.append(chr(Address_MSB[j]))
            l.append(chr(Address_LSB[j]))
            l.append(chr(Data_MSB[j]))
            l.append(chr(Data_LSB[j]))
        elif(protocolo == 2):
            l.append('2')
            l.append(chr(Address_MSB[j]))
            l.append(chr(Address_LSB[j]))
        elif(protocolo == 3):
            l.append('3')
            l.append(chr(Address_MSB[direccion]))
            l.append(chr(Address_LSB[direccion]))
        else: 
            raise IOError("El protocolo no es valido")
        j += 1
    else:
      j = 0
    return l
    
################################################################################
def to_number(lis):

    l = list()
    for i in range(len(lis)):
        l.append(ord(lis[i]))

    return l

################################################################################
def export_CSV(a, b, filename):

  filename = filename+'.csv'
  with open(filename, mode='w') as csv_file:
    fieldnames = ['Package', 'Data']
    writer = csv.DictWriter(csv_file, fieldnames=fieldnames)

    writer.writeheader()
    for i in range(len(a)):
      writer.writerow({'Package': a[i], 'Data': b[i]})

################################################################################
def my_protocol(protocolo, direccion=0):

  global j
  MOST = 0
  LEAST = 0
  data = 0

  if( protocolo == 1 ):
    serialArduino = abrirPuertoArduino()
    for i in range(1000):
      listBytes = generarNumeros(protocolo) # lista sin CRC
      CRC_s = CRC(listBytes,len(listBytes))
      listBytes.append(chr(CRC_s))            # lista con CRC
      my_list = to_number(listBytes)
      enviarYrecibirbytes(serialArduino,my_list)
    cerrarPuerto(serialArduino)
    export_CSV(n, sine, "written_data")
#------------------------------------------------------------------------------#
  elif( protocolo == 2 ):
    serialArduino = abrirPuertoArduino()
    for i in range(1000):
      listBytes = generarNumeros(protocolo) # lista sin CRC
      CRC_s = CRC(listBytes,len(listBytes))
      listBytes.append(chr(CRC_s))            # lista con CRC
      my_list = to_number(listBytes)
      enviarYrecibirbytes(serialArduino,my_list)
      MOST = (serialArduino.read(1))
      LEAST = (serialArduino.read(1))
      mos = MOST[0]
      leas = LEAST[0]
      data = (((mos) << 8) | (leas))
      print("Data MSB: ", MOST)
      print("Data LSB: ", LEAST)
      print("Data: ", data)
      Data_Read.append(data)
    
    cerrarPuerto(serialArduino)
    export_CSV(n, Data_Read, "read_data")
#------------------------------------------------------------------------------#
  elif( protocolo == 3 ):
    serialArduino = abrirPuertoArduino()
    listBytes = generarNumeros(protocolo,direccion) # lista sin CRC
    CRC_s = CRC(listBytes,len(listBytes))
    listBytes.append(chr(CRC_s))            # lista con CRC
    my_list = to_number(listBytes)
    enviarYrecibirbytes(serialArduino,my_list)
    MOST = (serialArduino.read(1))
    LEAST = (serialArduino.read(1))
    mos = (MOST[0])
    leas = (LEAST[0])
    data = (((mos) << 8) | (leas))
    print("Data MSB: ", MOST)
    print("Data LSB: ", LEAST)
    print("Data: ", data)
    cerrarPuerto(serialArduino)

  else:
    raise IOError("El protocolo no es valido")

################################################################################
if _name_ == "_main_":
  
  # 1: escribir 1000 datos
  # 2: leer 1000 datos
  # 3: leer un solo dato, tambien se debe enviar el paquete que se quiere leer
  protocolo = 3
  my_protocol(protocolo,469)
  my_protocol(protocolo,10)
