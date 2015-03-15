﻿#ifndef INCOMING_DIALOG
#define INCOMING_DIALOG

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QLabel>
#include <QVBoxLayout>

class IncomingDialog 
	: public QDialog {
	Q_OBJECT
	
public:
	explicit IncomingDialog (const QString& title, const QString& text, QWidget *parent = 0);

signals:

public slots:
	bool onOkClicked ();
	bool onCancelClicked ();

	bool onCallEndUpdate ();
	bool onRingFinish(QAudio::State state);

protected:
	virtual void showEvent (QShowEvent * event);

private:
	QLabel * label;

	QDialogButtonBox * buttons;

	QVBoxLayout * layout;

	QFile * ringtone;
	QAudioOutput * audio;
};

#endif // INCOMING_DIALOG
