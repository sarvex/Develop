#ifndef DEAMON_VOICE
#define DEAMON_VOICE

#include "SerialPort.h"

#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QFile>
#include <QObject>
#include <QTimer>

class DeamonVoice : public QObject {
	Q_OBJECT
public:
	static DeamonVoice* getInstance ();

signals:
	bool voicePortUpdate(QString name);

	bool modemPlugged ();
	bool modemUnplugged ();

public slots:
	bool checkPort ();

	bool setSpeakerVolume (int value);
	bool muteMicrophone ();
	bool muteSpeaker ();

	bool startRingBack ();
	bool stopRingBack ();
	bool startCall ();
	bool stopCall ();

	bool onPortReadyRead ();
	bool onPortDsrChanged (bool status);

	bool onAudioReadyRead ();
	bool onAudioDsrChanged (bool status);

	bool incomingFinished (QAudio::State state);
	bool outgoingFinished (QAudio::State state);

protected:

private:
	bool openPort ();
	bool closePort ();

	bool applyVolume (QByteArray& data);

	/// Meta Start
	static int search (int value, int size);

	unsigned char linearToLaw (int value);
	unsigned short lawToLinear (unsigned char value);

	QByteArray encode (QByteArray& input);
	QByteArray decode (QByteArray& input);

	/// Meta End

	explicit DeamonVoice(QObject *parent = 0);
	static DeamonVoice * singleton;

	QAudioOutput * incomingAudio;
	QAudioInput * outgoingAudio;

	QAudioDeviceInfo * defaultOutputDevice;
	QAudioDeviceInfo * defaultInputDevice;

	QIODevice * outputDevice;
	QIODevice * inputDevice;

	QString portName;
	SerialPort * port;

	QTimer * portTimer;

	bool call;
	bool ring;

	float speakerVolume;
	float micVolume;

	bool micMuted;
	bool speakerMuted;
};

#endif // DEAMON_VOICE
