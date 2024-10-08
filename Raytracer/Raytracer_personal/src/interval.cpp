#include "interval.hpp"

const interval interval::empty = interval(std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
const interval interval::universe = interval(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

interval operator+(const interval &ival, double displace) {
    return interval(ival.min + displace, ival.max + displace);
}

interval operator+(double displace, const interval &ival) {
    return ival + displace;
}