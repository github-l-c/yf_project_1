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
    bool ReadLibraryUrl(vector<string>&, const QString& fileName = "");
private:
    bool readHubsurlElement(vector<string>&);
    void readUrlElement(vector<string>&);
    void readAddressElement(vector<string>&);
    void skipUnknownElement();
    QTreeWidget* treeWidget;
    QXmlStreamReader reader;

private:
    Ui::MainWindowClass ui;
    vector<string>urls;
};


