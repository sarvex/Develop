#ifndef SPEED_LABEL
#define SPEED_LABEL

#include <QLabel>

class SpeedLabel
		: public QLabel{
	Q_OBJECT

public:
	explicit SpeedLabel (QWidget *parent = 0);

signals:

public slots:
	bool onSpeedUpdate (double upload, double download);

	bool onModemConnected (bool status);
	bool onModemPlugged (bool status);

protected:
	virtual void paintEvent (QPaintEvent * event);

private:
};

#endif // SPEED_LABEL
