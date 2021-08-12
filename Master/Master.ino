#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);
#include<RH_ASK.h>//for RX module
#include<SPI.h>
RH_ASK driver;
float source_current=0;
float previous_source_current = 0;
void setup()
{
  lcd.init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Source current ");
  lcd.setCursor(0,1);
  lcd.print("Consumer ");
  lcd.setCursor(0,2);
  lcd.print("Theft = ");

  driver.init();
  Serial.begin(9600);
}
void loop()
{
  uint8_t buf[12];
  uint8_t buflen = sizeof(buf);
  if(driver.recv(buf, &buflen))
  {
    float source_current = 0;
    unsigned long tm = millis ();
  }
  unsigned long tm = millis();
  while(millis()-tm<50){
    float sc = (float(analogRead(A0)*5)/1024.0-2.4911)/(0.098*7.346)/sqrt(2);
    if(sc>source_current){
      source_current = sc;
      if((source_current-previous_source_current >0? source_current-previous_source_current:previous_source_current-source_current))
      lcd.setCursor(15,0);
      if(source_current<2.45){
        lcd.print(source_current);
        lcd.setCursor(19,0);
        lcd.print("A");
      }
      else{
        lcd.print("MAX")
        ;}
        String recieved = (char*)buf;
         float consumer_current = recieved.toFloat();
        lcd.setCursor(15,1);
        if(consumer_current<2.45)
        {
          lcd.print(consumer_current);
          lcd.setCursor(19,1);
          lcd.print("A");
      }
      else{
        lcd.print("MAX");
      }
      lcd.setCursor(6,2);
      if(source_current <2.45 && consumer_current<2.45)
      {
        if(((source_current-consumer_current)>=0?(source_current-consumer_current):(consumer_current-source_current))<source_current*0.05)
        {
          lcd.print("none");
        }else{
          lcd.print(source_current-consumer_current);
          lcd.print("A");
          Serial.print("Theft = ");
          Serial.print(source_current-consumer_current);
          Serial.println("A");
        }
      }
      else{
        lcd.print("--------------"); //if measured current exceed wcs2702 current limit impossible to find power theft
      }
    }
    else{
      previous_source_current=source_current;
    }
  }
}
