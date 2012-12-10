#ifndef LOCALDATABASE_H
#define LOCALDATABASE_H

#include <QSqlDatabase>
#include <QDir>
#include <QFile>
#include "Struct/PersonalInfoStruct.h"

bool localDatabase_insertFriend(QSqlDatabase& database,PersonalInfoStruct& info);
bool localDatabase_updateFriend(QSqlDatabase& database,PersonalInfoStruct& info);
bool localDatabase_deleteFriend(QSqlDatabase& database,qint32 id);
bool localDatabase_readUserInfo(QDir dataDir,PersonalInfoStruct& info);
bool localDatabase_writeUserInfo(QDir dataDir,PersonalInfoStruct& info);

#endif // LOCALDATABASE_H