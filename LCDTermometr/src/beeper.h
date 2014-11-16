#ifndef BEEPER_H
#define BEEPER_H

#include <Arduino.h>

namespace BEEPER_MODE
{
    enum BMODE {
        STOP = 0,
        SINGLE = 1,
        REPEAT = 2
    };
}

class Beeper
{
public:
    // constructor
    Beeper(int digital_pin_id=6);

    // copy constructor, disallowed
    Beeper(const Beeper&){}

    // destructor
    ~Beeper();

    // Beep for miliseconds

    // might by sync or async
    void beep(unsigned int time, bool async=false);

    // only async
    void repeat(unsigned int time);


    void update();

    // stop beeping
    void reset();

protected:
    void start_beep();
    void stop_beep();

private:
    bool is_beeping;
    int pin;
    BEEPER_MODE::BMODE mode;
    unsigned int beep_time;
    unsigned long next_change;
};

#endif // BEEPER_H
