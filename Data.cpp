#include "Data.h"
#include "Define.h"

Data::Data ()
{

    this->download = 0;
    this->upload = 0;
    this->start = QDateTime::currentDateTime ();
}

Data::Data (
           const Data& data)
: upload (data.upload),
download (data.download),
start (data.start),
index (data.index)
{
}

Data::Data (
           double upload,
           double download,
           const QDateTime& start,
           int index)
: upload (upload),
download (download),
start (start),
index (index)
{
}

Data&
Data::operator = (
                 const Data& data) {

    if (this == &data) return (*this);

    this->upload = data.upload;
    this->download = data.download;
    this->start = data.start;
    this->index = data.index;

    return (*this);
}

bool
Data::operator == (
                  const Data& data) const
{

    if (this == &data) return (true);

    if (this->upload != data.upload) return (false);
    if (this->download != data.download) return (false);
    if (this->start != data.start) return (false);
    if (this->index != data.index) return (false);

    return (true);
}

QTextStream&
Data::operator << (
                  QTextStream& textStream) const
{

    textStream << this->upload;
    textStream << this->download;
    textStream << this->start.toString (DATETIME_FORMAT);
    textStream << this->index;

    return (textStream);
}

QTextStream&
Data::operator >> (
                  QTextStream& textStream)
{

    textStream >> this->upload;
    textStream >> this->download;

    QString start;
    textStream >> start;
    this->start.fromString (start, DATETIME_FORMAT);

    textStream >> index;

    return (textStream);
}

bool
Data::addUpload (
                double upload)
{

    this->upload += upload;

    return (true);
}

bool
Data::addDownload (
                  double download)
{

    this->download += download;

    return (true);
}

double
Data::getUpload () const
{

    return (this->upload);
}

double
Data::getDownload () const
{

    return (this->download);
}

QDateTime
Data::getStart () const
{

    return (this->start);
}

int
Data::getIndex () const
{

    return (this->index);
}

bool
Data::setUpload (
                double upload)
{

    this->upload = upload;
    return (true);
}

bool
Data::setDownload (
                  double download)
{

    this->download = download;
    return (true);
}

bool
Data::setStart (
               const QDateTime& start)
{

    this->start = start;
    return (true);
}

bool
Data::setIndex (int index)
{

    this->index = index;
    return (true);
}
