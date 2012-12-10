#include <QString>

#ifndef CIPHER_H
#define CIPHER_H

void cipher_BitXor(QByteArray &Datas,QString password);//异或加密
void cipher_Encryption(QByteArray &Datas,QString password);//正偏移加密(与负偏移解密配套)
void cipher_Decryption(QByteArray &Datas,QString password);//负偏移解密(与正偏移加密配套)

#endif // CIPHER_H