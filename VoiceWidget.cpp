#include "VoiceWidget.h"

#include "At.h"
#include "ContactSingleSelectDialog.h"
#include "DeamonInterface.h"
#include "DeamonVoice.h"
#include "Define.h"
#include "ContactWidget.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>

VoiceWidget::VoiceWidget (
		QWidget *parent)
	: QWidget(parent) {

	this->initialize ();

	this->createDialpad ();
	this->createTable ();
	this->createLayout ();

	this->connect ();
}

bool
VoiceWidget::initialize () {
	this->incomingCall = false;
	this->calling = false;

	this->startTime.start ();

	this->icon << QIcon (":/allCalls.png")
			   << QIcon (":/answeredCalls.png")
			   << QIcon (":/missedCalls.png")
			   << QIcon (":/dialledCalls.png");

	return true;
}

bool VoiceWidget::createTable () {

	this->table = new QTableWidget (this);
	this->table->setShowGrid (false);
	this->table->setSelectionBehavior (QAbstractItemView::SelectRows);

	this->table->setColumnCount (Call::FieldCount);

	this->table->setColumnHidden (Call::Number, true);
	this->table->setColumnHidden (Call::StartTime, true);
	this->table->setColumnHidden (Call::Index, true);
	this->table->setColumnHidden (Call::Type, true);
	this->table->setColumnHidden (Call::Unread, true);

	this->table->verticalHeader ()->setDefaultSectionSize (18);
	this->table->verticalHeader ()->setVisible (false);

	this->table->horizontalHeader ()->setStretchLastSection (true);
	this->table->setHorizontalHeaderLabels (Call::Header);

	this->table->setGeometry (350, 50, 300, 100);

	this->table->sortByColumn (Call::StartTime, Qt::DescendingOrder);

	QObject::connect (this->table,
					  SIGNAL(customContextMenuRequested(const QPoint&)),
					  this,
					  SLOT(onContextMenu(const QPoint&)));

	QObject::connect (this->table,
					  SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
					  this,
					  SLOT(onTableItemDoubleClicked(QTableWidgetItem*)));

	return true;
}

bool
VoiceWidget::onContextMenu(
		const QPoint&) {

	return true;
}

bool
VoiceWidget::onMuteMicrophoneClicked () {

	this->microphoneButton->setSelect ();

	DeamonVoice::getInstance ()->muteMicrophone ();

	return true;
}

bool
VoiceWidget::onMuteSpeakerClicked () {

	this->speakerButton->setSelect ();

	DeamonVoice::getInstance ()->muteSpeaker ();

	return true;
}

bool
VoiceWidget::onVolumeUpClicked () {

	this->volumeSlider->setValue (this->volumeSlider->value () + 10);

	return true;
}


bool
VoiceWidget::onVolumeDownClicked () {

	this->volumeSlider->setValue (this->volumeSlider->value () - 10);

	return true;
}


bool VoiceWidget::createLayout () {

	this->voiceCallBackground = new QLabel (this);
	this->voiceCallBackground->setPixmap (QPixmap(":/voiceCallBackground.png"));
	this->voiceCallBackground->setGeometry (10, 15, 327, 380);

	this->background = new QLabel (this);
	this->background->setPixmap (QPixmap(":/toolBar.png"));
	this->background->setGeometry (350, 160, 284, 33);

	this->microphoneButton = new ImageButton("",
											 ":/mic.png",
											 ":/mic.png",
											 ":/micPress.png",
											 false, this);
	this->microphoneButton->setGeometry (355, 160, 36, 36);

	this->speakerButton = new ImageButton("",
										  ":/volume.png",
										  ":/volume.png",
										  ":/volumePress.png",
										  false, this);
	this->speakerButton->setGeometry (440, 160, 36, 36);

	this->volumeUpButton = new ImageButton("",
										   ":/volumePlusEnable.png",
										   ":/volumePlusEnable.png",
										   ":/volumePlusPress.png",
										   false, this);
	this->volumeUpButton->setGeometry (600, 158, 36, 36);

	this->volumeDownButton = new ImageButton("",
											 ":/volumeMinusEnable.png",
											 ":/volumeMinusEnable.png",
											 ":/volumeMinusPress.png",
											 false, this);
	this->volumeDownButton->setGeometry (465, 158, 36, 36);

	this->volumeSlider = new QSlider(Qt::Horizontal, this);
	this->volumeSlider->setMinimum (0);
	this->volumeSlider->setMaximum (100);
	this->volumeSlider->setTickInterval (10);
	this->volumeSlider->setValue (100);
	this->volumeSlider->setGeometry (500, 166, 100, 20);

	this->recommendationLabel = new QLabel(RECOMMENDATION, this);
	this->recommendationLabel->setGeometry (350, 360, 470, 50);

	QObject::connect (this->microphoneButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onMuteMicrophoneClicked()));

	QObject::connect (this->speakerButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onMuteSpeakerClicked()));

	QObject::connect (this->volumeUpButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onVolumeUpClicked()));

	QObject::connect (this->volumeDownButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onVolumeDownClicked()));

	QObject::connect (this->volumeSlider,
					  SIGNAL(sliderMoved(int)),
					  DeamonVoice::getInstance (),
					  SLOT(setSpeakerVolume(int)));

	QObject::connect (this->dialingNumber,
					  SIGNAL(textChanged(QString)),
					  this,
					  SLOT(onDialingNumberTextChanged()));

	QObject::connect (this->dialingNumber,
					  SIGNAL(returnPressed()),
					  this->callButton,
					  SIGNAL(clicked()));

	QObject::connect (this->volumeSlider,
					  SIGNAL(valueChanged(int)),
					  this,
					  SLOT(onVolumeSliderValueChanged(int)));

	return true;
}

bool  VoiceWidget::createDialpad () {

	this->dialingNumber = new QLineEdit(this);
	this->dialingNumber->setPlaceholderText ("Enter the number to be dialed");
	this->dialingNumber->setGeometry (350, 20, 300, 25);

	this->callButton = new ImageButton("",
									   ":/callUpDefault.png",
									   ":/callUpHover.png",
									   ":/callUpPress.png",
									   ":/callUpDisable.png",
									   false, this);
	this->callButton->setGeometry (517, 195, 65, 62);

	this->hangButton = new ImageButton("",
									   ":/callDownDefault.png",
									   ":/callDownHover.png",
									   ":/callDownPress.png",
									   ":/callDownDisable.png",
									   false, this);
	this->hangButton->setGeometry (574, 195, 65, 62);
	this->hangButton->setEnabled (false);

	this->dialBack = new ImageButton("",
									 ":/dialBackDefault.png",
									 ":/dialBackHover.png",
									 ":/dialBackPress.png",
									 ":/dialBackDisable.png",
									 false, this);

	this->dialBack->setGeometry (572, 279, 68, 33);

	this->dialOne = new ImageButton("1",
									":/dialPadDefault.png",
									":/dialPadHover.png",
									":/dialPadPress.png",
									":/dialPadDisable.png",
									false, this);
	this->dialOne->setGeometry (343, 195, 68, 33);

	this->dialTwo = new ImageButton("2",
									":/dialPadDefault.png",
									":/dialPadHover.png",
									":/dialPadPress.png",
									":/dialPadDisable.png",
									false, this);
	this->dialTwo->setGeometry (400, 195, 68, 33);

	this->dialThree = new ImageButton("3",
									  ":/dialPadDefault.png",
									  ":/dialPadHover.png",
									  ":/dialPadPress.png",
									  ":/dialPadDisable.png",
									  false, this);
	this->dialThree->setGeometry (457, 195, 68, 33);

	this->dialFour = new ImageButton("4",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialFour->setGeometry (343, 223, 68, 33);

	this->dialFive = new ImageButton("5",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialFive->setGeometry (400, 223, 68, 33);

	this->dialSix = new ImageButton("6",
									":/dialPadDefault.png",
									":/dialPadHover.png",
									":/dialPadPress.png",
									":/dialPadDisable.png",
									false, this);
	this->dialSix->setGeometry (457, 223, 68, 33);

	this->dialSeven = new ImageButton("7",
									  ":/dialPadDefault.png",
									  ":/dialPadHover.png",
									  ":/dialPadPress.png",
									  ":/dialPadDisable.png",
									  false, this);
	this->dialSeven->setGeometry (343, 251, 68, 33);

	this->dialEight = new ImageButton("8",
									  ":/dialPadDefault.png",
									  ":/dialPadHover.png",
									  ":/dialPadPress.png",
									  ":/dialPadDisable.png",
									  false, this);
	this->dialEight->setGeometry (400, 251, 68, 33);

	this->dialNine = new ImageButton("9",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialNine->setGeometry (457, 251, 68, 33);

	this->dialZero = new ImageButton("0",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialZero->setGeometry (343, 279, 68, 33);

	this->dialStar = new ImageButton("*",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialStar->setGeometry (400, 279, 68, 33);

	this->dialHash = new ImageButton("#",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialHash->setGeometry (457, 279, 68, 33);

	this->phonebook = new ImageButton("",
									  ":/dialPhoneBookDefault.png",
									  ":/dialPhoneBookHover.png",
									  ":/dialPhoneBookPress.png",
									  false, this);
	this->phonebook->setGeometry (514, 251, 68, 33);

	this->dialPlus = new ImageButton("+",
									 ":/dialPadDefault.png",
									 ":/dialPadHover.png",
									 ":/dialPadPress.png",
									 ":/dialPadDisable.png",
									 false, this);
	this->dialPlus->setGeometry (514, 279, 68, 33);

	this->dialClear = new ImageButton("C",
									  ":/dialPadDefault.png",
									  ":/dialPadHover.png",
									  ":/dialPadPress.png",
									  ":/dialPadDisable.png",
									  false, this);
	this->dialClear->setGeometry (572, 251, 68, 33);

	QObject::connect (this->callButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onCallButtonClicked()));

	QObject::connect (this->hangButton,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onHangButtonClicked()));

	QObject::connect (this->dialBack,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialBackButtonClicked()));

	QObject::connect (this->dialOne,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialOneButtonClicked()));

	QObject::connect (this->dialTwo,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialTwoButtonClicked()));

	QObject::connect (this->dialThree,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialThreeButtonClicked()));

	QObject::connect (this->dialFour,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialFourButtonClicked()));

	QObject::connect (this->dialFive,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialFiveButtonClicked()));

	QObject::connect (this->dialSix,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialSixButtonClicked()));

	QObject::connect (this->dialSeven,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialSevenButtonClicked()));

	QObject::connect (this->dialEight,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialEightButtonClicked()));

	QObject::connect (this->dialNine,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialNineButtonClicked()));

	QObject::connect (this->dialZero,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialZeroButtonClicked()));

	QObject::connect (this->dialStar,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialStarButtonClicked()));

	QObject::connect (this->dialHash,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialHashButtonClicked()));

	QObject::connect (this->phonebook,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onPhoneBookClicked()));

	QObject::connect (this->dialPlus,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialPlusButtonClicked()));

	QObject::connect (this->dialClear,
					  SIGNAL(clicked()),
					  this,
					  SLOT(onDialCancelButtonClicked()));

	return true;
}

void
VoiceWidget::showEvent (QShowEvent * event) {

	this->dialingNumber->setFocus ();

	QWidget::showEvent (event);
}

Call VoiceWidget::rowToCall (
		int index) {

	return Call (this->table->item (index, Call::Number)->text (),
				 this->table->item (index, Call::Name)->text (),
				 QDateTime::fromString (this->table->item (
											index, Call::StartTime)->text (),
										DATETIME_FORMAT),
				 QTime::fromString (this->table->item (
										index, Call::Duration)->text (),
									TIME_FORMAT),
				 static_cast<Call::Types> (this->table->item (
											  index, Call::Type)
										  ->text ().toInt ()),
				 this->table->item (index, Call::Index)->text ().toInt ());
}

bool
VoiceWidget::callToRow (
		Call call) {

	int index = 0;
	this->table->insertRow (index);

	QTableWidgetItem * dataItem = NULL;

	if (call.getName ().isEmpty ()) {
		call.setName (call.getNumber ());
	}

	dataItem = new QTableWidgetItem(call.getNumber ());
	dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	dataItem->setTextAlignment (Qt::AlignLeft);
	this->table->setItem (index, Call::Number, dataItem);

	dataItem = new QTableWidgetItem(call.getName ());
	dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	dataItem->setTextAlignment (Qt::AlignLeft);
	dataItem->setIcon (this->icon.at (call.getType ()));
	this->table->setItem (index, Call::Name, dataItem);

	dataItem = new QTableWidgetItem(call.getStartTime ()
									.toString (DATETIME_FORMAT));
	dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	dataItem->setTextAlignment (Qt::AlignLeft);
	this->table->setItem (index, Call::StartTime, dataItem);

	dataItem = new QTableWidgetItem(call.getDuration ()
									.toString (TIME_FORMAT));
	dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	dataItem->setTextAlignment (Qt::AlignLeft);
	this->table->setItem (index, Call::Duration, dataItem);

	dataItem = new QTableWidgetItem(QString::number (
										static_cast<int>(call.getType ())));
	dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	dataItem->setTextAlignment (Qt::AlignLeft);
	this->table->setItem (index, Call::Type, dataItem);

	dataItem = new QTableWidgetItem(QString::number (call.getIndex ()));
	dataItem->setFlags (Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	dataItem->setTextAlignment (Qt::AlignLeft);
	this->table->setItem (index, Call::Index, dataItem);

	if (call.getName ().contains (call.getNumber ())) {

		emit this->number (call.getNumber ());
	}

	return true;
}

bool
VoiceWidget::dial () {

	this->callButton->setEnabled (false);
	this->hangButton->setEnabled (true);

	this->callTimer->start (1000);
	this->startTime.restart ();

	DeamonInterface::getInstance ()->write (
				At::dial + this->dialingNumber->text ()+ SEMI_COLON);

	return true;
}

bool
VoiceWidget::dial (
		QString number,
		bool connect) {

	this->dialingNumber->setText (number.trimmed());

	if (true == connect) {
		this->dial ();

		Call call (this->dialingNumber->text (),
				   "",
				   QDateTime::currentDateTime (),
				   QTime(),
				   Call::Dialed);

		this->callToRow (call);
	}

	return true;
}

bool
VoiceWidget::onCallButtonClicked () {

	if ( this->dialingNumber->text ().isEmpty ()) {
		this->dialingNumber->setFocus ();
		return false;
	}

	this->calling = true;

	this->callTimer->start (1000);
	this->startTime.restart ();

	if (false == this->incomingCall) {

		this->callButton->setEnabled (false);
		this->hangButton->setEnabled (true);

		this->dial (this->dialingNumber->text (), true);

	} else {

		this->incomingCall = false;
		DeamonInterface::getInstance ()->write (At::answer);

		this->table->item (0, Call::Type)->setText (
					QString::number (Call::Answered));
		this->table->item (0, Call::Name)->setIcon (
					this->icon.at (Call::Answered));

	}

	return (true);
}

bool
VoiceWidget::onCallTimer () {

	this->duration = QTime (0,0).addMSecs (this->startTime.elapsed ());

	this->table->item (0, Call::Duration)->setText (
				this->duration.toString (TIME_FORMAT));

	return true;
}

bool
VoiceWidget::onIncomingCallUpdate (
		Call call) {

	this->incomingCall = true;

	this->dialingNumber->setText (call.getNumber ());

	this->callButton->setEnabled (true);
	this->hangButton->setEnabled (true);

	this->callToRow (call);

	return true;
}

bool
VoiceWidget::onHangButtonClicked () {

	this->calling = false;

	DeamonInterface::getInstance ()->write (At::disconnectCall);

	this->callTimer->stop ();

	if (true == this->incomingCall) {
		this->incomingCall = false;

		this->table->item (0, Call::Type)->setText (
					QString::number (Call::Answered));
		this->table->item (0, Call::Name)->setIcon (
					this->icon.at (Call::Answered));

		emit answered (this->rowToCall (0));

	} else {

		emit dialed (this->rowToCall (0));
	}

	this->hangButton->setEnabled (false);
	this->callButton->setEnabled (true);

	this->dialingNumber->clear ();

	return (true);
}



bool
VoiceWidget::onPhoneBookClicked () {

	bool result = false;

	emit readContactUpdate (false);

	ContactSingleSelectDialog contact;

	if ( QDialog::Accepted == contact.exec ()) {

		this->dialingNumber->setText (contact.getNumber ());
		result = true;
	}

	emit readContactUpdate (true);
	return result;
}

bool
VoiceWidget::onDialCancelButtonClicked () {

	QString number = this->dialingNumber->text ();
	number.chop (1);
	this->dialingNumber->setText (number);

	return (true);
}

bool
VoiceWidget::onDialBackButtonClicked () {

	return (true);
}

bool
VoiceWidget::onVolumeSliderValueChanged (
		int value) {

	Q_UNUSED (value);

	return (true);
}

bool
VoiceWidget::onModemPlugged (
		bool status) {

	if (false == status) {

		this->callButton->setEnabled (false);
		this->hangButton->setEnabled (false);
		this->dialBack->setEnabled (false);
	}

	return true;
}

bool
VoiceWidget::onModemConnected (
		bool status) {
	Q_UNUSED (status);

	return true;
}

bool
VoiceWidget::onDialingNumberTextChanged () {

	this->callButton->setEnabled (
				false == this->dialingNumber->text ().isEmpty ());

	return true;
}

bool
VoiceWidget::name (
		QString number,
		QString name) {

	this->table->setSortingEnabled (false);

	for (int index = 0; index < this->table->rowCount (); index++) {

		if (this->table->item (index, Call::Number)->text ().contains (number)) {

			this->table->item (index, Call::Name)->setText (name);
		}
	}

	this->table->setSortingEnabled (true);

	return true;
}

bool
VoiceWidget::onCallAcceptUpdate () {

	return true;
}

bool
VoiceWidget::onCallEndUpdate () {

	this->calling = false;

	if (true == this->incomingCall) {

		this->table->item (0, Call::Type)->setText (
					QString::number (static_cast<int> (Call::Missed)));

		this->table->item (0, Call::Name)->setIcon (
					this->icon.at (Call::Missed));

		this->incomingCall = false;

		emit missed (this->rowToCall (0));
	}

	this->callButton->setEnabled (true);
	this->hangButton->setEnabled (false);

	this->callTimer->stop ();

	return true;
}

bool
VoiceWidget::connect () {
	this->callTimer = new QTimer ();

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(incomingCallUpdate (Call)),
					  this,
					  SLOT(onIncomingCallUpdate(Call)) );

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(callAcceptUpdate()),
					  this,
					  SLOT(onCallAcceptUpdate ()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(callEndUpdate()),
					  this,
					  SLOT(onCallEndUpdate ()));

	QObject::connect (DeamonInterface::getInstance (),
					  SIGNAL(modemPlugged (bool)),
					  this,
					  SLOT(onModemPlugged (bool)));

	QObject::connect (this->callTimer,
					  SIGNAL(timeout()),
					  this,
					  SLOT(onCallTimer()));

	return true;
}

bool
VoiceWidget::onTableItemDoubleClicked (
		QTableWidgetItem* current) {

	if (NULL == current) {

		return false;

	} else {

		int row = this->table->currentRow ();

		this->dialingNumber->setText (this->table->item (row, Call::Number)->text ());

		return true;
	}
}

bool
VoiceWidget::onDialOneButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "1");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "1" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialTwoButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "2");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "2" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialThreeButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "3");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "3" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialFourButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "4");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "4" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialFiveButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "5");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "5" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialSixButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "6");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "6" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialSevenButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "7");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "7" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialEightButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "8");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "8" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialNineButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "9");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "9" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialZeroButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "0");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "0" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialStarButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "*");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "*" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialHashButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "#");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "#" + At::duration);
	}

	return (true);
}

bool
VoiceWidget::onDialPlusButtonClicked () {

	this->dialingNumber->setText (this->dialingNumber->text () + "+");

	if (true == this->calling) {

		DeamonInterface::getInstance ()->write (
					At::dtmf + "+" + At::duration);
	}

	return (true);
}
