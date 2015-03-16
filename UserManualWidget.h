#ifndef USER_MANUAL_WIDGET
#define USER_MANUAL_WIDGET

#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QSplitter>
#include <QtCore/QStringList>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
