/*
 * File: main.cpp
 * Project: arduino-pin-timer
 * Created Date: 01.02.2023 21:01:00
 * Author: 3urobeat
 * 
 * Last Modified: 02.02.2023 11:50:21
 * Modified By: 3urobeat
 * 
 * Copyright (c) 2023 3urobeat <https://github.com/HerrEurobeat>
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. 
 */


// Script intended for turning on & off 4 pins with increasing length
// The script will alternate between all 4 pins instead of leaving on all of them at the same time to reduce heat buildup in the connected device


#include <Arduino.h>


// Config vars
const float startAfterMin = 1;   // Min time in minutes to start after poweron
const float startAfterMax = 2.5; // Max time in minutes to start after poweron

const int totalTime = 2; // Total time in minutes

const int pauseTime = 30; // Time in seconds between intervals
const int baseTime  = 0;  // Base time in seconds where stepSize will be added to
const int stepSize  = 10; // Time in seconds to increase

const int pins[] = { D6, D7, D8 };


// Runtime vars
unsigned int pinSize = sizeof(pins) / sizeof(pins[0]);
unsigned int counter = 0;


// Functions
void setAllPins(int val) {
    for (uint8_t i = 0; i < pinSize; i++) {
        digitalWrite(pins[i], val);
    }
}


void setup()
{
    // Initialize pins
    for (uint8_t i = 0; i < pinSize; i++) {
        pinMode(pins[i], OUTPUT);
    }

    // Indicate function by turning on and off 2 times shortly
    for (uint8_t i = 0; i < 2; i++) {
        setAllPins(HIGH);
        delay(500);
        setAllPins(LOW);
        delay(500);
    }

    // Wait random amount of time between min and max before starting
    delay((rand() + startAfterMin) / startAfterMax);
}


void loop()
{
    // Stop if total time was reached and signal it with two short power ons & offs
    if (millis() >= (totalTime * 60 * 1000)) {
        for (uint8_t i = 0; i < 2; i++) {
            setAllPins(HIGH);
            delay(500);
            setAllPins(LOW);
            delay(500);
        }

        // Wait a really long time, aka turn off
        delay(276447230);
    }

    // Increase counter
    counter++;

    // Iterate over all pins and turn them on after another
    for (uint8_t i = 0; i < pinSize; i++) {
        digitalWrite(pins[i], HIGH);
        delay((unsigned long) (((baseTime + (counter * stepSize)) * 1000) / pinSize)); // Add stepSize counter times to baseTime, convert to ms and divide by amount of pins so that each pin
        digitalWrite(pins[i], LOW);
    }

    // Wait before next iteration
    delay(pauseTime * 1000);
}