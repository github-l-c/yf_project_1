#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    setWindowTitle(tr("XML Reader"));
    QTextEdit* text_edit = new QTextEdit(this);
    setCentralWidget(text_edit);

    vector<string>libraryUrls;
    ReadLibraryUrl(libraryUrls);
    for (int i = 0; i < libraryUrls.size(); ++i)
    {
        text_edit->append(QString::fromStdString(libraryUrls[i]));
    }
}

bool MainWindow::ReadLibraryUrl(vector<string>& libraryUrls, const QString& fileName)
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
                if (readHubsurlElement(libraryUrls) == false)//递归下降算法，层层读取
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

bool MainWindow::readHubsurlElement(vector<string>& libraryUrls)
{
    if (!(reader.isStartElement() && reader.name() == "hubsurl"))//检查是否是在根标签不是则会报错
    {
        return false;
    }
    reader.readNext(); // 读取下一个记号，它返回记号的类型
    while (!reader.atEnd())
    {
		//读取到关闭标签“</hubsurl>”
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
		//读取到新的顶级索引“<url>”
        if (reader.isStartElement())
        {
            if (reader.name() == "url")
            {
                readUrlElement(libraryUrls);
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

void MainWindow::readUrlElement(vector<string>& libraryUrls)
{
    reader.readNext();
    while (!reader.atEnd())
    {
		//读取到关闭标签“</url>”跳出循环
        if (reader.isEndElement())
        {
            reader.readNext();
            break;
        }
		//再次读取到下一个顶级标签
        if (reader.isStartElement())
        {
            if (reader.name() == "url")
            {
                readUrlElement(libraryUrls);
            }
            else if (reader.name() == "address")
            {
                readAddressElement(libraryUrls);
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

void MainWindow::readAddressElement(vector<string>& libraryUrls)
{
    QString page = reader.readElementText();
    if (reader.isEndElement())
    {
        reader.readNext();
    }
    libraryUrls.push_back(page.toStdString());
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