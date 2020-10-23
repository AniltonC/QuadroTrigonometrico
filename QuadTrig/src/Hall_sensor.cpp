/*
||
|| @file Hall_sensor.cpp
|| @version 1.0
|| @author Anilton Carlos de Lima Reis
|| @contact aclr@icomp.ufam.edu.br
||
|| @description
|| | Esta é uma implementação básica da biblioteca Hall Sensor para Quadro Trigonométrico.
|| #
||
*/

/*
 * Tabela de Leitura dos Sensores
 *  SEM LEITURA   - 111
 *  0| 90|180|270 - 110
 * 15|105|195|285 - 101
 * 30|120|210|300 - 100
 * 45|135|225|315 - 011
 * 60|150|240|330 - 010
 * 75|165|255|345 - 001
 */

#include "Arduino.h"
#include "Hall_sensor.h"

Hall_sensor::Hall_sensor(int d0_pin, int d3_pin, int d4_pin)
{
    this->d0_pin = d0_pin;
    this->d3_pin = d3_pin;
    this->d4_pin = d4_pin;
}

void Hall_sensor::reset()
{
    this->quadrant = 0;
    this->angle = 0;
    this->angle_Deg = 0;
    this->angle_Rad = 0;
}

void Hall_sensor::read_Pins()
{
    this->dig_Pins[0] = digitalRead(d0_pin);
    this->dig_Pins[1] = digitalRead(d3_pin);
    this->dig_Pins[2] = digitalRead(d4_pin);
}

int Hall_sensor::get_Ang_Ajuste()
{
    read_Pins();

    if (this->dig_Pins[0] == 1)
    {
        if (this->dig_Pins[1] == 1)
        {
            if (this->dig_Pins[2] == 1)
            {
                return -1;
            }
            else
            {
                this->angle = 0; // 0°
            }
        }
        else
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle = 15; // 15°
            }
            else
            {
                this->angle = 30; // 30°
            }
        }
    }
    else
    {
        if (this->dig_Pins[1] == 1)
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle = 45; // 45°
            }
            else
            {
                this->angle = 60; // 60°
            }
        }
        else
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle = 75; // 75°
            }
        }
    }

    return this->angle;
}

void Hall_sensor::read_Angle()
{
    read_Pins();

    int last_angle = this->angle;

    if (this->dig_Pins[0] == 1)
    {
        if (this->dig_Pins[1] == 1)
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle_Deg = (last_angle + quadrant * 90);
                return;
            }
            else
            {
                this->angle = 0; // 0°
            }
        }
        else
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle = 15; // 15°
            }
            else
            {
                this->angle = 30; // 30°
            }
        }
    }
    else
    {
        if (this->dig_Pins[1] == 1)
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle = 45; // 45°
            }
            else
            {
                this->angle = 60; // 60°
            }
        }
        else
        {
            if (this->dig_Pins[2] == 1)
            {
                this->angle = 75; // 75°
            }
        }
    }

    if (last_angle == 75 && this->angle == 0)
    {
        if (this->quadrant == 3)
        {
            this->quadrant = 0;
        }
        else
        {
            this->quadrant++;
        }
    }
    else if (last_angle == 0 && this->angle == 75)
    {
        if (this->quadrant == 0)
        {
            this->quadrant = 3;
        }
        else
        {
            this->quadrant--;
        }
    }

    this->angle_Deg = this->angle + this->quadrant * 90;
    this->angle_Rad = (float(this->angle_Deg) * PI) / 180.0;
}

String Hall_sensor::get_Angle_Deg()
{
    String saida = String(this->angle_Deg);
    return String(saida);
}

String Hall_sensor::get_Angle_Rad()
{
    String saida = String(this->angle_Rad, 2);
    return String(saida);
}

String Hall_sensor::get_Sine()
{
    String saida = String(sin(this->angle_Rad), 2);
    if (saida == "-0.00")
    {
        saida = "0.00";
    }
    return String(saida);
}

String Hall_sensor::get_Cosine()
{
    String saida = String(cos(this->angle_Rad), 2);
    if (saida == "-0.00")
    {
        saida = "0.00";
    }
    return String(saida);
}

String Hall_sensor::get_Tangent()
{
    if (this->angle_Deg == 90 || this->angle_Deg == 270)
    {
        return "nExiste";
    }
    else
    {
        String saida = String(tan(this->angle_Rad), 2);
        return String(saida);
    }
}
