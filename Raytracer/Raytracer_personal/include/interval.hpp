#pragma once

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <limits>

class interval {
    public:
        double min, max;

        interval() : min(std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}
        interval(double min, double max) : min(min), max(max) {}

        interval(const interval &a, const interval &b) {
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        double size() const { return max - min; }
        bool contains(double x) const { return min <= x && x <= max; }
        bool surrounds(double x) const { return min < x && x < max; }

        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        interval expand(double delta) const {
            auto pad = delta / 2;
            return interval(min - pad, max + pad);
        }

        static const interval empty;
        static const interval universe;
};

inline const interval interval::empty = interval(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
inline const interval interval::universe = interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

inline interval operator+(const interval &ival, double displace) {
    return interval(ival.min + displace, ival.max + displace);
}

inline interval operator+(double displace, const interval &ival) {
    return ival + displace;
}

#endif
