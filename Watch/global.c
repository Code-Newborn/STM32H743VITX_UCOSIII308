/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

// static time_s chargeFinishTime;

void global_init() {

}

void global_update() {
    // 检测USB连接，并且可以刷新数据，这里我实现不了

    //	static BOOL lastUsbConnected;
    //	static BOOL lastCharging;
    //	static millis8_t lastCheck;

    //	millis8_t now = millis();
    //	if((millis8_t)(now - lastCheck) < 150)
    //		return;
    //	lastCheck = now;
    //
    //  BOOL usbConnected = (UDADDR);
    //	BOOL charging = CHARGING();

    //	if(usbConnected != lastUsbConnected)
    //	{
    //		lastUsbConnected = usbConnected;
    //		tune_play(usbConnected ? tuneUsbPlugin : tuneUsbUnplug, VOL_UI, PRIO_UI);
    //	}

    //	if(charging != lastCharging)
    //	{
    //		lastCharging = charging;
    //		if(!charging && usbConnected)
    //		{
    //			tune_play(tuneUsbCharged, VOL_UI, PRIO_UI);
    //
    //			// save time ONLY WHEN USB UNPLUGGED
    //			memcpy(&chargeFinishTime, &timeDate, sizeof(time_s));
    //		}
    //	}
}

// Dividing by 10 and mod 10 is done a lot in the code.
// Putting them in their own functions makes the code smaller, but slightly slower.
byte div10( byte val ) {
    return val / 10;
}

byte mod10( byte val ) {
    return val % 10;
}
