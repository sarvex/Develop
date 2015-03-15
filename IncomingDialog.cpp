#include "IncomingDialog.h"
#include "DeamonInterface.h"
#include "Define.h"

#include <QIcon>
#include <QApplication>
#include <QDesktopWidget>

IncomingDialog::IncomingDialog (
        const QString& title,
        const QString& text,
        QWidget *parent)
    : QDialog(parent) {

    this->setWindowFlags (Qt::WindowSystemMenuHint
                          | Qt::WindowStaysOnTopHint
                          | Qt::WindowTitleHint
                          | Qt::Tool);

    this->setWindowIcon (QIcon (APPLICATION_ICON));
    this->setWindowTitle (title);

    this->label = new QLabel (text);

    this->buttons = new QDialogButtonBox (
                        QDialogButtonBox::Ok
                        |QDialogButtonBox::Cancel);

    this->layout = new QVBoxLayout;
    this->layout->addWidget (this->label);
    this->layout->addWidget (this->buttons);

    this->setLayout (this->layout);

    QObject::connect (this->buttons,
                      SIGNAL(accepted()),
                      this,
                      SLOT(onOkClicked()));

    QObject::connect (this->buttons,
                      SIGNAL(rejected()),
                      this,
                      SLOT(onCancelClicked()));

    QObject::connect (DeamonInterface::getInstance (),
                      SIGNAL(callEndUpdate()),
                      this,
                      SLOT(onCallEndUpdate()));

    this->ringtone = new QFile(":/aircel.wav");
    this->ringtone->open (QIODevice::ReadOnly);

    QAudioFormat format;
    format.setSampleRate (8000);
    format.setChannelCount (1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo audioInformation(QAudioDeviceInfo::defaultOutputDevice());
    if (false == audioInformation.isFormatSupported(format)) {
        format = audioInformation.nearestFormat (format);
    }

    this->audio = new QAudioOutput(format, this);

    QObject::connect(this->audio,
                     SIGNAL(stateChanged(QAudio::State)),
                     this,
                     SLOT(onRingFinish(QAudio::State)));
    this->audio->start(this->ringtone);
}

void
IncomingDialog::showEvent (
        QShowEvent * event) {
    Q_UNUSED (event);

    QRect desktop = QApplication::desktop ()->availableGeometry ();
    QRect dialog = this->frameGeometry ();

    this->move ((desktop.width () - dialog.width ()),
                (desktop.height () - dialog.height ()));
}

bool
IncomingDialog::onCallEndUpdate () {

    this->done (QDialog::Rejected);

    return true;
}

bool
IncomingDialog::onCancelClicked () {

    this->done (QDialog::Rejected);

    return true;
}

bool
IncomingDialog::onOkClicked () {

    this->done (QDialog::Accepted);

    return true;
}

bool
IncomingDialog::onRingFinish (QAudio::State state) {
    switch (state) {

        case QAudio::ActiveState: {
        } break;

        case QAudio::IdleState:{
        } break;

        case QAudio::StoppedState:{
            this->audio->stop ();
            this->ringtone->close ();
        } break;

        case QAudio::SuspendedState:{
        } break;
    }

    return true;
}
