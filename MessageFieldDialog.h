#ifndef MESSAGE_FIELD_DIALOG
#define MESSAGE_FIELD_DIALOG

#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>

class MessageFieldDialog : public QDialog {
    Q_OBJECT
public:
    explicit MessageFieldDialog(QWidget *parent = 0);
    QList<bool> getFields ();

signals:

public slots:
    bool onOkClicked ();
    bool onCancelClicked ();

protected:

private:
    QListWidget * list;

    QDialogButtonBox * buttons;

    QVBoxLayout * layout;

    QCheckBox * name;
    QCheckBox * number;
    QCheckBox * content;
    QCheckBox * time;
    QCheckBox * type;

    QListWidgetItem * nameItem;
    QListWidgetItem * numberItem;
    QListWidgetItem * contentItem;
    QListWidgetItem * timeItem;
    QListWidgetItem * typeItem;
};

#endif // MESSAGE_FIELD_DIALOG
