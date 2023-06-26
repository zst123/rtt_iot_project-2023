/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-13     songw4232    first version
 */

#include <Arduino.h>
#include "stm32u575xx.h"
#include "./Filters/Filters.h"

extern "C" float TRMS;

// Current Transformer
#define CT_EFFICIENCY (0.9)
#define CT_RESISTOR (1200.0)
#define CT_PIN1 (A4) // CT voltage generation
#define CT_PIN2 (A5) // Midpoint reference

// RMS filter
RunningStatistics inputStats;
float windowLength = 0.5;     // seconds to average the signal
float TRMS = 0; // estimated true RMS

// Track time in milliseconds since last reading 
unsigned long printPeriod = 250; // in milliseconds
unsigned long previousMillis = 0;

void setup(void)
{
    /* put your setup code here, to run once: */
    Serial.begin();
    pinMode(LD1, OUTPUT);

    // Delay 5 seconds to wait for rw007 to setup first
    delay(5000);

    // RMS filter
    inputStats.setWindowSecs(windowLength);
}

void loop(void)
{
    /* put your main code here, to run repeatedly: */
    delayMicroseconds(100);

    // Read waveform and subtract biasing
    const int adcValue = analogRead(CT_PIN1) - analogRead(CT_PIN2);
    const float adcVoltage = (adcValue * 3.3 / 1024);

    // Get primary side current from the secondary voltage (turns ratio is 2000)
    float milliamp = adcVoltage * (2000.0 / CT_RESISTOR) * 1000 / CT_EFFICIENCY;
    inputStats.input(milliamp);  // log to stats function
    //Serial.print(milliamp, DEC); 

    // Update the calculation
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
        previousMillis = millis();   // update time
        digitalWrite(LD1, !digitalRead(LD1)); // toggle green LED
        TRMS = inputStats.sigma();
        // Serial.print(",");
        // Serial.print(TRMS, DEC); 
    }
    // Serial.println();
}
