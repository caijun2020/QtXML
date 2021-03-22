#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "QtXmlOperation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::MainWindow *ui;

    QtXmlOperation *m_xml;

    QTreeWidget *m_treeWidget;

    void xmlTest();

    void initWidgetFont();  // Init the Font type and size of the widget
    void initWidgetStyle(); // Init Icon of the widget

    // Parse xml file to QTreeWidgetItem
    void parseDomToItem(const QDomElement &element, QTreeWidgetItem *parent);
    QTreeWidgetItem* createItem(const QDomElement &element, QTreeWidgetItem *parent);

    // Parse XML to QTreeWidget
    bool convertXMLToQTreeWidget(QString file);

};

#endif // MAINWINDOW_H
