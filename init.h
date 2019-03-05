#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include"user.h"
#include"book.h"
#include "record.h"
//全局变量
extern FILE *file;//
extern char bookfile[];//文件存放位置
extern char userfile[];
extern char recordfile[];
extern int booksnum,id,userSum; //booksum为图书种数,usersum同  id为在库图书的最大id（方便为新书分配id）
extern int isAdmin;             //isAdmin 当前登陆用户是否是管理员
extern int itemInPage ;        //每页显示的条目数量
extern int maxLandBookNum;   //每个用户最多借书的本数
extern int recordNum;
extern int showlen;
extern user * userLogin ; //已登陆的用户的地址
extern book * the_BookLine_Head ; //首节点
extern book * the_BookLine_Rear ; //尾节点，实现双向链表
extern user * the_UserLine_Head;
extern user * the_UserLine_Rear ;
extern record *the_RecordLine_Head;
extern int bookDataBlockSize;//保存的单个图书数据块大小
extern int userDataBlockSize;//同上
extern int recordDataBlockSize;

//初始化函数
void userline_Init();//用户链表初始化
void bookline_Init();//初始化
void init();
void record_line_init();
void welcome();//欢迎界面
char * gettime();
#endif // FUNCTION_H_INCLUDED
