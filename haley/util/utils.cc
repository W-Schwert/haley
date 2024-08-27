#include "utils.h"

#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

namespace haley {
static uint64_t s_start = GetCurrentMS();

uint32_t GetThreadID() {
    return syscall(SYS_getpid);
}

uint32_t GetFiberID() {
    // TODO: implement this in future
    return 0;
}

uint64_t GetRunTime() {
    uint64_t now_ms = GetCurrentMS();
    return now_ms - s_start;
}

uint64_t GetCurrentMS() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000ul + tv.tv_usec / 1000;
}

uint64_t GetCurrentUS() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 * 1000ul + tv.tv_usec;
}
}   // namespace haley
