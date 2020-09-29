/**
#include <Arduino.h>

#include <Keypad_I2C.h>        // I2C Keypad library by Joe Young https://github.com/joeyoung/arduino_keypads
#include <LiquidCrystal_I2C.h> // I2C LCD Library by Francisco Malpartida https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
#include <Hall_sensor.h>

#define lcd_addr 0x27    // I2C address of typical I2C LCD Backpack
#define keypad_addr 0x20 // I2C address of I2C Expander module (A0-A1-A2 dip switch to off position)

// LCD Pins to I2C LCD Backpack - These are default for HD44780 LCD's
#define Rs_pin 0
#define Rw_pin 1
#define En_pin 2
#define BACKLIGHT_PIN 3
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

// Pinos Sensores Hall
#define D0_SHpin 16
#define D3_SHpin 0
#define D4_SHpin 2

// Create instance for LCD called: i2c_lcd
LiquidCrystal_I2C i2c_lcd(lcd_addr, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

// Define the keypad pins
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

// Keypad pins connected to the I2C-Expander pins P0-P6
byte rowPins[ROWS] = {0, 1, 2, 3}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6};    // connect to the column pinouts of the keypad

// Create instance of the Keypad name I2C_Keypad and using the PCF8574 chip
Keypad_I2C I2C_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, keypad_addr, PCF8574);

Hall_sensor Quad_Trig(D0_SHpin, D3_SHpin, D4_SHpin);

bool init_Complete = false, in_zero = false, resetting = false;
int angulo, angulo_anterior = -1;
double seno, cosseno, tangente;

char key;
void setup()
{

  i2c_lcd.begin(16, 2); //  our LCD is a 16x2, change for your LCD if needed

  // LCD Backlight ON
  i2c_lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  i2c_lcd.setBacklight(HIGH);
  i2c_lcd.clear(); // Clear the LCD screen

  I2C_Keypad.begin();

  pinMode(D0_SHpin, INPUT);
  pinMode(D3_SHpin, INPUT);
  pinMode(D4_SHpin, INPUT);
}

void loop()
{


  if (init_Complete == false)
  {

    if (in_zero == false)
    {
      i2c_lcd.setCursor(0, 0);
      i2c_lcd.printf("Gire o eixo 360'");
      i2c_lcd.setCursor(0, 1);
      i2c_lcd.printf("e posicione em 0");
    }

    Quad_Trig.read_Pins();
    if (Quad_Trig.in_Zero() == true)
    {
      in_zero = true;
      i2c_lcd.clear();
      i2c_lcd.setCursor(0, 0);
      i2c_lcd.printf("Pressione * para");
      i2c_lcd.setCursor(0, 1);
      i2c_lcd.printf("iniciar");

      key = I2C_Keypad.getKey();
      if (key == '*')
      {
        Quad_Trig.reset();
        init_Complete = true;
        i2c_lcd.clear();
      }
    }
  }
  else
  {
    if (resetting == false)
    {
      Quad_Trig.read_Pins();

      angulo = Quad_Trig.get_Angle();
      seno = Quad_Trig.get_Seno();
      cosseno = Quad_Trig.get_Cosseno();
      tangente = Quad_Trig.get_Tangente();

      i2c_lcd.setCursor(0, 0);
      i2c_lcd.printf("Ang:");
      i2c_lcd.print(angulo);
      if (angulo <= 90)
      {
        i2c_lcd.printf("'  ");
      }
      else
      {
        i2c_lcd.printf("' ");
      }

      i2c_lcd.setCursor(9, 0);
      i2c_lcd.printf("T:");
      if (angulo == 90 || angulo == 270)
      {
        i2c_lcd.printf("nExst");
      }
      else
      {
        i2c_lcd.print(tangente, 2);
        if (tangente >= 0.0)
        {
          i2c_lcd.printf(" ");
        }
      }

      i2c_lcd.setCursor(0, 1);
      i2c_lcd.printf("S:");
      i2c_lcd.print(seno, 2);
      if (seno >= 0.0)
      {
        i2c_lcd.printf(" ");
      }
      i2c_lcd.setCursor(9, 1);
      i2c_lcd.printf("C:");
      i2c_lcd.print(cosseno, 2);
      if (cosseno >= 0.0)
      {
        i2c_lcd.printf(" ");
      }
      key = I2C_Keypad.getKey();
      if (key == '#')
      {
        resetting = true;
        i2c_lcd.clear();
      }
    }
    else
    {
      i2c_lcd.setCursor(0, 0);
      i2c_lcd.printf("Press # to Reset");
      i2c_lcd.setCursor(0, 1);
      i2c_lcd.printf("or * to Cancel");

      key = I2C_Keypad.getKey();
      if (key == '#')
      {
        in_zero = false;
        init_Complete = false;
        resetting = false;
        i2c_lcd.clear();
      }
      else if (key == '*')
      {
        resetting = false;
        i2c_lcd.clear();
      }
    }
  }
}
*/