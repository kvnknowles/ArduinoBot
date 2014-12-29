
//
// Copyright oddWires 2013
// May be freely used by any purchaser of an oddWires Arduino Kit for their sole purpose
//
// Motor Shield Connections						
//					
// Servo Connections					
//	Brown	SER1 -			
//	Red	SER1 +			
//	Orange	SER1 S			
//					
// HC-SR04 Ultrasonic Module					
//					
//	Vcc	+5V			
//	Trig	A4			
//	Echo	A5			
//	GND			
//
// Motor 1
// LEFT motor (looking from rear to the robot
//
// This motor will need the inline connector and some wire extensions to reach the terminals
//
// Black wire to left M1 terminal
// Red wire to right motor terminal
//
// Motor 2
// RIGHT motor (looking from rear to the robot
//
// This motor will need the inline connector and some wire extensions to reach the terminals
//
// Black wire to RIGHT M1 terminal
// Red wire to LEFT motor terminal
//
// 6V 4 x AA Battery Box 
//
// Connect to Blue ternminal marked EXT_PWR
// Be very careful - do NOT connect the RED wire to GND or you could destroy the motor board
// +6 V (Red wire)   M+
// GND (Black wire)  GND
//



// Ultrasonic sensor

#include "NewPing.h"

#define TRIG_PIN  A4  
#define ECHO_PIN  A5  

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.

float dangerThreshold = 20.0; // 20 cm - depends on the floor surface and speed setting
 
// Servo

#include "Servo.h" 

#define LEFT 180
#define CENTER 90
#define RIGHT 0

Servo ultrasonicServo;  // create servo object to control a servo 
 
int currentPos = 0;    // variable to store the servo position 

// Motors
#include "AFMotor.h"

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

// PWM motor control settings applied to ENABLE1, ENABLE2

#define STOP 0
#define SLOW 150
#define MEDIUM 200
#define FAST 255

// run motor at this speed - set to a number between 128 and 255
// less than 128 any the motors may not have enough torque to move
// (of course you could enhance the speed control to accelerate/decelerate when more/less clear space available as indicated by ping)
int throttle = MEDIUM;

// setup
void setup() {
    
    // setup motor
    motor1.setSpeed(SLOW);
    motor2.setSpeed(SLOW);
   
    motor1.run(RELEASE);
    motor2.run(RELEASE);    

    // setup servo
    ultrasonicServo.attach(10,650,1075);  // attaches the servo on pin 5 to the servo object
   
    // move servo to center 
    servo_position(CENTER);  
}

// main loop - note that the loop itself does not block (no waiting in the loop)
// functions that need to block (complete before continuing the main loop) need to manage their own delays
//
// the algorithm used is designed to be simple to understand - you can go on to make this much more sophisticated
// guiding the steering to veer away before getting into danger for example
// and accelerating from a stop and decelerating as you get closer to objects
// and of course you could use more ultrasonic detectors, IR sensors etc. to really fine tune your robot's operation

void loop() {
  
    float distanceForward = ping();
    
    if (distanceForward > dangerThreshold) //if path is clear
    {
        drive_forward();
    }
    else // if path is blocked
    {
        freewheel();  // see if we can implment faster stop
        
        // look left
        servo_position(LEFT);  
        float distanceLeft = ping();
        
        // look right
        servo_position(RIGHT);  
        float distanceRight = ping();
        
        // re-center the ultrasonic
        servo_position(CENTER);
        
        // go the least obstructed way
               
        if (distanceLeft > distanceRight && distanceLeft > dangerThreshold)       //if left is less obstructed 
        {
            rotate_left();
        }
        else if (distanceRight > distanceLeft && distanceRight > dangerThreshold) //if right is less obstructed 
        {
           rotate_right();
        }
        else // equally blocked or less than danger threshold left or right
        {
            u_turn();
        }       
    }
}

// You may need to vary the delays in the drive functions to enable the function to complete

void freewheel(){
    motor1.run(RELEASE);
    motor2.run(RELEASE);
}

void brake(){
    motor1.run(BRAKE);
    motor2.run(BRAKE);
    delay(100); // braking time
}

void drive_forward(){
    motor1.run(FORWARD);
    motor2.run(FORWARD);
}

void drive_backward(){
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
}

void turn_left(){
    motor1.run(RELEASE);
    motor2.run(FORWARD);
    delay(600);
}

void turn_right(){
    motor2.run(RELEASE);
    motor1.run(FORWARD);
    delay(600);
}

void rotate_left(){
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    delay(375);
}

void rotate_right(){
    motor2.run(BACKWARD);
    motor1.run(FORWARD);
    delay(375);
}

void u_turn(){
    motor2.run(BACKWARD);
    motor1.run(FORWARD);
    delay(700); // twice as long as rotate right to end up 180 degrees around
}

// servo control

//void servo_position(int newPos){
//    if (newPos > currentPos){
//        for(int pos=currentPos; pos < newPos; pos += 1)  // goes from 0 degrees to 180 degrees 
//        {                                                // in steps of 1 degree 
//            ultrasonicServo.write(pos);                  // tell servo to go to position in variable 'pos' 
//            delay(15);                                   // waits 15ms for the servo to reach the position 
//        }
//        currentPos = newPos;  
//    }
//    else if (newPos < currentPos){
//        for(int pos=currentPos; pos > newPos; pos -= 1)  // goes from 0 degrees to 180 degrees 
//        {                                                // in steps of 1 degree 
//            ultrasonicServo.write(pos);                  // tell servo to go to position in variable 'pos' 
//            delay(15);                                   // waits 15ms for the servo to reach the position 
//        }
//        currentPos = newPos;  
//    }  
//}

void servo_position(int newPos){
  ultrasonicServo.write(newPos);
  delay(800);
}


// ultrasonic ping

float ping(){
    delay(50);                        // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    unsigned int uS = sonar.ping();   // Send ping, get ping time in microseconds (uS).
    if (uS == 0)                      // out of range (0 = outside set distance range, no ping echo)
        return MAX_DISTANCE;
    else                              // in range
        return uS / US_ROUNDTRIP_CM;  // Convert ping time to distance 
}








