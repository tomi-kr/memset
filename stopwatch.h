#include <Windows.h>

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

struct StopWatch {
    LARGE_INTEGER StartingTime;
    double        Frequency;

    StopWatch() {
		Reset();
    }

    void Reset() {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        Frequency = double(freq.QuadPart);
        QueryPerformanceCounter(&StartingTime);
    }

    double GetTimeMs() {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        return ((double(EndingTime.QuadPart - StartingTime.QuadPart) * 1000.0) / Frequency);
    }

    double GetTimeMicro() {
        LARGE_INTEGER EndingTime;
        QueryPerformanceCounter(&EndingTime);
        return ((double(EndingTime.QuadPart - StartingTime.QuadPart) * 1000000.0) / Frequency);
    }

    static StopWatch *it() {
        static StopWatch timer;
        return &timer;
    }

    static double MSec() {
        return it()->GetTimeMs();
    }

    static double USec() {
        return it()->GetTimeMicro();
    }
};

#endif
