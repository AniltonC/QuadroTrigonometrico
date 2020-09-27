/*
 * Tabela de Leitura dos Sensores
 *  SEM LEITURA   - 111
 *  0| 90|180|270 - 110
 * 15|105|195|285 - 101
 * 30|120|210|300 - 100
 * 45|135|225|315 - 011
 * 60|150|240|330 - 010
 * 75|165|255|345 - 001
 *
 */

#include "Arduino.h"
#include "Hall_sensor.h"
#include "stdlib.h"


Hall_sensor::Hall_sensor(int d0_pin, int d3_pin, int d4_pin) {
    this->d0_pin = d0_pin;
    this->d3_pin = d3_pin;
    this->d4_pin = d4_pin;
}

void Hall_sensor::reset() {
    this->quadrante = 0;
    this->angulo = 0;
}

bool Hall_sensor::in_Zero(){
    if(dig_Pins[0] == 1 && dig_Pins[1] == 1 && dig_Pins[2] == 0) {
        return true;
    }
    return false;
}

void Hall_sensor::read_Pins() {
    this->dig_Pins[0]= digitalRead(d0_pin);
    this->dig_Pins[1] = digitalRead(d3_pin);
    this->dig_Pins[2] = digitalRead(d4_pin);
}

int Hall_sensor::get_Angle() {
    int last_angle = this->angulo;

    if (this->dig_Pins[0] == 1) {
        if (this->dig_Pins[1] == 1) {
            if (this->dig_Pins[2] == 1) {
                return (last_angle + quadrante*90);
            }
            else {
                this->angulo = 0; // 0°
            }
        }
        else {
            if (this->dig_Pins[2] == 1) {
                this->angulo = 15; // 15°
            }
            else { 
                this->angulo = 30; // 30°
            }
        }
    } else {
        if (this->dig_Pins[1] == 1) {
            if (this->dig_Pins[2] == 1) {
                this->angulo = 45; // 45°
            }
            else {
                this->angulo = 60; // 60°
            }
        }
        else {
            if (this->dig_Pins[2] == 1) {
                this->angulo = 75; // 75°
            }
        }
    }

    if(last_angle == 75 && this->angulo == 0) {
        if(this->quadrante == 3) {
            this->quadrante = 0;
        }
        else {
            this->quadrante++;
        }
    }
    else if(last_angle == 0 && this->angulo == 75) {
        if(this->quadrante == 0) {
            this->quadrante = 3;
        }
        else {
            this->quadrante--;
        }
    }
    int angulo_quad = this->angulo + this->quadrante * 90;
    this->angulo_rad = (float (angulo_quad) * PI ) / 180.0;

    char angle_str[4];
    itoa(angulo_quad, angle_str, 10);
    this->angulo_str = String(angle_str);

    return angulo_quad;
}

double Hall_sensor::get_Seno(){
    return sin(this->angulo_rad);
}

String Hall_sensor::get_Str_Angle(){
    char out[50];
    sprintf(out, "%.2f", sin(this->angulo_rad));
    return String(out);
}

double Hall_sensor::get_Cosseno(){
    return cos(this->angulo_rad);
}

String Hall_sensor::get_Str_Cosseno(){
    char out[50];
    sprintf(out, "%.2f", cos(this->angulo_rad));
    return String(out);
}

double Hall_sensor::get_Tangente(){
    return tan(this->angulo_rad);
}

String Hall_sensor::get_Str_Tangente(){
    if(this->angulo == 90){
        return "Não existe.";
    } else{
        char out[50];
        sprintf(out, "%.2f", tan(this->angulo_rad));
        return String(out);
    }
}