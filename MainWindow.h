#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QXmlStreamReader>
#include <QMainWindow>
#include <QTreeWidget>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <string>
#include <vector>
#include <QTextEdit>
using namespace std;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow() {};
    //读取文件，并进行根目录解析
    bool ReadLibraryUrl(vector<string>&, const QString& fileName = "");
private:
    //读取根目录
    bool readHubsurlElement(vector<string>&);
    //递归读取标签为“url”的字符块，当读取到的标签为"address"时中止。
    void readUrlElement(vector<string>&);
    //读取库地址
    void readAddressElement(vector<string>&);
	//跳过未知标签的字符块
    void skipUnknownElement();
    QTreeWidget* treeWidget;
    QXmlStreamReader reader;

private:
    Ui::MainWindowClass ui;
};


