#pragma once
#include<cmath>

namespace DigitalImageProcessLearning {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// GreyStretch 摘要
	/// </summary>
	public ref class GreyStretch : public System::Windows::Forms::Form
	{
    public:
        int ***pSrcImage;
        int ***pDestImage;

        int nWidth;
        int nHeight;

        Bitmap^ DestBitmap;
        PictureBox^ DestPictBox;

        double dPosX1,dPosX2,dPosY1,dPosY2;
        bool   bIsDrag;
        bool   bIsPoint1,bIsPoint2;


	public:
		GreyStretch(void)
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
		}

        void SetImage(int*** pSrcImage, int*** pDestImage, int nHeight, int nWidth, Bitmap^ DestBitmap, PictureBox^ DestPictBox)
        {
            this->pSrcImage = pSrcImage;
            this->pDestImage = pDestImage;
            this->nWidth = nWidth;
            this->nHeight = nHeight;
            this->DestBitmap = DestBitmap;
            this->DestPictBox = DestPictBox;

            dPosX1=dPosY1=50;
            dPosX2=dPosY2=150;
            bIsDrag=false;
            bIsPoint1 = bIsPoint2 = false;
        }


        void GrayStretch(int*** pixel,int*** tempPixel,int width,
            int height,double X1,double Y1,double X2,double Y2)
        {
            int map[256];

            double dTemp;

            for (int x=0;x<256;x++)
            {
                if(x<X1)
                {
                    if(fabs(X1-0)>0.0001)
                        dTemp=(float)Y1/(float)X1*x;
                    else
                        dTemp = 0;
                }
                else if(x<=X2)
                {
                    if(fabs(X2-X1)>0.0001)
                        dTemp=(float)(Y2-Y1)/(float)(X2-X1)*(x-X1)+Y1;
                    else
                        dTemp=x;
                }
                else
                {
                    if(fabs(Y2-255)>0.0001)
                        dTemp=(float)(255-Y2)/(float)(255-X2)*(x-X2)+Y2;
                    else
                        dTemp=255;
                }
                map[x]=int(dTemp+0.5);
            }
            for(int i = 0;i<height;i++)
            {
                for(int j=0;j<width;j++)
                {
                    int x = pixel[i][j][0];
                    tempPixel[i][j][0]=tempPixel[i][j][1]=tempPixel[i][j][2]=map[x];
                }
            }
        }
        //画线函数
        void RefreshLine()
        {
            GrayStretch(pSrcImage, pDestImage, nWidth, nHeight, dPosX1, dPosY1, dPosX2, dPosY2);
            for (int i = 0; i < nHeight; i++)
            {
                for (int j = 0; j < nWidth; j++)
                {
                    DestBitmap->SetPixel(j, i, Color::FromArgb(255, pDestImage[i][j][0], 
                        pDestImage[i][j][0], pDestImage[i][j][0]));
                }
            }
            DestPictBox->Refresh();

            int width = pictureBox1->Width;
            int height = pictureBox1->Height;
            float xMargin = 25;
            float yMargin = 20;
            Bitmap^ bmap = gcnew Bitmap(width, height);
            Graphics^ graphic = System::Drawing::Graphics::FromImage(bmap);
            pictureBox1->Image = bmap;
            // 使用白色背景
            Pen^ blackPen = gcnew Pen(Color::Black);
            SolidBrush^ brush = gcnew SolidBrush(Color::White);
            graphic->DrawLine(blackPen, 1, 1, width-1, 1);
            graphic->DrawLine(blackPen,1, 1, 1, height-1);
            graphic->DrawLine(blackPen, 1, height-1, width-1, height-1);
            graphic->DrawLine(blackPen, width-1, 1, width-1, height-1);
            blackPen->LineJoin = System::Drawing::Drawing2D::LineJoin::Bevel;
            blackPen->EndCap   = System::Drawing::Drawing2D::LineCap::ArrowAnchor;

            graphic->DrawLine(blackPen, xMargin, height-yMargin, width-xMargin, height-yMargin);
            graphic->DrawLine(blackPen, xMargin, height-yMargin, xMargin, yMargin);

            System::Drawing::Font^ font = gcnew System::Drawing::Font("Time New Roman",8,System::Drawing::FontStyle::Regular);

            graphic->TextRenderingHint::set(System::Drawing::Text::TextRenderingHint::SingleBitPerPixelGridFit);

            SolidBrush^ blackBrush = gcnew SolidBrush(System::Drawing::Color::Black);

            graphic->DrawString("x", font, blackBrush, width-xMargin+3, height-yMargin-5); 
            graphic->DrawString("y", font, blackBrush, xMargin/2, yMargin/2);
            graphic->DrawString("0", font, blackBrush, xMargin/2, height-yMargin+2);

            blackPen->LineJoin = System::Drawing::Drawing2D::LineJoin::Bevel;
            blackPen->EndCap   = System::Drawing::Drawing2D::LineCap::Custom;

            array<int>^ x_label = {50,100,150,200,255};
            size_t j = 0;

            for ( size_t i = 0; i < 255/5+1; i++ )
            {	
                if( j+1 == i/10 )
                {
                    graphic->DrawLine(blackPen, xMargin+5*i, height-yMargin+5, xMargin+5*i, height-yMargin);
                    graphic->DrawString(x_label[j].ToString(), font, blackBrush, xMargin+5*(i-2), height-yMargin+5);
                    j++;
                }
                else
                {
                    graphic->DrawLine(blackPen, xMargin+5*i, height-yMargin+3, xMargin+5*i, height-yMargin);

                }
            }

            array<int>^ y_label = {255/5,2*255/5,3*255/5,4*255/5,255};

            for ( int i = 0; i < (int)((height-2*yMargin)/5); i++ )
            {
                if( i == (int)((height-2*yMargin)/5)-1)
                {
                    graphic->DrawLine(blackPen, xMargin-3, height-yMargin-5*i, xMargin, height-yMargin-5*i);
                    graphic->DrawString(y_label[4].ToString(), font, blackBrush, xMargin-24, height-yMargin-5*i);
                }
                else
                    graphic->DrawLine(blackPen, xMargin-3, height-yMargin-5*i, xMargin, height-yMargin-5*i);
            }
            Pen^ bluePen = gcnew Pen(Color::Blue);
            Pen^ redPen = gcnew Pen(Color::Red);

            Rectangle rect1((int)dPosX1,height-(int)dPosY1, 5, 5);
            graphic->DrawLine(blackPen, xMargin, height-yMargin, (float)dPosX1, height-(float)dPosY1);
            graphic->DrawEllipse(redPen,rect1); 
            graphic->DrawLine(bluePen, (float)dPosX1, height-(float)dPosY1, (float)dPosX2, height-(float)dPosY2);
            Rectangle rect2((int)dPosX2, height-(int)dPosY2, 5,5);
            graphic->DrawEllipse(redPen,rect2); 
            graphic->DrawLine(blackPen, (float)dPosX2, height-(float)dPosY2, width-xMargin, (float)yMargin);

            pictureBox1->Refresh();
        }

	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~GreyStretch()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    protected: 
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::TextBox^  textBox2;
    private: System::Windows::Forms::TextBox^  textBox4;
    private: System::Windows::Forms::TextBox^  textBox3;

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
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->textBox4 = (gcnew System::Windows::Forms::TextBox());
            this->textBox3 = (gcnew System::Windows::Forms::TextBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
            this->SuspendLayout();
            // 
            // pictureBox1
            // 
            this->pictureBox1->Location = System::Drawing::Point(49, 12);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(691, 503);
            this->pictureBox1->TabIndex = 0;
            this->pictureBox1->TabStop = false;
            this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &GreyStretch::pictureBox1_MouseDown);
            this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &GreyStretch::pictureBox1_MouseMove);
            this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &GreyStretch::pictureBox1_MouseUp);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"黑体", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->label1->Location = System::Drawing::Point(10, 534);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(36, 24);
            this->label1->TabIndex = 1;
            this->label1->Text = L"X1";
            this->label1->Click += gcnew System::EventHandler(this, &GreyStretch::label1_Click);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"黑体", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->label2->Location = System::Drawing::Point(10, 580);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(36, 24);
            this->label2->TabIndex = 2;
            this->label2->Text = L"Y1";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Font = (gcnew System::Drawing::Font(L"黑体", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->label3->Location = System::Drawing::Point(422, 580);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(36, 24);
            this->label3->TabIndex = 4;
            this->label3->Text = L"Y2";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Font = (gcnew System::Drawing::Font(L"黑体", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->label4->Location = System::Drawing::Point(422, 534);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(36, 24);
            this->label4->TabIndex = 3;
            this->label4->Text = L"X2";
            // 
            // textBox1
            // 
            this->textBox1->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->textBox1->Location = System::Drawing::Point(63, 536);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(266, 31);
            this->textBox1->TabIndex = 5;
            this->textBox1->TextChanged += gcnew System::EventHandler(this, &GreyStretch::textBox1_TextChanged);
            // 
            // textBox2
            // 
            this->textBox2->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->textBox2->Location = System::Drawing::Point(63, 580);
            this->textBox2->Name = L"textBox2";
            this->textBox2->Size = System::Drawing::Size(266, 31);
            this->textBox2->TabIndex = 6;
            this->textBox2->TextChanged += gcnew System::EventHandler(this, &GreyStretch::textBox2_TextChanged);
            // 
            // textBox4
            // 
            this->textBox4->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->textBox4->Location = System::Drawing::Point(474, 582);
            this->textBox4->Name = L"textBox4";
            this->textBox4->Size = System::Drawing::Size(266, 31);
            this->textBox4->TabIndex = 8;
            this->textBox4->TextChanged += gcnew System::EventHandler(this, &GreyStretch::textBox4_TextChanged);
            // 
            // textBox3
            // 
            this->textBox3->Font = (gcnew System::Drawing::Font(L"微软雅黑", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->textBox3->Location = System::Drawing::Point(474, 538);
            this->textBox3->Name = L"textBox3";
            this->textBox3->Size = System::Drawing::Size(266, 31);
            this->textBox3->TabIndex = 7;
            this->textBox3->TextChanged += gcnew System::EventHandler(this, &GreyStretch::textBox3_TextChanged);
            // 
            // GreyStretch
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(803, 626);
            this->Controls->Add(this->textBox4);
            this->Controls->Add(this->textBox3);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->textBox1);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->pictureBox1);
            this->Name = L"GreyStretch";
            this->Text = L"GreyStretch";
            this->Load += gcnew System::EventHandler(this, &GreyStretch::GreyStretch_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
             }
    private: System::Void GreyStretch_Load(System::Object^  sender, System::EventArgs^  e) {
                 textBox1->Text="50";
                 textBox2->Text="50";
                 textBox3->Text="150";
                 textBox4->Text="150";
                 RefreshLine();
             }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             String^ str = textBox1->Text;
             if(str->Length==0)
                 return;
             dPosX1 = Convert::ToDouble(textBox1->Text);
             if(dPosX1<0)
             {
                 dPosX1=0;
                 textBox1->Text="0";
             }
             if(dPosX1>dPosX2)
             {
                 dPosX1=dPosX2;
             }
             RefreshLine();
         }
private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             String^ str = textBox2->Text;
             if(str->Length==0)
                 return;
             dPosY1 = Convert::ToDouble(textBox2->Text);
             if(dPosY1<0)
             {
                 dPosY1=0;
                 textBox2->Text="0";
             }
             if(dPosY1>dPosY2)
             {
                 dPosY1=dPosY2;
             }
             RefreshLine();
         }
private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             String^ str = textBox3->Text;
             if(str->Length==0)
                 return;
             dPosX2 = Convert::ToDouble(textBox3->Text);
             if(dPosX2<dPosX1)
             {
                 dPosX2=dPosX1;
             }
             if(dPosX2>255)
             {
                 dPosX2=255;
                 textBox3->Text="255";
             }
             RefreshLine();

         }
private: System::Void textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             String^ str = textBox4->Text;
             if(str->Length==0)
                 return;
             dPosY2 = Convert::ToDouble(textBox4->Text);
             if(dPosY2<dPosY1)
             {
                 dPosY2=dPosY1;
             }
             if(dPosY2>255)
             {
                 dPosY2=255;
                 textBox4->Text="255";
             }
             RefreshLine();

         }
private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
             int height = pictureBox1->Height;
             if(e->X > dPosX1-3 && e->X < dPosX1 + 8 && e->Y > height-dPosY1-3 && e->Y < height-dPosY1+8)
             {
                 bIsPoint1=true;bIsPoint2=false;bIsDrag=true;
             }
             else if(e->X>dPosX2-3&&e->X<dPosX2+8&&e->Y>height-dPosY2-3&&e->Y<height-dPosY2+8)
             {
                 bIsPoint1=false;bIsPoint2=true;bIsDrag=true;
             }
             else
             {
                 bIsDrag=false;
             }
         }
private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
             if(bIsDrag==true)
             {
                 if(bIsPoint1==true)
                 {
                     dPosX1=e->X;
                     dPosY1=pictureBox1->Height - e->Y;
                     if(dPosX1 < 0) { dPosX1=0;textBox1->Text="0";}
                     if(dPosX1>dPosX2) {dPosX1 = dPosX2;}
                     if(dPosY1<0) {dPosY1 = 0;textBox2->Text="0";}
                     if(dPosY1>dPosY2) {dPosY1 = dPosY2;}
                     textBox1->Text = Convert::ToString(dPosX1);textBox1->Refresh();
                     textBox2->Text = Convert::ToString(dPosY1);textBox2->Refresh();
                 }
                 else if(bIsPoint2 = true)
                 {
                     dPosX2=e->X;
                     dPosY2=pictureBox1->Height - e->Y;
                     if(dPosX2 < dPosX1) { dPosX2 = dPosX1;}
                     if(dPosX2>255) {dPosX2 = 255;textBox3->Text="255";}
                     if(dPosY2<dPosY1) {dPosY2 = dPosY1;}
                     if(dPosY2>255) {dPosY2 = 255;textBox4->Text="255";}
                     textBox3->Text = Convert::ToString(dPosX2);textBox3->Refresh();
                     textBox4->Text = Convert::ToString(dPosY2);textBox4->Refresh();
                 }
                 RefreshLine();
             }
         }
private: System::Void pictureBox1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
             		    bIsPoint1=false;
                        bIsPoint2=false;bIsDrag=false;
         }
};
}
