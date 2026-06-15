/** DC-34 Reference Firmware demonstrates how to drive the hardware on the
 *  CPEC DEFCON 34 Mech Fighter's badge. 
 * 
 * Developers should program their Arduino IDE to progam for the ESP32-S3-MINI-1
 * 
 * @author Ministry
 * @version 06 June 2026
 */

// Code is built for the Arduino IDE 1.8.5
// In the Arduino IDE Install these additional board managers 
// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// File > Preferences (then paste into Additional Board Manager URLs)
// 
// Then, before verifying, install the following libraries:
// Sketch > Include Library > Manage Libraries... 
// - Adafruit seesaw Library
// - Adafruit BusIO 
// - Adafruit ST7735 and ST7789 Library
// - Adafruit GFX Library 
// - Adafruit ILI9341
// 
// Before uploading, select the following
// Tools > Board: "Adafruit Feather ESP32-S3 No PSRAM"
// 

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Query-SAO.h"

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire

//#define DEBUG_ENABLE 0

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
const uint16_t BUZZER_OUT = 33;

// Pin definitions (match wiring)
const uint16_t  TFT_SCLK = 36;
const uint16_t  TFT_MISO = 37;
const uint16_t  TFT_MOSI = 35;
const uint16_t  TFT_DC   = 26; // WRB
const uint16_t  TFT_CS   = 38;
const uint16_t  TFT_RST  = 21;

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
const uint8_t PWM_RUMBLE = 17;
const uint8_t PWM_BACKLIGHT = 47;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC,
      TFT_MOSI, TFT_SCLK, TFT_RST, -1);

/* Variables that will change: */
// Allocated in .bss section 
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousSaoMillis = 0;

int RUMBLE_DUTY = 0;
int BACKLIGHT_DUTY = 128;

// constants won't change:
const long interval = 1000;           // interval at which to blink LES (milliseconds)
const long saoInterval = 1333;        // Interval to check SAO boards 

const uint32_t PWM_FREQ_BACKLIGHT = 20000; // 20 kHz 
const uint32_t PWM_FREQ_RUMBLE = 20000; // 20 kHz 
const uint8_t  PWM_RESOLUTION = 8; 

/* Function Calls */

// This interrupt triggers when a button is pressed 
void ARDUINO_ISR_ATTR buttonISR () {
    #ifdef DEBUG_ENABLE 
        Serial.print("Button Interrupt Triggered: ");
    #endif 
  
    static unsigned long lastUpDownButtonPress;
    static unsigned long lastAButtonPress;
    static unsigned long lastBButtonPress;
    int upState = digitalRead(BUTTON_UP);
    int downState = digitalRead(BUTTON_DOWN);
    int aState = digitalRead(BUTTON_A);
    int bState = digitalRead(BUTTON_B);

    #ifdef DEBUG_ENABLE 
        Serial.print(upState);
        Serial.print(" ");
        Serial.print(downState);
        Serial.print(" ");
        Serial.print(aState);
        Serial.print(" ");
        Serial.print(bState);
        Serial.println(" ");
    #endif 

    unsigned long currentButtonPressTime = millis();
    // Scale up or down the intensity of the backlight 
    
    if ( upState != downState)
    {
        // Last button Press
        unsigned long currentUpDownButtonPress = millis();
        if (currentUpDownButtonPress - lastUpDownButtonPress > 10)
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
            ledcWrite(PWM_BACKLIGHT, BACKLIGHT_DUTY);
        }
        lastUpDownButtonPress = millis();
    }

    
    // Active Low, so when A is low it is high
    if ( aState == LOW )
    {
        if(currentButtonPressTime - lastAButtonPress > 20)
        {
            if (RUMBLE_DUTY < 256)
            {
                RUMBLE_DUTY += 1;
            }
        }
        digitalWrite(LED_LEG_LEFT_RED, HIGH);
        digitalWrite(LED_LEG_LEFT_YELLOW, HIGH);
        lastAButtonPress = millis();
    }
    else
    {
        if(currentButtonPressTime - lastAButtonPress > 20)
        {
            RUMBLE_DUTY = 0;
            digitalWrite(LED_LEG_LEFT_RED, LOW);
            digitalWrite(LED_LEG_LEFT_YELLOW, LOW);
            lastAButtonPress = millis();
        }
    }
    ledcWrite(PWM_RUMBLE, BACKLIGHT_DUTY);

    // Active Low, so when B is low it is high
    if ( bState == LOW )
    {
        if(currentButtonPressTime - lastBButtonPress > 20)
        {
            digitalWrite(BUZZER_OUT, HIGH);
            digitalWrite(LED_LEG_RIGHT_YELLOW, HIGH);
            digitalWrite(LED_LEG_RIGHT_RED, HIGH);
            lastBButtonPress = millis();
        }
    }
    else
    {
        if(currentButtonPressTime - lastBButtonPress > 20)
        {
            digitalWrite(BUZZER_OUT, LOW);
            digitalWrite(LED_LEG_RIGHT_YELLOW, LOW);
            digitalWrite(LED_LEG_RIGHT_RED, LOW);
            lastBButtonPress = millis();
        }
    }
}

void setup() {
    #ifdef DEBUG_ENABLE 
        Serial.begin(9600);
        while (!Serial)
            delay(10);
        Serial.println("\nDC 34 Reference Firmware");
    #endif 
    
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
    //pinMode(LED_ARM_RIGHT_YELLOW, OUTPUT); 
    pinMode(BUZZER_OUT, OUTPUT);
    //pinMode(PWM_BACKLIGHT, OUTPUT);

    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_START, INPUT_PULLUP);
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);

    attachInterrupt(BUTTON_SELECT, buttonISR, CHANGE);
    attachInterrupt(BUTTON_START, buttonISR, CHANGE);
    attachInterrupt(BUTTON_UP, buttonISR, CHANGE);
    attachInterrupt(BUTTON_LEFT, buttonISR, CHANGE);
    attachInterrupt(BUTTON_RIGHT, buttonISR, CHANGE);
    attachInterrupt(BUTTON_DOWN, buttonISR, CHANGE);
    attachInterrupt(BUTTON_A, buttonISR, CHANGE);
    attachInterrupt(BUTTON_B, buttonISR, CHANGE);
    
    bool pwmRumbleAttachSuccess = ledcAttach(PWM_RUMBLE, PWM_FREQ_RUMBLE, PWM_RESOLUTION);
    bool pwmBackightAttachSuccess = ledcAttach(PWM_BACKLIGHT, PWM_FREQ_BACKLIGHT, PWM_RESOLUTION);
    //digitalWrite(PWM_BACKLIGHT, HIGH);
    ledcAttach(LED_ARM_RIGHT_YELLOW, PWM_FREQ_BACKLIGHT, PWM_RESOLUTION);

    #ifdef DEBUG_ENABLE 
        if (pwmRumbleAttachSuccess) {
            Serial.println("Rumble PWM attach success");
        } 
        else {
            Serial.println("Rumble PWM attach not successful");
        }
        if (pwmBackightAttachSuccess) {
            Serial.println("Backlight PWM attach success");
        } 
        else {
            Serial.println("Backlight PWM attach not successful");
        }
    #endif 
    
    bool pwmRumbleWriteSuccess = ledcWrite(PWM_RUMBLE, RUMBLE_DUTY);
    bool pwmBackightWriteSuccess = ledcWrite(PWM_BACKLIGHT, BACKLIGHT_DUTY);
    ledcWrite(LED_ARM_RIGHT_YELLOW, 0);

    #ifdef DEBUG_ENABLE 
        if (pwmRumbleWriteSuccess) {
            Serial.println("Rumble PWM write success");
        } 
        else {
            Serial.println("Rumble PWM write not successful");
        }
        if (pwmBackightWriteSuccess) {
            Serial.println("Backlight PWM write success");
        } 
        else {
            Serial.println("Backlight PWM write not successful");
        }
    #endif 

    tft.begin();

    tft.setRotation(1); // Landscape
    tft.fillScreen(ILI9341_BLACK);

    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(1);
    tft.setCursor(40, 120);

    tft.println("Hello World!");
}

void loop() {
    unsigned long currentMillis = millis();
    query_sao_collection_t saoResponse;
    
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
        digitalWrite(LED_ARM_RIGHT_RED, ledState);
        //digitalWrite(LED_ARM_RIGHT_YELLOW, ledState); 
        digitalWrite(BUZZER_OUT, ledState);

        if (ledState == HIGH)
        {
            ledcFade(LED_ARM_RIGHT_YELLOW, 0, 255, 500);
        }
        else
        {
            ledcFade(LED_ARM_RIGHT_YELLOW, 255, 0, 500);
        }


    }
    // Query SAOs and write to the screen 
    if (currentMillis - previousSaoMillis >= saoInterval) {
        saoResponse = query_sao();
        tft.fillScreen(ILI9341_BLACK);
        tft.setTextSize(1);
        tft.setCursor(40, 120);
        tft.println("Hello World!");
        tft.setCursor(10, 10);
        tft.println(sao_return_to_string(saoResponse.SAO0));
        tft.setCursor(10, 30);
        tft.println(sao_return_to_string(saoResponse.SAO1));
        tft.setCursor(10, 50);
        tft.println(sao_return_to_string(saoResponse.SAO2));
        previousSaoMillis = millis();
    }

    //buttonISR();
}

