#pragma once

namespace Keyboard_testing {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ Q;
	protected:
	private: System::Windows::Forms::CheckBox^ Shift;
	private: System::Windows::Forms::TextBox^ textBox1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Q = (gcnew System::Windows::Forms::Button());
			this->Shift = (gcnew System::Windows::Forms::CheckBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// Q
			// 
			this->Q->Location = System::Drawing::Point(38, 38);
			this->Q->Name = L"Q";
			this->Q->Size = System::Drawing::Size(70, 70);
			this->Q->TabIndex = 0;
			this->Q->Text = L"Q";
			this->Q->UseVisualStyleBackColor = true;
			// 
			// Shift
			// 
			this->Shift->Appearance = System::Windows::Forms::Appearance::Button;
			this->Shift->Location = System::Drawing::Point(38, 114);
			this->Shift->Name = L"Shift";
			this->Shift->Size = System::Drawing::Size(90, 60);
			this->Shift->TabIndex = 1;
			this->Shift->Text = L"Shift";
			this->Shift->UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(379, 64);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(173, 20);
			this->textBox1->TabIndex = 2;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(637, 261);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->Shift);
			this->Controls->Add(this->Q);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	};
}
