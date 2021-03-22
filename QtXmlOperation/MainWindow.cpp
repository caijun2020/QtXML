#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QUrl>
#include <QFileInfo>
#include <QHeaderView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_xml(NULL),
    m_treeWidget(new QTreeWidget(this))
{
    ui->setupUi(this);

    // Init Widget Font type and size
    initWidgetFont();

    // Init Widget Style
    initWidgetStyle();

    // XML unit test
    //xmlTest();

    // Set Window Title
    this->setWindowTitle(tr("XML Viewer"));

    // Set Menu Bar Version Info
    ui->menuVersion->addAction("V1.0 2021-Mar-19");
}

MainWindow::~MainWindow()
{
    delete ui;

    if(NULL != m_xml)
    {
        delete m_xml;
    }

    delete m_treeWidget;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(obj == m_treeWidget)
    {
        if(e->type() == QEvent::DragEnter)
        {
            QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(e);
            dee->acceptProposedAction();

            return true;
        }
        else if(e->type() == QEvent::Drop)
        {
            QDropEvent *de = dynamic_cast<QDropEvent *>(e);
            QList<QUrl> urls = de->mimeData()->urls();

            if(urls.isEmpty())
            {
                return true;
            }

            QString path = urls.first().toLocalFile();
            convertXMLToQTreeWidget(path);

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // pass the event on to the parent class
        return eventFilter(obj, e);
    }
}

void MainWindow::initWidgetFont()
{
}

void MainWindow::initWidgetStyle()
{
    QStringList headers;
    headers << "Items" << "Attributes" << "Text";
    m_treeWidget->setHeaderLabels(headers);

    setCentralWidget(m_treeWidget);

    // Enable drag&drop for
    m_treeWidget->installEventFilter(this);
    m_treeWidget->setAcceptDrops(true);
}

void MainWindow::xmlTest()
{
    if(NULL != m_xml)
    {
        delete m_xml;
    }

    m_xml = new QtXmlOperation();
    m_xml->createDocument("WorkItemResult");

    QStringList attrName;
    attrName << "DataType" << "Value";

    QStringList attrs;
    attrs << "Int32" << "1";
    m_xml->insertNode("", "SrcWINum", "", attrName, attrs);

    attrs.clear();
    attrs << "Int32" << "2";
    m_xml->insertNode("", "DstWINum", "", attrName, attrs);

    attrs.clear();
    attrs << "Int32" << "3";
    m_xml->insertNode("", "Progress", "", attrName, attrs);

    attrs.clear();
    attrs << "Int32" << "4";
    m_xml->insertNode("", "Progress", "", attrName, attrs);

    m_xml->saveAs("./1.xml");

    delete m_xml;
    m_xml = new QtXmlOperation("./1.xml");

    m_xml->deleteNode("WorkItemResult/Progress");

    m_xml->saveAs("./2.xml");

    attrs.clear();
    attrs << "Int32" << "11";
    m_xml->insertNode("", "SrcWINum1", "src-1", attrName, attrs);
    m_xml->insertNode("WorkItemResult/SrcWINum", "SrcWINum1", "", attrName, attrs);


    qDebug() << "SrcWINum1 text=" << m_xml->readText("SrcWINum1");
    qDebug() << "DstWINum text=" << m_xml->readText("DstWINum");
    qDebug() << "DstWINum attribute DataType=" << m_xml->readAttribute("DstWINum", "DataType");
    qDebug() << "DstWINum attribute Value=" << m_xml->readAttribute("DstWINum", "Value");
    qDebug() << "DstWINum count=" << m_xml->getNodeCount("DstWINum");
    qDebug() << "SrcWINum1 count=" << m_xml->getNodeCount("SrcWINum1");

    m_xml->saveAs("./3.xml");

    m_xml->insertNode("WorkItemResult/SrcWINum/SrcWINum1", "SrcWINum1", "abc", attrName, attrs);
    qDebug() << "SrcWINum1 count=" << m_xml->getNodeCount("SrcWINum1");
    qDebug() << "WorkItemResult/SrcWINum1 text=" << m_xml->readText("WorkItemResult/SrcWINum1");
    qDebug() << "WorkItemResult/SrcWINum/SrcWINum1 text=" << m_xml->readText("WorkItemResult/SrcWINum/SrcWINum1");
    qDebug() << "WorkItemResult/SrcWINum/SrcWINum1/SrcWINum1 text=" << m_xml->readText("WorkItemResult/SrcWINum/SrcWINum1/SrcWINum1");

    m_xml->saveAs("./4.xml");

}

void MainWindow::parseDomToItem(const QDomElement &element, QTreeWidgetItem *parent)
{
    if(element.isNull())
    {
        return;
    }

    QTreeWidgetItem *item = createItem(element, parent);

    QDomNodeList children = element.childNodes();
    for(int i = 0; i < children.size(); i++)
    {
        parseDomToItem(children.at(i).toElement(), item);
    }

}

QTreeWidgetItem* MainWindow::createItem(const QDomElement &element, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = NULL;

    if(!element.isNull())
    {
        item = new QTreeWidgetItem(parent);

        // Column 0 display tag
        item->setText(0, element.tagName());

        QString attr = "";
        if(element.hasAttributes())
        {
            for(int i = 0; i < element.attributes().size(); i++)
            {
                attr.append(element.attributes().item(i).toAttr().name());
                attr.append("=");
                attr.append(element.attributes().item(i).toAttr().value());
                attr.append(" ");
            }

            // Column 1 display attributes
            item->setText(1, attr);
        }

        if(!element.text().isEmpty())
        {
            // Column 2 display text
            item->setText(2, element.text());
        }
    }

    return item;
}


bool MainWindow::convertXMLToQTreeWidget(QString file)
{
    bool ret = false;

    // Clear tree widget
    m_treeWidget->clear();

    // Check opened file suffix .xml
    QFileInfo fileInfo(file);
    if("xml" != fileInfo.suffix().toLower())
    {
        qDebug() << QString("%1 is not an xml file!").arg(file);
        return ret;
    }

    if(NULL != m_xml)
    {
        delete m_xml;
    }

    m_xml = new QtXmlOperation(file);

    QTreeWidgetItem *rootItem = m_treeWidget->invisibleRootItem();
    parseDomToItem(m_xml->getRootElement(), rootItem);

    // Expand all the items
    m_treeWidget->expandAll();

    // Auto resize the width
    m_treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

    ret= true;

    return ret;
}
