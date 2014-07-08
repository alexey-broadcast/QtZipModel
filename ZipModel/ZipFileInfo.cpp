#include "ZipFileInfo.h"

ZipFileInfo::ZipFileInfo(const QuaZipFileInfo &f, ZipFileInfo* p):
    fileInfo(f),
    parent(p)
{
    mapped = !isDir();
    children.clear();
}

ZipFileInfo::~ZipFileInfo()
{
    children.clear();
}

ZipFileInfo &ZipFileInfo::operator=(const ZipFileInfo &another)
{
    this->fileInfo = another.fileInfo;
    this->mapped = another.mapped;
    this->parent = another.parent;
    this->children = another.children;
    return *this;
}

bool ZipFileInfo::operator ==(const ZipFileInfo &another) const
{
    if(this->children != another.children)
        return false;
    if(this->parent != another.parent)
        return false;
    if(this->fileInfo.name != another.fileInfo.name)
        return false;
    return true;
}

bool ZipFileInfo::isDir() const
{
    QString s = fileInfo.name;
    if(s.at(s.length()-1) == '/')
        return true;
    else
        return false;
}

QString ZipFileInfo::type() const
{
    if(isDir())
        return "Folder";
    else
    {
        QString s = fileInfo.name;
        return s.mid(s.lastIndexOf('.'));
    }
}

QString ZipFileInfo::size() const
{
    quint32 s = fileInfo.uncompressedSize;
    if(s < 1024)
        return QString("%1 Bytes").arg(s);
    if(s < 1024 * 1024)
        return QString("%1 KB").arg((double)s / 1024, 0, 'g', 4);
    if(s < 1024 * 1024 * 1024)
        return QString("%1 MB").arg((double)s / (1024 * 1024), 0, 'g', 4);
    return QString("%1 GB").arg((double)s / (1024 * 1024 * 1024), 0, 'g', 4);
}

int ZipFileInfo::childrenCount() const
{
    if(!isDir())
        return 0;
    else
        return children.size();
}

QString ZipFileInfo::filePath() const
{
    if(!parent)
        return fileInfo.name;

    return parent->filePath() + fileInfo.name;
}
