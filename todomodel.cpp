#include "todomodel.h"
#include "todolist.h"

ToDoModel::ToDoModel(QObject *parent)
    : QAbstractListModel(parent),
      m_list(nullptr)
{
}

int ToDoModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
        return 0;

    // FIXME: Implement me!
    return m_list->items().size();
}

QVariant ToDoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_list)
        return QVariant();

    ToDoItem item = m_list->items().at(index.row());

    switch (role) {
        case CompleteRole:
            return QVariant(item.itemComplete);
        case DescriptionRole:
            return QVariant(item.itemDescription);
        case DetailsRole:
            return QVariant(item.itemDetails);
    }
    return QVariant();
}

bool ToDoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_list)
        return false;

    ToDoItem item = m_list->items().at(index.row());

    switch(role)
    {
        case CompleteRole:
            item.itemComplete = value.toBool();
            break;
        case DescriptionRole:
            item.itemDescription = value.toString();
            break;
        case DetailsRole:
            item.itemDetails = value.toString();
            break;
    }

    if (m_list->setItemAt(index.row(), item)) {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ToDoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

QHash<int, QByteArray> ToDoModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DescriptionRole] = "description";
    names[CompleteRole] = "complete";
    names[DetailsRole] = "details";
    return names;
}

ToDoList *ToDoModel::list() const
{
    return m_list;
}

void ToDoModel::setList(ToDoList *list)
{
    beginResetModel();

    if(m_list)
        m_list->disconnect(this);

    m_list = list;

    if(m_list)
    {
        connect(m_list, &ToDoList::preAddItem, this, [=](){
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });
        connect(m_list, &ToDoList::postAddItem, this, [=](){
            endInsertRows();
        });
        connect(m_list, &ToDoList::preRemoveItem, this, [=](int index){
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(m_list, &ToDoList::postRemoveItem, this, [=](){
            endRemoveRows();
        });
    }

    endResetModel();
}
