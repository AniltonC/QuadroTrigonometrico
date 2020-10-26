/*
||
|| @file TrigonometricBoard.h
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

/*
 * Operation mode flags:
 * Simple - Count from 0° to 360°.
 * Continuous - The whole set of integers.
 */
enum Mode
{
    simple = 0x0,
    continuous = 0x1
};

enum Axis_Position
{
    a00 = 0,
    a15 = 15,
    a30 = 30,
    a45 = 45,
    a60 = 60,
    a75 = 75
};

enum Unit_Measurement
{
    degree = 0,
    radian = 1
};

//GPIO Address of Sensor Pinout
struct Pin_Address
{
    unsigned int D0;
    unsigned int D3;
    unsigned int D4;
};

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
struct Pin_Value
{
    bool D0;
    bool D3;
    bool D4;
};

struct Angle
{
    // Quadrant in which the axis is located.
    int quadrant;
    // Angle from 0 to 75 where the axis is.
    Axis_Position axis_pos;
    // Angle in Degrees.
    int degree;
    // Angle in Radians.
    float radian;
    // Count of complete laps.
    int lap;
};

class TrigonometricBoard
{
private:
    Pin_Address pin_addr;
    Pin_Value pin_value;
    Mode count_mode;
    Angle angle;
    
    /*!
     @method
     @abstract   Reads the values of the GPIO pins.
     @discussion Reads the value of the GPIO pins to assess the position of the axis.
     */
    void read_Pins();

public:
    /*!
     @method
     @abstract   Class constructor.
     @discussion Initializes class variables and defines the Sensors address of the
     Trigonometric Table. The constructor does not initialize the Sensors.
     */
    TrigonometricBoard();

    /*!
     @function
     @abstract   Position of the Axis.
     @discussion Returns the quadrant angle that the axis is located.
     */
    int axis_Position();

    /*!
     @function
     @abstract   Board inicialization.
     @discussion Initializes the board with a certain mode of operation. 


     @param      count_mode[in] Operating mode of the board.
     Simple: counting from 0 to 360 degrees.
     Continuous: counting from -1800 to 1800 degrees, starting at 0 °.
     */
    void begin(Mode count_mode);

    /*!
     @method
     @abstract   Board reset.
     @discussion Reset the board reading values.
     */
    void reset();

    /*!
     @method
     @abstract   New operating mode.
     @discussion Changes the operating mode of the board.
     */
    void change_Mode();

    /*!
     @method                                                                                                                
     @abstract   Angle of the board.
     @discussion Performs the angle reading according to the selected mode.
     */
    void read_Angle();

    /*!
     @method                                                                                                                
     @abstract   Angle of the board.
     @discussion Returns the angle where the axis is located.


     @param      unit[in] Unit of measurement of the output.
     */
    String get_Angle(Unit_Measurement unit);

    /*!
     @method                                                                                                                
     @abstract   Sine of the last angle.
     @discussion Returns the sine of the last angle found by the read_Angle() function.
     */
    String get_Sine(void);

    /*!
     @method                                                                                                                
     @abstract   Cosine of the last angle.
     @discussion Returns the cosine of the last angle found by the read_Angle() function.
     */
    String get_Cosine(void);

    /*!
     @method                                                                                                                
     @abstract   Tangent of the last angle.
     @discussion Returns the tangent of the last angle found by the read_Angle() function.
     */
    String get_Tangent(void);
};