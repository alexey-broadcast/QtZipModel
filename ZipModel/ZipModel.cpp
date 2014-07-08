#include "ZipModel.h"

ZipModel::ZipModel(const QString& path, QObject *parent) :
    QAbstractItemModel(parent)
{
    zip = new QuaZip(path);
    fetchRootDirectory();
}

ZipModel::~ZipModel()
{
    delete zip;
}

QModelIndex ZipModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        return createIndex(row, column, const_cast<ZipFileInfo*>(&nodes[row]));

    ZipFileInfo* parentInfo = FileInfo(parent);
    return createIndex(row, column, &parentInfo->children[row]);
}

QModelIndex ZipModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    ZipFileInfo* childInfo = FileInfo(child);
    ZipFileInfo* parentInfo = childInfo->parent;
    if (parentInfo != 0)
        return createIndex(findRow(parentInfo), RamificationColumn, parentInfo);
    else
        return QModelIndex();
}

int ZipModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return nodes.size();

    return FileInfo(parent)->childrenCount();
}

int ZipModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return ColumnCount;
}

#include <QIcon>

QVariant ZipModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const ZipFileInfo* nodeInfo;
    if(index.parent().isValid())
        nodeInfo = FileInfo(index);
    else
        nodeInfo = &nodes.at(index.row());

    switch (index.column()) {
    case NameColumn:
        if(role == Qt::DecorationRole)
        {
            if(nodeInfo->isDir())
                return QIcon(":/icons/folderIcon");
            else
                return QIcon(":/icons/fileIcon");
        }
        else if(role == Qt::DisplayRole)
        {
            return nodeInfo->fileInfo.name;
        }
        break;
    case ModificationDateColumn:
        if (role == Qt::DisplayRole)
            return nodeInfo->fileInfo.dateTime;
        break;
    case SizeColumn:
        if (role == Qt::DisplayRole)
            return nodeInfo->isDir()? QVariant(): nodeInfo->size();
        break;
    case TypeColumn:
        if (role == Qt::DisplayRole)
            return nodeInfo->type();
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant ZipModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole &&
       section < ColumnCount)
    {
        QStringList headers;
        headers << "Name" << "Type" << "Size (Bytes)" << "DateModified";
        return headers[section];
    }
    return QVariant();
}

bool ZipModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    const ZipFileInfo* parentInfo = FileInfo(parent);
    return !parentInfo->mapped;
}

void ZipModel::fetchMore(const QModelIndex &parent)
{
    ZipFileInfo* parentInfo = FileInfo(parent);
    QuaZipDir dir(zip, parentInfo->filePath());
    QList<QuaZipFileInfo> children = dir.entryInfoList(QDir::AllEntries | QDir::NoDot,
                                                       QDir::Name);

    beginInsertRows(parent, 0, children.size() - 1);
    for(int i = 0; i < children.count(); ++i)
    {
        ZipFileInfo nodeInfo(children[i], parentInfo);
        nodeInfo.mapped = !nodeInfo.isDir();
        parentInfo->children.push_back(nodeInfo);
    }
    parentInfo->mapped = true;
    endInsertRows();
}

Qt::ItemFlags ZipModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags;
}

bool ZipModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        const ZipFileInfo* parentInfo = FileInfo(parent);
        if(!parentInfo->isDir())
            return false;
        if(!parentInfo->mapped)
            return true;
    }
    return QAbstractItemModel::hasChildren(parent);
}

int ZipModel::findRow(const ZipFileInfo *nodeInfo) const
{
    const QList<ZipFileInfo>& parentInfoChildren = nodeInfo->parent != 0 ?
                                                   nodeInfo->parent->children : nodes;
    QList<ZipFileInfo>::const_iterator position = qFind(parentInfoChildren, *nodeInfo);
    return std::distance(parentInfoChildren.begin(), position);
}

void ZipModel::fetchRootDirectory()
{
    if((zip->getMode() == QuaZip::mdNotOpen && zip->open(QuaZip::mdUnzip)) ||
       (zip->getMode() == QuaZip::mdUnzip))
    {
        QuaZipDir dir(zip);
        QList<QuaZipFileInfo> lst = dir.entryInfoList(QDir::AllEntries);
        nodes.clear();
        for(int i = 0; i < lst.count(); ++i)
            nodes.append(ZipFileInfo(lst[i]));
    }
}
