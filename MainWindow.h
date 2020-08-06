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
    //��ȡ�ļ��������и�Ŀ¼����
    bool ReadLibraryUrl(vector<string>&, const QString& fileName = "");
private:
    //��ȡ��Ŀ¼
    bool readHubsurlElement(vector<string>&);
    //�ݹ��ȡ��ǩΪ��url�����ַ��飬����ȡ���ı�ǩΪ"address"ʱ��ֹ��
    void readUrlElement(vector<string>&);
    //��ȡ���ַ
    void readAddressElement(vector<string>&);
	//����δ֪��ǩ���ַ���
    void skipUnknownElement();
    QTreeWidget* treeWidget;
    QXmlStreamReader reader;

private:
    Ui::MainWindowClass ui;
};


