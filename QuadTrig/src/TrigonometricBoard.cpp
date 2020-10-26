/*
||
|| @file TrigonometricBoard.cpp
|| @version 1.1
|| @author Anilton Carlos
|| @contact aclr@icomp.ufam.edu.br / aniltoncarlos9@gmail.com
||
|| @description
|| | This is a basic implementation of the Trigonometric Board library.
|| #
||
*/
#include <TrigonometricBoard.h>

TrigonometricBoard::TrigonometricBoard()
{
    pin_addr.D0 = 16;
    pin_addr.D3 = 0;
    pin_addr.D4 = 2;
}

void TrigonometricBoard::begin(Mode count_mode = simple)
{
    this->count_mode = count_mode;
    pinMode(pin_addr.D0, INPUT);
    pinMode(pin_addr.D3, INPUT);
    pinMode(pin_addr.D4, INPUT);
}

void TrigonometricBoard::reset()
{
    angle.axis_pos = a00;
    angle.quadrant = 0;
    angle.degree = 0;
    angle.radian = 0;
    angle.lap = 0;
}

void TrigonometricBoard::change_Mode()
{
    if (count_mode == simple)
        count_mode = continuous;
    else
        count_mode = simple;
}

void TrigonometricBoard::read_Pins()
{
    pin_value.D0 = digitalRead(pin_addr.D0);
    pin_value.D3 = digitalRead(pin_addr.D3);
    pin_value.D4 = digitalRead(pin_addr.D4);
}

/*
 * Value present in the GPIO address.
 * Sensor Reading Table:
 *      No Reading      - 111
 * 00 | 090 | 180 | 270 - 110
 * 15 | 105 | 195 | 285 - 101
 * 30 | 120 | 210 | 300 - 100
 * 45 | 135 | 225 | 315 - 011
 * 60 | 150 | 240 | 330 - 010
 * 75 | 165 | 255 | 345 - 001
 */
int TrigonometricBoard::axis_Position()
{
    read_Pins();

    if (pin_value.D0 == 1)
    {
        if (pin_value.D3 == 1)
        {
            if (pin_value.D4 == 1)
                return -1;
            else
                angle.axis_pos = a00;
        }
        else
        {
            if (pin_value.D4 == 1)
                angle.axis_pos = a15;
            else
                angle.axis_pos = a30;
        }
    }
    else
    {
        if (pin_value.D3 == 1)
        {
            if (pin_value.D4 == 1)
                angle.axis_pos = a45;
            else
                angle.axis_pos = a60;
        }
        else
        {
            if (pin_value.D4 == 1)
                angle.axis_pos = a75;
        }
    }

    return angle.axis_pos;
}

void TrigonometricBoard::read_Angle()
{
    int last_degree = angle.degree;
    int last_pos = angle.axis_pos;
    axis_Position();
    if (count_mode == continuous)
    {
        if (last_pos == 75)
        {
            if (angle.axis_pos == 0)
            {
                if (last_degree >= 75)
                {
                    if (angle.quadrant == 3)
                    {
                        angle.quadrant = 0;
                        angle.lap++;
                    }
                    else
                        angle.quadrant++;
                }
                else if (last_degree == -15)
                    angle.quadrant++;
                else if (last_degree < -15)
                {
                    if (angle.quadrant == -1)
                    {
                        angle.quadrant = -4;
                        angle.lap++;
                    }
                    else
                        angle.quadrant++;
                }
            }
        }
        else if (last_pos == 0)
        {
            if (angle.axis_pos == 75)
            {
                if (last_degree == 0)
                    angle.quadrant--;
                else if (last_degree >= 75)
                {
                    if (angle.quadrant == 0)
                    {
                        angle.quadrant = 3;
                        angle.lap--;
                    }
                    else
                        angle.quadrant--;
                }
                else if (last_degree <= -75)
                {
                    if (angle.quadrant == -4)
                    {
                        angle.quadrant = -1;
                        angle.lap--;
                    }
                    else
                    {
                        angle.quadrant--;
                    }
                }
            }
        }
        angle.degree = angle.axis_pos + angle.quadrant * 90 + angle.lap * 360;
    }
    else
    {
        if (last_pos == 75)
        {
            if (angle.axis_pos == 0)
            {
                if (angle.quadrant == 3)
                    angle.quadrant = 0;
                else
                    angle.quadrant++;
            }
        }
        else if (last_pos == 0)
        {
            if (angle.axis_pos == 75)
            {
                if (angle.quadrant == 0)
                    angle.quadrant = 3;
                else
                    angle.quadrant--;
            }
        }
        angle.degree = angle.axis_pos + angle.quadrant * 90;
    }

    angle.radian = (angle.degree * PI) / 180.0;
}

String TrigonometricBoard::get_Angle(Unit_Measurement unit = degree)
{
    if (unit == degree)
        return String(angle.degree);
    else
        return String(angle.radian, 2);
}

String TrigonometricBoard::get_Sine()
{
    String saida = String(sin(angle.radian), 2);
    if (saida == "-0.00")
    {
        saida = "0.00";
    }
    return String(saida);
}

String TrigonometricBoard::get_Cosine()
{
    String saida = String(cos(angle.radian), 2);
    if (saida == "-0.00")
    {
        saida = "0.00";
    }
    return String(saida);
}

String TrigonometricBoard::get_Tangent()
{
    int abs_angle = angle.degree;
    if (count_mode == continuous)
    {
        if (abs_angle < 0)
            abs_angle *= -1;
        abs_angle -= angle.lap * 360;
    }
    if (abs_angle == 90 || abs_angle == 270)
        return "nExiste";
    else
        return String(tan(angle.radian), 2);
}
