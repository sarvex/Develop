#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool checkRegistry ();

    bool runSetup ();
    bool runApplication ();

    bool checkModem ();
    bool eject ();

    bool installDriver ();

    QString installDirectory;
    bool amd;
};

#endif // MAINWINDOW_H
