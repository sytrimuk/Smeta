object EditDialog: TEditDialog
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'EditDialog'
  ClientHeight = 103
  ClientWidth = 462
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 13
    Width = 82
    Height = 13
    Caption = #1058#1077#1082#1089#1090' '#1080#1079' '#1103#1095#1077#1081#1082#1080
  end
  object Edit1: TEdit
    Left = 8
    Top = 32
    Width = 446
    Height = 21
    TabOrder = 0
    OnKeyPress = Edit1KeyPress
  end
  object Button1: TButton
    Left = 272
    Top = 59
    Width = 75
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 379
    Top = 59
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    OnClick = Button2Click
  end
end
