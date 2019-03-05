
#include"book.h"
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



int login(char *name, char *pwd, int loginAsAdmin); //登陆
void user_Query_Some();//模糊查询用户
void regist();//注册
void user_Lend_Book_Record(char* name);//用户借书记录
void addUserSingle();//单个增加用户
void addUserBatch();//批量增加用户
void user_Change_Password(char* name);//改密码
void user_Delete_Single();//单个删除
void user_Regist_Batch();//批量注册用户
void user_Delete_Batch();//批量删除用户
void user_Permit();//提权
void user_LendBook(book *thebook);//借书
void user_ReturnBook(book *thebook);//还书
void showUsersList(user *thehead, int searchNum, bool isTheMianLine); //以链的方式显示列表
void freenode_User(user *thehead);//释放节点
void queryAllUsers();//查询所有用户
void rewriteAll_UserData();//将链表中所有数据重写入文件
user *findUser(char *name);//按 唯一识别福 name 查找某用户再链表中的地址


#endif // USER_H_INCLUDED
