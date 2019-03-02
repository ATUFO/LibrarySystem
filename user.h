#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED
//用户
typedef struct User
{
    char name[16],passWord[16];
    int isAdminstrator;//是否是管理员
    int lendedBookId[10],lendNum;//landedbookid 为已借书的id   lendnum为已借图书数目
    User *before,*next;
} user;
#endif // USER_H_INCLUDED
