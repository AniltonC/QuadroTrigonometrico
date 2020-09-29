class Hall_sensor
{
private:
    int d0_pin, d3_pin, d4_pin;
    bool dig_Pins[3];
    int quadrant;
    int angle;
    int angle_Deg;
    float angle_Rad;
public:
    Hall_sensor(int d0_pin, int d3_pin, int d4_pin);

    void reset(void);
    void read_Pins(void);
    int get_Ang_Ajuste(void);
    void read_Angle(void);

    String get_Angle_Deg(void);
    String get_Angle_Rad(void);
    String get_Sine(void);
    String get_Cosine(void);
    String get_Tangent(void);
};