#pragma once
#include <memory.h>
#include "edge_detection.h"
#include "Transform_FFT.h"
#include "frequencyFilter.h"
#include "segmentation.h"
#include "correlation_convolution.h"
#include "GreyStretch.h"
#include "spatialFilter.h"

namespace DigitalImageProcessLearning {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    /// <summary>
    /// Form1 摘要
    /// </summary>
    public ref class Form1 : public System::Windows::Forms::Form
    {
    private:
        int** originalImage;       //处理前的数组
        int** processedImage;      //处理后的数组
        int *** originalRGB;
        int *** processedRGB;
        int M, N;                  //M x N 的图像
        Bitmap^ originalBitmap;    //处理前BitMap
        Bitmap^ processedBitmap;   //处理后bmp
        double * dHistogram;         //频率直方图
        int * histogram;
        double ** twoDHistogram;
        int ** twoHistogram;
        static double c_low = 0.5;
        static double c_high = 0.8;
        GreyStretch greyStretch;
        int *** pSrcImage;
        int *** pDestImage;
        int * intensityMapping; //map for spatial transform

    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::PictureBox^  originalBox;

    private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::ToolStripMenuItem^  frequencyTransformToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  dFTToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  iDFTToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  frequencyDomainFilteringToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  highPassToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  lowPassToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  idealToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  butterworthToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  expoToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  ladderToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  idealToolStripMenuItem1;
    private: System::Windows::Forms::ToolStripMenuItem^  butterworthToolStripMenuItem1;
    private: System::Windows::Forms::ToolStripMenuItem^  exponentialToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  ladderToolStripMenuItem1;
    private: System::Windows::Forms::ToolStripMenuItem^  segmentationToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  iterativeThresholdingToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  maxEntropyToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  loGToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  dToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  dToolStripMenuItem1;
    private: System::Windows::Forms::ToolStripMenuItem^  enhancementToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  greyStretchToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  logToolStripMenuItem1;
    private: System::Windows::Forms::ToolStripMenuItem^  gammaToolStripMenuItem;

    private: System::Windows::Forms::PictureBox^  processedBox;


    public:
        Form1(void)
        {
            InitializeComponent();
            //
            //TODO: 在此处添加构造函数代码
            //
        }
        //void loadImage (Bitmap^ ori, Bitmap^ pro, int** originalImage, int** processedImage, int& M, int& N, PictureBox^ originalBox, PictureBox^ processedBox);
        //loads image from disk

        //void saveImage (Bitmap^ pro);
        //saves the processed image

        //void displayImage (Bitmap^ pro, int** processedImage, int& M, int& N);
        //displays the processed image on the windows form

        void displayProcessedImage() {
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    processedBitmap -> SetPixel(j, i, Color :: FromArgb((unsigned char)processedImage[i][j], (unsigned char)processedImage[i][j], (unsigned char)processedImage[i][j]));
                }
            }
            processedBox->Refresh();
        }

        void generateHistogram(int** image, int M, int N) {

            for (int i = 0; i < 256; ++i) {
                histogram[i] = 0;
            }

            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j)
                    ++histogram[image[i][j]];
            }

            double total = M * N;

            for (int i = 0; i < 256; ++i)
                dHistogram[i] = (double) ( (double)histogram[i] / total );

            //generates 2d histogram
            int fn[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};//four neighbors

            for (int i = 1; i < M - 1; ++i) {
                for (int j = 1; j < N - 1; ++j) {
                    //gets the average value of its 4 neighbors
                    int average = 0;
                    for (int t = 0; t < 4; ++t)
                        average += originalImage[ i + fn[t][0] ][ j + fn[t][1] ];
                    average /= 4;
                    ++twoHistogram[originalImage[i][j]][average];
                }
            }

            total = (double)(M - 1) * (N - 1);

            for (int i = 0; i < 256; ++i) {
                for (int j = 0; j < 256; ++j) {
                    twoDHistogram[i][j] = (double)twoHistogram[i][j] / total;
                }
            }
        }
    protected:
        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        ~Form1()
        {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::MenuStrip^  menuStrip1;
    protected: 
    private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  edgeDetectionToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  sobelToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  prewittToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  laplaceToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  kirschToolStripMenuItem;

    private:
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        void InitializeComponent(void)
        {
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->edgeDetectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->sobelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->prewittToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->laplaceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->kirschToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->frequencyTransformToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->dFTToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->iDFTToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->frequencyDomainFilteringToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->highPassToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->idealToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->butterworthToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->expoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ladderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->lowPassToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->idealToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->butterworthToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->exponentialToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->ladderToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->segmentationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->iterativeThresholdingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->maxEntropyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->dToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->dToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->loGToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->enhancementToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->greyStretchToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->logToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->originalBox = (gcnew System::Windows::Forms::PictureBox());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->processedBox = (gcnew System::Windows::Forms::PictureBox());
            this->gammaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->menuStrip1->SuspendLayout();
            this->groupBox1->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originalBox))->BeginInit();
            this->groupBox2->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->processedBox))->BeginInit();
            this->SuspendLayout();
            // 
            // menuStrip1
            // 
            this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->fileToolStripMenuItem, 
                this->edgeDetectionToolStripMenuItem, this->frequencyTransformToolStripMenuItem, this->frequencyDomainFilteringToolStripMenuItem, 
                this->segmentationToolStripMenuItem, this->enhancementToolStripMenuItem});
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(1226, 32);
            this->menuStrip1->TabIndex = 0;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
                this->saveToolStripMenuItem});
            this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
            this->fileToolStripMenuItem->Size = System::Drawing::Size(52, 28);
            this->fileToolStripMenuItem->Text = L"File";
            // 
            // openToolStripMenuItem
            // 
            this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
            this->openToolStripMenuItem->Size = System::Drawing::Size(128, 28);
            this->openToolStripMenuItem->Text = L"Open";
            this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
            this->saveToolStripMenuItem->Size = System::Drawing::Size(128, 28);
            this->saveToolStripMenuItem->Text = L"Save";
            this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
            // 
            // edgeDetectionToolStripMenuItem
            // 
            this->edgeDetectionToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->sobelToolStripMenuItem, 
                this->prewittToolStripMenuItem, this->laplaceToolStripMenuItem, this->kirschToolStripMenuItem});
            this->edgeDetectionToolStripMenuItem->Name = L"edgeDetectionToolStripMenuItem";
            this->edgeDetectionToolStripMenuItem->Size = System::Drawing::Size(155, 28);
            this->edgeDetectionToolStripMenuItem->Text = L"Edge Detection";
            // 
            // sobelToolStripMenuItem
            // 
            this->sobelToolStripMenuItem->Name = L"sobelToolStripMenuItem";
            this->sobelToolStripMenuItem->Size = System::Drawing::Size(145, 28);
            this->sobelToolStripMenuItem->Text = L"Sobel";
            this->sobelToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sobelToolStripMenuItem_Click);
            // 
            // prewittToolStripMenuItem
            // 
            this->prewittToolStripMenuItem->Name = L"prewittToolStripMenuItem";
            this->prewittToolStripMenuItem->Size = System::Drawing::Size(145, 28);
            this->prewittToolStripMenuItem->Text = L"Prewitt";
            this->prewittToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::prewittToolStripMenuItem_Click);
            // 
            // laplaceToolStripMenuItem
            // 
            this->laplaceToolStripMenuItem->Name = L"laplaceToolStripMenuItem";
            this->laplaceToolStripMenuItem->Size = System::Drawing::Size(145, 28);
            this->laplaceToolStripMenuItem->Text = L"Laplace";
            this->laplaceToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::laplaceToolStripMenuItem_Click);
            // 
            // kirschToolStripMenuItem
            // 
            this->kirschToolStripMenuItem->Name = L"kirschToolStripMenuItem";
            this->kirschToolStripMenuItem->Size = System::Drawing::Size(145, 28);
            this->kirschToolStripMenuItem->Text = L"Kirsch";
            // 
            // frequencyTransformToolStripMenuItem
            // 
            this->frequencyTransformToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->dFTToolStripMenuItem, 
                this->iDFTToolStripMenuItem});
            this->frequencyTransformToolStripMenuItem->Name = L"frequencyTransformToolStripMenuItem";
            this->frequencyTransformToolStripMenuItem->Size = System::Drawing::Size(204, 28);
            this->frequencyTransformToolStripMenuItem->Text = L"Frequency Transform";
            // 
            // dFTToolStripMenuItem
            // 
            this->dFTToolStripMenuItem->Name = L"dFTToolStripMenuItem";
            this->dFTToolStripMenuItem->Size = System::Drawing::Size(119, 28);
            this->dFTToolStripMenuItem->Text = L"DFT";
            this->dFTToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dFTToolStripMenuItem_Click);
            // 
            // iDFTToolStripMenuItem
            // 
            this->iDFTToolStripMenuItem->Name = L"iDFTToolStripMenuItem";
            this->iDFTToolStripMenuItem->Size = System::Drawing::Size(119, 28);
            this->iDFTToolStripMenuItem->Text = L"IDFT";
            this->iDFTToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::iDFTToolStripMenuItem_Click);
            // 
            // frequencyDomainFilteringToolStripMenuItem
            // 
            this->frequencyDomainFilteringToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->highPassToolStripMenuItem, 
                this->lowPassToolStripMenuItem});
            this->frequencyDomainFilteringToolStripMenuItem->Name = L"frequencyDomainFilteringToolStripMenuItem";
            this->frequencyDomainFilteringToolStripMenuItem->Size = System::Drawing::Size(151, 28);
            this->frequencyDomainFilteringToolStripMenuItem->Text = L"Frequncy Filter";
            // 
            // highPassToolStripMenuItem
            // 
            this->highPassToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->idealToolStripMenuItem, 
                this->butterworthToolStripMenuItem, this->expoToolStripMenuItem, this->ladderToolStripMenuItem});
            this->highPassToolStripMenuItem->Name = L"highPassToolStripMenuItem";
            this->highPassToolStripMenuItem->Size = System::Drawing::Size(164, 28);
            this->highPassToolStripMenuItem->Text = L"High Pass";
            // 
            // idealToolStripMenuItem
            // 
            this->idealToolStripMenuItem->Name = L"idealToolStripMenuItem";
            this->idealToolStripMenuItem->Size = System::Drawing::Size(183, 28);
            this->idealToolStripMenuItem->Text = L"Ideal";
            this->idealToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::idealToolStripMenuItem_Click);
            // 
            // butterworthToolStripMenuItem
            // 
            this->butterworthToolStripMenuItem->Name = L"butterworthToolStripMenuItem";
            this->butterworthToolStripMenuItem->Size = System::Drawing::Size(183, 28);
            this->butterworthToolStripMenuItem->Text = L"Butterworth";
            this->butterworthToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::butterworthToolStripMenuItem_Click);
            // 
            // expoToolStripMenuItem
            // 
            this->expoToolStripMenuItem->Name = L"expoToolStripMenuItem";
            this->expoToolStripMenuItem->Size = System::Drawing::Size(183, 28);
            this->expoToolStripMenuItem->Text = L"Exponential";
            this->expoToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::expoToolStripMenuItem_Click);
            // 
            // ladderToolStripMenuItem
            // 
            this->ladderToolStripMenuItem->Name = L"ladderToolStripMenuItem";
            this->ladderToolStripMenuItem->Size = System::Drawing::Size(183, 28);
            this->ladderToolStripMenuItem->Text = L"Ladder";
            this->ladderToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ladderToolStripMenuItem_Click);
            // 
            // lowPassToolStripMenuItem
            // 
            this->lowPassToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->idealToolStripMenuItem1, 
                this->butterworthToolStripMenuItem1, this->exponentialToolStripMenuItem, this->ladderToolStripMenuItem1});
            this->lowPassToolStripMenuItem->Name = L"lowPassToolStripMenuItem";
            this->lowPassToolStripMenuItem->Size = System::Drawing::Size(164, 28);
            this->lowPassToolStripMenuItem->Text = L"Low Pass";
            // 
            // idealToolStripMenuItem1
            // 
            this->idealToolStripMenuItem1->Name = L"idealToolStripMenuItem1";
            this->idealToolStripMenuItem1->Size = System::Drawing::Size(183, 28);
            this->idealToolStripMenuItem1->Text = L"Ideal";
            this->idealToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::idealToolStripMenuItem1_Click);
            // 
            // butterworthToolStripMenuItem1
            // 
            this->butterworthToolStripMenuItem1->Name = L"butterworthToolStripMenuItem1";
            this->butterworthToolStripMenuItem1->Size = System::Drawing::Size(183, 28);
            this->butterworthToolStripMenuItem1->Text = L"Butterworth";
            this->butterworthToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::butterworthToolStripMenuItem1_Click);
            // 
            // exponentialToolStripMenuItem
            // 
            this->exponentialToolStripMenuItem->Name = L"exponentialToolStripMenuItem";
            this->exponentialToolStripMenuItem->Size = System::Drawing::Size(183, 28);
            this->exponentialToolStripMenuItem->Text = L"Exponential";
            this->exponentialToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exponentialToolStripMenuItem_Click);
            // 
            // ladderToolStripMenuItem1
            // 
            this->ladderToolStripMenuItem1->Name = L"ladderToolStripMenuItem1";
            this->ladderToolStripMenuItem1->Size = System::Drawing::Size(183, 28);
            this->ladderToolStripMenuItem1->Text = L"Ladder";
            this->ladderToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::ladderToolStripMenuItem1_Click);
            // 
            // segmentationToolStripMenuItem
            // 
            this->segmentationToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->iterativeThresholdingToolStripMenuItem, 
                this->maxEntropyToolStripMenuItem, this->loGToolStripMenuItem});
            this->segmentationToolStripMenuItem->Name = L"segmentationToolStripMenuItem";
            this->segmentationToolStripMenuItem->Size = System::Drawing::Size(143, 28);
            this->segmentationToolStripMenuItem->Text = L"Segmentation";
            // 
            // iterativeThresholdingToolStripMenuItem
            // 
            this->iterativeThresholdingToolStripMenuItem->Name = L"iterativeThresholdingToolStripMenuItem";
            this->iterativeThresholdingToolStripMenuItem->Size = System::Drawing::Size(265, 28);
            this->iterativeThresholdingToolStripMenuItem->Text = L"Iterative thresholding";
            this->iterativeThresholdingToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::iterativeThresholdingToolStripMenuItem_Click);
            // 
            // maxEntropyToolStripMenuItem
            // 
            this->maxEntropyToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->dToolStripMenuItem, 
                this->dToolStripMenuItem1});
            this->maxEntropyToolStripMenuItem->Name = L"maxEntropyToolStripMenuItem";
            this->maxEntropyToolStripMenuItem->Size = System::Drawing::Size(265, 28);
            this->maxEntropyToolStripMenuItem->Text = L"Max Entropy";
            // 
            // dToolStripMenuItem
            // 
            this->dToolStripMenuItem->Name = L"dToolStripMenuItem";
            this->dToolStripMenuItem->Size = System::Drawing::Size(105, 28);
            this->dToolStripMenuItem->Text = L"1D";
            this->dToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dToolStripMenuItem_Click);
            // 
            // dToolStripMenuItem1
            // 
            this->dToolStripMenuItem1->Name = L"dToolStripMenuItem1";
            this->dToolStripMenuItem1->Size = System::Drawing::Size(105, 28);
            this->dToolStripMenuItem1->Text = L"2D";
            this->dToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::dToolStripMenuItem1_Click);
            // 
            // loGToolStripMenuItem
            // 
            this->loGToolStripMenuItem->Name = L"loGToolStripMenuItem";
            this->loGToolStripMenuItem->Size = System::Drawing::Size(265, 28);
            this->loGToolStripMenuItem->Text = L"LoG";
            this->loGToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::loGToolStripMenuItem_Click);
            // 
            // enhancementToolStripMenuItem
            // 
            this->enhancementToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->greyStretchToolStripMenuItem, 
                this->logToolStripMenuItem1, this->gammaToolStripMenuItem});
            this->enhancementToolStripMenuItem->Name = L"enhancementToolStripMenuItem";
            this->enhancementToolStripMenuItem->Size = System::Drawing::Size(203, 28);
            this->enhancementToolStripMenuItem->Text = L"Spatial Enhancement";
            // 
            // greyStretchToolStripMenuItem
            // 
            this->greyStretchToolStripMenuItem->Name = L"greyStretchToolStripMenuItem";
            this->greyStretchToolStripMenuItem->Size = System::Drawing::Size(273, 28);
            this->greyStretchToolStripMenuItem->Text = L"Piecewise Grey Stretch";
            this->greyStretchToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::greyStretchToolStripMenuItem_Click);
            // 
            // logToolStripMenuItem1
            // 
            this->logToolStripMenuItem1->Name = L"logToolStripMenuItem1";
            this->logToolStripMenuItem1->Size = System::Drawing::Size(273, 28);
            this->logToolStripMenuItem1->Text = L"Log";
            this->logToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form1::logToolStripMenuItem1_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->originalBox);
            this->groupBox1->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->groupBox1->Location = System::Drawing::Point(12, 50);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(569, 553);
            this->groupBox1->TabIndex = 1;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"Original";
            // 
            // originalBox
            // 
            this->originalBox->Location = System::Drawing::Point(14, 24);
            this->originalBox->Name = L"originalBox";
            this->originalBox->Size = System::Drawing::Size(512, 512);
            this->originalBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->originalBox->TabIndex = 0;
            this->originalBox->TabStop = false;
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->processedBox);
            this->groupBox2->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->groupBox2->Location = System::Drawing::Point(612, 50);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(651, 553);
            this->groupBox2->TabIndex = 2;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"Processed";
            // 
            // processedBox
            // 
            this->processedBox->Location = System::Drawing::Point(24, 24);
            this->processedBox->Name = L"processedBox";
            this->processedBox->Size = System::Drawing::Size(512, 512);
            this->processedBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
            this->processedBox->TabIndex = 0;
            this->processedBox->TabStop = false;
            // 
            // gammaToolStripMenuItem
            // 
            this->gammaToolStripMenuItem->Name = L"gammaToolStripMenuItem";
            this->gammaToolStripMenuItem->Size = System::Drawing::Size(273, 28);
            this->gammaToolStripMenuItem->Text = L"Gamma";
            this->gammaToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gammaToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1226, 637);
            this->Controls->Add(this->groupBox2);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"Form1";
            this->Text = L"Form1";
            this->menuStrip1->ResumeLayout(false);
            this->menuStrip1->PerformLayout();
            this->groupBox1->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->originalBox))->EndInit();
            this->groupBox2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->processedBox))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
        /*isprocessedImage=1，读取processedBitmap灰度图，isprocessedImage=0，读originalBitmap的灰度图*/
    private: void originalImageInitial(int isprocessedImage) {
                 for ( int i = 0; i < M; i++ ) {            
                     for ( int j = 0; j < N; j++ ) {
                         //获得灰度图像，存放在originalImage[][]中
                         if(isprocessedImage==1)
                             originalImage[i][j] = (int)(processedBitmap->GetPixel(j,i).R*0.299 + processedBitmap->GetPixel(j,i).G*0.587 + processedBitmap->GetPixel(j,i).B*0.114);
                         else
                             originalImage[i][j] = (int)(originalBitmap->GetPixel(j,i).R*0.299 + originalBitmap->GetPixel(j,i).G*0.587 + originalBitmap->GetPixel(j,i).B*0.114);
                         if(originalImage[i][j]<0)
                             originalImage[i][j] = 0;
                         if(originalImage[i][j]>255)
                             originalImage[i][j] = 255;
                         //获得RGB图像，存放在originalRGB[][][]中
                         originalRGB[i][j][0] = (int)originalBitmap->GetPixel(j,i).R;
                         originalRGB[i][j][1] = (int)originalBitmap->GetPixel(j,i).G;
                         originalRGB[i][j][2] = (int)originalBitmap->GetPixel(j,i).B;
                     }
                 }
             }
    private: void originalImageDelete()	//释放内存空间
             {
                 if(M>0 && N>0)
                 {
                     for ( int i = 0; i < M; i++ ) {
                         delete [] originalImage[i];
                         delete [] processedImage[i];
                         for (int j = 0; j < N; j++)
                         {
                             delete [] originalRGB[i][j];
                             delete [] processedRGB[i][j];
                         }
                         delete [] originalRGB[i];
                         delete [] processedRGB[i];
                     }
                     delete [] originalImage;
                     delete [] processedImage;
                     delete [] originalRGB;
                     delete [] processedRGB;
                 }

                 M=0;
                 N=0;
             }

    private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 intensityMapping = new int[256];
                 histogram = new int[256];
                 dHistogram = new double[256];
                 twoHistogram = new int * [256];
                 twoDHistogram = new double*[256];
                 for (int i = 0; i < 256; ++i) {
                     twoHistogram[i] = new int [256];
                     twoDHistogram[i] = new double [256];
                     memset(twoHistogram[i], 0, (sizeof(int) * 256));
                     memset(twoDHistogram[i], 0, (sizeof(double) * 256));
                 }
                 OpenFileDialog^ openDialog = gcnew OpenFileDialog();
                 openDialog->InitialDirectory = "E:\\";
                 openDialog ->Filter = "Bitmap Image(*.bmp)|*.bmp|Image(*.jpeg)|*.jpg";
                 openDialog ->RestoreDirectory = true;
                 if (openDialog ->ShowDialog() == Windows::Forms::DialogResult ::OK) {

                     String^ fileName = openDialog->FileName->ToString();

                     originalBitmap = gcnew Bitmap(fileName, true);
                     processedBitmap = gcnew Bitmap(fileName, true);

                     M = originalBitmap -> Height;
                     N = originalBitmap -> Width;

                     originalBox -> Image = originalBitmap;
                     processedBox -> Image = processedBitmap;

                     originalBox -> Refresh();
                     processedBox -> Refresh();

                     originalImage = new int* [M];
                     processedImage = new int* [M];
                     originalRGB = new int**[M];
                     processedRGB = new int**[M];

                     for (int i = 0; i < M; ++i) {
                         originalImage[i] = new int[N];
                         processedImage[i] = new int[N];
                         originalRGB[i] = new int * [N];
                         processedRGB[i] = new int * [N];
                         for (int j = 0; j < N; ++j) {
                             originalRGB[i][j] = new int [3];
                             processedRGB[i][j] = new int [3];
                         }
                     }

                     originalImageInitial(0);
                 }
             }
    private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 SaveFileDialog ^ saveFile1 = gcnew SaveFileDialog ();
                 saveFile1 -> InitialDirectory = "E:\\";
                 saveFile1 -> Filter = "Bitmap Image|*.bmp|JPeg Image|*.jpg|Gif Image|*.gif";
                 saveFile1 -> Title = "Save an Image File";
                 saveFile1 -> ShowDialog ();

                 if (saveFile1 -> FileName != "") {
                     switch ( saveFile1->FilterIndex ) {
                     case 1 :
                         processedBitmap -> Save(saveFile1 -> FileName,  System:: Drawing :: Imaging:: ImageFormat::Jpeg);
                         break;
                     case 2 :
                         processedBitmap -> Save(saveFile1 -> FileName,  System:: Drawing :: Imaging:: ImageFormat::Bmp);
                         break;
                     case 3 :
                         processedBitmap -> Save(saveFile1 -> FileName,  System:: Drawing :: Imaging:: ImageFormat::Gif);
                         break;
                     }
                 }
             }
             //private: System::Void processedBox_Click(System::Object^  sender, System::EventArgs^  e) {
             //         }
    private: System::Void sobelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 sobel(originalImage, processedImage, M, N);
                 displayProcessedImage();
             }

    private: System::Void prewittToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 prewitt(originalImage, processedImage, M, N);
                 displayProcessedImage();
             }

    private: System::Void laplaceToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 laplace2(originalImage, processedImage, M, N);
                 displayProcessedImage();
             }
    private: System::Void dFTToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 int *m_height = 0,*m_width = 0;
                 int a = 0;
                 int b = 0;
                 m_height = &a;
                 m_width = &b;
                 double ** d_originalImage = new double*[M];
                 for (int i = 0; i<M; i++)
                     d_originalImage[i] = new double [N]; 
                 originalImageInitial(0);
                 for(int i=0;i<M;i++)
                 {         
                     for(int j=0;j<N;j++)
                         d_originalImage[i][j] = originalImage[i][j];
                 }
                 Transform_FFT*  imgFT = new Transform_FFT(d_originalImage,N,M);
                 imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage 频域幅值
                 double ** d_processedImage = new double*[a];
                 for (int i = 0; i<a; i++)
                     d_processedImage[i] = new double [b];

                 imgFT->D(2,d_processedImage);			//D转换
                 
                 for(int i=0;i<M;i++)
                 {
                     for(int j=0;j<N;j++)
                     {
                         processedImage[i][j] = (int)(d_processedImage[i+(a-M)/2][j+(b-N)/2]*10);
                         if (processedImage[i][j] > 255)						                 
                             processedImage[i][j] = 255;				
                     }
                 }
                 displayProcessedImage();
             }
    private: System::Void iDFTToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 int *m_height = 0,*m_width = 0;
                 int a = 0;
                 int b = 0;
                 m_height = &a;
                 m_width = &b;
                 double ** d_originalImage = new double*[M];
                 for (int i = 0; i<M;i++)
                     d_originalImage[i] = new double [N];
                 originalImageInitial(0);
                 for(int i=0;i<M;i++)
                 {
                     for(int j=0;j<N;j++)
                     {
                         d_originalImage[i][j] = originalImage[i][j];
                     }
                 }
                 Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

                 imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

                 double ** d_processedImage = new double*[a];
                 
                 for (int i = 0; i<a; i++)		
                     d_processedImage[i] = new double [b];		
                
                 imgFT->IFourierEx(d_processedImage);
                 
                 for(int i=0;i<M;i++)
                 {
                     for(int j=0;j<N;j++)
                     {
                         processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                     }
                 }

                 displayProcessedImage();
             }

private: System::Void idealToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_high * sqrt((double) (M * M) + (N * N));
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = ideal_highpass(u, v, d0);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }

private: System::Void butterworthToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_high * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             int n = 1;
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = butterworth_lowpass(u, v, d0, n);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void expoToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_low * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             int n = 1;
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = exponent_highpass(u, v, d0, n);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void exponentialToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_low * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             int n = 1;
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = exponent_lowpass(u, v, d0, n);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void butterworthToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_high * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             int n = 1;
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = butterworth_highpass(u, v, d0, n);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void ladderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_high * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width)),
                 d1 = c_low * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = ladder_highpass(u, v, d0, d1);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void idealToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }

             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_low * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = ideal_lowpass(u, v, d0);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void ladderToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
             int *m_height = 0,*m_width = 0;
             int a = 0;
             int b = 0;
             m_height = &a;
             m_width = &b;
             double ** d_originalImage = new double*[M];
             for (int i = 0; i<M;i++)
                 d_originalImage[i] = new double [N];
             originalImageInitial(0);
             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     d_originalImage[i][j] = originalImage[i][j];
                 }
             }
             Transform_FFT* imgFT = new Transform_FFT(d_originalImage, N, M);

             imgFT->FourierEx(m_height,m_width);//傅里叶变换，processedImage就是频域的幅度值

             double ** d_processedImage = new double*[a];

             for (int i = 0; i<a; i++)		
                 d_processedImage[i] = new double [b];

             ComplexNumber ** F = imgFT->m_pFFTBuf;
             double d0 = c_low * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             double d1 = c_high * sqrt((double) (*m_height * *m_height) + (*m_width * *m_width));
             for (int u = 0; u < *m_height; ++u) {
                 for (int v = 0; v < *m_width; ++v) {
                     double h = ladder_lowpass(u, v, d0, d1);
                     F[u][v].imag *= h;
                     F[u][v].real *= h;
                 }
             }

             imgFT->IFourierEx(d_processedImage);

             for(int i=0;i<M;i++)
             {
                 for(int j=0;j<N;j++)
                 {
                     processedImage[i][j] = (int)(d_processedImage[i+(a-M)][j]);
                 }
             }

             displayProcessedImage();
         }
private: System::Void iterativeThresholdingToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
         generateHistogram(originalImage, M, N);
         int threshold = iterative_threshold(dHistogram, 256, 1);
         for (int i = 0; i < M; ++i) {
             for (int j = 0; j < N; ++j) {
                 processedImage[i][j] = (originalImage[i][j] > threshold ? 255 : 0);
             }
         }
         displayProcessedImage();
         }
private: System::Void loGToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             
             double** filter = new double* [3];

             for (int i = 0; i < 3; ++i)
                 filter[i] = new double[3];

             double sigma = 1;

             generateGaussianFilter(filter, 3, 3, sigma);

             int** temp = new int*[M];

             for (int i = 0; i < M; ++i)
                 temp[i] = new int[N];

             correlate(originalImage, temp, filter, M, N, 3, 3);

             laplace2(temp, processedImage, M, N);

             for (int i = 0; i < 3; ++i) delete[] filter[i];
             delete[] filter;
             for (int i = 0; i < M; ++i)delete[] temp[i];
             delete[] temp;

             displayProcessedImage();
         }

private: System::Void dToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             generateHistogram(originalImage, M, N);
             int thresh = maxEntropy1D(dHistogram, 256);
             for (int i = 0; i < M; ++i) {
                 for (int j = 0; j < N; ++j) {
                     processedImage[i][j] = (originalImage[i][j] > thresh ? 255 : 0);
                 }
             }
             displayProcessedImage();
         }
private: System::Void dToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
             generateHistogram(originalImage, M, N);
             int thresh = maxEntropy2D(twoDHistogram, 256);
             for (int i = 0; i < M; ++i) {
                 for (int j = 0; j < N; ++j) {
                     processedImage[i][j] = (originalImage[i][j] > thresh ? 255 : 0);
                 }
             }
             displayProcessedImage();
         }
private: System::Void greyStretchToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             
             /* 如果不为空，则释放内存*/
             if (pSrcImage) {
                 for (int i = 0; i < M; i++)
                 {
                     for (int j = 0; j < N; j++)
                     {
                         delete [] pSrcImage[i][j];
                     }
                     delete [] pSrcImage[i];
                 }
                 delete [] pSrcImage;
                 pSrcImage = 0;
             }

             if (pDestImage){
                 for (int i = 0; i < M; i++)
                 {
                     for (int j = 0; j < N; j++)
                     {
                         delete [] pDestImage[i][j];
                     }
                     delete [] pDestImage[i];
                 }
                 delete [] pDestImage;
                 pDestImage = 0;
             }	

             /* 为图像分配三维数组*/
             pSrcImage = new int**[M];
             pDestImage = new int**[M];
             for (int i = 0; i < M; i++) 
             {
                 pSrcImage[i] = new int*[N];
                 pDestImage[i] = new int*[N];
                 for (int j = 0; j < N; j++)
                 {
                     pSrcImage[i][j] = new int[3];
                     pDestImage[i][j] = new int[3];
                 }
             }
             for (int i = 0; i < M; i++)
             {
                 for (int j = 0; j < N; j++) 
                 {
                     pSrcImage[i][j][0] = pSrcImage[i][j][1] = pSrcImage[i][j][2]
                     = (int)(originalBitmap->GetPixel(j,i).R*0.299 +
                         originalBitmap->GetPixel(j,i).G*0.587 +
                         originalBitmap->GetPixel(j,i).B*0.114);
                 }
             }
             /* 将图像指针传给灰度拉伸窗口*/
             greyStretch.SetImage(pSrcImage, pDestImage, M, N, processedBitmap, processedBox);
             greyStretch.Show();
         }
private: System::Void logToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
             double c = 30;
             generateLogFilter(intensityMapping, c, 256);
             for (int i = 0; i < M; ++i) {
                 for (int j = 0; j < N; ++j)
                     processedImage[i][j] = intensityMapping[originalImage[i][j]];
             }
             displayProcessedImage();
         }
private: System::Void gammaToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
             double c = 30  ;
             double gamma = 0.4;
             generateGammaFilter(intensityMapping, c, gamma, 256);
             for (int i = 0; i < M; ++i) {
                 for (int j = 0; j < N; ++j)
                     processedImage[i][j] = intensityMapping[originalImage[i][j]];
             }
             displayProcessedImage();
         }
};
}

