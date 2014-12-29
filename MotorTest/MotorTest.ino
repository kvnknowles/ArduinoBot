#include <ArduinoUnit.h>

class Motor
{
    public:
      Motor() : value(0) {
        
      }
      
      void run(uint8_t direction) {
        if(direction == FORWARD) {
          value = DRIVE_HIGH;
        } else {
          value = DRIVE_LOW;
        }
      }
      
      uint8_t read_pin(uint8_t pin) {
        return value;
      }
    
    static const uint8_t FORWARD = 1;  
    static const uint8_t STOPPED = 0;    
    static const uint8_t MOTORDATA = 8;
    static const uint8_t DRIVE_HIGH = 1;
    static const uint8_t DRIVE_LOW = 0;
      
    private:
      uint8_t value;
};

test(left_motor_forward)
{
  Motor* m = new Motor();
  m -> run(Motor::FORWARD);
  uint8_t actual = m -> read_pin(Motor::MOTORDATA);
  uint8_t expected = Motor::DRIVE_HIGH;
  assertEqual(actual, expected);
}

test(left_motor_stopped)
{
  Motor* m = new Motor();
  m -> run(Motor::STOPPED);
  uint8_t actual = m -> read_pin(Motor::MOTORDATA);
  uint8_t expected = Motor::DRIVE_LOW;
  assertEqual(actual, expected);
}

void setup()
{
  Serial.begin(9600);
  while(!Serial);
}

void loop()
{
  Test::run();
}
