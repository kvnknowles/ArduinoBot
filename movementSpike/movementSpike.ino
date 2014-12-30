#include <ArduinoUnit.h>
#include <AFMotor.h>

#define LEFT 1
#define RIGHT 2

#define STOP 0
#define SLOW 150
#define MEDIUM 200
#define FAST 255

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

class BaseMotor {
  public:
    virtual void setSpeed(uint8_t speed) = 0;
};

class FakeMotor : public BaseMotor {
  public:
    FakeMotor(uint8_t motor)
      : motor(motor),
        direction(RELEASE)
    {
    }
    
    void setSpeed(uint8_t speed) {
      
    }
    
    void run(uint8_t direction) {
      this->direction = direction;
    }
    
    uint8_t getDirection() {
      return direction;
    }
    
  private:
    uint8_t motor;
    uint8_t direction;
};

class Motor : BaseMotor {
  public:
    Motor(uint8_t motor_num)
      : speed(0),
        direction(0)
    {
      AF_DCMotor* m = new AF_DCMotor(motor_num);
      this -> motor = m;
    }
    
    void setSpeed(uint8_t speed)
    {
      this -> speed = speed;
    }
    
    void run(uint8_t direction)
    {
      this -> direction = direction;
      motor -> setSpeed(speed);
      motor -> run(direction); 
    }
    
    uint8_t getDirection()
    {
      return direction;
    }
    
    uint8_t getSpeed()
    {
      return speed;
    }
    
    uint8_t getState()
    {
      if(direction == RELEASE) {
        return RELEASE;
      }
      if(direction == FORWARD && speed > 0) {
        return FORWARD;
      } 
      if(direction == BACKWARD && speed > 0) {
        return BACKWARD;
      }
      return BRAKE;
    }
    
  private:
    AF_DCMotor* motor;
    uint8_t speed;
    uint8_t direction;
};

class BaseController
{
};

class Controller : public BaseController
{
  public:
    Controller() 
    {
      Motor* left = new Motor(LEFT);
      Motor* right = new Motor(RIGHT);
      this -> leftMotor = left;
      this -> rightMotor = right;
    }
    
    void runForward() 
    {
      leftMotor -> setSpeed(FAST);
      leftMotor -> run(FORWARD);
      rightMotor -> setSpeed(FAST);
      rightMotor -> run(FORWARD);
    }
    
    void runBackward() 
    {
      leftMotor -> setSpeed(FAST);
      leftMotor -> run(BACKWARD);
      rightMotor -> setSpeed(FAST);
      rightMotor -> run(BACKWARD);
    }
    
    void runRight()
    {
      leftMotor -> setSpeed(FAST);
      leftMotor -> run(FORWARD);
      rightMotor -> setSpeed(SLOW);
      rightMotor -> run(RELEASE);
    }
    
    void runLeft()
    {
      leftMotor -> setSpeed(SLOW);
      leftMotor -> run(RELEASE);
      rightMotor -> setSpeed(FAST);
      rightMotor -> run(FORWARD);
    }
    
    void runRelease()
    {
      leftMotor -> setSpeed(SLOW);
      leftMotor -> run(RELEASE);
      rightMotor -> setSpeed(FAST);
      rightMotor -> run(RELEASE);
    }
    
    void brake()
    {
      leftMotor -> setSpeed(STOP);
      rightMotor -> setSpeed(STOP);
      leftMotor -> run(BRAKE);
      rightMotor -> run(BRAKE);
    }
    
    
  private:
    Motor* leftMotor;
    Motor* rightMotor;
};

Controller* controller;

void setup() {
    controller = new Controller();  
}

void loop(){

  controller -> runForward();
  delay(2000);
  controller -> brake();

  controller -> runRight();
  delay(2000);
  controller -> brake();

  controller -> runLeft();
  delay(2000);
  controller -> brake();
  
  delay(1000);

  controller -> runBackward();
  delay(2000);
  controller -> brake();
}

