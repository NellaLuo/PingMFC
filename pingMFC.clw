; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPingMFCDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "pingMFC.h"

ClassCount=3
Class1=CPingMFCApp
Class2=CPingMFCDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PINGMFC_DIALOG

[CLS:CPingMFCApp]
Type=0
HeaderFile=pingMFC.h
ImplementationFile=pingMFC.cpp
Filter=N

[CLS:CPingMFCDlg]
Type=0
HeaderFile=pingMFCDlg.h
ImplementationFile=pingMFCDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_STATE

[CLS:CAboutDlg]
Type=0
HeaderFile=pingMFCDlg.h
ImplementationFile=pingMFCDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PINGMFC_DIALOG]
Type=1
Class=CPingMFCDlg
ControlCount=4
Control1=IDC_IP,static,1342308352
Control2=IDC_EDITip,edit,1350631552
Control3=IDC_ping,button,1342242816
Control4=IDC_STATE,static,1342308353

