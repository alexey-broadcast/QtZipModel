#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeView>
#include <ZipModel.h>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QTreeView* view;
    QString pathToArchive;
    ZipModel* model;

private slots:
    void onItemActivated(const QModelIndex&) const;//let's open some file
};

#endif // WIDGET_H
