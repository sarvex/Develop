#ifndef MESSAGE_WIDGET
#define MESSAGE_WIDGET

#include "Message.h"
#include "MessageContentWidget.h"

#include <QAction>
#include <QMenu>
#include <QPoint>
#include <QSplitter>
#include <QTableWidget>
#include <QTextEdit>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>


class MessageWidget : public QWidget {
	Q_OBJECT
public:
	explicit MessageWidget(QWidget *parent = 0);
	bool updateTree ();

signals:
	bool readContactUpdate (bool enable);
	bool messageAnnunciator (bool);
	bool number (QString number);
	bool save (QString number);
	bool dial (QString number,
			   bool connect);

public slots:
	bool onSelectUnread ();
	bool send (QString number,
			   QString content);

	bool name (QString number,
			   QString name);

	bool onMessageCountUpdate (QString count, QString total);
	bool onWriteMessageUpdate (QString index);
	bool onDeliveryReportSimUpdate (QString index);
	bool onMessageReceivedSimUpdate (QString index);

	bool addUpdate (Message message);
	bool add (Message message);

	bool onTreeCurrentItemChanged (QTreeWidgetItem* current,
								   QTreeWidgetItem* previous);
	bool onTableCurrentItemChanged (QTableWidgetItem* current,
									QTableWidgetItem* previous);

	bool onContextMenu (const QPoint&);

	bool onSearchTextChanged (QString filter);

	bool onNewTriggered();
	bool onEditTriggered();
	bool onImportantTriggered ();
	bool onDeleteTriggered();
	bool onRecoverTriggered();
	bool onSendTriggered();
	bool onCallTriggered();
	bool onSaveTriggered();
	bool onReplyTriggered();
	bool onForwardTriggered();
	bool onFindTriggered();
	bool onExportTriggered();

	bool onMoveToApplicationTriggered ();
	bool onMoveToSimTriggered ();
	bool onCopyToApplicationTriggered ();
	bool onCopyToSimTriggered ();

	bool onModemPlugged (bool status);
	bool onModemConnected (bool status);

protected:

private:
	bool createActions ();
	bool createToolbar ();
	bool createContextMenu ();

	bool createTree ();
	bool createTable ();
	bool createLayout ();

	bool initialize ();
	bool load ();
	bool connect ();

	bool edit (const Message& message);
	bool remove (int index);
	bool recover (int index);

	bool messageToRow (Message message);
	Message rowToMessage (int index);

	bool updateTable ();

	bool moveToApplication (int index);
	bool moveToSim (int index);

	bool copyToApplication (int index);
	bool copyToSim (int index);

	QString write (const QList<bool>& fields, int row);
	bool read (const QList<bool>& fields, const QString& string);

	QTreeWidget * tree;
	QList <QTreeWidgetItem *> item;

	QToolBar * toolbar;
	QTableWidget * table;

	MessageContentWidget * content;

	QMenu * menu;
	QList<QAction *> action;
	QList<QAction *> separator;

	QLineEdit * search;
	QAction * searchAction;

	QSplitter * splitter;
	QSplitter * reader;
	QVBoxLayout * layout;

	QList<QIcon> treeIcon;

	QStringList actionName;
	QStringList actionToolTip;
	QList<QIcon> actionIcon;
	QList<QKeySequence> actionShortcut;

	class Action {
	public:
		enum {
			New,
			Edit,
			Important,
			Delete,
			Recover,
			Send,
			Reply,
			Forward,
			Call,
			Save,
			Find,
			Export,
			MoveToApplication,
			MoveToSim,
			CopyToApplication,
			CopyToSim,
			Count
		};
	};

	class Separator {
	public:
		enum {
			One,
			Two,
			Three,
			Four,
			Five,
			Count
		};
	};
};

#endif // MSSAGE_WIDGET
