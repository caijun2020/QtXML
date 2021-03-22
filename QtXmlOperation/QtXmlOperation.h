/**********************************************************************
PACKAGE:        Utility
FILE:           QtXmlOperation.h
COPYRIGHT (C):  All rights reserved.

PURPOSE:        Provide XML read/write operation
**********************************************************************/
#ifndef QTXMLOPERATION_H
#define QTXMLOPERATION_H

#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>


class QtXmlOperation : public QObject
{
    Q_OBJECT
public:

    QtXmlOperation();
    QtXmlOperation(QString fileName);
    virtual ~QtXmlOperation();


    /*-----------------------------------------------------------------------
    FUNCTION:		createDocument
    PURPOSE:		Create an xml file in RAM with root element
    ARGUMENTS:		QString rootName, root element name
    RETURNS:		bool, true: successful, false: failed
    -----------------------------------------------------------------------*/
    bool createDocument(QString rootName = "");


    /*-----------------------------------------------------------------------
    FUNCTION:		createRoot
    PURPOSE:		Create the root element
    ARGUMENTS:		QString rootName, root element name
    RETURNS:		bool, true: successful, false: failed
    -----------------------------------------------------------------------*/
    bool createRoot(QString rootName);


    /*-----------------------------------------------------------------------
    FUNCTION:		openDocument
    PURPOSE:		Open an xml file in disk with fileName
    ARGUMENTS:		QString fileName, file name
    RETURNS:		bool, true: successful, false: failed
    -----------------------------------------------------------------------*/
    bool openDocument(QString fileName);


    /*-----------------------------------------------------------------------
    FUNCTION:		saveAs
    PURPOSE:		Save to .xml file to disk with fileName
    ARGUMENTS:		QString fileName, file name
    RETURNS:		bool, true: successful, false: failed
    -----------------------------------------------------------------------*/
    bool saveAs(QString fileName);


    /*-----------------------------------------------------------------------
    FUNCTION:		isFileExist
    PURPOSE:		Check wheter the xml file exist
    ARGUMENTS:		None
    RETURNS:		bool, true: file exist, false: no file
    -----------------------------------------------------------------------*/
    bool isFileExist();


    /*-----------------------------------------------------------------------
    FUNCTION:		isFileOpen
    PURPOSE:		Check wheter the xml file is open
    ARGUMENTS:		None
    RETURNS:		bool, true: open, false: closed
    -----------------------------------------------------------------------*/
    bool isFileOpen();


    /*-----------------------------------------------------------------------
    FUNCTION:		readText
    PURPOSE:		Get Text string of node
    ARGUMENTS:		QString nodeName, node name
                    int nodeIndex, node index(from 0 t0 n), default as 0 (1st one)
    RETURNS:		QString
    -----------------------------------------------------------------------*/
    QString readText(QString nodeName, int nodeIndex = 0);


    /*-----------------------------------------------------------------------
    FUNCTION:		readAttribute
    PURPOSE:		Get Attribute string of node by attrName
    ARGUMENTS:		QString nodeName, node name
                    QString attrName, attribute name
                    int nodeIndex, node index(from 0 t0 n), default as 0 (1st one)
    RETURNS:		QString
    -----------------------------------------------------------------------*/
    QString readAttribute(QString nodeName, QString attrName, int nodeIndex = 0);


    /*-----------------------------------------------------------------------
    FUNCTION:		insertNode
    PURPOSE:		Insert a node element
    ARGUMENTS:		QString parentNodeName, node name of parent
                    QString nodeName, node name
                    QString nodeText, node text
                    QStringList attrNames, attribute name
                    QStringList attrs, attributes
                    int parentIndex, parent node index(from 0 to n), default as 0 (1st one)
    RETURNS:		bool, true:successful, false: failed
    -----------------------------------------------------------------------*/
    bool insertNode(QString parentNodeName, QString nodeName, QString nodeText, QStringList attrNames , QStringList attrs, int parentIndex = 0);


    /*-----------------------------------------------------------------------
    FUNCTION:		deleteNode
    PURPOSE:		Delete a node element
    ARGUMENTS:		QString nodeName, node name
                    int nodeIndex, node index(from 0 t0 n), default as 0 (1st one)
    RETURNS:		bool, true:successful, false: failed
    -----------------------------------------------------------------------*/
    bool deleteNode(QString nodeName, int nodeIndex = 0);


    /*-----------------------------------------------------------------------
    FUNCTION:		replaceNode
    PURPOSE:		Replace a node element
    ARGUMENTS:		QString parentNodeName, node name of parent
                    QString nodeName, node name
                    QString nodeText, node text
                    QStringList attrNames, attribute name
                    QStringList attrs, attributes
                    int parentIndex, parent node index(from 0 to n), default as 0 (1st one)
    RETURNS:		bool, true:successful, false: failed
    -----------------------------------------------------------------------*/
    bool replaceNode(QString parentNodeName, QString nodeName, QString nodeText, QStringList attrNames, QStringList attrs, int parentIndex = 0);


    /*-----------------------------------------------------------------------
    FUNCTION:		getNodeCount
    PURPOSE:		Get the count of node by node names (names example: "root/abc/123")
    ARGUMENTS:		QString nodeNames, node names
    RETURNS:		int, the number of node
    -----------------------------------------------------------------------*/
    int getNodeCount(QString nodeNames);


    /*-----------------------------------------------------------------------
    FUNCTION:		getRootElement
    PURPOSE:		Get the root element reference
    ARGUMENTS:		None
    RETURNS:		QDomElement, return a root reference
    -----------------------------------------------------------------------*/
    QDomElement getRootElement();

    
signals:
    
public slots:

private:
    QDomDocument *m_doc;
    QFile *m_file;

    /*-----------------------------------------------------------------------
    FUNCTION:		findNodeByNames
    PURPOSE:		Find node reference by node names (names example: "root/abc/123")
    ARGUMENTS:		QString nodeNames, node names
                    int index, there may be severals nodes own the same name
    RETURNS:		QDomNode, returns a new reference on success or a null node an failure
    -----------------------------------------------------------------------*/
    QDomNode findNodeByNames(QString nodeNames, int index = 0);

    /*-----------------------------------------------------------------------
    FUNCTION:		findNode
    PURPOSE:		Find node reference by parent node and node name
    ARGUMENTS:		QDomNode parentNode, parent node ref
                    QString childNodeName, node name
                    bool preciseMatch, precise match flag

    RETURNS:		QDomNode, returns a new reference on success or a null node an failure
    -----------------------------------------------------------------------*/
    QDomNode findNode(QDomNode parentNode, QString childNodeName, bool preciseMatch = true);
    
};

#endif // QTXMLOPERATION_H
