//---------------------------------------------------------------------------

#ifndef EditFormH
#define EditFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TEditDialog : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
public:		// User declarations
	__fastcall TEditDialog(TComponent* Owner);
    bool SaveClicked;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditDialog *EditDialog;
//---------------------------------------------------------------------------
#endif
