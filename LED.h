#ifndef LED_H
#define LED_H

#include <string>

class LED {
private:
    std::string ledPath;
public:
    LED(int ledNumber);
    void turnOn();
    void turnOff();
    void blink(int numBlinks, int delay);
};

#endif
                    
