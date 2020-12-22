#pragma once

#include <vector>
#include "lock.h"

template<class T>
class VectorList {
  private:
    std::vector<T> members;
    std::vector<std::size_t> removed;
    openmp::Lock lock;

  public:
    // reserve memory for 'members' and 'removed'
    void reserve(std::size_t for_members, std::size_t for_removed) {
        members.reserve(for_members);
        removed.reserve(for_removed);
    }

    std::size_t size() const { return members.size(); }
    std::size_t removed_size() const { return removed.size(); }
    void clear() {
        auto guard = lock.guard();  // as long as this is in scope, no other thread can lock
        members.clear();
        removed.clear();
    }

    // access member - must not be called while another thread might call 'add'
    T& operator[](std::size_t i) { return members[i]; }

    // access member - must not be called while another thread might call 'add'
    const T& operator[](std::size_t i) const { return members[i]; }

    void remove_unsafe(std::size_t i) { removed.push_back(i); }

    template<typename... Args>
    void add_unsafe(Args&&... args) {
        if (!removed.empty()) {
            const auto i = removed.back();
            removed.pop_back();
            members[i] = T(std::forward<Args>(args)...);
        } else {
            members.emplace_back(std::forward<Args>(args)...);  // this might trigger a movement of memory, so
                                                                // parallel access to 'members' (e.g. via []) must be
                                                                // avoided ('remove' is ok, though)
        }
    }

    void remove(std::size_t i) {
        auto guard = lock.guard();  // as long as this is in scope, no other thread can lock
        remove_unsafe(i);
    }

    template<typename... Args>
    void add(Args&&... args) {
        auto guard = lock.guard();  // as long as this is in scope, no other thread can lock
        add_unsafe(std::forward<Args>(args)...);
    }

    void consolidate() {}
};
