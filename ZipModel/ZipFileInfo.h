/* ZipModel using Qt5 and QuaZip.
* Copyright (C) 2014 Alexey Lebedev alexey.broadcast@gmail.com
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/
#ifndef ZIPFILEINFO_H
#define ZIPFILEINFO_H

#include "QuaZip/quazip.h"
#include "QuaZip/quazipdir.h"
#include "QuaZip/quazipfile.h"

class ZipFileInfo
{
public:
    ZipFileInfo(const QuaZipFileInfo& f, ZipFileInfo* p = 0);
    ~ZipFileInfo();
    ZipFileInfo& operator= (const ZipFileInfo& another);
    bool operator== (const ZipFileInfo& another) const;

    bool mapped;//true if we have read children for this node
    QuaZipFileInfo fileInfo;
    ZipFileInfo* parent;
    QList<ZipFileInfo> children;

    bool isDir() const;
    QString type() const;//file extension
    QString size() const;//for display
    int childrenCount() const;

    QString filePath() const;//full path from archive root
private:
};

#endif // ZIPFILEINFO_H
