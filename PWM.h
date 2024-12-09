#ifndef PWM_H_
#define PWM_H_
#include<string>
using std::string;


// Changes made by Amer Qouneh:
// Changed old path and names to the new names used in 
// image verison 9.5 used in the textbook. 
// The author's code did not work with the new names.
//

#define PWM_PATH "/sys/class/pwm/"
//#define PWM_PATH "/sys/devices/ocp.3/" // old path

#define PWM_PERIOD "period"

//#define PWM_DUTY "duty" // maybe old name
#define PWM_DUTY "duty_cycle"

#define PWM_POLARITY "polarity"

//#define PWM_RUN "run"  // old name
#define PWM_RUN "enable"



namespace exploringBB {

class PWM {
public:
  enum POLARITY{ ACTIVE_HIGH=0, ACTIVE_LOW=1 };
private:
  string name, path;
  float analogFrequency;  //defaults to 100,000 Hz
  float analogMax;        //defaults to 3.3V

public:
  PWM(string pinName);

  virtual int setPeriod(unsigned int period_ns);
  virtual unsigned int getPeriod();
  virtual int setFrequency(float frequency_hz);
  virtual float getFrequency();
  virtual int setDutyCycle(unsigned int duration_ns);
  virtual int setDutyCycle(float percentage);
  virtual unsigned int getDutyCycle();
  virtual float getDutyCyclePercent();

  virtual int setPolarity(PWM::POLARITY);
  virtual void invertPolarity();
  virtual PWM::POLARITY getPolarity();

  virtual void setAnalogFrequency(float frequency_hz) { this->analogFrequency = frequency_hz; }
  virtual int calibrateAnalogMax(float analogMax); //must be between 3.2 and 3.4
  virtual int analogWrite(float voltage);

  virtual int run();
  virtual bool isRunning();
  virtual int stop();

  virtual ~PWM();
private:
  float period_nsToFrequency(unsigned int);
  unsigned int frequencyToPeriod_ns(float);
};
}

#endif /* PWM_H_ */
