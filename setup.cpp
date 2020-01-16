#include "setup.h"


LiquidCrystal_I2C lcd(0x27, 20, 4);


void welcomeScreen()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Electrospinning  ");
  lcd.setCursor(0, 1);
  lcd.print("    setup at MEL    ");
  lcd.setCursor(0, 2);
  lcd.print("Press select button");
  lcd.setCursor(0, 3);
  lcd.print("      to begin      ");
}
void SystemSetup()
{

  pinMode(EN, OUTPUT);
  pinMode(cntlPin, INPUT);
  pinMode(Lstep, OUTPUT);
  pinMode(Rstep, OUTPUT);
  pinMode(Ldir, OUTPUT);
  pinMode(Ldir, OUTPUT);
  digitalWrite(EN, LOW);
  digitalWrite(Lstep, HIGH);
  digitalWrite(Rstep, HIGH);
  digitalWrite(Ldir, LOW);
  digitalWrite(Rdir, LOW);
}

