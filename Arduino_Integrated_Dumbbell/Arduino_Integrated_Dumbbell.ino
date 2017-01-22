/*
Hackers:
Nicholas Nguyen
Lawton Lee
Elton Guan
Daniel Lui

*/
#include <rgb_lcd.h>
#include <Wire.h>
#include "MMA7660.h"

MMA7660 accelemeter;

float delaytime = 20;
int count = 0;
boolean moving = true;
int previous = 0;
rgb_lcd lcd;
int goal = 10;
double v = 0.0;
int buzzer = 8;

int E[7] = {41, 82, 165, 330, 659, 1319, 2637};

void setup()
{
	accelemeter.init();  
	Serial.begin(9600);
  
  lcd.begin(16,2); //set up LCD's number of columns and rows    
  lcd.setRGB(255, 0, 0);
  lcd.setCursor(0,0);
  lcd.print("Reps done: ");
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  delay(1000);
}


void loop()
{
  int reps = count/2;
  lcd.setCursor(10,0);
  lcd.print(reps);
  if (count % 2 == 0 && reps <= goal)
  {
    int colorR = 255 - (255*reps/goal); //red: 255 -> 0
    int colorG = 125*reps/goal; //green: 0 -> 125
    int colorB = 0;
    lcd.setRGB(colorR, colorG, colorB);
    
    if (reps == goal)
    {
      lcd.setRGB(0,255,0);
    }
  }
  lcd.setCursor(5,1);
	
	delay(delaytime);
	int8_t x;
	int8_t y;
	int8_t z;
  
	float ax,ay,az;
	accelemeter.getXYZ(&x,&y,&z);  

	accelemeter.getAcceleration(&ax,&ay,&az);
  az = az - 1.00; // gravity 
  Serial.print("az = ");
  Serial.println(az);
  lcd.print(millis()/1000);
  //lcd.print(az);
  if (moving == true) //previously moved
  {
    if (stopped(az)) //now stopped
    {
      previous++;
      if (previous >=15 )
      {
        count++;
        //Serial.print("Count: ");
        //Serial.println(count);
        tone(buzzer,E[3],25);
        previous = 0;
        moving = false;
      }
    }
    else //still moving
    {
      previous++;
    }
  }
  
  if (moving == false) //previously stopped
  {
    if (!stopped(az)) //now moving
    {
      previous++;
      if (previous >=15)
      {
        previous = 0;
        moving = true;
      }
    }
    else //still stopped
    {
      previous++;
    }
  }
}

bool stopped(float az)//If acceleration is low, assume it 0 (sensor fluctuations)
{
  if (abs(az) < 0.17)
  {
    return true;
  }
  else 
  {
    return false;
  }
}

