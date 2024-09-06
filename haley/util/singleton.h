#pragma once

namespace haley {
template<typename T>
class Singleton {
public:
    static T* GetInstance() {
        static T v;
        return &v;
    }
};
}
