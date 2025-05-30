#include "PrintDocumentManager.h"
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <System.IOUtils.hpp>
#include <Vcl.Forms.hpp>
#include <ShellAPI.h>

#pragma package(smart_init)

PrintDocumentManager::PrintDocumentManager()
{
    logoPath = IncludeTrailingPathDelimiter(ExtractFilePath(ParamStr(0))) + "data\\logo.png";
    configPath = IncludeTrailingPathDelimiter(ExtractFilePath(ParamStr(0))) + "data\\config.ini";
    ForceDirectories(ExtractFilePath(logoPath));
    if (memo) memo->Lines->Add("Инициализация PrintDocumentManager");
}

void PrintDocumentManager::UploadLogo(TImage* img)
{
    if (memo) memo->Lines->Add("Загрузка логотипа: " + logoPath);
    try {
        if(FileExists(logoPath)) DeleteFile(logoPath);
        img->Picture->SaveToFile(logoPath);
        if (memo) memo->Lines->Add("Логотип успешно сохранен");
    }
    catch (Exception &e) {
        if (memo) memo->Lines->Add("Ошибка сохранения логотипа: " + e.Message);
        ShowMessage("Ошибка сохранения логотипа: " + e.Message);
    }
}

void PrintDocumentManager::LoadLogo(TImage* img)
{
    if (memo) memo->Lines->Add("Загрузка логотипа: " + logoPath);
    try {
        if(FileExists(logoPath)) {
            img->Picture->LoadFromFile(logoPath);
            currentLogo = img;
            if (memo) memo->Lines->Add("Логотип успешно загружен");
        }
        else {
            if (memo) memo->Lines->Add("Файл логотипа не найден");
        }
    }
    catch (Exception &e) {
        if (memo) memo->Lines->Add("Ошибка загрузки логотипа: " + e.Message);
        ShowMessage("Ошибка загрузки логотипа: " + e.Message);
    }
}

void PrintDocumentManager::GenerateHTMLHeader(TStringList* html)
{
    html->Add("<!DOCTYPE html>");
    html->Add("<html lang='ru'>");
    html->Add("<head>");
    html->Add("<meta charset='UTF-8'>");
    html->Add("<title>Смета</title>");
    html->Add("<style>");
    html->Add("@page { size: A4; margin: 1cm; }");
    html->Add("body { font-family: 'Times New Roman', serif; font-size: 12pt; }");
    html->Add(".company-info { margin-bottom: 20px; }");
    html->Add(".logo { max-height: 80px; float: right; }");
    html->Add("table { width: 100%; border-collapse: collapse; margin: 15px 0; }");
    html->Add("th, td { border: 1px solid #000; padding: 5px; }");
    html->Add(".total-row { background-color: #f0f0f0; font-weight: bold; }");
    html->Add(".signature-block { margin-top: 50px; display: flex; justify-content: space-between; }");
    html->Add("</style>");
    html->Add("</head>");
    if (memo) memo->Lines->Add("Сгенерирован HTML заголовок");
}

void PrintDocumentManager::GenerateCompanyInfo(TStringList* html, const String& documentTitle,
                                             const Consignee& sender, const Consignee& receiver)
{
    html->Add("<div class='company-info' style='display: flex; align-items: center;'>");

    if(FileExists(logoPath)) {
        html->Add("<img class='logo' src='" + logoPath + "' alt='Логотип' style='margin-right: 20px; max-height: 80px;'>");
        if (memo) memo->Lines->Add("Добавлен логотип в HTML");
    }

    html->Add("<div>");
    html->Add("<h2>" + documentTitle + "</h2>");
    html->Add("</div>");
    html->Add("</div>");

    html->Add("<div style='margin-top: 20px;'>");
    html->Add("<p><strong>Грузоотправитель:</strong> " +
              sender.Name + ", " + sender.Address + ", тел. " + sender.PhoneNumber + "</p>");
    html->Add("<p><strong>Грузополучатель:</strong> " +
              receiver.Name + ", " + receiver.Address + ", тел. " + receiver.PhoneNumber + "</p>");
    html->Add("</div>");
    if (memo) memo->Lines->Add("Сгенерирована информация о компании");
}

void PrintDocumentManager::GenerateHTMLTable(TStringGrid* grid, TStringList* html, const String& totalSum)
{
    html->Add("<table>");
    html->Add("<thead><tr>");

    for(int col = 0; col < grid->ColCount; col++) {
        html->Add("<th>" + grid->Cells[col][0] + "</th>");
    }
    html->Add("</tr></thead>");

    html->Add("<tbody>");
    for(int row = 1; row < grid->RowCount; row++) {
        html->Add("<tr>");
        for(int col = 0; col < grid->ColCount; col++) {
            html->Add("<td>" + grid->Cells[col][row] + "</td>");
        }
        html->Add("</tr>");
    }

    html->Add("<tr class='total-row'>");
    html->Add("<td colspan='" + IntToStr(grid->ColCount-1) + "' style='text-align: right;'>Итого:</td>");
    html->Add("<td>" + totalSum + " руб.</td>");
    html->Add("</tr>");

    html->Add("</tbody></table>");
    if (memo) memo->Lines->Add("Сгенерирована HTML таблица");
}

void PrintDocumentManager::GenerateSignatures(TStringList* html)
{
    const String months[12] = {
        "января", "февраля", "марта", "апреля", "мая", "июня",
        "июля", "августа", "сентября", "октября", "ноября", "декабря"
    };

    TDateTime currentDate = Now();
    int day = StrToInt(FormatDateTime("d", currentDate));
    int month = StrToInt(FormatDateTime("m", currentDate));
    int year = StrToInt(FormatDateTime("yyyy", currentDate));

    String dateStr = "«" + IntToStr(day) + "» " + months[month-1] + " " + IntToStr(year) + " г.";

    html->Add("<div class='signature-block'>");
    html->Add("<div style='margin-right: 50px;'>");
    html->Add("<p>_________________________</p>");
    html->Add("<p>Подпись исполнителя</p>");
    html->Add("</div>");
    html->Add("<div>");
    html->Add("<p>_________________________</p>");
    html->Add("<p>Подпись заказчика</p>");
    html->Add("</div>");
    html->Add("<div style='margin-left: auto;'>");
    html->Add("<p>" + dateStr + "</p>");
    html->Add("<p>Дата составления</p>");
    html->Add("</div>");
    html->Add("</div>");
    if (memo) memo->Lines->Add("Сгенерированы подписи в HTML");
}

void PrintDocumentManager::ExportDocument(TStringGrid* grid, const String& sumValue,
                                       const String& documentTitle,
                                       const Consignee& sender,
                                       const Consignee& receiver)
{
    if (memo) memo->Lines->Add("Начало экспорта документа");
    TStringList* html = new TStringList();
    try {
        GenerateHTMLHeader(html);
        html->Add("<body>");

        GenerateCompanyInfo(html, documentTitle, sender, receiver);
        GenerateHTMLTable(grid, html, sumValue);
        GenerateSignatures(html);

        html->Add("</body></html>");

        TSaveDialog* saveDialog = new TSaveDialog(NULL);
        saveDialog->Filter = "HTML files (*.html)|*.html";
        saveDialog->DefaultExt = "html";
        saveDialog->FileName = "Смета_" + FormatDateTime("ddmmyyyy", Now()) + ".html";

        if(saveDialog->Execute()) {
            String fileName = saveDialog->FileName;
            if (memo) memo->Lines->Add("Сохранение файла: " + fileName);

            TFileStream *fs = new TFileStream(fileName, fmCreate);
            try {
                const Byte BOM[] = {0xEF, 0xBB, 0xBF};
                fs->Write(BOM, sizeof(BOM));

                String htmlText = html->Text;
                TEncoding *encoding = TEncoding::UTF8;
                TBytes bytes = encoding->GetBytes(htmlText);
                fs->Write(bytes, bytes.Length);
                if (memo) memo->Lines->Add("Файл успешно сохранен");
            }
            __finally {
                delete fs;
            }

            if(GetAutoOpen()) {
                if (memo) memo->Lines->Add("Автооткрытие документа");
                ShellExecute(0, L"open", fileName.w_str(), NULL, NULL, SW_SHOWNORMAL);
            }
        }
		delete saveDialog;
        delete html;
    }
    catch (Exception &e) {
        if (memo) memo->Lines->Add("Ошибка экспорта: " + e.Message);
        ShowMessage("Ошибка экспорта: " + e.Message);
	}
    if (memo) memo->Lines->Add("Экспорт завершен");
}

void PrintDocumentManager::PrintDocument(TStringGrid* grid, const String& sumValue,
                                      const String& documentTitle,
                                      const Consignee& sender,
                                      const Consignee& receiver,
                                      const String& fileName)
{
    if (memo) memo->Lines->Add("Начало печати документа: " + fileName);
    TStringList* html = new TStringList();
    try {
        GenerateHTMLHeader(html);
        html->Add("<body>");
        GenerateCompanyInfo(html, documentTitle, sender, receiver);
        GenerateHTMLTable(grid, html, sumValue);
        GenerateSignatures(html);
        html->Add("</body></html>");

        if (memo) memo->Lines->Add("Сохранение HTML для печати");
        TFileStream *fs = new TFileStream(fileName, fmCreate);
        try {
            const Byte BOM[] = {0xEF, 0xBB, 0xBF};
            fs->Write(BOM, sizeof(BOM));

            String htmlText = html->Text;
            TEncoding *encoding = TEncoding::UTF8;
            TBytes bytes = encoding->GetBytes(htmlText);
            fs->Write(bytes, bytes.Length);
            if (memo) memo->Lines->Add("HTML сохранен для печати");
        }
        __finally {
            delete fs;
        }

        String printCommand = "print";
        if (memo) memo->Lines->Add("Отправка на печать: " + fileName);
        ShellExecute(0, printCommand.w_str(), fileName.w_str(), NULL, NULL, SW_HIDE);
		if (memo) memo->Lines->Add("Документ отправлен на печать");
		delete html;
    }
    catch (Exception &e) {
        if (memo) memo->Lines->Add("Ошибка печати: " + e.Message);
        ShowMessage("Ошибка печати: " + e.Message);
    }
    if (memo) memo->Lines->Add("Печать завершена");
}

void PrintDocumentManager::SetAutoOpen(bool value)
{
    SaveToINI("AutoOpen", value ? "1" : "0");
    if (memo) memo->Lines->Add("Установка автооткрытия: " + String(value));
}

bool PrintDocumentManager::GetAutoOpen()
{
    bool result = LoadFromINI("AutoOpen") == "1";
    if (memo) memo->Lines->Add("Получение автооткрытия: " + String(result));
    return result;
}

void PrintDocumentManager::SaveToINI(const String& key, const String& value)
{
    TIniFile* ini = new TIniFile(configPath);
    try {
        ini->WriteString("Settings", key, value);
        if (memo) memo->Lines->Add("Сохранение в INI: " + key + " = " + value);
    }
    __finally {
        delete ini;
    }
}

String PrintDocumentManager::LoadFromINI(const String& key)
{
    TIniFile* ini = new TIniFile(configPath);
    try {
        String result = ini->ReadString("Settings", key, "");
        if (memo) memo->Lines->Add("Чтение из INI: " + key + " = " + result);
        return result;
    }
    __finally {
        delete ini;
    }
}
