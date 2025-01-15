#include <chrono>

#pragma once
class Timer{
	public:
		Timer() {startTime = std::chrono::steady_clock::now(); endTime = std::chrono::steady_clock::now();}
		void start() {startTime = std::chrono::steady_clock::now();}
		void stop() {endTime = std::chrono::steady_clock::now();}
		double getCurrentTime() {return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();}
		
		double elapsedTime() { return  std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.0;}
	private:
		std::chrono::time_point<std::chrono::steady_clock> startTime;
		std::chrono::time_point<std::chrono::steady_clock> endTime;
};
