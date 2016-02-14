/*  COMPOSTROMETRE_OLED
  
    registra la temperatra maxima aconseguida aquest periode i la mostra durant 2 segons per la pantalla al premer un polsador,
    seguidament es mostra per pantalla durant 2 segons el temps transcorregut desde que s'han assolit els 55ºC necessaris per matar les llavors.
    
    al premer l'altre polsador es reinicia el periode.

*/



#define boto_1 3
#define boto_2 2

float Tmax=0;


/**********************************************************************
deformacio de Datalogger de temperatura Nergizo - www.nergiza.com 	 
 *********************************************************************/
 #include <OneWire.h>
 
int DS18S20_Pin = 5; //DS18S20 Signal pin on digital 5
//Temperature chip i/o
OneWire ds(DS18S20_Pin); // on digital pin 5
int segundosencendido=0;
int horas=0;
int minutos=0;
int segundos=0;
int conta=0; //Contador de nº registros
int intervalo=1000; //Intervalo para contaje en ms
//********************************************************************
//********************************************************************



/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

This example is for a 128x32 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
//********************************************************************
//********************************************************************






void setup()   {                
  Serial.begin(9600);
  
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Compost'o'   metre");
  display.display();
  delay(2000);

  float temperature = getTemp();         //  AIXOS ES PER EVITAR EL MOLEST -85º QUE SURT AL PRINCIPI..
  temperature = getTemp();               //

  display.clearDisplay();
  display.setTextSize(4);
  display.display();  
  
    
  pinMode(boto_1, INPUT);
  pinMode(boto_2, INPUT);
  
  
  
}






void loop() {
  
// control del temps
  segundosencendido =millis()/1000;
  horas =segundosencendido/3600;
  minutos =segundosencendido/60-horas*60;
  segundos =segundosencendido-minutos*60;

// mesura Tº  i estableix la Tmax
  float temperature = getTemp();
  if (temperature > Tmax) { Tmax = temperature;  Serial.println("novamax"); }

// maneig dels posadors
  int Pant;
  int Reset;
  if (digitalRead(boto_1)==HIGH) { Pant=true;    Serial.println("       pant=true"); }
  else  { Pant=false;    Serial.println("pant=false"); }
  if (digitalRead(boto_2)==HIGH) { Reset=true;    Serial.println("        reset=true");} 
  else { Reset=false;    Serial.println("reset=false"); }

if(Pant==true) {
 // imprimir en serial port:
    Serial.print(conta);
    Serial.println(";");
    Serial.print(horas);
    Serial.print(":");
    Serial.print(minutos);
    Serial.print(":");
    Serial.print(segundos);
    Serial.println(";");
    Serial.print(temperature);
    Serial.print((char)176);    // el simbol de grau º
    Serial.println("C");    
    Serial.print("Tmax=     ");    
    Serial.print(Tmax);
    Serial.println(" C");
 // imprimir en pantalla OLED
    display.clearDisplay();
    display.display();  
    display.setCursor(0,0);
    display.println(temperature);
    display.display();      
  delay (1000);
    display.clearDisplay();
    display.display();  
    display.setTextSize(2);
    display.setCursor(0,15);
    display.println("maxima");
    display.setTextSize(4);
    display.setCursor(75,0);
    display.println(Tmax);    
    display.display(); 
  delay (1500);
    display.clearDisplay();
    display.display();  

}

if (Reset==true) {
  // reinicia el periode
  Tmax=0;   
    display.clearDisplay();
      display.display();  
    display.setTextSize(3);
    display.setCursor(0,0);
    display.println("reinici");
      display.display(); 
    display.setTextSize(4);
    delay (500);
    display.clearDisplay();
      display.display();  
}
  
    delay(intervalo); 
    conta++; //incrementa el contador
    
}





float getTemp(){
      //returns the temperature from one DS18S20 in DEG Celsius
        
        byte data[12];
        byte addr[8];
        
        if ( !ds.search(addr)) {
        //no more sensors on chain, reset search
           ds.reset_search();      
        return -1001;
        }
        
        if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC no valido!\n");
        return -1002;
        }
        
        if ( addr[0] != 0x10 && addr[0] != 0x28) {
        Serial.print("Dispositivo no reconocido");
        return -1003;
        }
      
        ds.reset();
        ds.select(addr);
        ds.write(0x44,1); // start conversion, with parasite power on at the end
        
        byte present = ds.reset();
        ds.select(addr);  
        ds.write(0xBE); // Read Scratchpad
        
        
        for (int i = 0; i < 9; i++) { // we need 9 bytes
          data[i] = ds.read();
        }
        
        ds.reset_search();
        
        byte MSB = data[1];
        byte LSB = data[0];
        float CONT_REMAIN = data[6];
        float CONT_PER_C = data[7];
        
        float tempRead = ((MSB << 8) | LSB); //using two's compliment 
        
        float TemperatureSum = tempRead/16;
        
        return TemperatureSum;
      
}

  
  
  
  
  
  
  
  


