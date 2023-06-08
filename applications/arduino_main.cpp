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

void setup(void)
{
    /* put your setup code here, to run once: */
    Serial.begin();
}

void loop(void)
{
    /* put your main code here, to run repeatedly: */
    Serial.print("## A0:");  Serial.print(analogRead(A0));
    Serial.print(" | A1:");  Serial.print(analogRead(A1));
    Serial.print(" | A2:");  Serial.print(analogRead(A2));
    Serial.print(" | A3:");  Serial.print(analogRead(A3));
    Serial.print(" | A4:");  Serial.print(analogRead(A4));
    Serial.print(" | A5:");  Serial.println(analogRead(A5));
    delay(2000);
}
