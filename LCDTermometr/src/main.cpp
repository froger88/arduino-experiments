// Access to the Arduino Libraries
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <Wire.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <inttypes.h>
#include <DallasTemperature.h>
#include "beeper.h"


void loop(LiquidCrystal* lcd) {
//  // set the cursor to column 0, line 1
//  // (note: line 1 is the second row, since counting begins with 0):


//  lcd->setCursor(0, 1);
//  // print the number of seconds since reset:
//  int triggerPin = 9;
//  int echoPin = 8;
//  pinMode(triggerPin, OUTPUT);
//  pinMode(echoPin, INPUT);
//  digitalWrite(triggerPin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(triggerPin, LOW);
//  int distance = pulseIn(echoPin, HIGH) / 2;
//  distance /= 29;
//  //int d = millis()/1000;
//  char text[15];
//  delay(30);

//  int triggerPin2 = 7;
//  int echoPin2 = 10;
//  pinMode(triggerPin2, OUTPUT);
//  pinMode(echoPin2, INPUT);
//  digitalWrite(triggerPin2, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(triggerPin2, LOW);
//  int distance2 = pulseIn(echoPin2, HIGH) / 2;
//  distance2 /= 29;
//  sprintf(text, "A:%3dcm B:%3dcm ", distance, distance2);
//  lcd->print(text);
//  lcd->setCursor(0,0);
//  char text2[15];
//  float L3 = sqrt(pow(distance,2)+pow(distance2,2));
//  int H = (int)(distance*distance2)/L3;
//  sprintf(text2, "H = %d cm   AGL", H);
//  lcd->print(text2);
}

int main()
{
    // Initialize Arduino Librairies
    init();


    pinMode(8, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(A3, INPUT);
    //pinMode(A0, INPUT);
    digitalWrite(8, HIGH);

    digitalWrite(13,HIGH);
    char text[15];

    Beeper b(8);
    uint8_t *address = new uint8_t;
    OneWire onewire(A0);
    onewire.reset_search();
    while(onewire.search(address))
    {
      if (address[0] != 0x28)
         continue;

      if (OneWire::crc8(address, 7) != address[7])
      {
          sprintf(text,"WrongAddress\0\0\0");
          break;
       }
    }


    LiquidCrystal lcd(12, 11, 2, 3, 4, 5);
    lcd.begin(16, 2);

    lcd.print("hello, foka!");

    DallasTemperature sensors(&onewire);
    sensors.begin();
    delay(2000);
    while(true) {
        lcd.setCursor(0,1);
        sensors.requestTemperatures();
        float temperature = sensors.getTempCByIndex(0); //sensors.readTemperature(address);

        int temp_a = (10*temperature)/10;
        int temp_b = (10*temperature);
        //sprintf(text, "%d", int(temperature));
        sprintf(text, "%d.%d C   ", temp_a, temp_b%10);
        lcd.print(text);

        if(digitalRead(A3) == HIGH) {
            b.beep(10000,true);
        }
        b.update();
        delay(1000);

    }
}

