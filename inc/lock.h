#pragma once

#ifdef _OPENMP
#include <omp.h>

namespace openmp {
class Lock {
  private:
    omp_lock_t lock = {};

  public:
    class Guard {
        friend class Lock;

      private:
        omp_lock_t* lock;
        Guard(omp_lock_t* lock_p) : lock(lock_p) { omp_set_lock(lock); }

      public:
        ~Guard() { omp_unset_lock(lock); }
    };

    Lock() { omp_init_lock(&lock); }
    ~Lock() { omp_destroy_lock(&lock); }
    Guard guard() { return Guard(&lock); }
    template<typename Func>
    void call(const Func& f) {
        omp_set_lock(&lock);
        f();
        omp_unset_lock(&lock);
    }
};
}  // namespace openmp

#else

namespace openmp {
class Lock {
  public:
    class Guard {};

    Guard guard() { return Guard(); }
    template<typename Func>
    void call(const Func& f) {
        f();
    }
};
}  // namespace openmp

#endif
