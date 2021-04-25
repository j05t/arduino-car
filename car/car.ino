// code adapted from tutorials at www.elegoo.com

#include <IRremote.h>
#define RECV_PIN  11        //Infrared signal receiving pin
#define LED       13        //define LED pin
IRrecv irrecv(RECV_PIN);
decode_results results;

// enable must be PWM capable to set speed 0-255
// port 3 might conflict with IR library
#define ENA 9
#define IN1 8
#define IN2 7

#define ENB 6
#define IN3 2
#define IN4 4

#define LEFT 92
#define RIGHT 91
#define FORWARD 90
#define BACKWARD 89

void setup() {
  //---set pin direction
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // motor 1 off
  analogWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  // motor 2 off
  analogWrite(ENB, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // IR receiver
  pinMode(LED, OUTPUT); //initialize LED as an output
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}


void loop() {

  ir_rcv();
  // motor_test() ;
}

void setSpeed(const byte side,
              const byte speed,
              const byte direction) {
  if (side == LEFT) {
    if (direction == FORWARD) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
    analogWrite(ENA, speed);
  } else {
    if (direction == FORWARD) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    analogWrite(ENB, speed);
  }
}

// minimum speed for motor is about 120
void motor_test() {
  setSpeed(LEFT, 120, FORWARD);
  setSpeed(RIGHT, 120, FORWARD);

  delay(1000);

  setSpeed(LEFT, 120, BACKWARD);
  setSpeed(RIGHT, 120, BACKWARD);

  delay(1000);

  setSpeed(LEFT, 0, BACKWARD);
  setSpeed(RIGHT, 0, BACKWARD);

  delay(2000);
}

byte y = 0;
byte dir = FORWARD;
unsigned int emptyLoops = 0;

void ir_rcv() {
  if (irrecv.decode(&results)) {
    if ( results.value >= 0 && results.value < 256) {
      digitalWrite( LED, HIGH );
      y = results.value;

      if (y >= 127) {
        dir = FORWARD;
        y -= 127; // center position
      } else if (y < 127) {
        dir = BACKWARD;
        y = 127 - y;
      }
      setSpeed(RIGHT, y, dir);
      setSpeed(LEFT, y, dir);

      emptyLoops = 0;
    } else {
      digitalWrite( LED, LOW );
    }
    irrecv.resume();        // prepare to receive the next value
  } else { // decelerate if no signal received
    emptyLoops++;

    if (y > 0 && emptyLoops > 5000) {
      y--;
      setSpeed(RIGHT, y, dir);
      setSpeed(LEFT, y, dir);
      Serial.print(emptyLoops);
      Serial.println(" , slowing down");
    }
  }

  //Serial.println(y);
}
