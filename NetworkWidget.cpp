#include "NetworkWidget.h"

NetworkWidget::NetworkWidget (
        QWidget *parent)
    : QWidget(parent){

    this->tabs = new QTabWidget;

    this->networkModeWidget = new NetworkModeWidget;
    this->networkSelectionWidget = new NetworkSelectionWidget;

    this->tabs->addTab (this->networkModeWidget , "Network Mode");
    this->tabs->addTab (this->networkSelectionWidget, "Network Selection");

    this->layout = new QVBoxLayout;
    this->layout->setContentsMargins (0, 0, 0, 0);
    this->layout->addWidget (this->tabs);

    this->setLayout (this->layout);

    QObject::connect (this->networkModeWidget,
                      SIGNAL(resetNetwork ()),
                      this,
                      SIGNAL(resetNetwork()));
}
