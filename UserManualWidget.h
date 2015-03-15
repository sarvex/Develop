#ifndef USER_MANUAL_WIDGET
#define USER_MANUAL_WIDGET

#include <QListWidget>
#include <QListWidgetItem>
#include <QSplitter>
#include <QStringList>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class UserManualWidget
		: public QWidget {
	Q_OBJECT

public:
	explicit UserManualWidget (QWidget *parent = 0);

signals:

public slots:
	bool onCurrentRowChanged (int index);

protected:

private:
	bool loadManual();

	QListWidget * list;
	QListWidgetItem * items;

	QTextEdit * content;
	
	QSplitter * splitter;
	QVBoxLayout * layout;

	QStringList manual;
};

#endif // USER_MANUAL_WIDGET
