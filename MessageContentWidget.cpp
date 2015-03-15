#include "Define.h"
#include "MessageContentWidget.h"

MessageContentWidget::MessageContentWidget (
        QWidget *parent)
    : QScrollArea (parent) {

    this->setWidgetResizable (false);

    this->nameLabel = new QLabel("Name");
    this->nameLabel->setAlignment (Qt::AlignRight);
    this->nameLabel->setToolTip ("Name of Sender");

    this->name = new QLineEdit;
    this->name->setPlaceholderText ("Name...");
    this->name->setReadOnly (true);

    this->numberLabel = new QLabel("Number");
    this->numberLabel->setAlignment (Qt::AlignRight);
    this->numberLabel->setToolTip ("Number of Sender");

    this->number = new QLineEdit;
    this->number->setPlaceholderText ("Number...");
    this->number->setReadOnly (true);

    this->content = new QTextEdit;
    this->content->setReadOnly (true);

    this->timeLabel = new QLabel("Time");
    this->timeLabel->setAlignment (Qt::AlignRight);
    this->timeLabel->setToolTip ("Time of Message");

    this->time = new QLineEdit;
    this->time->setPlaceholderText ("Time...");
    this->time->setReadOnly (true);

    this->dateLabel = new QLabel("Date");
    this->dateLabel->setAlignment (Qt::AlignRight);
    this->dateLabel->setToolTip ("Date of Message");

    this->date = new QLineEdit;
    this->date->setPlaceholderText ("Date...");
    this->date->setReadOnly (true);

    this->layout = new QGridLayout;
    this->layout->addWidget (this->nameLabel, 0, 0, 1, 1);
    this->layout->addWidget (this->name, 0, 1, 1, 1);
    this->layout->addWidget (this->numberLabel, 0, 2, 1, 1);
    this->layout->addWidget (this->number, 0, 3, 1, 1);
    this->layout->addWidget (this->content, 1, 0, 2, 4);
    this->layout->addWidget (this->dateLabel, 3, 0, 1, 1);
    this->layout->addWidget (this->date, 3, 1, 1, 1);
    this->layout->addWidget (this->timeLabel, 3, 2, 1, 1);
    this->layout->addWidget (this->time, 3, 3, 1, 1);

    this->setLayout (this->layout);
}

bool
MessageContentWidget::clear () {

    this->name->clear ();
    this->name->setToolTip ("Name");

    this->number->clear ();
    this->number->setToolTip ("Number");

    this->content->clear ();
    this->content->setToolTip ("Content");

    this->time->clear ();
    this->time->setToolTip ("Time");

    this->date->clear ();
    this->date->setToolTip ("Date");

    return true;
}

bool
MessageContentWidget::load (
        const Message& message) {

    if (message.getName().contains (message.getNumber ())) {

        this->name->clear ();
        this->name->setToolTip ("No Contact Entry");

    } else {

        this->name->setText (message.getName ());
        this->name->setToolTip (message.getName ());
    }

    this->number->setText (message.getNumber ());
    this->number->setToolTip (message.getNumber ());

    this->content->setText (message.getContent ());
    this->content->setToolTip (message.getContent ());

    this->date->setText (message.getTime ().date ().toString (DATE_FORMAT));
    this->date->setToolTip (message.getTime ().date ().toString (DATE_FORMAT));

    this->time->setText (message.getTime ().time ().toString (TIME_FORMAT));
    this->time->setToolTip (message.getTime ().time ().toString (TIME_FORMAT));

    return true;
}
