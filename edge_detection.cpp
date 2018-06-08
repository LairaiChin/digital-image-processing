#include "edge_detection.h"
#include "stdafx.h"
#include "correlation_convolution.h"
#include <cmath>
#include <memory.h>

void sobel (int** originalImage, int** processedImage, int M, int N) {

    int tempX [3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int tempY [3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    };

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                processedImage[i][j] = originalImage[i][j];
                continue;
            } /*Simplified manipulation on edge values*/

            int sx = 0, sy = 0;

            for (int s = -1; s <= 1; ++s) {
                for (int t = -1; t <= 1; ++t) {
                    sx += ( tempX[ 1+s ][ 1+t ] * originalImage[ i+s ][ j+t ]);
                    sy += ( tempY[ 1+s ][ 1+t ] * originalImage[ i+s ][ j+t ]);
                }
            }

            int value = abs(sx + sy);

            processedImage[i][j] = (value > 255 ? 255 : value);
        }
    }
}

void prewitt (int** originalImage, int** processedImage, int M, int N) {
    int tempX [3][3] = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    int tempY [3][3] = {
        {1, 1, 1},
        {0, 0, 0},
        {-1, -1, -1}
    };

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                processedImage[i][j] = originalImage[i][j];
                continue;
            } /*Simplified manipulation on edge values*/

            int px = 0, py = 0;

            for (int s = -1; s <= 1; ++s) {
                for (int t = -1; t <= 1; ++t) {
                    px += ( tempX[ 1+s ][ 1+t ] * originalImage[ i+s ][ j+t ]);
                    py += ( tempY[ 1+s ][ 1+t ] * originalImage[ i+s ][ j+t ]);
                }
            }

            int value = abs(px + py);

            processedImage[i][j] = (value > 255 ? 255 : value);
        }
    }
}

void laplace (int** originalImage, int** processedImage, int M, int N) {
    int ** filter = new int * [3];
    for (int i = 0; i < 3; ++i) {
        filter[i] = new int[3];
        memset(filter[i], 0, (sizeof(int) * 3));
    }

    filter[0][0] = -1;
    filter[0][2] = -1;
    filter[1][1] = 4;
    filter[2][0] = -1;
    filter[2][2] = -1;

    correlate(originalImage, filter, processedImage, M, N, 3, 3);
    
    //release the space
}

void laplace2 (int** originalImage, int** processedImage, int M, int N) {
    int tempLaplace [3][3] = {
        /*{-1, 0, -1},
        {0, -4, 0},
        {-1, 0, -1}*/
        {1, 1, 1},
        {1, -8, 1},
        {1, 1, 1}
    };//template 2

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                processedImage[i][j] = originalImage[i][j];
                continue;
            } /*Simplified manipulation on edge values*/

            int value = 0;

            for (int x = -1; x < 2; ++x) {
                for (int y = -1; y < 2; ++y) {
                    value += tempLaplace[ 1+x ][ 1+y ] * originalImage[ i+x ][ j+y ];
                }
            }

            processedImage[i][j] = (value < 0 ? 0 : (value > 255 ? 255 : value));
        }
    }
    
}

void generateGaussianFilter(double ** filter, int m_filter, int n_filter, double sigma) {
    int centerX = ( m_filter - 1 ) >> 1;
    int centerY = ( n_filter - 1 ) >> 1;
    double sum = 0;
    for (int x = -centerX; x <= centerX; ++x) {
        for (int y = -centerY; y <= centerY; ++y) {
            filter[centerX + x][centerY + y] = exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += filter[centerX + x][centerY + y];
        }
    }
    /*To make sure all weights sum up to 1*/
    for (int x = -centerX; x <= centerX; ++x) {
        for (int y = -centerY; y <= centerY; ++y) {
            filter[centerX + x][centerY + y] /= sum;
        }
    }
}
