#pragma once

#include <algorithm>
#include <iostream>
#include <locale>
#include <memory>
#include <random>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

template<class T>
class VectorList {
  private:
    std::vector<T> members;
    std::vector<std::unique_ptr<std::vector<std::size_t>>> removed;
    std::vector<std::unique_ptr<std::vector<T>>> additional;
    std::size_t thread_used = 0;
    std::mt19937_64 rng;

  public:
    explicit VectorList(std::size_t threads_count) : rng(1234) {
        additional.resize(threads_count);
        removed.resize(threads_count);
        for (std::size_t i = 0; i < threads_count; ++i) {
            additional[i].reset(new std::vector<T>());
            removed[i].reset(new std::vector<std::size_t>());
        }
    }

    VectorList(VectorList& other) : members(other.members), rng(1234) {
        const auto threads_count = other.additional.size();
        additional.resize(threads_count);
        removed.resize(threads_count);
        for (std::size_t i = 0; i < threads_count; ++i) {
            additional[i].reset(new std::vector<T>(*other.additional[i]));
            removed[i].reset(new std::vector<std::size_t>(*other.removed[i]));
        }
    }

    VectorList(VectorList&&) = default;

    VectorList& operator=(VectorList& other) {
        const auto threads_count = other.additional.size();
        members = other.members;
        additional.resize(threads_count);
        removed.resize(threads_count);
        for (std::size_t i = 0; i < threads_count; ++i) {
            additional[i].reset(new std::vector<T>(*other.additional[i]));
            removed[i].reset(new std::vector<std::size_t>(*other.removed[i]));
        }
        return *this;
    }

    void reserve(std::size_t for_members) { members.reserve(for_members); }

    std::size_t size() const { return members.size(); }

    void clear() {
        members.clear();
        for (std::size_t i = 0; i < removed.size(); ++i) {
            removed[i]->clear();
            additional[i]->clear();
        }
    }

    T& operator[](std::size_t i) { return members[i]; }
    const T& operator[](std::size_t i) const { return members[i]; }

    void remove(std::size_t i) {
#ifdef _OPENMP
        const auto thread_id = omp_get_thread_num();
#else
        const auto thread_id = 0;
#endif
        removed[thread_id]->push_back(i);
    }

    template<typename... Args>
    void add(Args&&... args) {  // must not be called in a loop that also accesses using []
#ifdef _OPENMP
        const auto thread_id = omp_get_thread_num();
#else
        const auto thread_id = 0;
#endif
        auto& r = removed[thread_id];
        if (!r->empty()) {
            const auto i = r->back();
            r->pop_back();
            members[i] = T(std::forward<Args>(args)...);
        } else {
            additional[thread_id]->emplace_back(std::forward<Args>(args)...);
        }
    }

    template<typename... Args>
    void add_directly(Args&&... args) {  // must not be called in a loop that also accesses using []
        const auto threads_count = additional.size();
        auto& r = removed[thread_used];
        if (!r->empty()) {
            const auto i = r->back();
            r->pop_back();
            members[i] = T(std::forward<Args>(args)...);
        } else {
            members.emplace_back(std::forward<Args>(args)...);
            thread_used = (thread_used + 1) % threads_count;
        }
    }

    void consolidate() {  // must be called after a loop with adds or removes
        const auto threads_count = additional.size();
        std::size_t reserve = 0;
#ifdef DEBUG
        std::cout << "\n\n";
#endif
        for (std::size_t i = 0; i < threads_count; ++i) {
#ifdef DEBUG
            std::cout << "Thread " << i << ": additional=" << additional[i]->size() << "/" << additional[i]->capacity() << " removed=" << removed[i]->size()
                      << "/" << removed[i]->capacity() << "\n";
#endif
            reserve += additional[i]->size();
        }
        if (reserve > 0) {
            members.reserve(members.size() + reserve);
            for (auto& a : additional) {
                if (!a->empty()) {
                    std::move(std::begin(*a), std::end(*a), std::back_inserter(members));
                    a->clear();
                }
            }
            std::shuffle(std::begin(additional), std::end(additional), rng);
        }
        std::shuffle(std::begin(removed), std::end(removed), rng);
#ifdef DEBUG
        std::cout << "Total: " << members.size() << "/" << members.capacity() << "\n\n";
#endif
    }
};
