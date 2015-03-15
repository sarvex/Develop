#ifndef TIME_LABEL
#define TIME_LABEL

#include <QLabel>
#include <QTime>
#include <QTimer>

class TimeLabel : public QLabel {
	Q_OBJECT

public:
	explicit TimeLabel (QWidget *parent = 0);

signals:

public slots:
	bool onModemConnected (bool status);
	bool onModemPlugged (bool status);

	bool onTimer ();

protected:
	virtual void paintEvent (QPaintEvent * event);

private:

	QTime duration;
	QTime start;

	QTimer * timer;
};

#endif // TIME_LABEL

