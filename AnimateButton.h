#ifndef ANIMATE_BUTTON
#define ANIMATE_BUTTON

#include <QImage>
#include <QList>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QTimer>

class AnimateButton
: public QPushButton
{
    Q_OBJECT

public:
    AnimateButton (const QString& first,
                   const QString& second,
                   const QString& third,
                   const QString& fourth,
                   const QString& fifth,
                   const QString& sixth,
                   QWidget *parent = 0);

    AnimateButton (const QStringList& buttons,
                   QWidget *parent = 0);

    bool setImages (const QStringList& buttons);

    signals:

public slots:

protected:
    virtual void paintEvent (QPaintEvent * event);
    virtual void hideEvent (QHideEvent * event);
    virtual void showEvent (QShowEvent * event);

private:
    bool initialize();

    QList<QImage> imageList;

    QTime startTime;
    QTimer * timer;
};

#endif // ANIMATE_BUTTON
