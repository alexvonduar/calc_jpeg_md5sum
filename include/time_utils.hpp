#ifndef _TIME_UTILS_HPP_
#define _TIME_UTILS_HPP_

#include <time.h>
#include <inttypes.h>

class TimeLapse
{
  public:
    int64_t t;
    TimeLapse() { t = 0; }
    void start()
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        t = ts.tv_sec;
        t *= 1000000000;
        t += ts.tv_nsec;
    }
    void stop()
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        int64_t _t = ts.tv_sec;
        _t *= 1000000000;
        _t += ts.tv_nsec;
        t = _t - t;
    }

    int64_t interval()
    {
        return t;
    }
};

#endif //_TIME_UTILS_HPP_
