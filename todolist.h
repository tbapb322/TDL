#ifndef TODOLIST_H
#define TODOLIST_H

#include <QObject>
#include <QVector>

struct ToDoItem
{
    bool itemComplete;
    QString itemDescription;
    QString itemDetails;
};

class ToDoList : public QObject
{
    Q_OBJECT
public:

    explicit ToDoList(QObject *parent = nullptr);

    QVector<ToDoItem> items() const;

    bool setItemAt(int index, const ToDoItem &item);

signals:
    void preAddItem();
    void postAddItem();

    void preRemoveItem(int index);
    void postRemoveItem();

public slots:
    bool saveList();
    bool loadList();
    void addItem(QString itemDescription);
    bool setItemDetails(int index, const QString& details);
    void removeCompletedItems();

private:
    QVector<ToDoItem> m_items;
};

#endif // TODOLIST_H
