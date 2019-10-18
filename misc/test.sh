adb root
adb push fw.bin /sdcard/
adb shell "echo t "/sdcard/fw.bin" > /proc/android_touch/debug"
adb shell "cat /proc/android_touch/debug"
adb shell "echo v > /proc/android_touch/debug"
adb shell "cat /proc/android_touch/debug"
