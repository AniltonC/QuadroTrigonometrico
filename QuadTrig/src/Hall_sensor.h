class Hall_sensor
{
private:
    int d0_pin, d3_pin, d4_pin;
    bool dig_Pins[3];
    //bool d0, d3, d4;
    int quadrante;
    int angulo;
    String angulo_str;
    float angulo_rad;
public:
    Hall_sensor(int d0_pin, int d3_pin, int d4_pin);

    void reset(void);
    void read_Pins(void);
    int get_Angle(void);
    String get_Str_Angle(void);
    bool in_Zero(void);
    double get_Seno(void);
    String get_Str_Seno(void);
    double get_Cosseno(void);
    String get_Str_Cosseno(void);
    double get_Tangente(void);
    String get_Str_Tangente(void);
};