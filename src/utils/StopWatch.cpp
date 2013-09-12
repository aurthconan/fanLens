#include "StopWatch.h"

#include <iostream>
#include <string>

using namespace std;

StopWatch::StopWatch()
    : mStartTime(0) {
}

void StopWatch::start() {
    mStartTime = clock();
}

void StopWatch::stop( const char* tag ) {
    cout << string(tag) << ": Time Elapsed " << ((clock()-mStartTime)*1000.0f)/CLOCKS_PER_SEC << " millisecond" << endl;
}

