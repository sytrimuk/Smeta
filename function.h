//---------------------------------------------------------------------------

#ifndef functionH
#define functionH
#include <System.SysUtils.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.DApt.hpp>
#include <Vcl.Dialogs.hpp>
#include <System.IOUtils.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Samples.Spin.hpp>
#include <vector>
String GetDatabasePath(bool forCreate, const String &initialDir);
void ClearStringGrid(TStringGrid *StringGrid, int ColCount, int RowCount);
bool CreateTable(TFDQuery *query, String ParamCreate, String SQLq);
bool CheckAndCreateTable(TFDQuery *query, const String &dbFilePath);
bool LoadDataFromDB(TSpinEdit *SpinEdit1, TSpinEdit *SpinEdit2, TStringGrid *StringGrid, TFDQuery *query, std::vector<String> &_columnNames, const String &tableName);
void AddToListBox1(std::vector<String>& columnNames);
void AddToListBox2(TFDQuery *query, String tableName);
void SumCol(TStringGrid *StringGrid1, TSpinEdit *SpinEdit1, TSpinEdit *SpinEdit2);
//---------------------------------------------------------------------------
extern TMemo *memo;
extern TListBox *list1;
extern TListBox *list2;
extern TEdit *editR;
#endif
