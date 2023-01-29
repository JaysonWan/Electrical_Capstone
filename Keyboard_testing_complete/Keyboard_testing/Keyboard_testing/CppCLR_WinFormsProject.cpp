#include "pch.h"

//using namespace System;

// int main(array<System::String ^> ^args)
// {
//    return 0;
// }

#include "Form1.h"

using namespace System::Windows::Forms;

//MAKE SURE TO COME BACK TO THIS
//[STAThread]
int main()
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	System::Windows::Forms::Application::Run(gcnew CppCLRWinFormsProject::Form1());
  return 0;
}