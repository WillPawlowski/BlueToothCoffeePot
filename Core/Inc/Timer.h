/*
 * Timer.h
 *
 *  Created on: Dec 4, 2023
 *      Author: Wjpaw
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

class Timer {
private:
	static void (*executeMethod)();
	int endTime {0};
	int currTime = 0;
	int hours{0};
	int minutes{0};
	bool enabled = false;

public:
	Timer(void (*method)());
	Timer(int hours, int minutes, void (*method)());
	~Timer() = default;

	int setUpTimer(int hours, int minutes);
	void incrementTimer(int increment);
	void updateExecuteMethod(void (*method)());
	void stopTimer();
	int getTimeLeft();

};

#endif /* SRC_TIMER_H_ */
