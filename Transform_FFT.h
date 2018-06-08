#include <math.h>
#define TPI 3.1415926
struct ComplexNumber
{
    double imag;//虚部
    double real;//实部
};
public class Transform_FFT
{
public:
    double ** m_pImgData;      //输入图像
    double ** m_pImgDataOut; //输出图像
    int width, height, m_imgWidthOut, m_imgHeightOut; //输入输出图像的高宽	
    //输入图像非2的幂次方阵时，扩展图像的缓冲区指针
    double ** m_pImgExpandBuf;
    ComplexNumber ** m_pFFTBuf; //傅里叶变换缓冲区
    bool m_isExpanded; //图像是否被扩展的标志变量
public:
    Transform_FFT(double ** m_pImgData,int width, int height); //构造函数	
    ~Transform_FFT(); //析构函数	
    void Fourier(); //图像傅里叶变换
    void IFourier(); //傅里叶反变换
    void FourierEx(int *,int *); //扩展的傅里叶变换	
    void IFourierEx(double **); //扩展的傅里叶反变换  
private:	
    void FFT1D(ComplexNumber *arrayBuf, int n); //一维FFT正变换	
    void IFFT1D(ComplexNumber *arrayBuf, int n); //一维IFFT反变换
    //二维FFT变换
    void ImgFFT2D(double ** imgBuf, int thiswidth, int thisheight, double ** imgBufOut);
    //二维FFT反变换
    void ImgIFFT2D(double ** imgBufOut, int thiswidth, int thisheight);
public:
    void D(double K, double ** /*int *,int**/);   //D转换
};
