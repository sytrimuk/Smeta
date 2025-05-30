#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"
#include "PrintDocumentManager.h"
#include "EditForm.h"
#include "function.h"
#include <SysUtils.hpp>
#include <ShellAPI.h>
#include <map>
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PythonEngine"
#pragma link "PythonEngine"
#pragma resource "*.dfm"
TForm1 *Form1;
PrintDocumentManager *docManager;
bool check1 = false;
bool AfterCreation = false;
bool BOpen = true;
int CountRow = 0;
String StringListBox1, StringListBox2;
String dbPath;
String tableName;
TMemo *memo = new TMemo((TComponent*)nullptr);
TListBox *list1 = new TListBox((TComponent*)nullptr);
TListBox *list2 = new TListBox((TComponent*)nullptr);
TEdit *editR = new TEdit((TComponent*)nullptr);
std::vector<String> _columnNames;
std::map<String, Consignee> consigneesMap;
Consignee currentConsignee;
Consignee shipperInfo;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    memo = Memo1;
    list1 = ListBox1;
    list2 = ListBox2;
    editR = EditTotalSum;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Open1Click(TObject *Sender)
{
    if(BOpen){
        dbPath = GetDatabasePath(false, ExtractFilePath(Application -> ExeName) + "data\\db");
            if (dbPath.IsEmpty())
        return;
    }

    FDConnection1 -> Params -> Values["Database"] = dbPath;
    try
    {
        memo->Lines->Add("Открытие базы данных: " + dbPath);
        check1 = true;
        FDConnection1 -> Connected = true;
        if (dbPath != "")
        {
            ClearStringGrid(StringGrid1, StringGrid1->ColCount, StringGrid1->RowCount+1);
            tableName = TPath::GetFileNameWithoutExtension(dbPath);
            ListBox1 -> Clear();
            ListBox2 -> Clear();
            if (LoadDataFromDB(SpinEdit1, SpinEdit2, StringGrid1, FDQuery1, _columnNames, tableName))
            {
                memo->Lines->Add("Данные успешно загружены из таблицы: " + tableName);
                Create1 -> Enabled = false;
                Close1 -> Enabled = true;
                StringGrid1 -> Visible = true;
                Panel1 -> Visible = true;
                TabSheet1 -> Enabled = true;
                TabSheet2 -> Enabled = true;
                AfterCreation = true;
                Button4 -> Visible = false;
                SpinEdit1 -> Visible = false;
                SpinEdit2 -> Visible = false;
                Label1 -> Visible = false;
                Label2 -> Visible = false;
                Label3 -> Visible = true;
                EditTotalSum -> Visible = true;
                PageControl1 -> ActivePage = TabSheet1;
                SumCol(StringGrid1, SpinEdit1, SpinEdit2);
            }
            else
            {
                memo->Lines->Add("Таблица пуста: " + tableName);
                ShowMessage("Таблица пуста.");
            }
        }
        else
        {
            memo->Lines->Add("Ошибка при проверке/создании таблицы");
            ShowMessage("Ошибка при проверке/создании таблицы.");
        }
        check1 = false;
    }
    catch (Exception &e)
    {
        memo->Lines->Add("Ошибка при открытии базы данных: " + e.Message);
        ShowMessage("Ошибка: " + e.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Close1Click(TObject *Sender)
{
    if (FDConnection1->Connected)
    {
        memo->Lines->Add("Закрытие базы данных: " + dbPath);
        FDConnection1->Connected = false;
        Open1->Enabled = true;
        Create1->Enabled = true;
        Close1->Enabled = false;
        StringGrid1->Visible = false;
        TabSheet1->Enabled = false;
        TabSheet2->Enabled = false;
        ClearStringGrid(StringGrid1, StringGrid1->ColCount, StringGrid1->RowCount+1);
        Panel1->Visible = false;
        SpinEdit1->Value = 1;
        SpinEdit2->Value = 1;
        ListBox1->Clear();
        ListBox2->Clear();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Create1Click(TObject *Sender)
{
    memo->Lines->Add("Создание новой базы данных");
    Close1Click(Sender);
    ListBox1 -> Clear();
    ListBox2 -> Clear();
    SpinEdit2 -> Visible = true;
    Label1 -> Visible = true;
    dbPath = GetDatabasePath(true, ExtractFilePath(Application->ExeName) + "data\\db");
    if (dbPath.IsEmpty() || dbPath == "ЕСТЬ ЧИСЛА ИЛИ ЧТО-ТО ПОХОЖЕЕ"){
        memo->Lines->Add("Некорректное имя файла базы данных");
        ShowMessage("Что-то не так с названием файла :)");
        return;
    }
    tableName = TPath::GetFileNameWithoutExtension(dbPath);
    memo->Lines->Add("Создание таблицы: " + tableName);

    try
    {
        ClearStringGrid(StringGrid1, StringGrid1->ColCount, StringGrid1->RowCount+1);
        Open1 -> Enabled = false;
        Close1 -> Enabled = true;
        StringGrid1 -> Visible = true;
        Panel1 -> Visible = true;
        TabSheet1 -> Enabled = true;
        Button4 -> Visible = true;
        SpinEdit1 -> ReadOnly = false;
        SpinEdit2 -> ReadOnly = false;
        Label3 -> Visible = false;
        EditTotalSum -> Visible = false;
        BOpen = false;
    }
    catch (Exception &e)
    {
        memo->Lines->Add("Ошибка при создании таблицы: " + e.Message);
        ShowMessage("Ошибка: " + e.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    memo->Lines->Add("Инициализация приложения");
    Close1->Enabled = false;
    StringGrid1->Visible = false;
    SpinEdit1->Text = "5";
    SpinEdit2->Text = "1";
    Panel1->Visible = false;
    TabSheet1->Enabled = false;
    TabSheet2->Enabled = false;
    Button4->Visible = false;
    SpinEdit1->ReadOnly = true;
    SpinEdit2->ReadOnly = true;

    String dataDir = ExtractFilePath(Application->ExeName) + "data";
    if (!TDirectory::Exists(dataDir)) {
        TDirectory::CreateDirectory(dataDir);
        memo->Lines->Add("Создана директория данных: " + dataDir);
    }

    String dbFolder = ExtractFilePath(Application->ExeName) + "data\\db";
    if (!TDirectory::Exists(dbFolder)) {
        TDirectory::CreateDirectory(dbFolder);
        memo->Lines->Add("Создана директория баз данных: " + dbFolder);
    }

    String dbPath = ExtractFilePath(Application->ExeName) + "data\\consignees.db";
    if (!FileExists(dbPath)) {
        CreateConsigneesTable();
        memo->Lines->Add("Создана таблица грузополучателей");
    }

    StringGrid1->Cells[0][0] = "Наименование мат-лов";
    StringGrid1->Cells[1][0] = "Ед. Измерения";
    StringGrid1->Cells[2][0] = "Кол-во";
    StringGrid1->Cells[3][0] = "Стоимость";
    StringGrid1->Cells[4][0] = "Стоимость мат-лов";

    docManager = new PrintDocumentManager();
    docManager->LoadLogo(Image1);
    AutomaticOpening->Checked = docManager->GetAutoOpen();
    memo->Lines->Add("Менеджер документов инициализирован");

    shipperInfo.Name = "Моя компания";
    shipperInfo.Address = "г. Москва, ул. Примерная, д. 1";
    shipperInfo.PhoneNumber = "+7 (495) 123-45-67";

    consigneeEditPhoneNumber->EditMask = "!\\+7 (999) 000-00-00;1;_";
    shipperEditPhoneNumber->EditMask = "!\\+7 (999) 000-00-00;1;_";

    LoadConsignees();
    LoadShipperInfo();
    memo->Lines->Add("Данные грузоотправителя и грузополучателей загружены");
}
//---------------------------------------------------------------------------
bool TForm1::ValidateAddress(String address)
{
    bool hasCity = address.Pos("г.") > 0;
    bool hasStreet = address.Pos("ул.") > 0;
    bool hasHouse = address.Pos("д.") > 0;

    if (!hasCity || !hasStreet || !hasHouse) {
        memo->Lines->Add("Некорректный адрес: " + address);
        ShowMessage(
            "Адрес должен содержать все обязательные части:\n"
            "- г. [Название города]\n"
            "- ул. [Название улицы]\n"
            "- д. [Номер дома]\n\n"
            "Пример: г. Москва, ул. Примерная, д. 15"
        );
        return false;
    }
    return true;
}

void TForm1::LoadConsignees()
{
    memo->Lines->Add("Загрузка грузополучателей");
    consigneesMap.clear();
    ListBox3->Clear();

    try {
        String consigneesDBPath = ExtractFilePath(Application->ExeName) + "data\\consignees.db";
        FDConnection1->Params->Values["Database"] = consigneesDBPath;
        FDConnection1->Connected = true;

        TFDQuery* q = new TFDQuery(this);
        try {
            q->Connection = FDConnection1;
            q->SQL->Text = "SELECT name FROM sqlite_master WHERE type='table' AND name='consignees'";
            q->Open();

            if (q->RecordCount == 0) {
                CreateConsigneesTable();
                memo->Lines->Add("Таблица грузополучателей создана");
            }

            q->SQL->Text = "SELECT * FROM consignees";
            q->Open();

            while (!q->Eof) {
                Consignee c;
                c.Name = q->FieldByName("name")->AsString;
                c.Address = q->FieldByName("address")->AsString;
                c.PhoneNumber = q->FieldByName("phone")->AsString;

                consigneesMap[c.Name] = c;
                ListBox3->Items->Add(c.Name);
                q->Next();
            }
            memo->Lines->Add("Загружено грузополучателей: " + String(consigneesMap.size()));
        }
        __finally {
            q->Free();
            FDConnection1->Connected = false;
        }
    }
    catch (Exception &e) {
        memo->Lines->Add("Ошибка при загрузке грузополучателей: " + e.Message);
        ShowMessage("Ошибка: " + e.Message);
    }
}

void TForm1::CreateConsigneesTable()
{
    TFDQuery* q = new TFDQuery(this);
    try {
        q->Connection = FDConnection1;
        q->SQL->Text =
            "CREATE TABLE consignees ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL UNIQUE, "
            "address TEXT, "
            "phone TEXT)";
        q->ExecSQL();
    }
    __finally {
        q->Free();
    }
}

void TForm1::LoadShipperInfo()
{
    TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "data\\config.ini");
    shipperInfo.Name = ini->ReadString("Shipper", "Name", "Моя компания");
    shipperInfo.Address = ini->ReadString("Shipper", "Address", "г. Москва, ул. Примерная, д. 1");
    shipperInfo.PhoneNumber = ini->ReadString("Shipper", "Phone", "+7 (495) 123-45-67");
    delete ini;

    IncNameEdit->Text = shipperInfo.Name;
    shipperEditAddress->Text = shipperInfo.Address;
    shipperEditPhoneNumber->Text = shipperInfo.PhoneNumber;
    memo->Lines->Add("Данные грузоотправителя загружены: " + shipperInfo.Name);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpinEdit1Change(TObject *Sender)
{
    SpinEdit1 -> Text = 5;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpinEdit2Change(TObject *Sender)
{
    if(SpinEdit2 -> Text.ToInt() < 0)
        SpinEdit2 -> Text = 0;
    StringGrid1 -> RowCount = SpinEdit2 -> Text.ToInt();

    if(SpinEdit2 -> Text.ToInt() < CountRow)
        check1 = true;
    CountRow = SpinEdit2 -> Text.ToInt();
    memo->Lines->Add("Изменено количество строк: " + IntToStr(SpinEdit2->Text.ToInt()));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    TCanvas *Canvas = StringGrid1->Canvas;
    String text = StringGrid1->Cells[ACol][ARow];
    int textWidth = Canvas->TextWidth(text) + 10;
    int textHeight = Canvas->TextHeight(text);

    if (textWidth > StringGrid1->ColWidths[ACol]) {
        StringGrid1->ColWidths[ACol] = textWidth;
    }

    if ((ACol >= 0 && ACol <= 4) && ARow == 0) {
        Canvas->Brush->Color = clBlack;
        Canvas->Font->Color = clWhite;
    } else if (ARow == 0 || ACol == 0) {
        Canvas->Brush->Color = clGray;
        Canvas->Font->Color = clWhite;
    } else {
        Canvas->Brush->Color = clWindow;
        Canvas->Font->Color = clWindowText;
    }

    if (textHeight + 2 > StringGrid1->RowHeights[ARow]) {
        StringGrid1->RowHeights[ARow] = textHeight + 2;
    }

    Canvas->FillRect(Rect);

    int textX = Rect.Left + (Rect.Width() - textWidth) / 2;
    int textY = Rect.Top + (Rect.Height() - textHeight) / 2;
    Canvas->TextRect(Rect, textX, textY, text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    Memo1 -> Clear();
    memo->Lines->Add("Логи очищены");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit2Change(TObject *Sender)
{
    try{
        if (Edit2 -> Text.IsEmpty()) {
            AddToListBox1(_columnNames);
            return;
        }

        String searchStr = Edit2 -> Text.Trim().LowerCase();
        memo->Lines->Add("Поиск в ListBox1: " + searchStr);

        TListBox* ListBox = new TListBox(Form1);
        ListBox -> Parent = Form1;

        for (int i = 0; i < ListBox1 -> Items -> Count; ++i) {
            ListBox -> Items -> Add(ListBox1 -> Items -> Strings[i]);
        }
        ListBox1->Clear();

        for (int i = 0; i < ListBox -> Items -> Count; ++i)
        {
            String listItem = ListBox -> Items -> Strings[i].LowerCase();
            if (listItem.Pos(searchStr) != 0)
            {
                ListBox1 -> Items -> Add(ListBox -> Items -> Strings[i]);
            }
        }
        delete ListBox;
    }
    catch (const Exception& e) {
        memo->Lines->Add("Ошибка поиска: " + e.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit4Change(TObject *Sender)
{
    try{
        if (Edit4->Text.IsEmpty()) {
            AddToListBox2(FDQuery1, tableName);
            return;
        }

        String searchStr = Edit4 -> Text.Trim().LowerCase();
        memo->Lines->Add("Поиск в ListBox2: " + searchStr);

        TListBox* ListBox = new TListBox(Form1);
        ListBox->Parent = Form1;

        for (int i = 0; i < ListBox2 -> Items -> Count; ++i) {
            ListBox -> Items -> Add(ListBox2 -> Items -> Strings[i]);
        }
        ListBox2->Clear();

        for (int i = 0; i < ListBox -> Items -> Count; ++i)
        {
            String listItem = ListBox -> Items -> Strings[i].LowerCase();
            if (listItem.Pos(searchStr) != 0)
            {
                ListBox2 -> Items -> Add(ListBox -> Items -> Strings[i]);
            }
        }
        delete ListBox;
    }
    catch (const Exception& e) {
        memo->Lines->Add("Ошибка поиска: " + e.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1Click(TObject *Sender)
{
    if(ListBox1 -> ItemIndex >= 0){
        StringListBox1 =  ListBox1 -> Items -> Strings[ListBox1 -> ItemIndex];
        Label4 -> Caption = StringListBox1;
        memo->Lines->Add("Выбрана колонка: " + StringListBox1);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox2Click(TObject *Sender)
{
    if(ListBox2 -> ItemIndex >= 0){
        StringListBox2 =  ListBox2 -> Items -> Strings[ListBox2 -> ItemIndex];
        Label4 -> Caption = StringListBox1 + "/" + StringListBox2;
        memo->Lines->Add("Выбрана строка: " + StringListBox2);

        int Tindex = 0;
        for(int i = 1; i < SpinEdit1 -> Text.ToInt() + 1; i++){
            if(StringGrid1 -> Cells[0][i] == StringListBox2){
                Tindex = i;
                break;
            }
        }

        for(int i = 1; i < SpinEdit2 -> Text.ToInt() + 1; i++){
            if(StringGrid1 -> Cells[i][0] == StringListBox1){
                Edit3 -> Text = StringGrid1 -> Cells[i][Tindex];
                break;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
    memo->Lines->Add("Создание таблицы: " + tableName);
    DeleteFile(dbPath);
    check1 = true;
    String ParamCreate = "CREATE TABLE '" + tableName + "' ( " +
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, " +
                  "'Наиминование мат-лов' Text NOT NULL UNIQUE, " +
                  "'Ед. Измерения' TEXT DEFAULT 'заполните поле', " +
                  "'Кол-во' REAL DEFAULT 0.0, " +
                  "'Стоимость за ед.' REAL DEFAULT 0.0, " +
                  "'Стоимость мат-лов' REAL DEFAULT 0.0);";

    String SQLq = "INSERT INTO " + tableName + " ('Наиминование мат-лов') \n VALUES";
    std::vector<String> NameOnRow;
    for(int i = 1; SpinEdit2 -> Text.ToInt() > i; i++)
        NameOnRow.push_back(StringGrid1 -> Cells[0][i]);
    for(int i = 0; i < NameOnRow.size(); i++){
        if(NameOnRow.at(i) == NameOnRow.at(NameOnRow.size() - 1))
            SQLq += "('" + NameOnRow.at(i) + "');";
        else
            SQLq += "('" + NameOnRow.at(i) + "'), \n";
    }

    try
    {
        FDConnection1 -> Params -> Values["Database"] = dbPath;
        FDConnection1 -> Connected = true;
        FDQuery1 -> SQL->Text = "DROP TABLE IF EXISTS '" + tableName + "';";
        FDQuery1 -> ExecSQL();
        if (CreateTable(FDQuery1, ParamCreate, SQLq))
        {
            memo->Lines->Add("Таблица успешно создана: " + tableName);
            ShowMessage("Таблица успешно создана.");
            TabSheet2 -> Visible = true;
            Button4 -> Visible = false;
            SpinEdit1 -> ReadOnly = true;
            SpinEdit2 -> ReadOnly = true;
            ClearStringGrid(StringGrid1, StringGrid1->ColCount, StringGrid1->RowCount+1);
            Open1Click(Sender);
        }
        else
        {
            memo->Lines->Add("Ошибка при создании таблицы: " + tableName);
            ShowMessage("Ошибка при проверке/создании таблицы.");
            Memo1 -> Lines -> Add("Запрос на создание таблицы: " + ParamCreate);
            Memo1 -> Lines -> Add("Запрос добавления в таблицу имён: " + SQLq);
        }
    }
    catch (Exception &e)
    {
        memo->Lines->Add("Ошибка при создании таблицы: " + e.Message);
        Memo1 -> Lines -> Add(e.Message);
    }
    check1 = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1SelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
    TEditDialog *editDialog = new TEditDialog(this);
    editDialog -> Left = this -> Left + 100;
    editDialog -> Top = this -> Top + 100;
    editDialog -> Edit1 -> Text = StringGrid1 -> Cells[ACol][ARow];
    if(AfterCreation || check1) check1 = false;
    else if(ACol == 0 && ARow >= 1){
        memo->Lines->Add("Редактирование ячейки: [" + IntToStr(ACol) + "," + IntToStr(ARow) + "]");
        editDialog -> ShowModal();
        if (editDialog -> SaveClicked)
        {
            if(ARow == 0 && ACol == 0) false;
            else if(ACol == 0 && ARow > 0){
                StringGrid1 -> Cells[ACol][ARow] = editDialog -> Edit1 -> Text;
                memo->Lines->Add("Сохранено значение: " + editDialog->Edit1->Text);
            }
        }
    }
    delete editDialog;
    editDialog = nullptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
    memo->Lines->Add("Обновление значения: " + Edit3->Text);
    AnsiString dValue = Edit3 -> Text;
    Edit3 -> Text = "";
    for (int i = 1; i < dValue.Length() + 1; i++) {
        if (dValue[i] == ',')
            dValue[i] = '.';
    } Edit3->Text = dValue;
    FDQuery1 -> SQL -> Text = "UPDATE " + tableName + " SET \"" + StringListBox1 + "\" = '" + Edit3 -> Text + "' WHERE \"Наиминование мат-лов\" = \"" + StringListBox2 + "\"";
    FDQuery1->ExecSQL();
    memo->Lines->Add("SQL запрос: " + FDQuery1->SQL->Text);
    Memo1 -> Lines -> Add(FDQuery1 -> SQL -> Text);

    ClearStringGrid(StringGrid1, StringGrid1->ColCount, StringGrid1->RowCount+1);
    LoadDataFromDB(SpinEdit1, SpinEdit2, StringGrid1, FDQuery1, _columnNames, tableName);

    try
    {
        if("Ед. Измерения" != Edit3 -> Text){
            int i = 0, j = 1;
            while(StringGrid1 -> Cells[i][j] != StringListBox2){
                j++;
            }
            i = 2;

            double value = 0.0;
            value = StringGrid1 -> Cells[i][j].ToDouble();
            value *= StringGrid1 -> Cells[i + 1][j].ToDouble();
            String temp = value;
            for (int i = 1; i < temp.Length() + 1; i++) {
                if (temp[i] == ',')
                    temp[i] = '.';
            }

            UnicodeString sql = "UPDATE " + tableName + " SET \"Стоимость мат-лов\" = " + temp +
                                " WHERE \"Наиминование мат-лов\" = '" + StringListBox2 + "'";
            FDQuery1 -> SQL -> Text = sql;
            FDQuery1->ExecSQL();
            memo->Lines->Add("Обновление стоимости: " + sql);
            try
            {
                SumCol(StringGrid1, SpinEdit1, SpinEdit2);
                FDQuery1 -> SQL->Text = "SELECT * FROM myRESULT";
                FDQuery1 -> Open();

                double resultValue = EditTotalSum -> Text.ToDouble();

                if (FDQuery1 -> IsEmpty())
                    FDQuery1 -> SQL -> Text = "INSERT INTO myRESULT (result) VALUES (" + (String)resultValue + ")";
                else
                    FDQuery1 -> SQL -> Text = "UPDATE myRESULT SET result = " + (String)resultValue;

                FDQuery1 -> ExecSQL();
                FDQuery1 -> Close();
                memo->Lines->Add("Обновление итоговой суммы: " + String(resultValue));
            }
            catch (Exception &e)
            {
                memo->Lines->Add("Ошибка при обновлении итоговой суммы: " + e.Message);
            }
        }

        AddToListBox2(FDQuery1, tableName);
        ClearStringGrid(StringGrid1, StringGrid1->ColCount, StringGrid1->RowCount+1);
        LoadDataFromDB(SpinEdit1, SpinEdit2, StringGrid1, FDQuery1, _columnNames, tableName);
    }
    catch (Exception &e)
    {
        memo->Lines->Add("Ошибка при обновлении данных: " + e.Message);
        Memo1 -> Lines -> Add(FDQuery1 -> SQL -> Text);
        ShowMessage("Ошибка: " + e.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HTML1Click(TObject *Sender)
{
    memo->Lines->Add("Запуск экспорта/печати документа");
    // Создаем форму выбора
    TForm *selectForm = new TForm(this);
    selectForm->Caption = "Выберите грузополучателя";
    selectForm->Position = poScreenCenter;
    selectForm->Width = 400;
    selectForm->Height = 330;

    // Создаем ListBox
    TListBox *consigneeList = new TListBox(selectForm);
    consigneeList->Parent = selectForm;
    consigneeList->Align = alTop;
    consigneeList->Height = 200;

    // Заполняем список грузополучателей
    for (std::map<String, Consignee>::iterator it = consigneesMap.begin(); it != consigneesMap.end(); ++it) {
        consigneeList->Items->Add(it->first);
    }
    memo->Lines->Add("Список грузополучателей загружен");

    // Создаем кнопку "Экспорт"
    TButton *btnExport = new TButton(selectForm);
    btnExport->Parent = selectForm;
    btnExport->Caption = "Экспорт";
    btnExport->ModalResult = mrOk;
    btnExport->Top = 210;
    btnExport->Left = 50;
    btnExport->Width = 80;

    // Создаем кнопку "Печать"
    TButton *btnPrint = new TButton(selectForm);
    btnPrint->Parent = selectForm;
    btnPrint->Caption = "Печать";
    btnPrint->ModalResult = mrYes;
    btnPrint->Top = 210;
    btnPrint->Left = 140;
    btnPrint->Width = 80;

    // Создаем кнопку "Отмена"
    TButton *btnCancel = new TButton(selectForm);
    btnCancel->Parent = selectForm;
    btnCancel->Caption = "Отмена";
    btnCancel->ModalResult = mrCancel;
    btnCancel->Top = 210;
    btnCancel->Left = 230;
    btnCancel->Width = 80;

    // Показываем форму
    memo->Lines->Add("Отображение диалога выбора грузополучателя");
    int result = selectForm->ShowModal();
    if (result == mrOk || result == mrYes) {
        if (consigneeList->ItemIndex >= 0) {
            String name = consigneeList->Items->Strings[consigneeList->ItemIndex];
            memo->Lines->Add("Выбран грузополучатель: " + name);

            if (consigneesMap.find(name) != consigneesMap.end()) {
                Consignee receiver = consigneesMap[name];
                memo->Lines->Add("Данные грузополучателя получены");

                // Диалог сохранения файла
                TSaveDialog* saveDialog = new TSaveDialog(NULL);
                saveDialog->Filter = "HTML files (*.html)|*.html";
                saveDialog->DefaultExt = "html";
                saveDialog->FileName = "Смета_" + FormatDateTime("ddmmyyyy", Now()) + ".html";

                if (saveDialog->Execute()) {
                    String fileName = saveDialog->FileName;
                    memo->Lines->Add("Файл для сохранения: " + fileName);

                    if (result == mrOk) {
                        memo->Lines->Add("Запуск экспорта документа");
                        // Экспорт
                        docManager->ExportDocument(
                            StringGrid1,
                            EditTotalSum->Text,
                            IncNameEdit->Text,
                            shipperInfo,
                            receiver
                        );
                    } else if (result == mrYes) {
                        memo->Lines->Add("Запуск печати документа");
                        // Печать
                        docManager->PrintDocument(
                            StringGrid1,
                            EditTotalSum->Text,
                            IncNameEdit->Text,
                            shipperInfo,
                            receiver,
                            fileName
                        );
                    }
                } else {
                    memo->Lines->Add("Пользователь отменил сохранение файла");
                }
                delete saveDialog;
            } else {
                memo->Lines->Add("Ошибка: грузополучатель не найден");
                ShowMessage("Ошибка: грузополучатель не найден!");
            }
        } else {
            memo->Lines->Add("Ошибка: грузополучатель не выбран");
            ShowMessage("Выберите грузополучателя!");
        }
    } else {
        memo->Lines->Add("Экспорт/печать отменен пользователем");
    }

    delete selectForm;
    memo->Lines->Add("Диалог выбора грузополучателя закрыт");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonChLogoClick(TObject *Sender)
{
    memo->Lines->Add("Замена логотипа");
    if(OpenPictureDialog1->Execute()) {
        Image1->Picture->LoadFromFile(OpenPictureDialog1->FileName);
        docManager->UploadLogo(Image1);
        memo->Lines->Add("Логотип обновлен: " + OpenPictureDialog1->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AutomaticOpeningClick(TObject *Sender)
{
    docManager->SetAutoOpen(AutomaticOpening->Checked);
    memo->Lines->Add("Автооткрытие документов: " + String(AutomaticOpening->Checked ? "включено" : "выключено"));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::shipperEditButtonClick(TObject *Sender)
{
    memo->Lines->Add("Сохранение данных грузоотправителя");
    if (!ValidateAddress(shipperEditAddress->Text)) {
        shipperEditAddress->SetFocus();
        return;
    }

    shipperInfo.Name = IncNameEdit->Text;
    shipperInfo.Address = shipperEditAddress->Text;
    shipperInfo.PhoneNumber = shipperEditPhoneNumber->Text;
    memo->Lines->Add("Данные грузоотправителя: " + shipperInfo.Name);

    // Сохранение в конфиг
    TIniFile* ini = new TIniFile(ExtractFilePath(Application->ExeName) + "data\\config.ini");
    try {
        ini->WriteString("Shipper", "Name", shipperInfo.Name);
        ini->WriteString("Shipper", "Address", shipperInfo.Address);
        ini->WriteString("Shipper", "Phone", shipperInfo.PhoneNumber);
        memo->Lines->Add("Данные сохранены в config.ini");
    }
    __finally {
        delete ini;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox3Click(TObject *Sender)
{
    if (ListBox3->ItemIndex < 0) return;

    String name = ListBox3->Items->Strings[ListBox3->ItemIndex];
    currentConsignee = consigneesMap[name];
    memo->Lines->Add("Выбран грузополучатель: " + name);

    consigneeEditName->Text = currentConsignee.Name;
    consigneeEditAddress->Text = currentConsignee.Address;
    consigneeEditPhoneNumber->Text = currentConsignee.PhoneNumber;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::consigneeButtonDeleteClick(TObject *Sender)
{
    if (ListBox3->ItemIndex < 0) return;

    String name = ListBox3->Items->Strings[ListBox3->ItemIndex];
    memo->Lines->Add("Удаление грузополучателя: " + name);

    // Удаление из БД
    TFDQuery* q = new TFDQuery(this);
    try {
        q->Connection = FDConnection1;
        q->SQL->Text = "DELETE FROM consignees WHERE name = :name";
        q->ParamByName("name")->AsString = name;
        q->ExecSQL();
        memo->Lines->Add("Грузополучатель удален из БД");

        // Обновление списка
        consigneesMap.erase(name);
        ListBox3->Items->Delete(ListBox3->ItemIndex);
        memo->Lines->Add("Грузополучатель удален из списка");
    }
    __finally {
        q->Free();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::consigneeButtonAddClick(TObject *Sender)
{
    memo->Lines->Add("Добавление нового грузополучателя");
    if (!ValidateAddress(consigneeEditAddress->Text)) {
        consigneeEditAddress->SetFocus();
        return;
    }

    Consignee c;
    c.Name = consigneeEditName->Text;
    c.Address = consigneeEditAddress->Text;
    c.PhoneNumber = consigneeEditPhoneNumber->Text;

    if (c.Name.IsEmpty()) {
        memo->Lines->Add("Ошибка: не указано название грузополучателя");
        ShowMessage("Введите название грузополучателя");
        return;
    }

    // Сохранение в БД
    TFDQuery* q = new TFDQuery(this);
    try {
        q->Connection = FDConnection1;
        q->SQL->Text = "INSERT INTO consignees (name, address, phone) VALUES (:name, :address, :phone)";
        q->ParamByName("name")->AsString = c.Name;
        q->ParamByName("address")->AsString = c.Address;
        q->ParamByName("phone")->AsString = c.PhoneNumber;
        q->ExecSQL();
        memo->Lines->Add("Грузополучатель добавлен в БД: " + c.Name);

        // Обновление списка
        consigneesMap[c.Name] = c;
        ListBox3->Items->Add(c.Name);
        memo->Lines->Add("Грузополучатель добавлен в список");
    }
    __finally {
        q->Free();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::shipperEditAddressExit(TObject *Sender)
{
    if (!ValidateAddress(shipperEditAddress->Text)) {
        shipperEditAddress->SetFocus();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::consigneeEditAddressExit(TObject *Sender)
{
    if (!ValidateAddress(consigneeEditAddress->Text)) {
        consigneeEditAddress->SetFocus();
    }
}
//---------------------------------------------------------------------------
