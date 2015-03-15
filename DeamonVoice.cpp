#include "SerialEnumerator.h"

#include "Define.h"
#include "DeamonInterface.h"
#include "DeamonVoice.h"

#include <QDebug>
#include <limits>
#include <QWidget>

using namespace std;
/// Meta Start

#define SIGN_BIT (0x80)
#define QUANT_MASK (0xF)
#define NSEGS  (8)
#define SEG_SHIFT (4)
#define SEG_MASK (0x70)
#define BIAS  (0x84)


int
DeamonVoice::search (
		int value,
		int size) {

	static short segment[8] = { 0xFF, 0x1FF, 0x3FF, 0x7FF,
								0xFFF, 0x1FFF, 0x3FFF, 0x7FFF };
	static short * table = segment;

	for (int index = 0; index < size; index++) {

		if (value <= *table++) {

			return index;
		}
	}

	return size;
}

//unsigned char DeamonVoice::linearToLaw (int value) {
//    int mask = 0;

//    if ( 0 >= value ) {
//        value = BIAS - value;
//        mask = 0x7F;
//    } else {
//        value = BIAS + value;
//        mask = 0xFF;
//    }

//    int segment = this->search (value, 8);

//    if (8 <= segment) {
//        return (0x7F ^ mask);
//    } else {
//        int temp = (segment << 4) | ((value >> (segment + 3)) & 0xF);
//        return (temp ^ mask);
//    }
//}


unsigned char
DeamonVoice::linearToLaw (
		int value) {

	int sign = (value & 0x8000) >> 8;

	if ( 0 != sign) value = -value;

	if (value > 32635) value = 32635;

	value += 0x84;

	int exponent = 7;

	for (int exponentMask = 0x4000;
		 (value & exponentMask) == 0;
		 exponent--, exponentMask >>= 1) {
	}

	int mantissa = (value >> (exponent + 3)) & 0x0F;

	byte mulaw = (byte) (sign | (exponent << 4) | mantissa);

	return (byte)~mulaw;

}

unsigned short
DeamonVoice::lawToLinear (
		unsigned char value) {

	value = ~value;

	int temp = ((value & QUANT_MASK) << 3) + BIAS;

	temp <<= ((unsigned)value & SEG_MASK) >> SEG_SHIFT;

	return ((value & SIGN_BIT) ? (BIAS - temp) : (temp - BIAS));
}

QByteArray DeamonVoice::encode (QByteArray& input) {

	qDebug () << "Entering DeamonVoice::encode";

	QByteArray output;
	output.resize (input.size () / 2);

	int data = 0;

	for (int index = 0; index < input.size (); index += 2) {

		data = input.at (index+1);
		data = data << 8;
		data = data | input.at (index);

		output [index/2] = linearToLaw (data);
	}

	qDebug () << "Exiting DeamonVoice::encode" << output;
	return output;
}

QByteArray DeamonVoice::decode (QByteArray& input) {

	qDebug () << "Entering DeamonVoice::decode" << input;

	QByteArray output;
	output.resize (input.size () * 2);

	unsigned short data = 0;

	for (int index = 0; index < input.size (); index++) {

		data = this->lawToLinear (input[index]);

		output[(index*2)+1] = (data & 0xFF00) >> 8;
		output[(index*2)] = (data & 0xFF);
	}

	qDebug () << "Exiting DeamonVoice::decode success";
	return output;
}

/// Meta End

DeamonVoice* DeamonVoice::singleton = NULL;

DeamonVoice*
DeamonVoice::getInstance () {

	if (NULL == singleton) {

		singleton = new DeamonVoice();
	}

	return (singleton);
}

DeamonVoice::DeamonVoice (
		QObject *parent)
	: QObject(parent),
	  outputDevice (NULL),
	  inputDevice (NULL),
	  port (NULL),
	  call (false),
	  ring (false),
	  speakerVolume (1.0),
	  micVolume (1.0),
	  micMuted (false),
	  speakerMuted (false) {

	QAudioFormat format;
	format.setSampleRate (8000);
	format.setChannelCount (1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	this->defaultOutputDevice = new QAudioDeviceInfo (
									QAudioDeviceInfo::defaultOutputDevice ());

	if (false == this->defaultOutputDevice->isFormatSupported (format)) {

		qDebug () << "Default Output Device does not support Format";
		format = this->defaultOutputDevice->nearestFormat (format);
	}

	this->incomingAudio = new QAudioOutput(format, this);

	format.setSampleRate (8000);
	format.setChannelCount (1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	this->defaultInputDevice = new  QAudioDeviceInfo (
								   QAudioDeviceInfo::defaultInputDevice ());

	if (false == this->defaultInputDevice->isFormatSupported (format)) {

		qDebug () << "Default Input Device does not support Format";
		format = this->defaultInputDevice->nearestFormat (format);
	}

	this->outgoingAudio = new QAudioInput (format, this);

	this->checkPort ();

	QObject::connect (this->outgoingAudio,
					  SIGNAL(stateChanged(QAudio::State)),
					  this,
					  SLOT(outgoingFinished(QAudio::State)));

	QObject::connect (this->incomingAudio,
					  SIGNAL(stateChanged(QAudio::State)),
					  this,
					  SLOT(incomingFinished(QAudio::State)));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(ringBackUpdate()),
					  this,
					  SLOT(startRingBack()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(stopRingUpdate()),
					  this,
					  SLOT(stopRingBack()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(callAcceptUpdate()),
					  this,
					  SLOT(startCall()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(callAnswerUpdate()),
					  this,
					  SLOT(startCall()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(callEndUpdate()),
					  this,
					  SLOT(stopCall()));
}

bool
DeamonVoice::checkPort () {

	bool result = false;
	this->portName = "";

	QList<PortInformation> ports = SerialEnumerator::getPorts ();

	foreach (PortInformation port, ports) {

		if (VENDOR_ID == port.vendorID) {

			if (PRODUCT_ID == port.productID) {

				if (port.friendName.contains (VOICE_PORT_NAME)) {

					this->portName = port.portName.remove ("\\\\.\\");
					emit voicePortUpdate (this->portName);

					result = true;
					break;
				}
			}
		}
	}

	if ( true == result ) {

		emit modemPlugged ();

	} else {

		emit modemUnplugged ();
	}

	return result;
}

bool
DeamonVoice::openPort() {

	qDebug () << "Entering DeamonVoice::openPort()";

	bool result = false;

	if (true == this->portName.isEmpty ()) {

		if ( NULL != this->port) {

			delete this->port;
			this->port = NULL;
		}

		result = false;

	} else {

		if ( NULL == this->port) {

			this->port = new SerialPort(portName, SerialPort::EventDriven);

			if ( true == this->port->open (QIODevice::ReadWrite) ) {

				qDebug () << "Successfully opened Voice Port" << this->portName;

				this->port->setBaudRate (BAUD115200);
				this->port->setDataBits (DATA_8);
				this->port->setParity (PAR_NONE);
				this->port->setStopBits (STOP_1);
				this->port->setFlowControl (FLOW_HARDWARE);

				QObject::connect(this->port,
								 SIGNAL(readyRead()),
								 this,
								 SLOT(onPortReadyRead()));

				result = true;

			} else {

				result = false;
			}

		} else {

			result = true;
		}
	}

	qDebug () << "Exiting DeamonVoice::openPort()";
	return result;
}

bool
DeamonVoice::onPortReadyRead () {

	QByteArray input;
	QByteArray output;

	int size = this->port->bytesAvailable();
	qDebug () << "Port Available" << size;

	input.resize (size);

	int read = this->port->read(input.data (), size);
	qDebug() << "Port read:" << read;

	if (true == this->speakerMuted) return false;

	output.resize (read*2);

	int data = 0;

	for (int index = 0; index < read; index++) {

		data = this->lawToLinear (input[index]);

		output[(index*2)+1] = (data & 0xFF00) >> 8;
		output[(index*2)] = (data & 0xFF);
	}

	this->applyVolume (output);

	if ( NULL != this->outputDevice) {
		int write = this->outputDevice->write (output.data (), read*2);
		qDebug() << "Speaker written:" << write;

		this->outputDevice->waitForBytesWritten (-1);
	}

	return true;
}

bool DeamonVoice::onPortDsrChanged (bool) {

	return true;
}

bool DeamonVoice::onAudioReadyRead () {

	QByteArray input;
	QByteArray output;

	int size = this->outgoingAudio->bytesReady ();
	qDebug () << "Mic Available" << size;

	input.resize (size);

	int read = this->inputDevice->read(input.data (), size);
	qDebug() << "Mic read:" << read;

	if (true == this->micMuted) return false;

	output.resize (read/2);

	int value = 0;

	for (int index = 0; index < read; index += 2) {

		value = 0;
		value = (((input[index+1]) << 8) | (input[index]));

		output [index/2] = linearToLaw (value);
	}

	int write = this->port->write (output.data (), read/2);

	this->port->waitForBytesWritten (-1);

	qDebug() << "Port written:" << write << this->port->bytesToWrite ();

	return true;
}

bool
DeamonVoice::onAudioDsrChanged (bool) {

	return true;
}

bool
DeamonVoice::closePort () {

	this->port->close ();
	QObject::disconnect(this->port,
						SIGNAL(readyRead()),
						this,
						SLOT(onPortReadyRead()));

	if (NULL != this->port) {

		delete this->port;
		this->port = NULL;
	}

	return true;
}

bool
DeamonVoice::applyVolume (
		QByteArray& input) {

	qint16* data = reinterpret_cast <qint16*> (input.data());

	qint32 temp;

	for(int index = 0; index < input.size()/2 ; index++) {

		temp = qint32 ( float(data[index]) * this->speakerVolume);

		if(temp > MAX_16_BIT) {

			temp = MAX_16_BIT;

		} else if (temp < MIN_16_BIT) {

			temp = MIN_16_BIT;
		}

		data[index]=temp;
	}

	return true;
}

bool
DeamonVoice::startRingBack () {

	qDebug () << "Entering DeamonVoice::startRingBack";

	if (false == this->ring) {

		this->ring = true;

		if (false == this->openPort ()) {

			qDebug () << "Unable to open Voice Port";
			return false;
		}

		this->outputDevice = this->incomingAudio->start ();

		return true;

	} else {

		return false;
	}

	qDebug () << "Exiting DeamonVoice::startRingBack";
}

bool
DeamonVoice:: stopRingBack () {

	if (true == this->ring) {

		this->ring = false;
		this->incomingAudio->stop ();
		this->closePort ();

		return true;

	} else {

		return false;
	}
}

bool
DeamonVoice::incomingFinished (
		QAudio::State state) {

	switch (state) {

		case QAudio::ActiveState: {
		} break;

		case QAudio::IdleState:{
		} break;

		case QAudio::StoppedState:{
			if (true == this->ring) {

				this->ring = false;
				this->incomingAudio->stop ();
				this->closePort ();

				return true;
			}
		} break;

		case QAudio::SuspendedState:{
		} break;
	}

	return true;
}

bool
DeamonVoice::outgoingFinished (
		QAudio::State state) {

	switch (state) {

		case QAudio::ActiveState: {
		} break;

		case QAudio::IdleState:{
		} break;

		case QAudio::StoppedState:{
		} break;

		case QAudio::SuspendedState:{
		} break;
	}

	return true;
}

bool
DeamonVoice::startCall () {

	if (false == this->call) {
		this->call = true;

		if (false == this->ring) {

			if (false == this->openPort ()) {

				return false;
			}

			this->outputDevice = this->incomingAudio->start ();
		}

		this->inputDevice = this->outgoingAudio->start ();

		QObject::connect (this->inputDevice,
						  SIGNAL(readyRead()),
						  this,
						  SLOT(onAudioReadyRead()));

		return true;

	} else {

		return false;
	}
}

bool
DeamonVoice::stopCall () {

	if (true == this->call) {

		this->call = false;

		this->incomingAudio->stop ();

		this->outgoingAudio->stop ();
		QObject::disconnect (this->inputDevice,
							 SIGNAL(readyRead()),
							 this,
							 SLOT(onAudioReadyRead()));


		this->closePort ();

		return true;

	} else {

		return false;
	}
}

bool
DeamonVoice::setSpeakerVolume (
		int value) {

	this->speakerVolume = value/100;

	return true;
}

bool
DeamonVoice::muteMicrophone () {

	this->micMuted = !this->micMuted;

	return true;
}

bool
DeamonVoice::muteSpeaker () {

	this->speakerVolume = !this->speakerVolume;

	return true;
}
