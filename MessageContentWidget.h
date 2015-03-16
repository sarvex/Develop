#ifndef MESSAGECONTENTWIDGET_H
#define MESSAGECONTENTWIDGET_H

#include "Message.h"

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTextEdit>

class MessageContentWidget : public QScrollArea {
    Q_OBJECT
public:
    explicit MessageContentWidget(QWidget *parent = 0);

    bool load (const Message& message);

    bool clear ();

signals:

public slots:

protected:

private:
    QLabel * nameLabel;
    QLabel * numberLabel;
    QLabel * timeLabel;
    QLabel * dateLabel;


    QLineEdit * name;
    QLineEdit * number;
    QTextEdit * content;
    QLineEdit * time;
    QLineEdit * date;

    QGridLayout * layout;
};

#endif // MESSlayoutWIDGET_H
