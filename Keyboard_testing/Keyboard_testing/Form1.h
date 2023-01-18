#pragma once
#include <string>
#include <string>
//#include "stdafx.h"
//#include <windows.h>
#include <iostream>
#include <conio.h>

namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			timer1->Start();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ Q;
	private: System::Windows::Forms::Button^ W;
	private: System::Windows::Forms::Button^ E;

	protected:


	private: System::Windows::Forms::CheckBox^ Shift;
	private: System::Windows::Forms::Button^ Backspace;
	private: System::Windows::Forms::Button^ Space;
	private: System::Windows::Forms::Button^ Enter;
	private: System::Windows::Forms::TextBox^ textBox1;

	private: System::Windows::Forms::Label^ label1;

	private: System::Windows::Forms::CheckBox^ CAPS;

	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Timer^ timer2;

	private: System::Windows::Forms::Button^ R;
	private: System::Windows::Forms::Button^ T;
	private: System::Windows::Forms::Button^ Y;
	private: System::Windows::Forms::Button^ U;
	private: System::Windows::Forms::Button^ I;
	private: System::Windows::Forms::Button^ O;
	private: System::Windows::Forms::Button^ P;
	private: System::Windows::Forms::Button^ A;
	private: System::Windows::Forms::Button^ Z;
	private: System::Windows::Forms::Button^ X;


	private: System::Windows::Forms::Button^ S;
	private: System::Windows::Forms::Button^ D;
	private: System::Windows::Forms::Button^ C;

	private: System::Windows::Forms::Button^ F;
	private: System::Windows::Forms::Button^ V;


	private: System::Windows::Forms::Button^ G;
	private: System::Windows::Forms::Button^ B;


	private: System::Windows::Forms::Button^ H;
	private: System::Windows::Forms::Button^ N;


	private: System::Windows::Forms::Button^ J;
	private: System::Windows::Forms::Button^ M;


	private: System::Windows::Forms::Button^ K;
	private: System::Windows::Forms::Button^ L;
	private: System::Windows::Forms::Button^ comma;
	private: System::Windows::Forms::Button^ Period;
	private: System::Windows::Forms::TextBox^ textBox2;
















	private: System::ComponentModel::IContainer^ components;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Q = (gcnew System::Windows::Forms::Button());
			this->W = (gcnew System::Windows::Forms::Button());
			this->E = (gcnew System::Windows::Forms::Button());
			this->Shift = (gcnew System::Windows::Forms::CheckBox());
			this->Backspace = (gcnew System::Windows::Forms::Button());
			this->Space = (gcnew System::Windows::Forms::Button());
			this->Enter = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->CAPS = (gcnew System::Windows::Forms::CheckBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->R = (gcnew System::Windows::Forms::Button());
			this->T = (gcnew System::Windows::Forms::Button());
			this->Y = (gcnew System::Windows::Forms::Button());
			this->U = (gcnew System::Windows::Forms::Button());
			this->I = (gcnew System::Windows::Forms::Button());
			this->O = (gcnew System::Windows::Forms::Button());
			this->P = (gcnew System::Windows::Forms::Button());
			this->A = (gcnew System::Windows::Forms::Button());
			this->Z = (gcnew System::Windows::Forms::Button());
			this->X = (gcnew System::Windows::Forms::Button());
			this->S = (gcnew System::Windows::Forms::Button());
			this->D = (gcnew System::Windows::Forms::Button());
			this->C = (gcnew System::Windows::Forms::Button());
			this->F = (gcnew System::Windows::Forms::Button());
			this->V = (gcnew System::Windows::Forms::Button());
			this->G = (gcnew System::Windows::Forms::Button());
			this->B = (gcnew System::Windows::Forms::Button());
			this->H = (gcnew System::Windows::Forms::Button());
			this->N = (gcnew System::Windows::Forms::Button());
			this->J = (gcnew System::Windows::Forms::Button());
			this->M = (gcnew System::Windows::Forms::Button());
			this->K = (gcnew System::Windows::Forms::Button());
			this->L = (gcnew System::Windows::Forms::Button());
			this->comma = (gcnew System::Windows::Forms::Button());
			this->Period = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// Q
			// 
			this->Q->Location = System::Drawing::Point(191, 207);
			this->Q->Margin = System::Windows::Forms::Padding(4);
			this->Q->Name = L"Q";
			this->Q->Size = System::Drawing::Size(50, 50);
			this->Q->TabIndex = 0;
			this->Q->Text = L"Q";
			this->Q->UseVisualStyleBackColor = true;
			this->Q->Click += gcnew System::EventHandler(this, &Form1::Q_Click);
			// 
			// W
			// 
			this->W->Location = System::Drawing::Point(249, 207);
			this->W->Margin = System::Windows::Forms::Padding(4);
			this->W->Name = L"W";
			this->W->Size = System::Drawing::Size(50, 50);
			this->W->TabIndex = 1;
			this->W->Text = L"W";
			this->W->UseVisualStyleBackColor = true;
			this->W->Click += gcnew System::EventHandler(this, &Form1::W_Click);
			// 
			// E
			// 
			this->E->Location = System::Drawing::Point(307, 207);
			this->E->Margin = System::Windows::Forms::Padding(4);
			this->E->Name = L"E";
			this->E->Size = System::Drawing::Size(50, 50);
			this->E->TabIndex = 2;
			this->E->Text = L"E";
			this->E->UseVisualStyleBackColor = true;
			this->E->Click += gcnew System::EventHandler(this, &Form1::E_Click);
			// 
			// Shift
			// 
			this->Shift->Appearance = System::Windows::Forms::Appearance::Button;
			this->Shift->Location = System::Drawing::Point(13, 299);
			this->Shift->Margin = System::Windows::Forms::Padding(4);
			this->Shift->Name = L"Shift";
			this->Shift->Size = System::Drawing::Size(168, 74);
			this->Shift->TabIndex = 31;
			this->Shift->TabStop = false;
			this->Shift->Text = L"Shift";
			this->Shift->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->Shift->UseVisualStyleBackColor = true;
			this->Shift->CheckedChanged += gcnew System::EventHandler(this, &Form1::Shift_CheckedChanged);
			// 
			// Backspace
			// 
			this->Backspace->Location = System::Drawing::Point(771, 207);
			this->Backspace->Margin = System::Windows::Forms::Padding(4);
			this->Backspace->Name = L"Backspace";
			this->Backspace->Size = System::Drawing::Size(168, 50);
			this->Backspace->TabIndex = 10;
			this->Backspace->Text = L"Backsapce";
			this->Backspace->UseVisualStyleBackColor = true;
			this->Backspace->Click += gcnew System::EventHandler(this, &Form1::Backspace_Click);
			// 
			// Space
			// 
			this->Space->Location = System::Drawing::Point(191, 380);
			this->Space->Margin = System::Windows::Forms::Padding(4);
			this->Space->Name = L"Space";
			this->Space->Size = System::Drawing::Size(748, 48);
			this->Space->TabIndex = 30;
			this->Space->Text = L"Space";
			this->Space->UseVisualStyleBackColor = true;
			this->Space->Click += gcnew System::EventHandler(this, &Form1::Space_Click);
			// 
			// Enter
			// 
			this->Enter->Location = System::Drawing::Point(713, 265);
			this->Enter->Margin = System::Windows::Forms::Padding(4);
			this->Enter->Name = L"Enter";
			this->Enter->Size = System::Drawing::Size(226, 107);
			this->Enter->TabIndex = 29;
			this->Enter->Text = L"Enter";
			this->Enter->UseVisualStyleBackColor = true;
			this->Enter->Click += gcnew System::EventHandler(this, &Form1::Enter_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(186, 161);
			this->textBox1->Margin = System::Windows::Forms::Padding(4);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(753, 38);
			this->textBox1->TabIndex = 7;
			this->textBox1->TabStop = false;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(775, 167);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 16);
			this->label1->TabIndex = 9;
			// 
			// CAPS
			// 
			this->CAPS->Appearance = System::Windows::Forms::Appearance::Button;
			this->CAPS->Location = System::Drawing::Point(63, 241);
			this->CAPS->Margin = System::Windows::Forms::Padding(4);
			this->CAPS->Name = L"CAPS";
			this->CAPS->Size = System::Drawing::Size(120, 50);
			this->CAPS->TabIndex = 11;
			this->CAPS->TabStop = false;
			this->CAPS->Text = L"Cap Lock";
			this->CAPS->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->CAPS->UseVisualStyleBackColor = true;
			this->CAPS->CheckedChanged += gcnew System::EventHandler(this, &Form1::CAPS_CheckedChanged);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// timer2
			// 
			this->timer2->Enabled = true;
			this->timer2->Interval = 2000;
			this->timer2->Tick += gcnew System::EventHandler(this, &Form1::timer2_Tick);
			// 
			// R
			// 
			this->R->Location = System::Drawing::Point(365, 207);
			this->R->Margin = System::Windows::Forms::Padding(4);
			this->R->Name = L"R";
			this->R->Size = System::Drawing::Size(50, 50);
			this->R->TabIndex = 3;
			this->R->Text = L"R";
			this->R->UseVisualStyleBackColor = true;
			this->R->Click += gcnew System::EventHandler(this, &Form1::R_Click);
			// 
			// T
			// 
			this->T->Location = System::Drawing::Point(423, 207);
			this->T->Margin = System::Windows::Forms::Padding(4);
			this->T->Name = L"T";
			this->T->Size = System::Drawing::Size(50, 50);
			this->T->TabIndex = 4;
			this->T->Text = L"T";
			this->T->UseVisualStyleBackColor = true;
			this->T->Click += gcnew System::EventHandler(this, &Form1::T_Click);
			// 
			// Y
			// 
			this->Y->Location = System::Drawing::Point(481, 207);
			this->Y->Margin = System::Windows::Forms::Padding(4);
			this->Y->Name = L"Y";
			this->Y->Size = System::Drawing::Size(50, 50);
			this->Y->TabIndex = 5;
			this->Y->Text = L"Y";
			this->Y->UseVisualStyleBackColor = true;
			this->Y->Click += gcnew System::EventHandler(this, &Form1::Y_Click);
			// 
			// U
			// 
			this->U->Location = System::Drawing::Point(539, 207);
			this->U->Margin = System::Windows::Forms::Padding(4);
			this->U->Name = L"U";
			this->U->Size = System::Drawing::Size(50, 50);
			this->U->TabIndex = 6;
			this->U->Text = L"U";
			this->U->UseVisualStyleBackColor = true;
			this->U->Click += gcnew System::EventHandler(this, &Form1::U_Click);
			// 
			// I
			// 
			this->I->Location = System::Drawing::Point(597, 207);
			this->I->Margin = System::Windows::Forms::Padding(4);
			this->I->Name = L"I";
			this->I->Size = System::Drawing::Size(50, 50);
			this->I->TabIndex = 7;
			this->I->Text = L"I";
			this->I->UseVisualStyleBackColor = true;
			this->I->Click += gcnew System::EventHandler(this, &Form1::I_Click);
			// 
			// O
			// 
			this->O->Location = System::Drawing::Point(655, 207);
			this->O->Margin = System::Windows::Forms::Padding(4);
			this->O->Name = L"O";
			this->O->Size = System::Drawing::Size(50, 50);
			this->O->TabIndex = 8;
			this->O->Text = L"O";
			this->O->UseVisualStyleBackColor = true;
			this->O->Click += gcnew System::EventHandler(this, &Form1::O_Click);
			// 
			// P
			// 
			this->P->Location = System::Drawing::Point(713, 207);
			this->P->Margin = System::Windows::Forms::Padding(4);
			this->P->Name = L"P";
			this->P->Size = System::Drawing::Size(50, 50);
			this->P->TabIndex = 9;
			this->P->Text = L"P";
			this->P->UseVisualStyleBackColor = true;
			this->P->Click += gcnew System::EventHandler(this, &Form1::P_Click);
			// 
			// A
			// 
			this->A->Location = System::Drawing::Point(191, 265);
			this->A->Margin = System::Windows::Forms::Padding(4);
			this->A->Name = L"A";
			this->A->Size = System::Drawing::Size(50, 50);
			this->A->TabIndex = 11;
			this->A->Text = L"A";
			this->A->UseVisualStyleBackColor = true;
			this->A->Click += gcnew System::EventHandler(this, &Form1::A_Click);
			// 
			// Z
			// 
			this->Z->Location = System::Drawing::Point(191, 323);
			this->Z->Margin = System::Windows::Forms::Padding(4);
			this->Z->Name = L"Z";
			this->Z->Size = System::Drawing::Size(50, 50);
			this->Z->TabIndex = 20;
			this->Z->Text = L"Z";
			this->Z->UseVisualStyleBackColor = true;
			this->Z->Click += gcnew System::EventHandler(this, &Form1::Z_Click);
			// 
			// X
			// 
			this->X->Location = System::Drawing::Point(249, 323);
			this->X->Margin = System::Windows::Forms::Padding(4);
			this->X->Name = L"X";
			this->X->Size = System::Drawing::Size(50, 50);
			this->X->TabIndex = 21;
			this->X->Text = L"X";
			this->X->UseVisualStyleBackColor = true;
			this->X->Click += gcnew System::EventHandler(this, &Form1::X_Click);
			// 
			// S
			// 
			this->S->Location = System::Drawing::Point(249, 265);
			this->S->Margin = System::Windows::Forms::Padding(4);
			this->S->Name = L"S";
			this->S->Size = System::Drawing::Size(50, 50);
			this->S->TabIndex = 12;
			this->S->Text = L"S";
			this->S->UseVisualStyleBackColor = true;
			this->S->Click += gcnew System::EventHandler(this, &Form1::S_Click);
			// 
			// D
			// 
			this->D->Location = System::Drawing::Point(307, 265);
			this->D->Margin = System::Windows::Forms::Padding(4);
			this->D->Name = L"D";
			this->D->Size = System::Drawing::Size(50, 50);
			this->D->TabIndex = 13;
			this->D->Text = L"D";
			this->D->UseVisualStyleBackColor = true;
			this->D->Click += gcnew System::EventHandler(this, &Form1::D_Click);
			// 
			// C
			// 
			this->C->Location = System::Drawing::Point(307, 323);
			this->C->Margin = System::Windows::Forms::Padding(4);
			this->C->Name = L"C";
			this->C->Size = System::Drawing::Size(50, 50);
			this->C->TabIndex = 22;
			this->C->Text = L"C";
			this->C->UseVisualStyleBackColor = true;
			this->C->Click += gcnew System::EventHandler(this, &Form1::C_Click);
			// 
			// F
			// 
			this->F->Location = System::Drawing::Point(365, 265);
			this->F->Margin = System::Windows::Forms::Padding(4);
			this->F->Name = L"F";
			this->F->Size = System::Drawing::Size(50, 50);
			this->F->TabIndex = 14;
			this->F->Text = L"F";
			this->F->UseVisualStyleBackColor = true;
			this->F->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// V
			// 
			this->V->Location = System::Drawing::Point(365, 322);
			this->V->Margin = System::Windows::Forms::Padding(4);
			this->V->Name = L"V";
			this->V->Size = System::Drawing::Size(50, 50);
			this->V->TabIndex = 23;
			this->V->Text = L"V";
			this->V->UseVisualStyleBackColor = true;
			this->V->Click += gcnew System::EventHandler(this, &Form1::V_Click);
			// 
			// G
			// 
			this->G->Location = System::Drawing::Point(423, 265);
			this->G->Margin = System::Windows::Forms::Padding(4);
			this->G->Name = L"G";
			this->G->Size = System::Drawing::Size(50, 50);
			this->G->TabIndex = 15;
			this->G->Text = L"G";
			this->G->UseVisualStyleBackColor = true;
			this->G->Click += gcnew System::EventHandler(this, &Form1::G_Click);
			// 
			// B
			// 
			this->B->Location = System::Drawing::Point(423, 323);
			this->B->Margin = System::Windows::Forms::Padding(4);
			this->B->Name = L"B";
			this->B->Size = System::Drawing::Size(50, 50);
			this->B->TabIndex = 24;
			this->B->Text = L"B";
			this->B->UseVisualStyleBackColor = true;
			this->B->Click += gcnew System::EventHandler(this, &Form1::B_Click);
			// 
			// H
			// 
			this->H->Location = System::Drawing::Point(481, 265);
			this->H->Margin = System::Windows::Forms::Padding(4);
			this->H->Name = L"H";
			this->H->Size = System::Drawing::Size(50, 50);
			this->H->TabIndex = 16;
			this->H->Text = L"H";
			this->H->UseVisualStyleBackColor = true;
			this->H->Click += gcnew System::EventHandler(this, &Form1::H_Click);
			// 
			// N
			// 
			this->N->Location = System::Drawing::Point(481, 322);
			this->N->Margin = System::Windows::Forms::Padding(4);
			this->N->Name = L"N";
			this->N->Size = System::Drawing::Size(50, 50);
			this->N->TabIndex = 25;
			this->N->Text = L"N";
			this->N->UseVisualStyleBackColor = true;
			this->N->Click += gcnew System::EventHandler(this, &Form1::button12_Click);
			// 
			// J
			// 
			this->J->Location = System::Drawing::Point(539, 265);
			this->J->Margin = System::Windows::Forms::Padding(4);
			this->J->Name = L"J";
			this->J->Size = System::Drawing::Size(50, 50);
			this->J->TabIndex = 17;
			this->J->Text = L"J";
			this->J->UseVisualStyleBackColor = true;
			this->J->Click += gcnew System::EventHandler(this, &Form1::J_Click);
			// 
			// M
			// 
			this->M->Location = System::Drawing::Point(539, 322);
			this->M->Margin = System::Windows::Forms::Padding(4);
			this->M->Name = L"M";
			this->M->Size = System::Drawing::Size(50, 50);
			this->M->TabIndex = 26;
			this->M->Text = L"M";
			this->M->UseVisualStyleBackColor = true;
			this->M->Click += gcnew System::EventHandler(this, &Form1::M_Click);
			// 
			// K
			// 
			this->K->Location = System::Drawing::Point(597, 265);
			this->K->Margin = System::Windows::Forms::Padding(4);
			this->K->Name = L"K";
			this->K->Size = System::Drawing::Size(50, 50);
			this->K->TabIndex = 18;
			this->K->Text = L"K";
			this->K->UseVisualStyleBackColor = true;
			this->K->Click += gcnew System::EventHandler(this, &Form1::K_Click);
			// 
			// L
			// 
			this->L->Location = System::Drawing::Point(655, 265);
			this->L->Margin = System::Windows::Forms::Padding(4);
			this->L->Name = L"L";
			this->L->Size = System::Drawing::Size(50, 50);
			this->L->TabIndex = 19;
			this->L->Text = L"L";
			this->L->UseVisualStyleBackColor = true;
			this->L->Click += gcnew System::EventHandler(this, &Form1::L_Click);
			// 
			// comma
			// 
			this->comma->Location = System::Drawing::Point(597, 322);
			this->comma->Margin = System::Windows::Forms::Padding(4);
			this->comma->Name = L"comma";
			this->comma->Size = System::Drawing::Size(50, 50);
			this->comma->TabIndex = 27;
			this->comma->Text = L",";
			this->comma->UseVisualStyleBackColor = true;
			this->comma->Click += gcnew System::EventHandler(this, &Form1::comma_Click);
			// 
			// Period
			// 
			this->Period->Location = System::Drawing::Point(655, 322);
			this->Period->Margin = System::Windows::Forms::Padding(4);
			this->Period->Name = L"Period";
			this->Period->Size = System::Drawing::Size(50, 50);
			this->Period->TabIndex = 28;
			this->Period->Text = L".";
			this->Period->UseVisualStyleBackColor = true;
			this->Period->Click += gcnew System::EventHandler(this, &Form1::Period_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(979, 438);
			this->textBox2->Margin = System::Windows::Forms::Padding(4);
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(10, 10);
			this->textBox2->TabIndex = 31;
			this->textBox2->TabStop = false;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1079, 648);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->Period);
			this->Controls->Add(this->comma);
			this->Controls->Add(this->L);
			this->Controls->Add(this->K);
			this->Controls->Add(this->M);
			this->Controls->Add(this->J);
			this->Controls->Add(this->N);
			this->Controls->Add(this->H);
			this->Controls->Add(this->B);
			this->Controls->Add(this->G);
			this->Controls->Add(this->V);
			this->Controls->Add(this->F);
			this->Controls->Add(this->C);
			this->Controls->Add(this->D);
			this->Controls->Add(this->S);
			this->Controls->Add(this->X);
			this->Controls->Add(this->Z);
			this->Controls->Add(this->A);
			this->Controls->Add(this->P);
			this->Controls->Add(this->O);
			this->Controls->Add(this->I);
			this->Controls->Add(this->U);
			this->Controls->Add(this->Y);
			this->Controls->Add(this->T);
			this->Controls->Add(this->R);
			this->Controls->Add(this->CAPS);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->Enter);
			this->Controls->Add(this->Space);
			this->Controls->Add(this->Backspace);
			this->Controls->Add(this->Shift);
			this->Controls->Add(this->E);
			this->Controls->Add(this->W);
			this->Controls->Add(this->Q);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		String^ word;
		char* pchar;
		int tab = 0;
		int tick = 0;
		int t2 = 0;
		//char* pchar = &word[];

		int cursor = 1; // cusor index

		///////////////////////// MOUSE MOVMENT ///////////////////////////////
		/*
		class click
		{
			[D11Import('useer32.d11')]
			static extern void mouseEvent(int dwFlags, int dx, int dy, int dw Data, int dwExtra);

			[Flags]

			public enum MouseEventFlags
			{
				LEFTDOWN = 0X00000002;
				LEFTUP = 0X00000004;
				RIGHTDOWN = 0X00000008;
				RIGTHUP = 0X0000010;
				MIDDLEDOWN = 0X00000020;
				MIDDLEUP = 0X00000040;
				MOVE = 0X00000001;
				ABSOLUTE = 0X00008000;
			};
			public void leftclick(Point p)
			{
				cursor.Position = p;
			}
			public void rightclick(Point p)
			{
				cursor.Position = p;
				mouse_event((int)(mouseEventFlags.RIGHTDOWN), 0, 0, 0, 0);
				mouse_event((int)(mouseEventFlags.RIGHTUP), 0, 0, 0, 0);

			}
			public void doubleclick(Point p)
			{
				cursor.Position = p;
				mouse_event((int)(mouseEventFlags.LEFTDOWN), 0, 0, 0, 0);
				mouse_event((int)(mouseEventFlags.LEFTDUP), 0, 0, 0, 0);

			}
		};
		*/



		   ///////////////////// LETTERS //////////////////////
	private: System::Void Q_Click(System::Object^ sender, System::EventArgs^ e) {

		if (Shift->Checked == true || CAPS->Checked == true)
		{
			word = word + "Q";
			Shift->Checked = false;
		}
		else
		{
			word = word + "q";
		}
		this->textBox1->Text = word;
		TabIndex = 2;
		//this->textBox2->;
		tab = 0;


	}
	private: System::Void W_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "W";

		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void E_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "E";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;

	}
	private: System::Void R_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "R";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}


		   ///////////////////// Key Board Functions //////////////////
	private: System::Void Backspace_Click(System::Object^ sender, System::EventArgs^ e) {


		if (textBox1->Text->Length > 0)
		{
			textBox1->Text = textBox1->Text->Remove(textBox1->Text->Length - 1, 1);
		}
		this->word = textBox1->Text;
		tab = 0;
		tick = 1;
		
		if (tick > 1){																												
			word = "";
			this->textBox1->Text = word;
			
		}
		this->Text = tick.ToString();
		tick = 0;

	}
	private: System::Void Shift_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		tab = 0;
	}
	private: System::Void Space_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + " ";

		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void Enter_Click(System::Object^ sender, System::EventArgs^ e) {

		word = "";

		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}

		   //////////////// Output //////////////////
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		tab = 0;

	}
	private: System::Void CursorIndex_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {



	}

	private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {

	}

	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		tab++;
		this->Text = tab.ToString();

		if (tab > 5) {
			SendKeys::Send("{TAB}");
		}
	}


	private: System::Void timer2_Tick(System::Object^ sender, System::EventArgs^ e) {
		t2++;
		this->textBox2->Text = t2.ToString();
		this->textBox2->Text = "hi";
	}
	private: System::Void textBox2_TextChanged_1(System::Object^ sender, System::EventArgs^ e) {
	}

	private: System::Void T_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "T";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void Y_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "Y";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void U_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "U";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void I_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "I";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void O_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "O";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
	private: System::Void P_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "P";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}

	private: System::Void A_Click(System::Object^ sender, System::EventArgs^ e) {
		word = word + "A";
		cursor++;
		this->textBox1->Text = word;
		tab = 0;
	}
private: System::Void S_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "S";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void D_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "D";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void button7_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "F";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void G_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "G";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void H_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "H";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void J_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "J";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void K_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "K";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void L_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "L";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void Z_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "Z";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void X_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "X";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void C_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "C";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void V_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "V";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void B_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "B";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void button12_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "N";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void M_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + "M";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void comma_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + ",";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void Period_Click(System::Object^ sender, System::EventArgs^ e) {
	word = word + ".";
	cursor++;
	this->textBox1->Text = word;
	tab = 0;
}
private: System::Void CAPS_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
}
};
	}

