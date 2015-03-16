TEMPLATE += app
QT += core gui network sql multimedia widgets

CONFIG += qt warn_on

win32 {
	LIBS += -lrasapi32
	DEFINES += WINVER=0x0501
}

TARGET = Aircel

RESOURCES += resource/image.qrc \
	resource/audio.qrc
RC_FILE = resource/aircel.rc

SOURCES += main.cpp\
	AnimateButton.cpp \
	At.cpp \
	Call.cpp \
	CallWidget.cpp \
	CheckBalanceWidget.cpp \
	Contact.cpp \
	ContactContentWidget.cpp \
	ContactDialog.cpp \
	ContactFieldDialog.cpp \
	ContactMultipleSelectDialog.cpp \
	ContactSelectDialog.cpp \
	ContactSingleSelectDialog.cpp \
	ContactWidget.cpp \
	Data.cpp \
	DataWidget.cpp \
	DeamonConnection.cpp \
	DeamonDatabase.cpp \
	DeamonInterface.cpp \
	DeamonSetting.cpp \
	DeamonVoice.cpp \
	DeamonWireless.cpp \
	MainWindow.cpp \
	ImageButton.cpp \
	SingleApplication.cpp \
	Message.cpp \
	SpeedLabel.cpp \
	ExitDialog.cpp \
	MessageDialog.cpp \
	PinActivateDialog.cpp \
	PinModifyDialog.cpp \
	PinUnlockDialog.cpp \
	PinVerifyDialog.cpp \
	DeviceWidget.cpp \
	HomeWidget.cpp \
	MessageWidget.cpp \
	MessageSettingWidget.cpp \
	NetworkWidget.cpp \
	NetworkModeWidget.cpp \
	NetworkSelectionWidget.cpp \
	NetworkSettingWidget.cpp \
	NetworkStatusWidget.cpp \
	OperatorWidget.cpp \
	PinWidget.cpp \
	PocketWidget.cpp \
	SettingWidget.cpp \
	SimUsimWidget.cpp \
	UserManualWidget.cpp \
	PinDialog.cpp \
	MessageFieldDialog.cpp \
	IncomingDialog.cpp \
	Setting.cpp \
	MessageContentWidget.cpp \
	VoiceWidget.cpp \
	TimeLabel.cpp

HEADERS  += MainWindow.h \
	AnimateButton.h \
	At.h \
	DeamonConnection.h \
	DeamonDatabase.h \
	DeamonSetting.h \
	Define.h \
	ImageButton.h \
	SingleApplication.h \
	Contact.h \
	Message.h \
	DeamonWireless.h \
	DeamonVoice.h \
	SpeedLabel.h \
	Call.h \
	DeamonInterface.h \
	ContactDialog.h \
	ExitDialog.h \
	MessageDialog.h \
	PinActivateDialog.h \
	PinModifyDialog.h \
	PinUnlockDialog.h \
	PinVerifyDialog.h \
	MessageWidget.h \
	MessageSettingWidget.h \
	NetworkWidget.h \
	HomeWidget.h \
	NetworkModeWidget.h \
	NetworkSelectionWidget.h \
	NetworkSettingWidget.h \
	NetworkStatusWidget.h \
	OperatorWidget.h \
	PinWidget.h \
	PocketWidget.h \
	SettingWidget.h \
	SimUsimWidget.h \
	UserManualWidget.h \
	DataWidget.h \
	CheckBalanceWidget.h \
	DeviceWidget.h \
	PinDialog.h \
	ContactMultipleSelectDialog.h \
	ContactSingleSelectDialog.h \
	ContactFieldDialog.h \
	MessageFieldDialog.h \
	IncomingDialog.h \
	Data.h \
	Setting.h \
	MessageContentWidget.h \
	ContactContentWidget.h \
	ContactWidget.h \
	CallWidget.h \
	ContactSelectDialog.h \
	VoiceWidget.h \
	TimeLabel.h

HEADERS += SerialPort.h \
		SerialEnumerator.h \

SOURCES	+= SerialPort.cpp

unix:SOURCES += SerialPortPosix.cpp
unix:!macx:SOURCES += SerialPortUnix.cpp

macx {
  SOURCES          += SerialPortOsx.cpp
  LIBS             += -framework IOKit -framework CoreFoundation
}

win32 {
  SOURCES          += SerialPortWin.cpp SerialEnumeratorWin.cpp
  LIBS             += -lsetupapi -ladvapi32 -luser32
}






