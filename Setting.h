#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QTextStream>

class Setting {
public:
	Setting ();
	Setting (const QString& value, bool enable, int index = 0);
	Setting (const Setting& setting);

	Setting& operator = (const Setting& setting);
	bool operator == (const Setting& setting) const;

	QTextStream& operator << (QTextStream& textStream) const;
	QTextStream& operator >> (QTextStream& textStream);

	int getIndex () const;
	QString getValue () const;
	bool getEnable () const;

	bool setIndex (int index);
	bool setValue (const QString& value);
	bool setEnable (bool enable);

protected:

private:
	QString value;
	bool enable;

	int index;
};

#endif // SETTING_H
