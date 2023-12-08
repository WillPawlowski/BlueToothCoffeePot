/*
 * Timer.cpp
 *
 *  Created on: Dec 4, 2023
 *      Author: Wjpaw
 */

#include "Timer.h"
#include <stdio.h>

void (*Timer::executeMethod)();

Timer::Timer(void (*method)()) {
	executeMethod = method;
}

Timer::Timer(int h, int m, void (*method)())
{
	printf("Setting Timer for %d hours and %d minutes\r\n", h, m);
	hours = h;
	minutes = m;
	endTime = 3600*h + 60*m;
	printf("Set for %d seconds\r\n", endTime);
	executeMethod = method;
	enabled = true;
}

int Timer::setUpTimer(int h, int m){
	endTime = 3600*h + 60*m;
	currTime = 0;
	enabled = true;
	return endTime;
}

void Timer::updateExecuteMethod(void (*method)())
{
	executeMethod = method;
}

void Timer::incrementTimer(int increment){
	if (enabled) {
		currTime = currTime + increment;
		if (currTime >= endTime) {
			printf("Timer Finished\r\n");
			enabled = false;
			executeMethod();
		}
	}
}

void Timer::stopTimer()
{
	enabled = false;
	endTime = 0;
	currTime = 0;
}

int Timer::getTimeLeft()
{
	return endTime - currTime;
}

