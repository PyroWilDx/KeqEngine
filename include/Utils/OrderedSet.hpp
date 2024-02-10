//
// Created by pyrow on 09/02/2024.
//

#ifndef ORDEREDSET_HPP
#define ORDEREDSET_HPP

#include <unordered_set>
#include <vector>

#ifdef __unix__

#include <cstddef>

#endif

template<class T>
class OrderedSet {

public:
    inline OrderedSet() {
        v = std::vector<T>();
        s = std::unordered_set<T>();
    }

    inline typename std::vector<T>::iterator begin() { return v.begin(); }

    inline typename std::vector<T>::iterator end() { return v.end(); }

    inline typename std::vector<T>::const_iterator begin() const { return v.begin(); }

    inline typename std::vector<T>::const_iterator end() const { return v.end(); }

    inline void insert(const T &e) {
        if (s.find(e) == s.end()) {
            s.insert(e);
            v.push_back(e);
        }
    }

    [[nodiscard]] inline bool empty() const { return s.empty(); }

    [[nodiscard]] inline size_t size() const { return s.size(); }

private:
    std::unordered_set<T> s;
    std::vector<T> v;

};

#endif
