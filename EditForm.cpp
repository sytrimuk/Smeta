//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EditForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditDialog *EditDialog;
//---------------------------------------------------------------------------
__fastcall TEditDialog::TEditDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditDialog::Button1Click(TObject *Sender)
{
	SaveClicked = true;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TEditDialog::Button2Click(TObject *Sender)
{
	SaveClicked = false;
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditDialog::Edit1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if(Key == 13)
		Button1Click(Sender);
	else if(Key == 27)
		Button2Click(Sender);
}
//---------------------------------------------------------------------------


