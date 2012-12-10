#-------------------------------------------------
#
# Project created by QtCreator 2011-09-26T14:07:58
#
#-------------------------------------------------

QT += core gui
QT += sql
QT += network
include(imagelabel.pri)
include(CompleteLineEdit.pri)
include(MoodLineEdit.pri)
TARGET = Chat2011
TEMPLATE = app


SOURCES += main.cpp\
	XmlReader.cpp \
	XmlWriter.cpp \
	Widget_Name_Mood.cpp \
	Cipher.cpp \
	MyListWidget.cpp \
	FriendWidgetItem.cpp \
	FindPasswd.cpp \
	acceptaddfriendrequet.cpp \
	acceptserverinfor.cpp \
	seekclientdialog.cpp \
	PersonInfoImageLabel.cpp \
	LoginDialog.cpp \
	FriendListDialog.cpp \
	FriendInfoDetailDialog.cpp \
	RegisterDialog.cpp \
	RegisterMoreInforForm.cpp \
	PasswdProtectForm.cpp \
	MyTextEdit.cpp \
	SingleChatDialog.cpp \
	MessageRemindForm.cpp \
	MyListView.cpp \
	ChatRecordDialog.cpp \
    SnapScreen/SnapWidget.cpp \
    SnapScreen/SnapToolBar.cpp \
    SnapScreen/SnapScreen.cpp \
    SnapScreen/SnapScope.cpp \
    LocalDataBase.cpp \
    Struct/PersonalInfoStruct.cpp \
    Struct/GroupMemberInfoStruct.cpp \
    Struct/GroupInfoStruct.cpp \
    SnapScreen/SnapText.cpp \
    TransferFileRemindForm.cpp

HEADERS  += \
	XmlReader.h \
	XmlWriter.h \
	Widget_Name_Mood.h \
	Cipher.h \
	MyListWidget.h \
	FriendWidgetItem.h \
	FindPasswd.h \
	acceptaddfriendrequet.h \
	seekclientdialog.h \
	acceptserverinfor.h \
	PersonInfoImageLabel.h \
	LoginDialog.h \
	FriendListDialog.h \
	FriendInfoDetailDialog.h \
	RegisterDialog.h \
	RegisterMoreInforForm.h \
	PasswdProtectForm.h \
	MyTextEdit.h \
	SingleChatDialog.h \
	MessageRemindForm.h \
	ChatRecordDialog.h \
	MyListView.h \
    SnapScreen/SnapWidget.h \
    SnapScreen/SnapToolBar.h \
    SnapScreen/SnapScreen.h \
    SnapScreen/SnapScope.h \
    LocalDataBase.h \
    Struct/PersonalInfoStruct.h \
    Struct/GroupMemberInfoStruct.h \
    Struct/GroupInfoStruct.h \
    SnapScreen/SnapText.h \
    TransferFileRemindForm.h

FORMS    += LoginDialog.ui \
	Widget_Name_Mood.ui \
	FindPasswd.ui \
	seekclientdialog.ui \
	acceptaddfriendrequet.ui \
	acceptserverinfor.ui \
	FriendListDialog.ui \
	FriendInfoDetailDialog.ui \
	RegisterDialog.ui \
	RegisterMoreInforForm.ui \
	PasswdProtectForm.ui \
	SingleChatDialog.ui \
	MessageRemindForm.ui \
	ChatRecordDialog.ui \
    TransferFileRemindForm.ui

RESOURCES += \
	images/images.qrc

OTHER_FILES +=


