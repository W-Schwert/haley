#pragma once

#include <cstdint>

namespace haley {
uint32_t GetThreadID();
uint32_t GetFiberID();
uint64_t GetRunTime();
uint64_t GetCurrentMS();
uint64_t GetCurrentUS();
}   // namespace haley
