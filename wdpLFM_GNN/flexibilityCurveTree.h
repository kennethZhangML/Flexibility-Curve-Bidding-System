// FlexibilityCurve.h
#ifndef FLEXIBILITY_CURVE_H
#define FLEXIBILITY_CURVE_H

#include <iostream>
#include <vector>
#include <algorithm>

class FlexibilityCurve {
private:
    std::vector<int> values;

public:
    FlexibilityCurve(const std::vector<int>& values);

    int ramp_up(int interval) const;
    int ramp_down(int interval) const;
    int total_ramp_up(int i, int i1) const;
    int total_ramp_down(int i, int i1) const;
    void print() const;
    int get_value(int value);
};

#endif // FLEXIBILITY_CURVE_H
