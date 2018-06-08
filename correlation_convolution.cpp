#include "correlation_convolution.h"
#include "stdafx.h"

void correlate(int ** originalImage, int ** processedImage, int ** filter, int M, int N, int m_f, int n_f) {

    int paddedRow = M + 2 * (m_f - 1), paddedCol = N + 2 * (n_f - 1);

    int ** padded = new int * [paddedRow];

    for (int i = 0; i < paddedRow; ++i) {
        padded[i] = new int[paddedCol];
    }

//    initializes the padded image
    for (int i = 0; i < paddedRow; ++i) {
        for (int j = 0; j < paddedCol; ++j) {
            if (i >= m_f - 1 && i <= m_f + M - 2 && j >= n_f - 1 && j <= n_f + N - 2) {
                padded[i][j] = originalImage[i - m_f + 1][j - n_f + 1];
            } else {
                padded[i][j] = 0;
            }
        }
    }

//    gets the result
    int a = (m_f - 1) >> 1, b = (n_f - 1) >> 1;
    for (int i = m_f - 1; i <= m_f + M - 2; ++i) {
        for (int j = n_f - 1; j <= n_f + N - 2; ++j) {
           
// omits the padded pixels
            int value = 0;
            for (int s = -a; s <= a; ++s) {
                for (int t = -b; t <= b; ++t) {
                    value += filter[ a+s ][ b+t ] * padded[ i+s ][ j+t ];
                }
            }

            processedImage[i-m_f+1][j-n_f+1] = (value < 0 ? 0 : (value > 255 ? 255 : value));
            
        }
    }

    for (int i = 0; i < paddedRow; ++i) {
        delete[] padded[i];
    }
    delete[] padded;

}

void correlate(int ** originalImage, int ** processedImage, double ** filter, int M, int N, int m_f, int n_f) {

    int paddedRow = M + 2 * (m_f - 1), paddedCol = N + 2 * (n_f - 1);

    int ** padded = new int * [paddedRow];

    for (int i = 0; i < paddedRow; ++i) {
        padded[i] = new int[paddedCol];
    }

    //initializes the padded image
    for (int i = 0; i < paddedRow; ++i) {
        for (int j = 0; j < paddedCol; ++j) {
            if (i >= m_f - 1 && i <= m_f + M - 2 && j >= n_f - 1 && j <= n_f + N - 2) {
                padded[i][j] = originalImage[i - m_f + 1][j - n_f + 1];
            } else {
                padded[i][j] = 0;
            }
        }
    }

    //gets the result
    int a = (m_f - 1) >> 1, b = (n_f - 1) >> 1;
    for (int i = m_f - 1; i <= m_f + M - 2; ++i) {
        for (int j = n_f - 1; j <= n_f + N - 2; ++j) {

            //omits the padded pixels
            double value = 0;
            for (int s = -a; s <= a; ++s) {
                for (int t = -b; t <= b; ++t) {
                    value += filter[ a+s ][ b+t ] * padded[ i+s ][ j+t ];
                }
            }

            processedImage[i - m_f + 1][j - n_f + 1] = (int)(value < 0 ? 0 : (value > 255 ? 255 : value));

        }
    }

    for (int i = 0; i < paddedRow; ++i) {
        delete[] padded[i];
    }
    delete[] padded;

}

void rotate (int ** filter, int m, int n) {

}