--<--------------------------------------------------------------------------------------------------------------------------------------->--
                                
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
--<--------------------------------------------------------------------------------------------------------------------------------------->--

# Communicate
Communication between PC-EEPROM (Serial-I2C)

This project was developed to write 1000 data from PC (serial) in 24LC256 EEPROM memory (I2C), using Arduino UNO as bridge between PC-EEPROM. 
Arduino does not save data in its memory, only act like controller Serial-I2C communication.
We create our communication protocols, the protocols are:

    1: Write 1000 data in memory from PC, the protocol write one-to-one data, type data is integer (16 bits - 2 bytes).
       Control word:
    
                        =======================================================================   
                        | '#' | '1' | Addres(MSB) | Addres(LSB) | Data(MSB) | Data(LSB) | CRC |   
                        =======================================================================       
       
    
    2: Read 1000 data from memory and write it in PC by serial communication, the protocol read one-to-one data.
       Control word:
       
                                   ===============================================                
                                   | '#' | '2' | Addres(MSB) | Addres(LSB) | CRC |                
                                   ===============================================                      
    
    
    3: Read only one data in specific address (package-data) request from PC.
       Control word:
       
                                   ===============================================                
                                   | '#' | '3' | Addres(MSB) | Addres(LSB) | CRC |                
                                   ===============================================                       
       
       
--<--------------------------------------------------------------------------------------------------------------------------------------->--

The command table is:


                                           =======================================                   
                                           | Command |         Function          |                   
                                           =======================================                   
                                           |   '1'   | Write 1000 data in memory |                   
                                           =======================================                   
                                           |   '2'   | Read 1000 data in memory  |                   
                                           =======================================                   
                                           |   '3'   |  Read one data in memory  |                   
                                           =======================================                   

 
--<--------------------------------------------------------------------------------------------------------------------------------------->--

IMPORTANT: We use "Conda" Python IDE. IF you want use this project, you have to install "PY Serial" like this:
      
      conda install -c conda-forge pyserial
      
      visit: https://pyserial.readthedocs.io/en/latest/pyserial.html
    
