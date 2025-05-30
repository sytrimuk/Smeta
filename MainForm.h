//---------------------------------------------------------------------------

#ifndef MainFormH
#define MainFormH
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.SQLite.hpp>
#include <FireDAC.Phys.SQLiteDef.hpp>
#include <FireDAC.Phys.SQLiteWrapper.Stat.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.ExprFuncs.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Samples.Spin.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Mask.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TFDConnection *FDConnection1;
	TFDQuery *FDQuery1;
	TFDPhysSQLiteDriverLink *FDPhysSQLiteDriverLink1;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Open1;
	TMenuItem *Create1;
	TStringGrid *StringGrid1;
	TSpinEdit *SpinEdit1;
	TSpinEdit *SpinEdit2;
	TMenuItem *Close1;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TMemo *Memo1;
	TButton *Button1;
	TTabSheet *TabSheet3;
	TEdit *EditTotalSum;
	TLabel *Label3;
	TTabSheet *TabSheet2;
	TListBox *ListBox1;
	TPanel *Panel2;
	TEdit *Edit2;
	TPanel *Panel3;
	TButton *Button2;
	TEdit *Edit3;
	TPanel *Panel4;
	TListBox *ListBox2;
	TEdit *Edit4;
	TLabel *Label4;
	TButton *Button4;
	TTabSheet *Settings;
	TMenuItem *N1;
	TMenuItem *HTML1;
	TCheckBox *AutomaticOpening;
	TButton *ButtonChLogo;
	TImage *Image1;
	TEdit *IncNameEdit;
	TOpenPictureDialog *OpenPictureDialog1;
	TTabSheet *consignee;
	TListBox *ListBox3;
	TEdit *consigneeEditName;
	TButton *consigneeButtonAdd;
	TButton *consigneeButtonDelete;
	TButton *shipperEditButton;
	TMaskEdit *shipperEditPhoneNumber;
	TMaskEdit *consigneeEditPhoneNumber;
	TMaskEdit *consigneeEditAddress;
	TMaskEdit *shipperEditAddress;
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Create1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SpinEdit1Change(TObject *Sender);
	void __fastcall SpinEdit2Change(TObject *Sender);
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Edit2Change(TObject *Sender);
	void __fastcall Edit4Change(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall ListBox2Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall HTML1Click(TObject *Sender);
	void __fastcall ButtonChLogoClick(TObject *Sender);
	void __fastcall AutomaticOpeningClick(TObject *Sender);
	void __fastcall shipperEditButtonClick(TObject *Sender);
	void __fastcall ListBox3Click(TObject *Sender);
	void __fastcall consigneeButtonDeleteClick(TObject *Sender);
	void __fastcall consigneeButtonAddClick(TObject *Sender);
	void __fastcall shipperEditAddressExit(TObject *Sender);
	void __fastcall consigneeEditAddressExit(TObject *Sender);
private:	// User declarations
	void LoadConsignees();
	void LoadShipperInfo();
	void CreateConsigneesTable();
    bool ValidateAddress(String address);
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
