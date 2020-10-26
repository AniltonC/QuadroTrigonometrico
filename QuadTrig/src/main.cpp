/*
||
|| @file main.cpp
|| @version 1.1
|| @author Anilton Carlos
|| @contact aclr@icomp.ufam.edu.br / aniltoncarlos9@gmail.com
||
|| @description
|| | This is a basic implementation of the Trigonometric Board library.
|| #
||
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <stdlib.h>

#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
//#include <Hall_sensor.h>
#include <TrigonometricBoard.h>

// Rede Wifi
const char* ssid = "REIS's HOUSE";
const char* password = "23063127";

//Your Domain name with URL path or IP address with path // URL do seu domínio ou IP local
String serverName = "http://192.168.0.108:5002/quadTrig";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;
int ang = 0;

/* Pinos Sensores Hall
#define D0_SHpin 16
#define D3_SHpin 0
#define D4_SHpin 2
// Create instance for LHall sensor
Hall_sensor Quad_Trig(continuosMode, D0_SHpin, D3_SHpin, D4_SHpin);
*/

TrigonometricBoard Board;

// Enderecos I2C do LCD e Teclado
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

// Create instance for LCD called: i2c_lcd
LiquidCrystal_I2C I2C_LCD(lcd_addr, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

// Define the keypad pins
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Keypad pins connected to the I2C-Expander pins P0-P6
byte rowPins[ROWS] = {0, 1, 2, 3}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6};    // connect to the column pinouts of the keypad

// Create instance of the Keypad name I2C_Keypad and using the PCF8574 chip
Keypad_I2C I2C_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS, keypad_addr, PCF8574);

// Variavel que identifica o usuario, requisitada ao ligar o quadro 
String idNumber;

void setup() {
    //Utilizando o PlatformIO é preciso alterar o monitor_speed manualmente no arquivo platformio.ini
    //monitor_speed = 115200
    Serial.begin(115200); 
    // Inicializacao do LCD
    I2C_LCD.begin(16,2);
    I2C_LCD.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
    I2C_LCD.setBacklight(HIGH);
    I2C_LCD.clear(); // Clear the LCD screen
    
    // Inicializacao do Teclado
    I2C_Keypad.begin();
    
    // Inicializacao dos Sensores
    //Quad_Trig.begin();
    Board.begin(continuous);

    // Conecção de rede wifi
    I2C_LCD.home();
    I2C_LCD.printf("Conectando Rede");
    I2C_LCD.setCursor(0,1);
    I2C_LCD.printf("Wifi");
    
    WiFi.begin(ssid, password);
    Serial.println("Connecting");

    int pointCounter = 0;
    while(WiFi.status() != WL_CONNECTED) {
        if(pointCounter == 3){
            I2C_LCD.setCursor(4,1);
            I2C_LCD.printf("   ");
            I2C_LCD.setCursor(4,1);
            pointCounter = 0;
        }
        I2C_LCD.printf(".");
        pointCounter++;
        
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
    
    I2C_LCD.clear();
    I2C_LCD.home();
    I2C_LCD.printf("   Rede Wifi    ");
    I2C_LCD.setCursor(0,1);
    I2C_LCD.printf("   Conectada    ");

    delay(1000);

    // Requisicao de Identificacao do usuario, criar um método getId() e verifyId()
    // Entrada de 8 digitos: matricula UFAM
    I2C_LCD.clear();
    I2C_LCD.home();
    I2C_LCD.printf("Insira seu");
    I2C_LCD.setCursor(0,1);
    I2C_LCD.printf("Id:");

    int idCounter = 0;
    bool idConfirm = 0;
    while (idConfirm == 0)
    {
        char inputNumber = I2C_Keypad.getKey();
        if(inputNumber >= '0' && inputNumber <= '9'){
            if(idCounter < 8){
                idNumber += inputNumber;
                I2C_LCD.print(inputNumber);
                idCounter++;
            }
        } else {
            if(inputNumber == '*'){
                if(idCounter > 0){
                    idCounter--;
                    idNumber.remove(idCounter);
                    I2C_LCD.moveCursorLeft();
                    I2C_LCD.print(' ');
                    I2C_LCD.moveCursorLeft();
                }
            } else if(inputNumber == '#' && idCounter == 8){
                idConfirm = 1;
            }
        } 
        delay(20);
    }

    // Ajuste de posicionamento do Quadro | O quadro começa a contar em 0°, portanto é preciso situar o eixo em 0°
    bool eixoSetup = 0;
    I2C_LCD.clear();
    I2C_LCD.home();
    I2C_LCD.printf("Situe o eixo em");
    I2C_LCD.setCursor(0,1);
    I2C_LCD.printf("00 e aperte #:");

    while(eixoSetup == 0){

        //int anguloAtual = Quad_Trig.get_Ang_Ajuste();
        int anguloAtual = Board.axis_Position();
        I2C_LCD.setCursor(14,1);
        I2C_LCD.print(anguloAtual);
        if(anguloAtual == 0){
            I2C_LCD.setCursor(15,1);
            I2C_LCD.print(anguloAtual);
            char inputCaracter = I2C_Keypad.getKey();
            if(inputCaracter == '#'){
                eixoSetup = !eixoSetup;
            }
        }

        delay(10);
    }
    I2C_LCD.clear();   
}

// Função que imprime o layout do LCD
void Lcd_Layout(){
    I2C_LCD.clear();
    I2C_LCD.home();
    I2C_LCD.print("Ang:");
    I2C_LCD.setCursor(9,0);
    I2C_LCD.print("T:");
    I2C_LCD.setCursor(0,1);
    I2C_LCD.print("S:");
    I2C_LCD.setCursor(9,1);
    I2C_LCD.print("C:");
}

// Função que realiza a requisição http no servidor e envia os dados atuais do quadro
void http_get(String serverPath){
    //Send an HTTP POST request every 10 minutes
    if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
        if(WiFi.status()== WL_CONNECTED){
            HTTPClient http;

            // Your Domain name with URL path or IP address with path
            http.begin(serverPath.c_str());
            
            // Send HTTP GET request
            int httpResponseCode = http.GET();
            
            if (httpResponseCode>0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String payload = http.getString();
            Serial.println(payload);
            }
            else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
            }
            // Free resources
            http.end();
        }
        else {
            Serial.println("WiFi Disconnected");
            I2C_LCD.clear();
            I2C_LCD.print("      Wifi      ");
            I2C_LCD.setCursor(0,1);
            I2C_LCD.print("  Desconectado  ");
            delay(1000);
        }
    lastTime = millis();
    }
}

String last_Angle = "";
bool print_Infos = 0;

void loop() {

    String actual_ServerPath = serverName + "?id=" + idNumber;
    //Quad_Trig.read_Angle();
    Board.read_Angle();
    //String actual_Angle = Quad_Trig.get_Angle_Deg();
    String actual_Angle = Board.get_Angle(degree);
    if(last_Angle != actual_Angle){
        print_Infos = 1;
        last_Angle = actual_Angle;
    }

    /*
     * Inserir método para reajuste de posição do eixo, caso esteja fora da posição correta
     * Caso seja pressionada alguma tecla específica, como #, resetar o posionamento do eixo Quad_Trig.reset();
     */
    
    /*
     * Caso tenha ocorrido alguma alteração de leitura do angulo:
     * imprimir as novas informações no LCD e enviar ao servidor
     */
    if(print_Infos == 1){
        Lcd_Layout();
        actual_ServerPath += "&ang=" + actual_Angle;
        if(actual_Angle.length() < 3){
            actual_Angle += ' ';
        }
        I2C_LCD.setCursor(4,0);
        I2C_LCD.print(actual_Angle);

        //String tangent = Quad_Trig.get_Tangent();
        String tangent = Board.get_Tangent();
        actual_ServerPath += "&tan=" + tangent;
        if(tangent.length() > 5){
            tangent = "nExst";
        } else if(tangent.length() < 5){
            tangent += ' ';
        }
        I2C_LCD.setCursor(11,0);
        I2C_LCD.print(tangent);

        //String sine = Quad_Trig.get_Sine();
        String sine = Board.get_Sine();
        actual_ServerPath += "&sen=" + sine;
        if(sine.length() < 5){
            sine += ' ';
        }
        I2C_LCD.setCursor(2,1);
        I2C_LCD.print(sine);

        //String cosine = Quad_Trig.get_Cosine();
        String cosine = Board.get_Cosine();
        actual_ServerPath += "&cos=" + cosine;
        if(sine.length() < 5){
            sine += ' ';
        }
        I2C_LCD.setCursor(11,1);
        I2C_LCD.print(cosine);

        print_Infos = 0;

        http_get(actual_ServerPath);
    }

}