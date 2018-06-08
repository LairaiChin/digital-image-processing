#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H
void sobel (int** originalImage, int** processedImage, int M, int N);
void prewitt (int** originalImage, int** processedImage, int M, int N);
void laplace (int** originalImage, int** processedImage, int M, int N);
void laplace2 (int** originalImage, int** processedImage, int M, int N);
void kirsch (int** originalImage, int** processedImage, int M, int N);
void generateGaussianFilter(double ** filter, int m_filter, int n_filter, double sigma);
#endif
