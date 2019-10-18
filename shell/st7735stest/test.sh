#!/bin/bash

var=1; while [ $var -gt 0 ]; do echo "1 1" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1;echo "1 0" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1; done &
var=1; while [ $var -gt 0 ]; do echo "2 1" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1;echo "2 0" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1; done &
var=1; while [ $var -gt 0 ]; do echo "3 1" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1;echo "3 0" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1; done &
var=1; while [ $var -gt 0 ]; do echo "8 1" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1;echo "8 0" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1; done &
var=1; while [ $var -gt 0 ]; do echo "11 1" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1;echo "11 0" >/sys/bus/platform/devices/et_lcd.0/lcdicon;sleep 1; done &
echo "255 255" >/sys/bus/platform/devices/et_lcd.0/lcdicon
