#include <Cipher.h>
//大漠算法！！！！
///////////////////////
//异或加/解密算法(以下参数类同)
//datas:待加/解密内容地址
//password:加密用的密码
void cipher_BitXor(QByteArray &datas,QString password)
{
    uint datasLen=datas.length();//明文长度datasLen
    uint pwdLen=password.length();//密码长度pwdLen
    char *data=datas.data();//指针Data用于处理明文里的数据
    uint i=0,j=0;
    for(;i<datasLen;++i)//对明文中的每个字节都进行加密
    {
        if(j<pwdLen)//先加密pwdLen个明文字节
        {
            data[i]^=password[j].toAscii();//用密码进行异或加密
            ++j;
        }
        else//再加密1个字节
        {
            data[i]^=pwdLen;//用pwdLen进行异或加密
            j=0;//如此循环使用密码
        }
    }//如此循环，直至每个字节都被加密了
}

//算数加密算法！！！
void cipher_Encryption(QByteArray &datas,QString password)
{
    uint datasLen=datas.length();//明文长度datasLen
    uint pwdLen=password.length();//密码长度pwdLen
    char *data=datas.data();//指针Data用于处理明文里的数据
    uint i=0,j=0;
    for(;i<datasLen;++i)//对明文中的每个字节都进行加密
    {
        if(j<pwdLen)//先加密pwdLen个明文字节
        {
            data[i]+=password[j].toAscii();//用密码进行正偏移加密
            ++j;
        }
        else//再加密1个字节
        {
            data[i]+=pwdLen;//用pwdLen进行正偏移加密
            j=0;//如此循环
        }
    }//如此循环，直至每个字节都被加密了
}

//算数解密算法！！！
void cipher_Decryption(QByteArray &datas,QString password)
{
    uint datasLen=datas.length();//明文长度datasLen
    uint pwdLen=password.length();//密码长度pwdLen
    char *data=datas.data();//指针Data用于处理明文里的数据
    uint i=0,j=0;
    for(;i<datasLen;++i)//对明文中的每个字节都进行加密
    {
        if(j<pwdLen)//先加密pwdLen个明文字节
        {
            data[i]-=password[j].toAscii();//用密码进行负偏移加密
            ++j;
        }
        else//再加密1个字节
        {
            data[i]-=pwdLen;//用pwdLen进行负偏移加密
            j=0;//如此循环
        }
    }//如此循环，直至每个字节都被加密了
}
