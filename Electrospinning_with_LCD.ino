//Code written by Sumukh Anil Purohit
//Licenced under GNU GPL v2
//
//Arduino Based Electrospinning setup with two motors
//
//include libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "setup.h"

extern LiquidCrystal_I2C lcd;

//#include <LiquidCrystal.h>
//LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //LCD pins
// Variables
int keypad_pin = A0;
int keypad_value = 0;
int keypad_value_old = 0;
long stepstomove = 400; //Minimum step size
char btn_push;
long totalSteps = 0;         //No of steps to move L
int delayTotL;     //Dummy for storing delay L
int delayTotR;     //Dummy for storing delay R
float mmPersL = 0;
float mmPersR = 0;
byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 5;
uint32_t last_pulse1;
uint32_t last_pulse2;
unsigned long stepsL = 0;
unsigned long stepsR = 0;

void setup()
{
  welcomeScreen();
  delay(1000);
  SystemSetup();
  MainMenuDisplay();
  delay(5);
}

void loop()
{
  last_pulse1 = micros();
  last_pulse2 = micros();
  lmotorInvert();
  rmotorInvert();
  btn_push = ReadKeypad();
  MainMenuBtn();
  if (btn_push == 'S') //enter selected menu
  {
    WaitBtnRelease();
    switch (mainMenuPage)
    {
      case 1:
        MenuA();  //steps per sec for 1
        break;
      case 2:
        MenuB();  //steps per sec for 2
        break;
      case 3:
        MenuC();  //  no of steps to move for 1
        break;
      case 4:     // confirm
        MenuD();
        break;
      case 5:     // start stepping
        MenuE();
        break;
    }

    MainMenuDisplay();
    WaitBtnRelease();
  }
  delay(5);

}

void MenuA()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Steps/s L     Speed");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      stepsL = stepsL + 10 ;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      stepsL = stepsL - 10;
    }
    mmPersL = (float)stepsL / 800.0;
    delayTotL = (1000000.0 / stepsL);
    lcd.setCursor(0, 1);
    lcd.print(stepsL);
    lcd.setCursor(9, 1);
    lcd.print((float)mmPersL, 4);
    lcd.setCursor(16, 1);
    lcd.print("mm/s");
    if (ReadKeypad() == 'R')
    {
      lcd.setCursor(0, 2);
      lcd.print("Stored  step size");
      lcd.setCursor(0, 3);
      lcd.print(stepsL);
      lcd.setCursor(8, 3);
      lcd.print((float)delayTotL, 4);
      lcd.setCursor(18, 3);
      lcd.print("us");
    }
  }
}

void MenuB()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Steps/s R     Speed");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      stepsR = stepsR + 10 ;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      stepsR = stepsR - 10;
    }
    mmPersR = (float)stepsR / 800.0;
    delayTotR = (1000000.0/stepsR);
    lcd.setCursor(0, 1);
    lcd.print(stepsR);
    lcd.setCursor(9, 1);
    lcd.print((float)mmPersR, 4);
    lcd.setCursor(16, 1);
    lcd.print("mm/s");
    if (ReadKeypad() == 'R')
    {
      lcd.setCursor(0, 2);
      lcd.print("Stored  step size");
      lcd.setCursor(0, 3);
      lcd.print(stepsR);
      lcd.setCursor(8, 3);
      lcd.print((float)delayTotR, 4);
      lcd.setCursor(18, 3);
      lcd.print("us");
    }
  }
}

void MenuC()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Steps to move");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      stepstomove = stepstomove + 400 ;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      stepstomove = stepstomove - 400 ;
    }
    lcd.setCursor(5, 1);
    lcd.print(stepstomove);
    if (ReadKeypad() == 'R')
    {
      totalSteps = stepstomove;
      lcd.setCursor(0, 2);
      lcd.print("Stored");
      lcd.setCursor(0, 3);
      lcd.print(totalSteps);
    }
  }
}

void MenuD() // check all parameters
{
  while (ReadKeypad() != 'L')
  {
    lcd.setCursor(0, 0);
    lcd.print("Steps/s L ");
    lcd.setCursor(10, 0);
    lcd.print(stepsL);
    lcd.setCursor(0, 1);
    lcd.print("Steps/s R ");
    lcd.setCursor(10, 1);
    lcd.print(stepsR);
    lcd.setCursor(0, 2);
    lcd.print("Total steps to move");
    lcd.setCursor(0, 3);
    lcd.print(totalSteps);
  }
}

void MenuE()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRESS RIGHT TO");
  lcd.setCursor(0, 1);
  lcd.print("START SPINNING");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'R')
    {
      if (digitalRead(cntlPin) == HIGH)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Electrospinning ON");
        lcd.setCursor(0, 1);
        lcd.print("!!!!HIGH VOLTAGE!!!!");
        lcd.setCursor(0, 2);
        lcd.print(" GROUND THE NEEDLE");
        lcd.setCursor(0, 3);
        lcd.print("BEFORE OPENING DOOR");
        while (ReadKeypad() != 'U')
        {
          if (micros() - last_pulse1 >= delayTotL)
          {
            last_pulse1 += delayTotL;
            digitalWrite(Ldir, LOW);
            digitalWrite(Lstep, LOW);
            digitalWrite(Lstep, HIGH);
          }
          if (micros() - last_pulse2 >= delayTotR)
          {
            last_pulse2 += delayTotR;
            digitalWrite(Rdir, LOW);
            digitalWrite(Rstep, LOW);
            digitalWrite(Rstep, HIGH);
          }
        }
      }
      if (ReadKeypad() == 'U')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("END / STOPPED");
        lcd.setCursor(0, 1);
        lcd.print("L Back  R Again");
      }
    }
  }
}

void MainMenuDisplay()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (mainMenuPage)
  {
    case 1:
      lcd.print("Steps per s L");
      break;
    case 2:
      lcd.print("Steps per s R");
      break;
    case 3:
      lcd.print("Steps to move");
      break;
    case 4:
      lcd.print("Confirm");
      break;
    case 5:
      lcd.print("Start");
      break;
  }
}

void MainMenuBtn()
{
  WaitBtnRelease();
  if (btn_push == 'U')
  {
    mainMenuPage++;
    if (mainMenuPage > mainMenuTotal)
      mainMenuPage = 1;
  }
  else if (btn_push == 'D')
  {
    mainMenuPage--;
    if (mainMenuPage == 0)
      mainMenuPage = mainMenuTotal;
  }

  if (mainMenuPage != mainMenuPageOld) //only update display when page change
  {
    MainMenuDisplay();
    mainMenuPageOld = mainMenuPage;
  }
}

char ReadKeypad()
{
  /* Keypad button analog Value
    no button pressed 1023
    select  741
    left    503
    down    326
    up      142
    right   0
  */
  keypad_value = analogRead(keypad_pin);
  if (keypad_value == 0)
    return 'R';
  else if ((keypad_value > 100) && (keypad_value < 180))
    return 'U';
  else if (keypad_value < 450)
    return 'D';
  else if (keypad_value < 580)
    return 'L';
  else if (keypad_value < 800)
    return 'S';
  else
    return 'N';
}

void WaitBtnRelease()
{
  while ( analogRead(keypad_pin) < 800) {}
}
void lmotorInvert()
{
  if (digitalRead(Lmot) == HIGH)
  {
    digitalWrite(Ldir, HIGH);         // invert L motor
    digitalWrite(Lstep, LOW);
    delayMicroseconds(150);
    digitalWrite(Lstep, LOW);
    delayMicroseconds(150);
  }
}
void rmotorInvert()
{
  if (digitalRead(Rmot) == LOW)
  {
    digitalWrite(Rdir, HIGH);                 // invert R motor
    digitalWrite(Rstep, HIGH);
    delayMicroseconds(150);
    digitalWrite(Rstep, LOW);
    delayMicroseconds(150);
  }
}
