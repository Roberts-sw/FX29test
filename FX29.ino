#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27,16,2);//address 0x27, 16 chars and 2 lines

#include <FX29.h>
FX29 sensor;

void setup()
{   //initialisatie:
    Serial.begin(9600);         //Baudrate voor seriëel
    sensor.begin(FX29_DEVICE);  //(bewaart het adres in "Wire")
    lcd.init();                 //initialize the lcd 
    lcd.backlight();

//Print a message to the LCD.
//  lcd.setCursor(2,0); lcd.print("Loading...");
    delay(1000);

//lcd: tekst eenmalig
    lcd.setCursor(0,0);         lcd.print("direct :");
    lcd.setCursor(0,1);         lcd.print("omgezet:");
}

void loop()
{//lees sensor, converteer naar (milli-)Newton
    u16 ruw=sensor.read();
    u16 mN =sensor.in_mN(ruw);

//resultaat via UART: tekst met regeleinde
    Serial.print("direct :");   Serial.println(ruw);
    Serial.print("omgezet:");   Serial.println(mN);
//wis vorige waarden op LCD:
    lcd.setCursor(8,0);         lcd.print("     ");//wis tot 5 cijfers
    lcd.setCursor(8,1);         lcd.print("     ");//wis tot 5 cijfers
//zet nieuw resultaat op LCD, wacht even:   
    lcd.setCursor(8,0);         lcd.print(ruw);
    lcd.setCursor(8,1);         lcd.print(mN);
    delay(500);
}
