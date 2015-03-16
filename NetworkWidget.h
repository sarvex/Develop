#ifndef NETWORK_WIDGET
#define NETWORK_WIDGET

#include "NetworkModeWidget.h"
#include "NetworkSelectionWidget.h"

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>

class NetworkWidget : public QWidget {
    Q_OBJECT
public:
     explicit NetworkWidget(QWidget *parent = 0);

signals:
    bool resetNetwork ();

public slots:

protected:

private:
    QTabWidget * tabs;

    NetworkModeWidget * networkModeWidget;
    NetworkSelectionWidget * networkSelectionWidget;

    QVBoxLayout * layout;
};

#endif // NETWORK_WIDGET
