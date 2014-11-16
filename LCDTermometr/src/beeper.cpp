#include "beeper.h"


/**
 * @brief Beeper::Beeper - constructor, initialize Beeper
 * @param digital_pin_id - digital PIN where buzzer is
 *      connected
 */
Beeper::Beeper(int digital_pin_id)
{
    this->pin = digital_pin_id;
    this->is_beeping = false;
    this->mode = BEEPER_MODE::STOP;

    // ensure no beeping
    this->reset();
}


/**
 * @brief Beeper::~Beeper - destructor
 */
Beeper::~Beeper()
{
    this->stop_beep();
}


/**
 * @brief Beeper::beep - method provide sync/asycn beeping
 * @param time - time of beep
 * @param async - default false, if async is set to true
 *      Beeper::update() need to be called repeatelly
 *      in main logic loop for status updates
 */
void Beeper::beep(unsigned int time, bool async)
{
    if (!async)
    {
        pinMode(this->pin, OUTPUT);
        digitalWrite(this->pin, HIGH);
        delay(time);
        digitalWrite(this->pin, LOW);
    }
    else
    {
        this->beep_time = time;
        this->next_change = millis() + time;
        this->mode = BEEPER_MODE::SINGLE;
        this->start_beep();
    }
}


/**
 * @brief Beeper::repeat - method provide async repeat beeping
 * @param time - time for beep/pause (beep and pause have the same time)
 */
void Beeper::repeat(unsigned int time)
{

    /*
     * check beeping time, if the same
     * just return
     */
    if (this->beep_time == time && this->mode == BEEPER_MODE::REPEAT)
    {
        return;
    }

    // reset beeper
    this->reset();

    // setup beep_time
    this->beep_time = time;
    this->mode = BEEPER_MODE::REPEAT;
    this->next_change = millis() + time;
    this->start_beep();
}


/**
 * @brief Beeper::reset - reset beeper to default
 *      method stop beeping, set mode to STOP
 *      also set next_change to 0
 */
void Beeper::reset()
{
    // stop beeping
    this->stop_beep();

    // reset mode
    this->beep_time = 0;
    this->mode = BEEPER_MODE::STOP;
    this->is_beeping = false;
    this->next_change = 0;
}


/**
 * @brief Beeper::update
 */
void Beeper::update()
{
    // check beeping time, if the same
    // just return

    if (this->is_beeping) {
        if (this->mode == BEEPER_MODE::SINGLE) {
            if(millis() > this->next_change) {
                this->reset();
            }
        }
        if (this->mode == BEEPER_MODE::REPEAT) {
            if(millis() > this->next_change) {
                this->next_change += this->beep_time;
                this->stop_beep();
            }
        }
    } else {
        // only handle repeat_mode, single mode beeping
        // is initialized when called
        if (this->mode == BEEPER_MODE::REPEAT) {
            if(millis() > this->next_change) {
                this->next_change += this->beep_time;
                this->start_beep();
            }
        }
    }
}


/**
 * @brief Beeper::start_beep
 */
void Beeper::start_beep()
{
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, HIGH);
    this->is_beeping = true;
}


/**
 * @brief Beeper::stop_beep
 */
void Beeper::stop_beep()
{
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, LOW);
    this->is_beeping = false;
}
