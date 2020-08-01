#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    setWindowTitle(tr("XML Reader"));
    QTextEdit* text_edit = new QTextEdit(this);
    setCentralWidget(text_edit);

    vector<string>LibraryUrls;
    ReadLibraryUrl(LibraryUrls);
    for (int i = 0; i < LibraryUrls.size(); ++i)
    {
        text_edit->append(QString::fromStdString(urls[i]));
    }
}

bool MainWindow::ReadLibraryUrl(vector<string>& LibraryUrls, const QString& fileName)
{
    QString urlFilePath = fileName;
    if (fileName == "")
        urlFilePath = QApplication::applicationDirPath() + "/demo.xml";
    QFile file(urlFilePath);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Cannot read file %1").arg(fileName));
        return false;
    }
    reader.setDevice(&file);
    while (!reader.atEnd())
    {

        if (reader.isStartElement())
        {
            if (reader.name() == "hubsurl")
            {
                if (readHubsurlElement(LibraryUrls) == false)//递归下降算法，层层读取
                {
                    return false;
                }
            }
            else
            {
                reader.raiseError(tr("Not a valid book file"));
            }
        }
        else
        {
            reader.readNext(); //循坏调用首次移动3次，后面移动一次
        }
    }
    file.close();

    if (reader.hasError())
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Failed to parse file %1").arg(fileName));
        return false;
    }
    else if (file.error() != QFile::NoError)
    {
        QMessageBox::critical(this, tr("Error"),
            tr("Cannot read file %1").arg(fileName));
        return false;
    }
    return true;
}

bool MainWindow::readHubsurlElement(vector<string>& LibraryUrls)
{
    if (!(reader.isStartElement() && reader.name() == "hubsurl"))//不是则会报错
    {
        return false;
    }
    reader.readNext(); // 读取下一个记号，它返回记号的类型
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.isStartElement())
        {
            if (reader.name() == "url")
            {
                readUrlElement(LibraryUrls);
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }
    return true;
}

void MainWindow::readUrlElement(vector<string>& LibraryUrls)
{
    
    string term = reader.attributes().value("term").toString().toStdString();
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.isStartElement())
        {
            if (reader.name() == "url")
            {
                readUrlElement(LibraryUrls);
            }
            else if (reader.name() == "address")
            {
                //urls.push_back(term);
                readAddressElement(LibraryUrls);
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            reader.readNext();
        }
    }


}

void MainWindow::readAddressElement(vector<string>& LibraryUrls)
{
    QString page = reader.readElementText();
    if (reader.isEndElement())
    {
        reader.readNext();
    }
    urls.push_back(page.toStdString());
    LibraryUrls.push_back(page.toStdString());
}

void MainWindow::skipUnknownElement()
{
    reader.readNext();
    while (!reader.atEnd())
    {
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }

        if (reader.isStartElement())
        {
            skipUnknownElement();
        }
        else
        {
            reader.readNext();
        }
    }
}