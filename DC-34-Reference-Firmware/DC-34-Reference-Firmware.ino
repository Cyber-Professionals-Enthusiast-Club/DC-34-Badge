/** DC-34 Reference Firmware demonstrates how to drive the hardware on the
 *  CPEC DEFCON 34 Mech Fighter's badge. 
 * 
 * Developers should program their Arduino IDE to progam for the ESP32-S3-MINI-1
 * 
 * @author Ministry
 * @version 06 June 2026
 */

#include <Arduino.h>

/* Const values (wont change */
// Allocated on the .text section

// This definition maps the LEDs to their respective IO Pins 
const uint16_t LED_TORSO_YELLOW     = 3;
const uint16_t LED_TORSO_RED        = 6;
const uint16_t LED_ARM_LEFT_RED     = 12;
const uint16_t LED_ARM_LEFT_YELLOW  = 13;
const uint16_t LED_HEAD_YELLOW      = 15;
const uint16_t LED_HEAD_RED         = 16;
const uint16_t LED_LEG_RIGHT_YELLOW = 39;
const uint16_t LED_LEG_RIGHT_RED    = 40;
const uint16_t LED_LEG_LEFT_RED     = 41;
const uint16_t LED_LEG_LEFT_YELLOW  = 42;
const uint16_t LED_ARM_RIGHT_RED    = 45;
const uint16_t LED_ARM_RIGHT_YELLOW = 46;

// This definition maps the buttons to pins 
const uint16_t BUTTON_SELECT = 1;
const uint16_t BUTTON_START = 2;
const uint16_t BUTTON_UP = 7;
const uint16_t BUTTON_LEFT = 8;
const uint16_t BUTTON_RIGHT = 9;
const uint16_t BUTTON_DOWN = 10;
const uint16_t BUTTON_A = 11;
const uint16_t BUTTON_B = 14;

// PWM Pins
const uint16_t PWM_RUMBLE = 17;
const uint16_t PWM_BACKLIGHT = 47;



/* Variables that will change: */
// Allocated in .bss section 
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

int RUMBLE_DUTY = 0;
int BACKLIGHT_DUTY = 128;

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

/* Function Calls */

// This interrupt triggers when a button is pressed 
void ARDUINO_ISR_ATTR buttonISR () {
    static unsigned long lastUpDownButtonPress;
    int upState = digitalRead(BUTTON_UP);
    int downState = digitalRead(BUTTON_DOWN);

    // Scale up or down the intensity of the backlight 
    if ( upState != downState)
    {
        // Last button Press
        unsigned long currentUpDownButtonPress = millis();
        if (currentUpDownButtonPress - lastUpDownButtonPress > 100)
        {
            if (upState == HIGH) {
                if (BACKLIGHT_DUTY < 256)
                {
                    BACKLIGHT_DUTY += 1;
                }
            }
            else {
                if (BACKLIGHT_DUTY > 0)
                {
                    BACKLIGHT_DUTY -= 1;
                }
            }
            analogWrite(PWM_BACKLIGHT, BACKLIGHT_DUTY);
        }
        lastUpDownButtonPress = millis();
    }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(LED_TORSO_YELLOW, OUTPUT);
    pinMode(LED_TORSO_RED, OUTPUT);
    pinMode(LED_ARM_LEFT_RED, OUTPUT);
    pinMode(LED_ARM_LEFT_YELLOW, OUTPUT);
    pinMode(LED_HEAD_YELLOW, OUTPUT);
    pinMode(LED_HEAD_RED, OUTPUT);
    pinMode(LED_LEG_RIGHT_YELLOW, OUTPUT);
    pinMode(LED_LEG_RIGHT_RED, OUTPUT);
    pinMode(LED_LEG_LEFT_RED, OUTPUT);
    pinMode(LED_LEG_LEFT_YELLOW, OUTPUT);
    pinMode(LED_ARM_RIGHT_RED, OUTPUT);
    pinMode(LED_ARM_RIGHT_YELLOW, OUTPUT); 

    pinMode(BUTTON_SELECT, INPUT);
    pinMode(BUTTON_START, INPUT);
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_LEFT, INPUT);
    pinMode(BUTTON_RIGHT, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_A, INPUT);
    pinMode(BUTTON_B, INPUT);

    attachInterrupt(BUTTON_SELECT, buttonISR, CHANGE);
    attachInterrupt(BUTTON_START, buttonISR, CHANGE);
    attachInterrupt(BUTTON_UP, buttonISR, CHANGE);
    attachInterrupt(BUTTON_LEFT, buttonISR, CHANGE);
    attachInterrupt(BUTTON_RIGHT, buttonISR, CHANGE);
    attachInterrupt(BUTTON_DOWN, buttonISR, CHANGE);
    attachInterrupt(BUTTON_A, buttonISR, CHANGE);
    attachInterrupt(BUTTON_B, buttonISR, CHANGE);

    pinMode(PWM_RUMBLE, OUTPUT);
    pinMode(PWM_BACKLIGHT, OUTPUT);

    analogWrite(PWM_RUMBLE, RUMBLE_DUTY);
    analogWrite(PWM_BACKLIGHT, BACKLIGHT_DUTY);
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;

        // if the LED is off turn it on and vice-versa:
        if (ledState == LOW) {
            ledState = HIGH;
        } else {
            ledState = LOW;
        }

        digitalWrite(LED_TORSO_YELLOW, ledState);
        digitalWrite(LED_TORSO_RED, ledState);
        digitalWrite(LED_ARM_LEFT_RED, ledState);
        digitalWrite(LED_ARM_LEFT_YELLOW, ledState);
        digitalWrite(LED_HEAD_YELLOW, ledState);
        digitalWrite(LED_HEAD_RED, ledState);
        digitalWrite(LED_LEG_RIGHT_YELLOW, ledState);
        digitalWrite(LED_LEG_RIGHT_RED, ledState);
        digitalWrite(LED_LEG_LEFT_RED, ledState);
        digitalWrite(LED_LEG_LEFT_YELLOW, ledState);
        digitalWrite(LED_ARM_RIGHT_RED, ledState);
        digitalWrite(LED_ARM_RIGHT_YELLOW, ledState); 
    }
}

