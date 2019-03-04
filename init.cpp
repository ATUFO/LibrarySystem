#include <iostream>
#include<cstdio>
#include<windows.h>
#include<stdlib.h>
#include<stack>
#include<math.h>
#include"user.h"
#include"book.h"
#include"init.h"
#include"menu.h"

using namespace std;
//===================初始化===============================================
FILE *file;//
char bookfile[] = "bookinfo.data";
char userfile[] = "userinfo.data";
int booksnum = 0, id = 0, userSum = 0  ;
int isAdmin = 0;
int itemInPage = 20;
int maxLandBookNum = 10 ;
int showlen = 18;
user * userLogin ;
book * the_BookLine_Head ;
book * the_BookLine_Rear ;
user * the_UserLine_Head;
user * the_UserLine_Rear ;
int bookDataBlockSize;
int userDataBlockSize;
void init()
{

    userLogin = (user *)malloc(sizeof(user)); //已登陆的用户的地址
    the_BookLine_Head = (book *)malloc(sizeof(book)); //首节点
    the_BookLine_Rear = (book *)malloc(sizeof(book)); //尾节点，实现双向链表
    the_UserLine_Head = (user *)malloc(sizeof(user));
    the_UserLine_Rear = (user *)malloc(sizeof(user));
    bookline_Init();
    userline_Init();
}

void welcome()//欢迎界面
{

    system("cls");
    isAdmin = 0;
    printf("================================================================LibrarySystem Menu=======================================================================================\n\n");
    printf("\n                                                                 [1] 用户登录\n\n");
    printf("                                                                 [2] 管理员登录\n\n");
    printf("                                                                 [3] 注册\n\n");
    int cmd;
    scanf("%d", &cmd);
    char name[16], pwd[16];
    switch(cmd)
    {
    case 1:
        system("cls");
        printf("================================================================LibrarySystem Menu=======================================================================================\n\n");
        printf("输入账号\n");
        scanf("%s", name);
        printf("输入密码\n");
        scanf("%s", pwd);
        if(login(name, pwd, 0))
        {
            userMenu();
        }

        else
        {
            printf("密码错误\n");
            getchar();
            getchar();
            welcome();
        }
        break;
    case 2:
        system("cls");
        printf("================================================================LibrarySystem Menu=======================================================================================\n\n");
        printf("输入账号\n");
        scanf("%s", name);
        printf("输入密码\n");
        scanf("%s", pwd);
        if(login(name, pwd, 1) && userLogin->isAdminstrator)
        {
            adminMenu();
        }

        else
        {
            printf("密码错误\n");
            getchar();
            getchar();
            welcome();
        }
        break;
    case 3:
        regist();
        break;

    }
}

void bookline_Init()//初始化
{
    bookDataBlockSize = sizeof(book) - sizeof(book *) * 2; //去除struct  book  的before 与next 的大小
    the_BookLine_Head->next = the_BookLine_Rear; //链表初始化
    the_BookLine_Rear->before = the_BookLine_Head;
    the_BookLine_Rear->next = NULL;
    the_BookLine_Head->before = NULL;

    file = fopen(bookfile, "rb+"); //打开文件

    book *t = (book *)malloc(sizeof(book)); //临时的book用来保存每次读取的数据

    while(fread(t, bookDataBlockSize, 1, file) != NULL)
    {
        book *node = (book *)malloc(sizeof(book));
        *node = *t;
        id = max(id, node->id); //获得最大id  方便下面分配id

        node->next = the_BookLine_Head->next; //插入节点
        the_BookLine_Head->next->before = node;
        the_BookLine_Head->next = node;
        node->before = the_BookLine_Head;

        booksnum++;//在馆书数目增加
    }
    free(t);//释放临时book
    fclose(file);//关闭流

}


void userline_Init()//用户链表初始化
{


    userDataBlockSize = sizeof(user) - sizeof(user *) * 2;
    the_UserLine_Head->next = the_UserLine_Rear; //链表初始化
    the_UserLine_Rear->before = the_UserLine_Head;
    the_UserLine_Rear->next = NULL;
    the_UserLine_Head->before = NULL;

    file = fopen(userfile, "rb+"); //打开文件

    user *t = (user *)malloc(sizeof(user)); //临时的user用来保存每次读取的数据

    while(fread(t, userDataBlockSize, 1, file) != NULL)
    {
        user *node = (user *)malloc(sizeof(user));
        *node = *t;
        node->next = the_UserLine_Head->next; //插入节点
        the_UserLine_Head->next->before = node;
        the_UserLine_Head->next = node;
        node->before = the_UserLine_Head;
        userSum++;
    }
    free(t);//释放临时user
    fclose(file);//关闭流
    if(userSum == 0)
    {
        /*第一次创建管理员
                          账号密码都是2*/
        user *newUser = (user *)malloc(sizeof(user));
        strcpy(newUser->name, "2");
        strcpy(newUser->passWord, "2");
        newUser->isAdminstrator = 1;
        newUser->lendNum = 0;
        newUser->next = the_UserLine_Head->next; //配置节点
        the_UserLine_Head->next->before = newUser;
        newUser->before = the_UserLine_Head;
        the_UserLine_Head->next = newUser;
        rewriteAll_UserData();
        userSum++;
    }


}



