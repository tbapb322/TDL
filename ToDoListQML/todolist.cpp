#include <QFile>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "todolist.h"

ToDoList::ToDoList(QObject *parent) : QObject(parent)
{
}

QVector<ToDoItem> ToDoList::items() const
{
    return m_items;
}

bool ToDoList::setItemAt(int index, const ToDoItem &item)
{
    if(index < 0 || index >= m_items.size())
        return false;

    m_items[index] = item;
    return true;
}

bool ToDoList::setItemDetails(int index, const QString &details)
{
    if(index < 0 || index >= m_items.size())
        return false;
    m_items[index].itemDetails = details;
    return true;
}

bool ToDoList::saveList()
{
    QString filename = "Save.txt";
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite))
    {
        file.resize(0);
        QJsonArray itemsArray;
        Q_FOREACH(const auto& item, m_items)
        {
            QJsonObject jsonItem;
            jsonItem["description"] = item.itemDescription;
            jsonItem["completed"] = item.itemComplete;
            jsonItem["details"] = item.itemDetails;
            itemsArray.push_back(jsonItem);
        }
        QJsonObject jsonFile;
        jsonFile["items"] = itemsArray;
        file.write(QJsonDocument(jsonFile).toJson(QJsonDocument::Indented));
    }
    file.close();
    return true;
}

bool ToDoList::loadList()
{
    m_items.clear();

    QString filename = "Save.txt";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray saveData = file.readAll();
        QJsonDocument jsonDoc(QJsonDocument::fromJson(saveData));
        QJsonArray itemsArr = jsonDoc.object()["items"].toArray();
        Q_FOREACH(const auto& item, itemsArr)
        {
            m_items.push_back({item["completed"].toBool(), QString(item["description"].toString()), QString(item["details"].toString())});
        }
    }
    return true;
}

void ToDoList::addItem(QString itemDescription)
{
    if(itemDescription == "")
        return;

    emit preAddItem();

    ToDoItem item;
    item.itemComplete = false;
    item.itemDescription = itemDescription;
    item.itemDetails = "";
    m_items.push_back(item);

    emit postAddItem();
}

void ToDoList::removeCompletedItems()
{
    for(int index = 0; index<m_items.size(); ++index)
    {
        if(m_items[index].itemComplete == true)
        {
            emit preRemoveItem(index);

            m_items.removeAt(index);
            --index;

            emit postRemoveItem();
        }
    }
}
