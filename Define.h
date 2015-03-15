#ifndef DEFINE
#define DEFINE

#define COMMA ','
#define SLASH '/'
#define QUOTE '\''
#define NEW_LINE '\r'
#define CARRIAGE_RETURN '\n'
#define NEW_LINE_CARRIAGE "\r\n"
#define COMMAND_PROMPT "> "
#define CTRL_Z "\x1A"
#define COLON ":"
#define SEMI_COLON ";"
#define QUOTES "\""
#define SPACE " "
#define PLUS "+"

#define CLOSE_PARANTHESIS ")"
#define OPEN_PARANTHESIS "("
#define NETWORK_MODE_SEPARATOR "),("
#define NETWORK_MODE_INVALID "),,("

#define MESSAGE_COUNT_INDEX 7

#define LOG_FILE_NAME "aircel"

#define APPLICATION_NAME "Aircel Dialer"
#define SPLASH_SCREEN ":/splash.png"
#define APPLICATION_ICON ":/logo.ico"

#define SPLASH_SCREEN_TIMEOUT 2500

#define SIGNAL_STRENGTH_TIMEOUT 10000
#define THREAD_SLEEP_TIMEOUT 150
#define PORT_TIMEOUT 500

#define DORMANT0 "^DSDORMANT:0"
#define DORMANT1 "^DSDORMANT:1"
#define ERROR_STRING "ERROR"

#define MODEM "USB\\VID_22F4&PID_0026&MI_03"
#define MODEM_REVISION "USB\\VID_22F4&PID_0026&Rev_0000&MI_03"

#define OLIVE_MODEM_NAME "Olive Modem"
#define OLIVE_DEVICE_TYPE "RASDT_Modem"
#define VENDOR_ID 0x22F4
#define PRODUCT_ID 0x0026

#define INTERFACE_PORT_NAME "Olive Interface"
#define VOICE_PORT_NAME "Olive Voice"
#define DIAGNOSTIC_PORT_NAME "Olive Diagnostic"
#define DIALUP_CONNECTION_NAME "Olive Connection"

#define DIALUP_NUMBER "*98#"
#define DIALUP_USERNAME ""
#define DIALUP_PASSWORD ""

#define CUSTOMER_CALL_NUMBER "121"

#define DATABASE_TYPE "QSQLITE"
#define DATABASE_CONNECTION "database"
#define DATABASE_NAME "olive"
#define DATABASE_HOST "OliveHost"
#define DATABASE_USER "OliveUser"
#define DATABASE_PASSWORD "OlivePassword"

#define CREATE_TABLE_SETTING "CREATE TABLE IF NOT EXISTS settings(value VARCHAR (100), enable BOOLEAN, identifier INTEGER PRIMARY KEY AUTOINCREMENT)"
#define SELECT_SETTING "SELECT value, enable, identifier FROM settings"
#define READ_SETTING "SELECT value, enable, identifier FROM settings WHERE identifier = :identifier"
#define EDIT_SETTING "UPDATE settings SET value = :value, enable = :enable WHERE identifier = :identifier"
#define INSERT_SETTING "INSERT INTO settings(value, enable) VALUES (:value, :enable)"
#define DELETE_SETTING "DELETE FROM settings WHERE (identifier = :identifier)"
#define DELETE_ALL_SETTING "DELETE FROM settings"

#define CREATE_TABLE_MESSAGE "CREATE TABLE IF NOT EXISTS messages(number VARCHAR (20), name VARCHAR (20), content VARCHAR (256), time DATEIME, type INTEGER, identifier INTEGER PRIMARY KEY AUTOINCREMENT, unread BOOLEAN)"
#define SELECT_MESSAGE "SELECT number, name, content, time, type, identifier, unread FROM messages"
#define READ_MESSAGE "SELECT number, name, content, time, type, identifier, unread FROM messages WHERE (identifier = :identifier)"
#define EDIT_MESSAGE "UPDATE messages SET number = :number, name = :name , content = :content, time = :time, type = :type, unread = :unread WHERE identifier = :identifier"
#define INSERT_MESSAGE "INSERT INTO messages(number, name, content, time, type, unread) VALUES (:number, :name, :content, :time, :type, :unread)"
#define DELETE_MESSAGE "DELETE FROM messages WHERE (identifier = :identifier)"
#define DELETE_ALL_MESSAGE "DELETE FROM messages"

#define CREATE_TABLE_CONTACT "CREATE TABLE IF NOT EXISTS contacts(name VARCHAR (20), mobile VARCHAR (20), office VARCHAR (20), home VARCHAR (20), email VARCHAR (64), remark VARCHAR (128), type INTEGER, identifier INTEGER PRIMARY KEY AUTOINCREMENT)"
#define SELECT_CONTACT "SELECT name, mobile, office, home, email, remark, type, identifier FROM contacts"
#define READ_CONTACT "SELECT name, mobile, office, home, email, remark, type, identifier FROM contacts WHERE identifier = :identifier"
#define EDIT_CONTACT "UPDATE contacts SET name = :name, mobile = :mobile, office = :office, home = :home, email = :email, remark = :remark, type = :type WHERE identifier = :identifier"
#define INSERT_CONTACT "INSERT INTO contacts(name, mobile, office, home, email, remark, type) VALUES (:name, :mobile, :office, :home, :email, :remark, :type)"
#define DELETE_CONTACT "DELETE FROM contacts WHERE (identifier = :identifier)"
#define DELETE_ALL_CONTACT "DELETE FROM contacts"

#define CREATE_TABLE_CALLS "CREATE TABLE IF NOT EXISTS calls(number VARCHAR (20), name VARCHAR (20), start DATETIME, duration TIME, type INTEGER, identifier INTEGER PRIMARY KEY ASC AUTOINCREMENT, unread BOOLEAN)"
#define SELECT_CALL "SELECT number, name, start, duration, type, identifier, unread  FROM calls"
#define READ_CALL "SELECT number, name, start, duration, type, identifier, unread FROM calls WHERE identifier = :identifier"
#define EDIT_CALL "UPDATE calls SET number = :number, name = :name, start = :start, duration = :duration, type = :type, unread = :unread WHERE identifier = :identifier"
#define INSERT_CALL "INSERT INTO calls (number, name, start, duration, type, unread) VALUES (:number, :name, :start, :duration, :type, :unread)"
#define DELETE_CALL "DELETE FROM calls WHERE (identifier = :identifier)"
#define DELETE_ALL_CALL "DELETE FROM calls"

#define CREATE_TABLE_DATA "CREATE TABLE IF NOT EXISTS data(download DOUBLE, upload DOUBLE, start DATETIME, identifier INTEGER PRIMARY KEY ASC AUTOINCREMENT)"
#define SELECT_DATA "SELECT download, upload, start, identifier FROM data"
#define READ_DATA "SELECT download, upload, start, identifier FROM data WHERE identifier = :identifier"
#define EDIT_DATA "UPDATE data SET download = :download, upload = :upload, start = :start WHERE identifier = :identifier"
#define INSERT_DATA "INSERT INTO data (download, upload, start) VALUES (:download, :upload, :start)"
#define DELETE_DATA "DELETE FROM data WHERE (identifier = :identifier)"
#define DELETE_ALL_DATA "DELETE FROM data"
#define CHECK_DATA "SELECT SUM(download), SUM(upload) FROM data WHERE 1 GROUP BY DATE(start) ORDER BY start DESC Limit 365"

#define GET_INDEX "SELECT last_insert_rowid() as last_insert"

#define SETTING ":setting"
#define VALUE ":value"
#define NUMBER ":number"
#define CONTENT ":content"
#define TIME ":time"
#define TYPE ":type"
#define IDENTIFIER ":identifier"
#define NAME ":name"
#define MOBILE ":mobile"
#define OFFICE ":office"
#define HOME ":home"
#define EMAIL ":email"
#define REMARK ":remark"
#define START ":start"
#define DURATION ":duration"
#define DOWNLOAD ":download"
#define UPLOAD ":upload"
#define START ":start"
#define ENABLE ":enable"
#define UNREAD ":unread"

#define WEBSITE "http://www.aircel.com"
#define POSTPAID_BILL_URL "https://epayment.aircel.com"
#define PREPAID_BILL_URL "https://epayment.aircel.com/recharge"
#define MUSIC_URL "www.airceldevices.com/dongles/010902/index.php"
#define GAMES_URL "www.airceldevices.com/dongles/020397/index.php"
#define DEFAULT_URL "www.airceldevices.com/dongles/112915/index.php"

#define BALANCE_NUMBER "*126*6#"
#define FLASH_MESSAGE_TIMEOUT 30000

#define CONFIRMATION_DIALOG "Confirmation !"
#define ERROR_DIALOG "Critical Error!"
#define DELETE_CONFIRMATION "Are you sure you want to delete ?"
#define EMPTY_INPUT "No field should be left Empty"
#define MINIMUM_LANGTH "Minimum length should be 4 digits"
#define PIN_MISMATCH "Pin Mismatch. Try Again"

#define NEW_MESSAGE "New Message"
#define NEW_REPORT "New Delivery Report"
#define NEW_CALL "Incoming Call. Do you want to receive?"
#define NEW_MESSAGE_VIEW "New Message Received. Do you want to view?"
#define NEW_REPORT_VIEW "Delivery Report Received. Do you want to view?"
#define NEW_CALL_VIEW "Incoming Call. Do you want to view?"

#define NO_SELECTION "No Selection"
#define NO_SELECTION_MESSAGE "Kindly select a Message!"
#define NO_SELECTION_CONTACT "Kindly select a Contact!"
#define NO_SELECTION_NETWORK "Kindly select a Network!"
#define NO_SELECTION_CALL "Kindly select a Record!"

#define NEW_CONTACT "New Contact"

#define TRUE_STRING "true"
#define FALSE_STRING "false"

#define TRUE_INT "1"
#define FALSE_INT "0"

#define MESSAGE_LENGTH 160

#define TIME_FORMAT "hh:mm:ss"
#define DATE_FORMAT "yy/MM/dd"
#define DATETIME_FORMAT "yy/MM/dd hh:mm:ss"
#define INPUT_DATETIME_FORMAT "yy/MM/dd hh:mm:ss"

#define INBOX_READ "REC READ"
#define INBOX_UNREAD "REC UNREAD"
#define OUTBOX_SENT "STO SENT"
#define OUTBOX_UNSENT "STO UNSENT"

#define COLUMN 0

#define MESSAGE_DELIVERY_TEXT "Successfully Delivered"


#define VERSION_URL "http://www.olivetelecom.in/setup/vmw210/aircel/version.inf"
#define SETUP_URL "http://www.olivetelecom.in/setup/vmw210/aircel/setup.exe"

#define VERSION "0002"
#define VERSION_NAME "Aircel Olive Dialer - 0002"
#define NETWORK_ERROR "Network Error"
#define VERSION_ERROR "Unable to contact Upgrader Server"
#define SETUP_ERROR "Unable to download the latest Setup"
#define NEW_VERSION "New Version"
#define NETWORK_SUCCESS "Network Success"
#define VERSION_SUCCESS "A new version is available, Click Upgrade to download"
#define VERSION_LATEST "You are using the latest version"
#define SETUP_SUCCESS "Latest version downloaded, Do you want to run the setup"

#define POCKET_INTERNET "PocketInternet SUPER PRO 3G"
#define COPYRIGHT "© 2005-2012 Olive Telecommunication Private Limited"
#define WARNING "Warning: This program is protected by copyright laws and international copyright treaties. No part of this software may be reproduced or transmitted in any form or by any means without prior written consent of Olive Telecommunication Private Limited."

#define ADD_CONTACT_HELP "Double click on the contact name to select the contact"
#define REMOVE_CONTACT_HELP "Double click on the selected number to remove it from the list"
#define RECOMMENDATION "Recommendation : Please use headset for better voice quality."

#define SIM_NAME_LIMIT 14
#define SIM_MOBILE_LIMIT 20

#define ENABLE_LABEL "Setting a PIN number for your SIM/USIM protects it from unauthorized use."
#define VERIFY_LABEL "Verify your PIN before using your SIM/USIM. Entering wrong PIN would lock your SIM/USIM."
#define UNLOCK_LABEL "You can unlock your SIM/USIM using PIN Unlock Code (PUK). Entering wrong PUK would\npermanently disable your SIM/USIM."
#define DATA_USAGE_NOTE "Note: The usage statistics captures your last 365 days data consumption and may not be accurate."
#define EXIT_MESSAGE "Do you want to Shutdown the device completely after quitting the application? \n\nNote : After shutting down the device, you need to remove the device and re-insert it into the computer to use it again"

#define CLOSE_TITLE "PocketInternet SUPER PRO 3G"
#define CLOSE_MESSAGE "Do you want to disconnect PocketInternet SUPER PRO 3G connection?"

#define MIN_16_BIT 0
#define MAX_16_BIT 65535

enum Tabs {
	NONE,
	HOME_TAB,
	MESSAGE_TAB,
	PHONEBOOK_TAB,
	MUSIC_TAB,
	GAMES_TAB,
	SETTING_TAB,
	VOICE_TAB,
	TAB_COUNT
};

#endif // DEFINE
