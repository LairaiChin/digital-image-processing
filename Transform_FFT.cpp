#include "StdAfx.h"
#include "Transform_FFT.h"
#include <stdlib.h>
#include <string>  
//构造函数
Transform_FFT::Transform_FFT(double ** m_pImgData,int width, int height)
{
    this->m_pImgData = m_pImgData; /*初始化输入图像*/
    m_pImgDataOut = NULL;	/*初始化输出图像*/
    this->width = width;	/*初始化输入宽度*/
    this->height = height;	/*初始化输入高度*/
    m_imgWidthOut = 0 ;		/*初始化输出宽度*/
    m_imgHeightOut = 0;		/*初始化输出高度*/
    m_pFFTBuf = NULL;		/*傅里叶变换缓冲区为空*/
    m_pImgExpandBuf = NULL;		/*输入图像扩展缓冲区指针初始化为0*/
    m_isExpanded = false;		/*图像扩展标识为假*/
}
Transform_FFT::~Transform_FFT( )  //析构函数
{   //释放输出图像缓冲区
    if (m_pImgDataOut != NULL)
    {          
        for (int a = 0; a < m_imgHeightOut; a++)
            delete [] m_pImgDataOut[a];
        delete []m_pImgDataOut;
        m_pImgDataOut = NULL;
    }
    //释放傅里叶变换缓冲区
    if (m_pFFTBuf != NULL)
    {          
        for (int a = 0; a < m_imgHeightOut; a++)
            delete [] m_pFFTBuf[a];
        delete []m_pFFTBuf;
        m_pFFTBuf = NULL;
    }
    //如果图像被扩展，释放图像扩展缓冲区
    if (m_isExpanded == false)
    {           
        for (int a = 0; a < m_imgHeightOut; a++)
            delete [] m_pImgExpandBuf[a];
        delete [] m_pImgExpandBuf;
        m_pImgExpandBuf = NULL;
    }
}

void Transform_FFT::FFT1D(ComplexNumber *arrayBuf, int n)
    /*对复数结构体数组arrayBuf 进行一维FFT，变换后结果仍存回arrayBuf中*/
{    
    int i,k,r;
    ComplexNumber *buf1 = new ComplexNumber[n]; //申请长度n复数缓冲区buf1
    memcpy(buf1,arrayBuf,sizeof(ComplexNumber)*n);  //将arrayBuf拷贝进buf1
    ComplexNumber *buf2 = new ComplexNumber[n];  //申请长度n复数缓冲区buf2
    //将arrayBuf数组元素基2抽取并重排， 0、1、2、3、4、5、6、7  八点序列对调后变为  0、4、2、6、1、5、3、7
    int t1,t2;
    for (r = 1;  pow(2.0,r) < n;  r++)
    {           
        t1= (int)pow(2.0, r);
        t2 =(int)pow(2.0,r-1);
        for (k = 0; k < t1; k++)
        {               
            for (i = 0; i < n/t1; i++) {
                buf2[k*n/t1+i].real = buf1[k/2*n/t2+i*2+k%2].real;
                buf2[k*n/t1+i].imag = buf1[k/2*n/t2+i*2+k%2].imag; 
            }
        }
        memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
    }
    //采用蝶形算法进行FFT ，buf1第r 级的输入，buf2存放第r 级的输出
    double c,s;
    for (r=1;pow(2.0,r)<=n;r++)
    {           
        t1 = (int)pow(2.0,r);
        for (k=0;k<n/t1;k++)
        {        
            for (i=t1/2;i<t1;i++)
            {                
                c = cos(-2*TPI*(i-t1/2)/t1);  //加权因子
                s = sin(-2*TPI*(i-t1/2)/t1);
                buf1[k*t1+i].real = buf2[k*t1+i].real*c-buf2[k*t1+i].imag*s;
                buf1[k*t1+i].imag = buf2[k*t1+i].imag*c+buf2[k*t1+i].real*s;
            }
        }
        for (k=0;k<n/t1;k++)
        {
            for (i=0;i<t1/2;i++)
            {
                buf2[k*t1+i].real = buf1[k*t1+i].real+buf1[k*t1+i+t1/2].real;
                buf2[k*t1+i].imag = buf1[k*t1+i].imag+buf1[k*t1+i+t1/2].imag;
            }
            for (i=t1/2;i<t1;i++)
            {
                buf2[k*t1+i].real = buf1[k*t1+i-t1/2].real-buf1[k*t1+i].real;
                buf2[k*t1+i].imag = buf1[k*t1+i-t1/2].imag-buf1[k*t1+i].imag;
            }
        }
        //第r级的输出存入buf1，作为下一级的输入数据
        memcpy(buf1,buf2,sizeof(ComplexNumber)*n);
    }
    //傅里叶变换的结果存入arrayBuf
    memcpy(arrayBuf,buf2,sizeof(ComplexNumber)*n);
    //释放缓冲区
    delete []buf2;
    delete []buf1;
}


void Transform_FFT::IFFT1D(ComplexNumber *arrayBuf, int n)
    /*对复数结构体数组arrayBuf进行一维IFFT，变换后结果仍存回arrayBuf中*/
{
    int i;
    //对数组arrayBuf求共轭

    for (i=0;i<n;i++)	
        arrayBuf[i].imag = - arrayBuf[i].imag;
    //一维快速傅里叶变换
    FFT1D(arrayBuf,n);  

    //时域数据求共轭，并除以N
    for (i=0;i<n;i++)
    {
        arrayBuf[i].real = arrayBuf[i].real/n;
        arrayBuf[i].imag = -arrayBuf[i].imag/n;
    }
}


/*图像数据二维快速傅里叶变换将图像数据变成复数形式，进行两个一维快速傅里叶变换，变换后的频谱以图像形式存入imgBufOut，要求图像宽和高都为2的幂次方。*/
void Transform_FFT::ImgFFT2D(double **imgBuf, int thiswidth, int thisheight, double **imgBufOut)
{   
    int i, j,u,v;	
    for (i=0;i<thisheight;i++)        //图像数据变成复数形式存入m_pFFTBuf
    {           
        for (j = 0;j<thiswidth;j++)
        {                
            double a  = imgBuf[i][j];
            m_pFFTBuf[i][j].real = a;
            m_pFFTBuf[i][j].imag = 0;
        }
    }	
    ComplexNumber *array = new ComplexNumber[thisheight];	//申请长度为height
    for (u=0;u<thiswidth;u++) //先纵向一维FFT
    {           
        for (v=0;v<thisheight;v++)
        {              
            array[v].real = m_pFFTBuf[v][u].real;
            array[v].imag = m_pFFTBuf[v][u].imag;
        }
        FFT1D(array,thisheight);
        for (v=0;v<thisheight;v++)
        {                
            m_pFFTBuf[v][u].real = array[v].real;
            m_pFFTBuf[v][u].imag = array[v].imag; 
        }		
    }

    delete[] array;
    array = new ComplexNumber [thiswidth];
    //再横向一维快速傅里叶变换
    for (v=0;v<thisheight;v++)
    {           
        for (u=0;u<thiswidth;u++)
        {                
            array[u].real = m_pFFTBuf[v][u].real;
            array[u].imag = m_pFFTBuf[v][u].imag;
        }
        FFT1D(array,thiswidth);
        for (u=0;u<thiswidth;u++)
        {
            m_pFFTBuf[v][u].real = array[u].real;
            m_pFFTBuf[v][u].imag = array[u].imag;
        }
    }
    delete[] array;
    //将频谱图输出
    double t;

    for (i=0;i<thisheight;i++)
    {
        for (j=0;j<thiswidth;j++)
        {
            t = sqrt(m_pFFTBuf[i][j].real*m_pFFTBuf[i][j].real + m_pFFTBuf[i][j].imag*m_pFFTBuf[i][j].imag);
            t = t/500;
            if (t>255)
            {
                imgBufOut[i][j] = 255;
            } 
            else
            {
                imgBufOut[i][j] = t;
            }
        }
    }
}

/*对缓冲区m_pFFTBuf的数据，进行两个一维IFFT，变换后频谱是图像的恢复数据，存入imgBufOut，此处图像的宽和高都为2的幂次方*/

void Transform_FFT::ImgIFFT2D(double **imgBufOut, int thiswidth, int thisheight)
{    
    if (!m_pFFTBuf)          
        return;  //若傅里叶变换缓冲区为空，则失败返回	
    int  i, u, v;	
    ComplexNumber *array = new ComplexNumber[thisheight];  //先纵向IFFT
    for (u=0;u<thiswidth;u++)
    {           
        for (v=0; v<thisheight; v++)
        {               
            array[v].real = m_pFFTBuf[v][u].real;
            array[v].imag = m_pFFTBuf[v][u].imag;
        }
        IFFT1D(array,thisheight);
        for (v=0; v<thisheight; v++)
        {               
            m_pFFTBuf[v][u].real = array[v].real;
            m_pFFTBuf[v][u].imag = array[v].imag;
        }
    }
    delete[] array;
    array = new ComplexNumber [thiswidth]; 	
    //再横向傅里叶反变换
    for (v=0;v<thisheight;v++)
    {
        for (u=0;u<thiswidth;u++)
        {
            array[u].real = m_pFFTBuf[v][u].real;
            array[u].imag = m_pFFTBuf[v][u].imag;
        }
        IFFT1D(array,thiswidth);
        for (u=0;u<thiswidth;u++)
        {
            m_pFFTBuf[v][u].real = array[u].real;
            m_pFFTBuf[v][u].imag = array[u].imag;
        }
    }
    delete []array;
    //求m_pFFTBuf的幅度谱，恢复图像存入imgBufOut
    double t;
    for (i=0; i<thisheight;i++)
    {
        for (int j = 0; j<thiswidth;j++)
        {
            t = sqrt(m_pFFTBuf[i][j].real*m_pFFTBuf[i][j].real + m_pFFTBuf[i][j].imag*m_pFFTBuf[i][j].imag);

            if (t>255)
                imgBufOut[i][j] = 255;			
            else
                imgBufOut[i][j] = t;

        }
    }
}

void Transform_FFT::Fourier()
    /*对输入图像进行快速傅里叶变换，要求输入图像的宽和高必须是2的幂次方*/
{
    //输出图像的大小与输入图像的大小相同
    m_imgWidthOut = width;
    m_imgHeightOut = height;

    //申请傅里叶变换缓冲区，并初始化为0
    m_pFFTBuf = new ComplexNumber*[m_imgHeightOut];
    for (int a = 0; a<m_imgHeightOut; a++)
    {
        m_pFFTBuf[a] = new ComplexNumber [m_imgWidthOut]; 
    }

    //输入图像数据进行二维傅里叶变换
    ImgFFT2D(m_pImgData,m_imgWidthOut,m_imgHeightOut,m_pImgDataOut);
}

void Transform_FFT::IFourier()
    /*输入图像的快速傅里叶反变换，与ImgFFT()成对使用*/
{
    //图像的二维傅里叶反变换，变换结果存入m_pImgDataOut
    ImgIFFT2D(m_pImgDataOut,m_imgWidthOut,m_imgHeightOut);
}


void Transform_FFT::FourierEx(int *d_height,int* d_width)
    /*图像长和宽是2的幂次方，直接用ImgFFT()，否则，输入图像的长和宽进行扩展后，进行FFT*/
{   //判断输入图像的长和宽是否是2的幂次方，若是isPowerOf2置1，否则isPowerOf2置0
    int  isPOwerOf2 = 1;
    int i = 1;
    while (i<width)
    {
        i *= 2;
    }
    if ( i != width)//宽不是2的幂次方
    {
        isPOwerOf2 = 0;
    }
    i =1;
    while (i<height)
    {
        i = i*2;
    }
    if (i != height)//高不是2的幂次方
    {           
        isPOwerOf2 = 0;
    }
    //输入图像的长和宽都是2的幂次方，则不需要扩展，直接调用ImgFFT()函数即可
    if (isPOwerOf2)
    {           
        Fourier();
    } 
    else
    {           //输入图像的长或宽不是2的幂次方，则将图像补零扩展
        int i = 1;
        while (i<width)
        {
            i *= 2;
        }
        m_imgWidthOut = i;

        i =1;
        while (i<height)
        {
            i = i*2;
        }
        m_imgHeightOut = i;

        *d_height = m_imgHeightOut;
        *d_width = m_imgWidthOut;
        //申请扩展缓冲区m_pImgExpandBuf
        m_pImgExpandBuf = new double*[m_imgHeightOut];
        for (int a = 0; a<m_imgHeightOut; a++)
            m_pImgExpandBuf[a] = new double [m_imgWidthOut]; 

        //m_pImgExpandBuf缓冲区置0
        for (int m=0;m<m_imgHeightOut;m++)
        {
            for (int n=0;n<m_imgWidthOut;n++)
                m_pImgExpandBuf[m][n] =0; 
        }

        //输入图像数据拷贝到m_pImgExpandBuf得左下角部分，其余为0
        for (int m=0;m<height;m++)
        {
            for (int n=0;n<width;n++)
                //旋转到中心位置
                m_pImgExpandBuf[m+m_imgHeightOut-height][n] =m_pImgData[m][n]*pow((-1.0),(m+n)); 
        }
        //申请输出图像缓冲区，并初始化为0
        m_pImgDataOut = new double*[m_imgHeightOut];
        for (int a = 0; a<m_imgHeightOut; a++)
            m_pImgDataOut[a] = new double [m_imgWidthOut]; 

        for (int m=0;m<m_imgHeightOut;m++)
        {                
            for (int n=0;n<m_imgWidthOut;n++)
                m_pImgDataOut[m][n] =0; 
        }
        //申请傅里叶缓冲区，并初始化为0
        m_pFFTBuf = new ComplexNumber*[m_imgHeightOut];
        for (int a = 0; a<m_imgHeightOut; a++)
            m_pFFTBuf[a] = new ComplexNumber [m_imgWidthOut]; 

        //扩展后的数据进行二维FFT
        ImgFFT2D(m_pImgExpandBuf, m_imgWidthOut, m_imgHeightOut, m_pImgDataOut);
        
        //图像扩展标识变量置1
        m_isExpanded = true;
    }
}

void Transform_FFT::IFourierEx(double **d_proImg)
    /*输入图像扩展后的快速傅里叶反变换，与ImgFFTEx( )成对调用*/
{
    //图像的二维IFFT，变换结果存入m_pImgDataOut
    ImgIFFT2D(m_pImgDataOut,m_imgWidthOut,m_imgHeightOut);

    //将m_pImgDataOut中图像数据拷贝出
    for (int i=0;i<m_imgHeightOut;i++)
    {
        for (int j=0;j<m_imgWidthOut;j++)
        {
            d_proImg[i][j] = m_pImgDataOut[i][j];
        }
    }
}


void Transform_FFT::D(double K,double ** d_proImag)
    /*   D = ln(1+K|F(u,v)|)  即：proImage[u][v] = log(1+K*proImage[u][v])
    K为常数，通常取值为[1,10],可以调节显示的最大值和最小值 */
{
    int u,v;
    for(u=0;u<m_imgHeightOut;u++)
    {
        for(v=0;v<m_imgWidthOut;v++)
        {
            m_pImgDataOut[u][v] = log(1+K*m_pImgDataOut[u][v]);
        }
    }
    for (int i=0;i<m_imgHeightOut;i++)
    {
        for (int j=0;j<m_imgWidthOut;j++)
        {
            d_proImag[i][j] = m_pImgDataOut[i][j];
        }
    }

}




