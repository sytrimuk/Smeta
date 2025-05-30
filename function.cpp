//---------------------------------------------------------------------------

#pragma hdrstop

#include "function.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

String GetDatabasePath(bool forCreate, const String &initialDir)
{
	String dbPath;

	if (forCreate)
	{
		TSaveDialog *SaveDialog1 = new TSaveDialog(nullptr);
		SaveDialog1 -> InitialDir = initialDir;
		SaveDialog1 -> DefaultExt = "db";
		SaveDialog1 -> Filter = "Database files (*.db)|*.db|All files (*.*)|*.*";
		if (SaveDialog1 -> Execute())
		{
			dbPath = SaveDialog1 -> FileName;
			TStringList *ListStr = new TStringList;
			ListStr -> Delimiter = '\\';
			ListStr -> DelimitedText = dbPath;
			String db = (*ListStr)[ListStr -> Count - 1];
			ShowMessage(db);
			for(int i = 1; i < db.Length(); i++){
				char TEMP = db[i];
				if(TEMP >= 48 && TEMP <= 57){
					db = "ЕСТЬ ЧИСЛА ИЛИ ЧТО-ТО ПОХОЖЕЕ";
					return db;
				}
			}
			delete ListStr;
			if (AnsiPos(".db", dbPath.LowerCase()) != dbPath.Length() - 2)
				dbPath += ".db";
		}
		delete SaveDialog1;
	}
	else
	{
		TOpenDialog *OpenDialog1 = new TOpenDialog(nullptr);
		OpenDialog1 -> InitialDir = initialDir;
		OpenDialog1 -> DefaultExt = "db";
		OpenDialog1 -> Filter = "Database files (*.db)|*.db|All files (*.*)|*.*";
		if (OpenDialog1 -> Execute())
		{
	  		dbPath = OpenDialog1 -> FileName;
		}
		delete OpenDialog1;
	}

	return dbPath;
}

void ClearStringGrid(TStringGrid *StringGrid, int ColCount, int RowCount)
{   
	for (int col = 0; col <= ColCount; col++)
	{
		for (int row = 1; row <= RowCount; row++)
		{
			StringGrid->Cells[col][row] = "";
		}
	}
}


bool CreateTable(TFDQuery *query, String ParamCreate, String SQLq)
{
	bool tableExists = false;
    try
	{
		query -> SQL -> Text = ParamCreate;
		query -> ExecSQL();

        query -> SQL -> Clear();

		query -> SQL -> Text = SQLq;
		query -> ExecSQL();

        query -> SQL -> Clear();

		query -> SQL -> Text = "CREATE TABLE myRESULT (result REAL)";
        query -> ExecSQL();

		tableExists = true;
    }
    catch (Exception &e)
    {
		ShowMessage("Ошибка при проверке/создании таблицы: " + e.Message);
        memo -> Lines -> Add("Ошибка при проверке/создании таблицы: " + e.Message);
		tableExists = false;
	}

	return tableExists;
}

bool LoadDataFromDB(TSpinEdit *SpinEdit1, TSpinEdit *SpinEdit2, TStringGrid *StringGrid, TFDQuery *query, std::vector<String> &_columnNames, const String &tableName)
{
	try
	{
		list1 -> Clear();
		list2 -> Clear();
		query -> SQL -> Text = "SELECT * FROM " + tableName + " WHERE 1=0";
		query -> Open();
		_columnNames.clear();
		int totalColCount = query -> FieldCount;
		for (int i = 0; i < totalColCount; i++)
		{
            String fieldName = query -> Fields -> Fields[i] -> FieldName;
			if (fieldName != "id")
			{
				_columnNames.push_back(fieldName);
			}
		}
        query->Close();

		String selectQuery = "SELECT ";
		list1 -> Clear();
		for (size_t i = 0; i < _columnNames.size(); i++)
		{
			selectQuery += "\"" + _columnNames[i] + "\"";
			if (i < _columnNames.size() - 1)
			{
				selectQuery += ", ";
			}
		}
		selectQuery += " FROM " + tableName;
		query -> SQL -> Text = selectQuery;
		query -> Open();

        AddToListBox1(_columnNames);

		if (query -> IsEmpty())
        {
			query -> Close();
            return false;
        }

		int colCount = _columnNames.size();
		int rowCount = query -> RecordCount;

		SpinEdit1 -> Value = colCount;
		SpinEdit2 -> Value = rowCount;

		StringGrid -> ColCount = colCount;
		StringGrid -> RowCount = rowCount + 1;

        for (int col = 0; col < colCount; col++)
        {
			StringGrid -> Cells[col][0] = _columnNames[col];
        }

        int row = 1;
		while (!query -> Eof)
		{
            for (int col = 0; col < colCount; col++)
            {
				StringGrid -> Cells[col][row] = query -> Fields -> FieldByName(_columnNames[col]) -> AsString;
            }
            query -> Next();
			row++;
		}

		query -> SQL -> Text = "SELECT \"Наиминование мат-лов\" FROM " + tableName;
		query -> Open();

		if (!query -> IsEmpty())
		{
			while (!query -> Eof)
			{
				String name = query->Fields->Fields[0]->AsString;
				list2->Items->Add(name);
				query->Next();
			}
		}

		query -> SQL -> Text = "SELECT result FROM myRESULT";
		query -> Open();
		if (!query -> IsEmpty())
		{
			editR -> Text = query -> Fields -> FieldByName("result") -> AsString;;
		}

		query -> Close();
		return true;
	}
	catch (Exception &e)
	{
		ShowMessage("Ошибка при загрузке данных в StringGrid: " + e.Message);
		memo -> Lines -> Add(("Ошибка при загрузке данных в StringGrid: " + e.Message));
		return false;
	}
}

void AddToListBox1(std::vector<String>& columnNames){
    list1 -> Clear();
	for(int i = 0; i < columnNames.size(); i++){
		if(columnNames[i] == "Наиминование мат-лов" || columnNames[i] == "Стоимость мат-лов") continue;
		list1 -> Items -> Add(columnNames[i]);
	}
}

void AddToListBox2(TFDQuery *query, String tableName){
    list2 -> Clear();
	query -> SQL -> Text = "SELECT \"Наиминование мат-лов\" FROM " + tableName;
	query -> Open();

	if (!query -> IsEmpty())
	{
		while (!query -> Eof)
		{
			list2 -> Items -> Add(query -> Fields -> FieldByName("Наиминование мат-лов")-> AsString);
			query -> Next();
		}
	}
}

void SumCol(TStringGrid *StringGrid1, TSpinEdit *SpinEdit1, TSpinEdit *SpinEdit2){
	for(int i = 0; i < SpinEdit1 -> Text.ToInt(); i++){
		if(StringGrid1 -> Cells[i][0] == "Стоимость мат-лов"){
			double sum = 0;
			for(int j = 1; j < SpinEdit2 -> Text.ToInt(); j++){
				sum += StringGrid1 -> Cells[i][j].ToDouble();
			}
			editR -> Text = (String)sum;
            break;
		}
	}
}

