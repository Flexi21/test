#!/bin/bash
adb wait-for-device;
adb root
sleep 2
adb remount
adb shell
