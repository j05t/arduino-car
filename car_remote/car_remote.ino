// most code from www.elegoo.com

#define SW_pin 2 // digital pin connected to switch output
#define X_pin  1 // analog pin connected to X output
#define Y_pin  0 // analog pin connected to Y output
uint32_t data = 0x0;

//An IR LED must be connected to Arduino PWM pin 3.
#include <IRremote.h>
#include <IRremote.h>
IRsend irsend; // we need to have version 2.5 of this library installed!
void setup()
{
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
}

void loop() {
  // void IRsend::sendRC5(uint32_t data, uint8_t nbits)
  // 32 bits available, we send x,y mapped to 128-255 + 1 bit per direction L/R
  // joystick delivers values between 0 to 1024 for x/y

  //data = 0x0;

  unsigned int x, y;
  x = analogRead(X_pin);
  y = analogRead(Y_pin);

  byte speedY = map(y, 0, 1023, 255, 0);

  Serial.print(speedY);
  Serial.print("\n");

  irsend.sendRC5(speedY, 8);
  delay(200);
}
