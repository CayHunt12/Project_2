#include "AnalogIn.h"
#include <fstream>
#include <iostream>
#include <sstream>

AnalogIn::AnalogIn() : number(0) {}

AnalogIn::AnalogIn(unsigned int n) : number(n) {}

void AnalogIn::setNumber(unsigned int n) {
    number = n;
    }

    int AnalogIn::readAdcSample() {
        std::ostringstream path;
        path << "/sys/bus/iio/devices/iio:device0/in_voltage" << number << "_raw";

        std::ifstream adcFile(path.str().c_str());
        if (!adcFile.is_open()) {
                std::cerr << "Failed to open ADC file: " <<
                path.str() << std::endl;
                return -1;
          }

          int value;
          adcFile >> value;
          adcFile.close();
          return value;
        }

        AnalogIn::~AnalogIn() {}
