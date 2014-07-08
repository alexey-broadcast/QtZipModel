#include "widget.h"
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    pathToArchive = QDir::currentPath() + "/ZipModel.zip";
    model = new ZipModel(pathToArchive, this);

    view = new QTreeView(this);
    view->setModel(model);

    connect(view, SIGNAL(activated(QModelIndex)),
            this, SLOT(onItemActivated(QModelIndex)));

    QVBoxLayout* vl = new QVBoxLayout(this);
    vl->addWidget(view);
    setLayout(vl);
}

Widget::~Widget()
{
}

#include <QDebug>
#include "QuaZip/JlCompress.h"
#include <QDesktopServices>
#include <QUrl>

void Widget::onItemActivated(const QModelIndex &i) const
{
    ZipFileInfo* zipfileinfo = model->FileInfo(i);
    if(zipfileinfo->isDir())
        return;

    QString path = zipfileinfo->filePath();

    QString file = QDir::tempPath() + '/' + zipfileinfo->fileInfo.name;
    if(JlCompress::extractFile(pathToArchive, path, file).length() > 0)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    }
    else
    {
        qDebug() << "Get some error on extracting " << file;
    }
}
