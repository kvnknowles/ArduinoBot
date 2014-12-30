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

class FakeController : public BaseController
{
  public:
    FakeController()
      : state(0)
    {
    }
    
    void runForward()
    {
      state = FORWARD;
    }
    
    uint8_t getState()
    {
      return state;
    }
   
   private:
     uint8_t state;
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
    
    uint8_t getState()
    {  
      if(isForward())
      {
        return FORWARD;
      }
      if(isRight())
      {
        return RIGHT;
      } 
      if(isLeft())
      {
        return LEFT;
      }  
      if(isBackward())
      {
        return BACKWARD;
      }
      return BRAKE;
    }
    
    boolean isForward()
    {
      uint8_t leftMotorState = leftMotor -> getState();
      uint8_t rightMotorState = rightMotor -> getState();
      
      return leftMotorState == FORWARD && rightMotorState == FORWARD;
    }
    
    boolean isRight()
    {
      uint8_t leftMotorState = leftMotor -> getState();
      uint8_t rightMotorState = rightMotor -> getState();
      
      return leftMotorState == FORWARD && rightMotorState == RELEASE;
    }
    
    boolean isLeft()
    {
      uint8_t leftMotorState = leftMotor -> getState();
      uint8_t rightMotorState = rightMotor -> getState();
      
      return leftMotorState == RELEASE && rightMotorState == FORWARD;
    }
    
    boolean isBackward()
    {
      uint8_t leftMotorState = leftMotor -> getState();
      uint8_t rightMotorState = rightMotor -> getState();
      
      return leftMotorState == BACKWARD && rightMotorState == BACKWARD;
    }
    
  private:
    Motor* leftMotor;
    Motor* rightMotor;
};

test(drive_forward)
{
  FakeController* controller = new FakeController();
  controller -> runForward();
  assertEqual(controller -> getState(), FORWARD);
}

test(drive_forward_real_for_two_seconds)
{
  Controller* controller = new Controller();
  controller -> runForward();
  uint8_t actualState = controller -> getState();
  assertEqual(actualState, FORWARD);
  delay(2000);
  controller -> brake();
}

test(drive_right_real_for_two_seconds)
{
  Controller* controller = new Controller();
  controller -> runRight();
  uint8_t actualState = controller -> getState();
  assertEqual(actualState, RIGHT);
  delay(2000);
  controller -> brake();
}

test(drive_left_real_for_two_seconds)
{
  Controller* controller = new Controller();
  controller -> runLeft();
  uint8_t actualState = controller -> getState();
  assertEqual(actualState, LEFT);
  delay(2000);
  controller -> brake();
}

test(drive_backward_real_for_two_seconds)
{
  Controller* controller = new Controller();
  controller -> runBackward();
  uint8_t actualState = controller -> getState();
  assertEqual(actualState, BACKWARD);
  delay(2000);
  controller -> brake();
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
