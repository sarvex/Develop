#ifndef DATA_H
#define DATA_H

#include <QDateTime>
#include <QTextStream>

class Data
{

public:
    Data ();
    Data (const Data& data);
    Data (double upload,
          double download,
          const QDateTime& start,
          int index = -1);

    Data& operator = (const Data& data);
    bool operator == (const Data& data) const;

    QTextStream& operator << (QTextStream& textStream) const;
    QTextStream& operator >> (QTextStream& textStream);

    bool addUpload (double upload);
    bool addDownload (double download);

    double getUpload () const;
    double getDownload () const;
    QDateTime getStart () const;
    int getIndex () const;

    bool setUpload (double upload);
    bool setDownload (double download);
    bool setStart (const QDateTime& start);
    bool setIndex (int index);

protected:

private:
    double upload;
    double download;

    QDateTime start;
    int index;
};

#endif // DATA_H
