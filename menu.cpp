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


void userMenu()//用户菜单
{
    system("cls");
    printf("==============================================================LibrarySystem Menu============================================================================\n");
    printf("当前共 %d 种书在馆\n", booksnum);
    printf("                                                                [1] 浏览图书\n");
    printf("                                                                [2] 搜索图书\n");
    printf("                                                                [3] 借书\n");
    printf("                                                                [4] 还书\n");
    printf("                                                                [5] 借书记录\n");
    printf("                                                                [6] 密码更改\n");
    printf("                                                                [0] 登出\n");
    int cmd;
    scanf("%d", &cmd);
    switch(cmd)
    {
    case 1:
        queryAllbooks();
        break;
    case 2:
        querySomebooks();
        break;
    case 3:
        user_LendBook();
        break;
    case 4:
        user_ReturnBook();
        break;
    case 5:
        user_Lend_Book_Record(userLogin->name);
        break;
    case 6:
        user_Change_Password(userLogin->name);
        break;
    case 0:
        welcome();
        break;
    }

}
void adminMenu()//管理员菜单
{


    system("cls");
    printf("==============================================================LibrarySystem Menu============================================================================\n");
      printf("当前共 %d 种书在馆，%d位用户\n", booksnum, userSum);
    printf("                                                                [1] 图书管理\n");
    printf("                                                                [2] 用户管理\n");
    printf("                                                                [0] 登出\n");
    int cmd;
    scanf("%d", &cmd);
    switch(cmd)
    {
    case 1:
        bookAdminMenu();
        break;
    case 2:
        userAdminMenu();
        break;
    case 0:
        welcome();

    }
}
void bookAdminMenu()
{
    system("cls");
    printf("==============================================================LibrarySystem Menu============================================================================\n");
    printf("当前共 %d 种书在馆\n", booksnum);
    printf("                                                                [1] 浏览图书\n");
    printf("                                                                [2] 搜索图书\n");
    printf("                                                                [3] 增加单本图书\n");
    printf("                                                                [4] 下架单本图书\n");//方法，整个链表重写
    printf("                                                                [5] 按时间批量下架图书\n");
    printf("                                                                [6] 修改图书信息\n");
    printf("                                                                [7] 导入图书数据\n");
    printf("                                                                [8] 导入失败，恢复数据\n");
    printf("                                                                [0] 返回\n");
    int cmd;
    scanf("%d", &cmd);
    switch(cmd)
    {
    case 1:
        queryAllbooks();
        break;
    case 2:
        querySomebooks();
        break;
    case 3:
        addbooks();
        break;
    case 4:
        deletebooks();
        break;
    case 5:
        deletebooks_Batch();
    case 6:
        updatabooks();
        break;
    case 7:
        importbooks();
        break;

    case 8:
        book_data_recover();
    case 0:
        adminMenu();

    }
}
void userAdminMenu()//用户管理菜单
{
    system("cls");
     printf("==============================================================LibrarySystem Menu============================================================================\n");
      printf("共 %d 位用户\n", userSum);
    printf("                                                                [1] 浏览用户\n");
    printf("                                                                [2] 搜索用户\n");
    printf("                                                                [3] 用户借书记录\n");
    printf("                                                                [4] 更改用户密码\n");
    printf("                                                                [5] 注册用户\n");
    printf("                                                                [6] 删除用户\n");
    printf("                                                                [7] 批量注册用户\n");
    printf("                                                                [8] 批量删除用户\n");
    printf("                                                                [9] 用户权限更改\n");
    printf("                                                                [0] 退出\n");
    int cmd;
    scanf("%d", &cmd);
    switch(cmd)
    {
        char name[16];
    case 1:
        queryAllUsers();
        break;
    case 2:
        user_Query_Some();
        break;
    case 3:
        system("cls");//因为管理员于用户公用一个方法，所以用了带参函数
        printf("输入查询的账号\n");
        scanf("%s", name);
        getchar();
        user_Lend_Book_Record(name);
        break;
    case 4:
        system("cls");//同上
        printf("输入改密码的账号\n");
        scanf("%s", name);
        getchar();
        user_Change_Password(name);
        break;
    case 5:
        regist();
        break;
    case 6:
        user_Delete_Single();
        break;
    case 7:
        user_Regist_Batch();

        break;
    case 8:
        user_Delete_Batch();
        break;
    case 9:

        user_Permit();
        break;
    case 0:
        adminMenu();
        break;
    }

}