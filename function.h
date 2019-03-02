#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include"user.h"
#include"book.h"


void userline_Init();//用户链表初始化
void bookline_Init();//初始化
void menu();//菜单
void welcome();//欢迎界面
int login(char *name, char *pwd, int loginAsAdmin); //登陆
void userMenu();//用户菜单
void adminMenu();//管理员菜单
void userAdminMenu();//用户管理菜单
void bookAdminMenu();

user *findUser(char *name);//按 唯一识别福 name 查找某用户再链表中的地址
book *findBookbyId(int id);//按id寻找图书


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
void book_data_recover();
void importbooks();
void addbooks();//添加图书
void deletebooks();//删除图书
void deletebooks_Batch();
void updatabooks();//跟新图书
void queryAllbooks();//查询所有图书
void querySomebooks();//模糊查询
void user_LendBook();//借书
void user_ReturnBook();//还书
void showBookDetails(book *thebook);//输出数目详细信息
void freenode_Book(book *thehead);//释放节点，配合模糊搜索
void freenode_User(user *thehead);//释放节点
void queryAllUsers();//查询所有用户
void rewriteAll_UserData();//将链表中所有数据重写入文件
void rewriteAll_BookData();//同上
void showUsersList(user *thehead, int searchNum, bool isTheMianLine); //以链的方式显示列表
void showBooksList(book *thehead, int searchNum, bool isTheMainLine); //输出书的列表
#endif // FUNCTION_H_INCLUDED
