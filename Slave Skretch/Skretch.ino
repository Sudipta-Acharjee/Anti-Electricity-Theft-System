#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include <RH_ASK.h>
#include <SPI.h>
Adafruit_LiquidCrystal lcd(10 ,11,5 , 4 ,3, 2);
RH_ASK driver;
int lcd_disp_time=0;
float consumer_current=0;
void setup(){
  driver.init();

  Serial.begin(9600);
}
void loop()
{
  unsigned long tm = millis();
  while(millis()-tm<50)
  {
    float cc = (float(analogRead(A5)*5/1024.0-2.4911)/(0.098*7.346)/sqrt(2));
    if(cc>consumer_current)
    {
      lcd.begin(16,2);
      consumer_current=cc;
      lcd.setCursor(0,1);
      Serial.print("Consumer current=");
      Serial.print(consumer_current);
      Serial.println("Amp");
      lcd.println("Consumer Current=");
      lcd.print(consumer_current);
      lcd.println("Amp");
    }
  }
  if(millis() - lcd_disp_time>200)
  {
    lcd_disp_time = millis();
  }


  String str = (String)consumer_current;
  uint8_t data[sizeof(str)];
  for(byte i = 0; i<sizeof(str); i++)
  {
    data[i]=str[i];
  }
  driver.send(data,sizeof(data));
  driver.waitPacketSent();
}
