#include "LED.h"
#include <fstream>
#include <iostream>
#include <unistd.h> // for usleep
#include <sstream>

LED::LED(int ledNumber) {
  std::ostringstream path;
  path << "/sys/class/leds/beaglebone:green:usr" << ledNumber << "/";
  ledPath = path.str();
}

void LED::turnOn() {
  std::ofstream file(ledPath + "brightness");
  if (file.is_open()) {
    file << "1";
    file.close();
  } else {
    std::cerr << "Error: Unable to turn on LED" << std::endl;
  }
}

void LED::turnOff() {
  std::ofstream file(ledPath + "brightness");
  if (file.is_open()) {
    file << "0";
    file.close();
  } else {
    std::cerr << "Error: Unable to turn off LED" << std::endl;
  }
}  
void LED::blink(int numBlinks, int delay) {
  for (int i = 0; i < numBlinks; ++i) {
    turnOn();
    usleep(delay * 1000);
    turnOff();
    usleep(delay * 1000);
  }
}  
