#include "stdafx.h"
#include "spatialFilter.h"
#include <cmath>

void generateLogFilter(int* map, double c, int L) {
    for (int r = 0; r < L; ++r) {
        int t = (int) (c * log ((double)(1 + r)));
        map[r] = (t > 255) ? 255 : t;
    }
}

void generateGammaFilter(int * map, double c, double gamma, int L) {
    for (int r = 0; r < L; ++r) {
        int t = (int) (c * pow (r, gamma));
        map[r] = (t > 255 ? 255 : t);
    }
}