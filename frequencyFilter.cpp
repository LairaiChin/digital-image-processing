#include "stdafx.h"
#include "frequencyFilter.h"
#include <cmath>

double ideal_lowpass(int u, int v, double d0) {
    double d = sqrt ((double) (u * u  + v * v));
    if (d <= d0) return 1;
    else return 0;
}

double butterworth_lowpass(int u, int v, double d0, int n) {
    double d = sqrt ((double) (u * u  + v * v));
    d /= d0;
    d = pow(d, 2 * n);
    ++d;
    d = 1 / d;
    return d;
}

double exponent_lowpass(int u, int v, double d0, int n) {
    double d = sqrt ((double) (u * u  + v * v));
    d /= d0;
    d = pow(d, n);
    return exp(-d);
}

double ladder_lowpass(int u, int v, double d0, double d1) {
    double d = sqrt ((double) (u * u  + v * v));
    if (d < d0) return 1;
    else if (d > 1) return 0;
    else return (d - d1) / (d0 - d1);
}

double ideal_highpass(int u, int v, double d0) {
    double d = sqrt ((double) (u * u  + v * v));
    if (d <= d0) return 0;
    else return 1;
}

double butterworth_highpass(int u, int v, double d0, int n) {
    double d = sqrt ((double) (u * u  + v * v));
    d = d0 / d;
    ++d;
    d = pow(d, 2 * n);
    return 1 / d;
}
double exponent_highpass(int u, int v, double d0, int n) {
    double d = sqrt ((double) (u * u  + v * v));
    d = d0 / d;
    d = pow(d, n);
    return exp(-d);
}
double ladder_highpass(int u, int v, double d0, double d1) {
    double d = sqrt ((double) (u * u  + v * v));
    if (d < d1) return 0;
    else if (d > d0) return 1;
    else return (d0 - d1) / (d - d1);
}
