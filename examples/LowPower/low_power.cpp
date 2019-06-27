/*
  ExternalWakeup

  This sketch demonstrates the usage of External Interrupts (on pins) to wakeup a chip in sleep mode.
  Sleep modes allow a significant drop in the power usage of a board while it does nothing waiting for an event to happen. Battery powered application can take advantage of these modes to enhance battery life significantly.

  In this sketch, shorting pin 8 to a GND will wake up the board.
  Please note that, if the processor is sleeping, a new sketch can't be uploaded. To overcome this, manually reset the board (usually with a single or double tap to the RESET button)

  This example code is in the public domain.
*/

#include "ArduinoLowPower.h"

uint8_t unused_pins[] = {0,  1,  3,  5,  6,  7,  8,  9,  10, 11, 12, 14,
                         15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
                         32, 33, 34, 35, 36, 37, 38, 39, 40, 41};

// Blink sequence number
// Declare it volatile since it's incremented inside an interrupt
volatile int repetitions = 1;

// Pin used to trigger a wakeup
const int pin = 2;

void repetitionsIncrease() {
  // This function will be called once on device wakeup
  // You can do some little operations here (like changing variables which will
  // be used in the loop)
  // Remember to avoid calling delay() and long running functions since this
  // functions executes in interrupt context
  repetitions++;
}

void setup() {
  // Give a chance to reflash
  delay(10*1000);

  // Configure all unused pins as input, enabled with built-in pullup
  for (uint8_t i = 0; i < sizeof(unused_pins); i++) {
    pinMode(unused_pins[i], INPUT_PULLUP);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  // Set pin 2 as INPUT (use SPDT switch to avoid spurious wakeup)
  pinMode(pin, INPUT);

  // Disable USB
  USBDevice.detach();
  
  // Attach a wakeup interrupt on pin 2, calling repetitionsIncrease when the device is woken up
  LowPower.attachInterruptWakeup(pin, repetitionsIncrease, CHANGE);
}

void loop() {
  for (int i = 0; i < repetitions; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  delay(1000);
  // Triggers an infinite sleep (the device will be woken up only by the registered wakeup sources)
  // The power consumption of the chip will drop consistently
  LowPower.sleep();
}
