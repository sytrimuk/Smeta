#ifndef PrintDocumentManagerH
#define PrintDocumentManagerH

#include <vcl.h>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Grids.hpp>
#include <IniFiles.hpp>
#include <ShellAPI.h>
#include <Vcl.OleCtrls.hpp>
#include "Consignee.h"

class PrintDocumentManager
{
private:
    String logoPath;
    String configPath;
    TImage* currentLogo;

    void SaveToINI(const String& key, const String& value);
    String LoadFromINI(const String& key);
    void GenerateHTMLHeader(TStringList* html);
    void GenerateCompanyInfo(TStringList* html, const String& documentTitle,
                            const Consignee& sender, const Consignee& receiver);
    void GenerateHTMLTable(TStringGrid* grid, TStringList* html, const String& totalSum);
    void GenerateSignatures(TStringList* html);

public:
    PrintDocumentManager();
    void UploadLogo(TImage* img);
    void LoadLogo(TImage* img);
    void SetAutoOpen(bool value);
    bool GetAutoOpen();
    void ExportDocument(TStringGrid* grid, const String& sumValue,
                       const String& documentTitle,
                       const Consignee& sender,
					   const Consignee& receiver);
    void PrintDocument(TStringGrid* grid, const String& sumValue,
                     const String& documentTitle,
                     const Consignee& sender,
                     const Consignee& receiver,
                     const String& fileName);
};
extern TMemo *memo;
#endif
