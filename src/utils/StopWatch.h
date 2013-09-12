#ifndef STOPWATCH_H

#define STOPWATCH_H

#include <time.h>

class StopWatch
{
public:
    StopWatch();

    void start();
    void stop( const char* tag );
private:
    clock_t mStartTime;
};


#endif /* end of include guard: STOPWATCH_H */
