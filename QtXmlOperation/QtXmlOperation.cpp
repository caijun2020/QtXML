/**********************************************************************
PACKAGE:        Utility
FILE:           QtXmlOperation.cpp
COPYRIGHT (C):  All rights reserved.

PURPOSE:        Provide XML read/write operation
**********************************************************************/

#include "QtXmlOperation.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDir>
#include <QDebug>

//#define ENABLE_DEBUG_TRACE_XML 1

QtXmlOperation::QtXmlOperation() :
    m_doc(new QDomDocument),
    m_file(NULL)
{
    m_doc->clear();
}

QtXmlOperation::QtXmlOperation(QString fileName) :
    m_doc(new QDomDocument),
    m_file(NULL)
{
    m_doc->clear();

    if(!fileName.isEmpty())
    {
        // Open file
        // If not exist, create a new file
        if(!openDocument(fileName))
        {
            createDocument();
            saveAs(fileName);
        }
    }
}

QtXmlOperation::~QtXmlOperation()
{
    if(NULL != m_file)
    {
        if(m_file->isOpen())
        {
            m_file->close();
        }
        delete m_file;
    }

    delete m_doc;
}

bool QtXmlOperation::createDocument(QString rootName)
{
    bool ret = false;

    m_doc->clear();
    QDomProcessingInstruction introduction = m_doc->createProcessingInstruction("xml", "version=\'1.0\' encoding=\'UTF-8\'");
    m_doc->appendChild(introduction);

    // Create root element
    createRoot(rootName);

    ret = true;

    return ret;
}

bool QtXmlOperation::createRoot(QString rootName)
{
    bool ret = false;

    // If root is not empty, append root element
    if(!rootName.isEmpty())
    {
        QDomElement root = m_doc->createElement(rootName);
        m_doc->appendChild(root);
    }

    return ret;
}

bool QtXmlOperation::openDocument(QString fileName)
{
    bool ret = false;

    if(NULL != m_file)
    {
        if(m_file->isOpen())
        {
            m_file->close();
        }
        delete m_file;
    }

    m_file = new QFile(fileName);

    if(m_file->exists())
    {
        if(m_file->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            m_doc->clear();

            QString errorStr = "";
            int errorLine = 0;
            int errorColumn = 0;

            if(m_doc->setContent(m_file, false, &errorStr, &errorLine, &errorColumn))
            {
                ret = true;
            }
            else
            {
                qDebug() << "Error: Parse error at line " << errorLine << ", "
                         << "column " << errorColumn << ": "
                         << qPrintable(errorStr);
            }
        }
    }

    return ret;
}

bool QtXmlOperation::saveAs(QString fileName)
{
    bool ret = false;

    QFile file(fileName);

    // If file not exist create one
    if (!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    // Overwrite the file
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        m_doc->save(stream, 4);
        file.close();

        ret = true;
    }

    return ret;
}

bool QtXmlOperation::isFileExist()
{
    bool ret = false;

    if(NULL != m_file)
    {
        ret = m_file->exists();
    }

    return ret;
}

bool QtXmlOperation::isFileOpen()
{
    bool ret = false;

    if(isFileExist())
    {
        if(NULL != m_file)
        {
            if(m_file->isOpen())
            {
                ret = true;
            }
        }
    }

    return ret;
}

QString QtXmlOperation::readText(QString nodeName, int nodeIndex)
{
    QString ret = "";

    QDomElement root = m_doc->documentElement();

    if(!root.isNull())
    {
        QDomElement currentNode = findNodeByNames(nodeName, nodeIndex).toElement();

        if(!currentNode.isNull())
        {
            ret = currentNode.text();
        }
    }

    return ret;
}

QString QtXmlOperation::readAttribute(QString nodeName, QString attrName, int nodeIndex)
{
    QString ret = "";

    QDomElement root = m_doc->documentElement();

    if(!root.isNull())
    {
        QDomElement currentNode = findNodeByNames(nodeName, nodeIndex).toElement();

        if(!currentNode.isNull())
        {
            ret = currentNode.attribute(attrName);
        }
    }

    return ret;
}

bool QtXmlOperation::insertNode(QString parentNodeName, QString nodeName, QString nodeText,
                                QStringList attrNames, QStringList attrs, int parentIndex)
{
    bool ret = false;

    QDomElement root = m_doc->documentElement();
    QDomElement currentNode;
    currentNode.clear();

    if(!root.isNull())
    {
        if(!parentNodeName.isEmpty())
        {
            currentNode = findNodeByNames(parentNodeName, parentIndex).toElement();
        }
        else
        {
            // parentNodeName = "" means insert into root node
            currentNode = root;
        }

        // Found parent node
        if(!currentNode.isNull())
        {
            QDomElement newNode = m_doc->createElement(nodeName);
            currentNode.appendChild(newNode);

            if(!nodeText.isEmpty())
            {
                QDomText newTextNode = m_doc->createTextNode(nodeText);
                newNode.appendChild(newTextNode);
            }

            if (attrNames.size() > 0)
            {
                for (int i = 0; i < attrNames.size(); ++i)
                {
                    if (i < attrs.size())
                    {
                        newNode.setAttribute(attrNames[i], attrs[i]);
                    }
                    else
                    {
                        newNode.setAttribute(attrNames[i], "");
                    }
                }
            }

            ret = true;
        }
    }

    return ret;
}

bool QtXmlOperation::deleteNode(QString nodeName, int nodeIndex)
{
    bool ret = false;

    QDomElement root = m_doc->documentElement();
    QDomElement currentNode;
    currentNode.clear();

    if(root.tagName() != nodeName)
    {
        currentNode = findNodeByNames(nodeName, nodeIndex).toElement();

        if(!currentNode.isNull())
        {
            QDomElement parentNode = currentNode.parentNode().toElement();

            if(!parentNode.isNull())
            {
                parentNode.removeChild(currentNode);
            }
            else
            {
                m_doc->removeChild(root);
            }

            ret = true;
        }
    }
    else
    {
        m_doc->removeChild(root);
        ret = true;
    }

    return ret;
}

bool QtXmlOperation::replaceNode(QString parentNodeName, QString nodeName, QString nodeText, QStringList attrNames, QStringList attrs, int parentIndex)
{
    bool ret = false;

    if(deleteNode(nodeName, parentIndex))
    {
        ret = insertNode(parentNodeName, nodeName, nodeText, attrNames, attrs, parentIndex);
    }

    return ret;
}

QDomNode QtXmlOperation::findNodeByNames(QString nodeNames, int index)
{
    QDomNode retNode;
    retNode.clear();
    int foundNodeNum = 0;

    if(!nodeNames.isEmpty())
    {
        // "\\W+", use any sequence of non-word characters as the separator
        QStringList tags = nodeNames.split(QRegExp("\\W+"), QString::SkipEmptyParts);
        QDomNodeList lists = m_doc->elementsByTagName(tags.at(0));
        QDomNode curretNode;

        //qDebug() << __FUNCTION__ << "(): " << "tags = " << tags;

        if(tags.size() > 1)
        {
            for(int cnt = 0; cnt < lists.size(); cnt++)
            {
                curretNode = lists.at(cnt);

                for(int tagNum = 0; tagNum < tags.size() - 1; tagNum++)
                {
                    curretNode = findNode(curretNode, tags.at(tagNum + 1));

                    // Not Found node
                    if(curretNode.isNull())
                    {
                        break;
                    }
                }

                // Found node
                if(!curretNode.isNull())
                {
                    retNode = curretNode;

                    if(index == foundNodeNum)
                    {
                        break;
                    }

                    foundNodeNum++;
                }
            }

        }
        else if(1 == tags.size())
        {
            if(index < lists.size())
            {
                retNode = lists.at(index);
            }
            else
            {
                retNode = lists.at(0);
            }

            foundNodeNum = lists.size();
        }
    }

    return retNode;
}


int QtXmlOperation::getNodeCount(QString nodeNames)
{
    QDomNode retNode;
    retNode.clear();
    int foundNodeNum = 0;

    if(!nodeNames.isEmpty())
    {
        // "\\W+", use any sequence of non-word characters as the separator
        QStringList tags = nodeNames.split(QRegExp("\\W+"), QString::SkipEmptyParts);
        QDomNodeList lists = m_doc->elementsByTagName(tags.at(0));
        QDomNode curretNode;

        if(tags.size() > 1)
        {
            for(int cnt = 0; cnt < lists.size(); cnt++)
            {
                curretNode = lists.at(cnt);

                for(int tagNum = 0; tagNum < tags.size() - 1; tagNum++)
                {
                    curretNode = findNode(curretNode, tags.at(tagNum + 1));

                    // Not Found node
                    if(curretNode.isNull())
                    {
                        break;
                    }
                }

                // Found node
                if(!curretNode.isNull())
                {
                    retNode = curretNode;
                    foundNodeNum++;
                }
            }

        }
        else if(1 == tags.size())
        {
            foundNodeNum = lists.size();
        }
    }

    return foundNodeNum;
}

QDomElement QtXmlOperation::getRootElement()
{
    QDomElement root = m_doc->documentElement();

    return root;
}

QDomNode QtXmlOperation::findNode(QDomNode parentNode, QString childNodeName, bool preciseMatch)
{
    QDomNode retNode;
    retNode.clear();

    if(!parentNode.isNull())
    {
        if(preciseMatch)
        {
            retNode = parentNode.namedItem(childNodeName);
        }
        else
        {
            QDomNodeList lists = parentNode.toElement().elementsByTagName(childNodeName);

            if(!lists.isEmpty())
            {
                retNode = lists.at(0);
            }
        }
    }

#ifdef ENABLE_DEBUG_TRACE_XML
    QString str;
    if(retNode.isNull())
    {
         str = QString("findNode() not found parentNode.tagName() =%1 childNodeName = %2")
                .arg(parentNode.toElement().tagName())
                .arg(childNodeName);
    }
    else
    {
        str = QString("findNode() found parentNode.tagName() =%1 childNodeName = %2")
                .arg(parentNode.toElement().tagName())
                .arg(childNodeName);
    }

    qDebug() << str;
#endif

    return retNode;
}
