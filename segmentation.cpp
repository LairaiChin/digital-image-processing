#include "segmentation.h"
#include "stdafx.h"
#include <cmath>
/**
 * histogram: the frequency of each pixel value
 */
int iterative_threshold(double * histogram, int histogram_len, int deltaT) {

    double * A = new double[256];//The integration of probability discrete function
    A[ 0 ] = histogram[ 0 ];
    for (int i = 1; i < 255; ++i) {
        A[i] = A[i - 1] + histogram[ i ];
    }
    A[255] = 1;

    int t, T;
    t = histogram_len >> 2;
    T = 0;

    while (abs(t - T) > deltaT) {
        T = t;
        double m1 = 0, m2 = 0;

        if (A[T] != 0) {
            for (int i = 0; i <= T; ++i)
                m1 += i * (histogram[i] / A[ T ]);
        }

        double p = 1 - A[ T ];
        if (p != 0) {
            for (int i = T + 1; i < histogram_len; ++i)
                m2 += i * (histogram[i] / p);
        }

        t = (int) ((m1 + m2) / 2);
    } 

    delete[] A;
    return t;
}

int maxEntropy1D (double * histogram, int L) {
    //pre-process of pt
    double * pt = new double [L];
    pt[0] = histogram[0];
    for (int i = 1; i < L - 1; ++i) {
        pt[i] = pt[i - 1] + histogram[i];
    }
    pt[L - 1] = 1;

    int T = -1;
    double m = 0;
    double he, ho;
    
    for (int t = 1; t < L - 1; ++t) {

        if (pt[t] == 0) continue;

        ho = 0;
        he = 0;

        for (int i = 0; i <= t; ++i) {
            if (histogram[i] == 0) continue;/*Don't forget this*/
            ho += - ((histogram[i] / pt[t]) * log(histogram[i] / pt[t]));
        } 

        double p = 1 - pt[t];

        for (int i = t + 1; i < L; ++i) {
            if (histogram[i] == 0) continue;/*Don't forget this*/
            he += -((histogram[i] / p) * log(histogram[i] / p));
        }

        if (he + ho > m) {
            m = he + ho;
            T = t;
        }
    }

    delete[] pt;

    return T;
}

int maxEntropy2D (double ** histogram, int L) {
/*A brutal and time-consuming approach*/

    int S = 0, T = 0;
    double m = 0;
    double ha = 0, hb = 0;

    for (int s = 1; s < L - 1; ++s) {

        for (int t = 1; t < L - 1; ++t) {

            double pa = 0, pb = 0;

            for (int i = 0; i <= s; ++i) {
                for (int j = 0; j <= t; ++j) {
                    pa += histogram[i][j];
                }
            }

            for (int i = s + 1; i < L; ++i) {
                for (int j = t + 1; j < L; ++j) {
                    pb += histogram[i][j];
                }
            }

            for (int i = 0; i <= s; ++i) {
                for (int j = 0; j <= t; ++j) {
                    if (histogram[i][j] == 0) continue;
                    double x = histogram[i][j] / pa;
                    ha += -(x * log(x));
                }
            }

            for (int i = s + 1; i < L; ++i) {
                for (int j = t + 1; j < L; ++j) {
                    if (histogram[i][j] == 0) continue;
                    double y = histogram[i][j] / pb;
                    hb += -(y * log(y));
                }
            }

            if (ha + hb > m) {
                m = ha + hb;
                S = s;
                T = t;
            }
        }
    }

    return S;
}