#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <Windows.h>
#include <windows.h>
#include "C:\Users\HP\Desktop\Detours-master\Detours-master\include\detours.h"
#include <QDebug>
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "detours.lib")


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList()<<"时间"<<"函数"<<"功能"<<"参数");
    connect(ui->pushButton_2,&QPushButton::clicked,[=](){
        path = QFileDialog::getOpenFileName(this,"选择目标文件","C:\\Users\\HP\\Desktop","(*.exe)");
        ui->lineEdit->setText(path);
//        QFile file(path);
//        file.open(QIODevice::ReadOnly);
//        file.close();
    });
    connect(ui->pushButton,&QPushButton::clicked,[=](){

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        si.cb = sizeof(STARTUPINFO);

        WCHAR DirPath[MAX_PATH + 1];
        wcscpy_s(DirPath, MAX_PATH, L"C:\\Users\\HP\\Desktop"); //Dll的文件夹

        char DLLPath[MAX_PATH + 1] = "C:\\Users\\HP\\Desktop\\Detour_dll.dll";//Dll的地址

        WCHAR EXE[MAX_PATH + 1] = { 0 };

        wcscpy_s(EXE, MAX_PATH, reinterpret_cast<const wchar_t *>(path.utf16()));


        if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE,
            CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi,
            DLLPath, NULL))
        {
            MessageBoxA(NULL, "INJECTED", NULL, NULL);
            ResumeThread(pi.hThread);
            WaitForSingleObject(pi.hProcess, INFINITE);
        }
        else
        {
            char error[100];
            sprintf_s(error, "%d", GetLastError());
            MessageBoxA(NULL, error, NULL, NULL);
        }

        qDebug()<<"1";
        QString dataPath = "D:\\aa.txt";
        QFile file(dataPath);
        file.open(QIODevice::ReadOnly);
        while (!file.atEnd()) {
            QString str = file.readLine();
            int len = str.length();
            int fengefu[4] = {0};
            qDebug()<<fengefu[0]<<fengefu[1]<<fengefu[2];
            int num = 0;
            for (int i = 0; i < len; i++) {
                if(str[i] == '!'){
                    fengefu[num] = i;
                    num++;
                    qDebug()<<i;
                }
            }
            QString str1,str2,str3,str4;
            str1 = str.mid(0,fengefu[0]-0);
            str2 = str.mid(fengefu[0]+1,fengefu[1]-fengefu[0]-1);
            str3 = str.mid(fengefu[1]+1,fengefu[2]-fengefu[1]-1);
            str4 = str.mid(fengefu[2]+1,fengefu[3]-fengefu[2]-1);

            qDebug()<<str;
            qDebug()<<str1<<str2<<str3;

            QTreeWidgetItem* item = new QTreeWidgetItem();

            item->setData(0, 0, str2);
            QTreeWidgetItem* item2 = new QTreeWidgetItem();
            item2->setData(0, 0, str1);
            item2->setData(1, 0, str2);
            item2->setData(2, 0, str3);
            item2->setData(3, 0, str4);
            item->addChild(item2);
            ui->treeWidget->addTopLevelItem(item);
        }
        file.close();
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

