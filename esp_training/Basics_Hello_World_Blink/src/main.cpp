/**
 * 
 * Hello World Blink
 * 
 * What you need:
 * - Basic software environment (VSC, PlatformIO)
 * - Basic hardware requirements (ESP32, Breadboard, MicroUSB)
 * - LED, Wire
 * 
 * 
 * #Part 1: Use Variables and Serial Monitor
 * Print out some static variables via serial monitor
 * 
 * 
 * #Part 2: Blink LED continuously
 * Connect led to board (e.g. via pin 21). 
 * Note: make sure to connect led's cathode to ground!
 * Make led shine.
 * Then, write some lines of code to blink the led continuously.
 * 
 * 
 * #Part 3: Touch sensor
 * Connect wire to touch pin (e.g. on Touch0 ~ pin 24)
 * Print touch sensor value to serial monitor
 * 
 * 
 * #Part 4: Touch to Blink
 * Write some lines of code to switch led on
 * only if wire is touched
 * 
 *  
*/

#include <Arduino.h>

int LED_R = 32;
int LED_G = 33;
int LED_B = 25;

void setup()
{
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  Serial.begin(115200);
}

void loop()
{
  Serial.println("LED Color: RED");
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  delay(1000);

  Serial.println("LED Color: BLUE");
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, LOW);
  delay(1000);

  Serial.println("LED Color: GREEN");
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, HIGH);
  delay(1000);
}