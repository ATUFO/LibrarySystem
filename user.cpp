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
int login(char *name, char *pwd, int loginAsAdmin)
{
    //注意身份鉴权
    user *theuser = findUser(name);
    if(theuser == 0 || strcmp(pwd, theuser->passWord))
    {
        return 0;
    }
    userLogin = theuser;
    isAdmin = loginAsAdmin;
    return 1;
}

void regist()
{
    user *newUser = (user *)malloc(sizeof(user));
    char name[16], pwd[16];
    system("cls");
    printf("===============================LibrarySystem Regist===================================\n");
    printf("输入账号\n");
    scanf("%s", newUser->name);
    while(findUser(newUser->name) != 0)
    {
        printf("账号已存在,输入账号\n");
        scanf("%s", newUser->name);
    }
    printf("输入密码\n");
    scanf("%s", newUser->passWord);
    newUser->isAdminstrator = 0;
    newUser->lendNum = 0;
    file = fopen(userfile, "ab+");


    newUser->next = the_UserLine_Head->next; //配置节点
    the_UserLine_Head->next->before = newUser;
    newUser->before = the_UserLine_Head;
    the_UserLine_Head->next = newUser;
    fwrite(newUser, userDataBlockSize, 1, file); //追加数据
    fclose(file);
    userSum++;
    isAdmin ? userAdminMenu() : welcome();
}


user * findUser(char *name)
{
    user *pre = the_UserLine_Head->next;
    while(pre->next)
    {
        if(!strcmp(pre->name, name))
            return pre;
        pre = pre->next;
    }
    return 0;
}
void queryAllUsers()
{
    showUsersList(the_UserLine_Head, userSum, true);
}
void user_Query_Some()
{
    char name[16];
    system("cls");
    printf("================================================================LibrarySystem Menu=======================================================================================\n");
    printf("                                                      搜索方式：\n\n");
    printf("                                                                [1] 用户名\n\n");
    printf("                                                                [2] 管理员\n\n");
    printf("                                                                [3] 已借书的ID\n\n");
    printf("                                                                [0] 返回\n\n");
    int method;
    scanf("%d", &method);
    if(method == 0)
        userAdminMenu();
    getchar();
    int goal, isSearch = 0, searchedNum = 0;
    char con[20];
    printf("输入关键词\n");
    user *queryHead = (user *)malloc(sizeof(user));
    queryHead->next = NULL;

    if( method == 3)
        scanf("%d", &goal);
    else
        scanf("%s", con); //int 和 string两种方式
    if(con[0] == 'Y')
        goal = 1;
    else
        goal = 0;
    user *pre = the_UserLine_Rear->before;

    while(pre->before)
    {
        switch(method)
        {

        case 1:
//            printf("%s  %s\n",pre->name,con);
            if(strstr(pre->name, con))
            {
                isSearch = 1;
                searchedNum++;
                user *t = (user *)malloc(sizeof(user));
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;
        case 2:

            if(goal == pre->isAdminstrator)
            {
                searchedNum++;
                user *t = (user *)malloc(sizeof(user));
                isSearch = 1;
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;

        case 3://
            int flag = 0;
            for(int i = 0; i < pre->lendNum; i++)
            {
                if(pre->lendedBookId[i] == goal)
                {
                    flag = 1;
                    break;
                }
            }
            if(flag)
            {
                isSearch = 1;
                searchedNum++;
                user *t = (user *)malloc(sizeof(user));
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;

        }
        pre = pre->before;
    }
    if(isSearch)
    {
        showUsersList(queryHead, searchedNum, false);
    }

    else
        printf("没有搜到\n");
    getchar();
    getchar();
    userAdminMenu();
}
void user_Lend_Book_Record(char* name)
{
    user *theUser = findUser(name);
    if(theUser)
    {
        book* lendBook_Head = (book *)malloc(sizeof(book));
        lendBook_Head->next = NULL;
        int i = theUser->lendNum;
        while(i--)
        {
            book* t = (book *)malloc(sizeof(book));
            *t = *findBookbyId(theUser->lendedBookId[i]);
            t->next = lendBook_Head->next;
            lendBook_Head->next = t;
        }
        showBooksList(lendBook_Head, theUser->lendNum, false);
        isAdmin ? userAdminMenu() : userMenu();
    }
    else
    {

        printf("没有该用户\n");
        getchar();
        getchar();
        isAdmin ? userAdminMenu() : userMenu();
    }

}


void user_Change_Password(char* name)
{
    printf("输入新密码\n");
    user * theUser = findUser(name);
    char password[16];
    scanf("%s", password);
    if(theUser != NULL)
    {
        strcpy(theUser->passWord, password);

        printf("%s密码更改成功\n新密码为%s\n", theUser->name, theUser->passWord);
        rewriteAll_UserData();
        getchar();
        getchar();
        isAdmin ? userAdminMenu() : userMenu();
    }
    else
    {
        printf("失败\n");
        getchar();
        isAdmin ? userAdminMenu() : userMenu();
    }

}
void user_Delete_Single()
{
    system("cls");
    printf("输入待删除用户的用户名：\n");
    char name[16];
    scanf("%s", &name);
    getchar();
    user* pre = the_UserLine_Head->next; //从首节点开始
    while(pre)   //访问下下节点，因为当时是单链表，方便操作
    {
        if(!strcmp(pre->name, name))   //找到图书
        {
            if(pre->lendNum == 0)
            {
                //细节展示
                printf("%-20s%-20s%-20s%-5s\n", "用户名", "密码", "管理员", "共借书");
                printf("%-20s%-20s%-20s%-5d\n", pre->name, pre->passWord, pre->isAdminstrator ? "是" : " ", pre->lendNum);
                printf("确认删除%s？[Y]/[N]\n", pre->name);
                char t;
                scanf("%c", &t);
                if(t == 'Y')
                {
                    user *e = pre;
                    pre->before->next = pre->next;
                    pre->next->before = pre->before;
                    free(e);//释放被删除的图书的内存
                    rewriteAll_UserData();//重新写入所有节点
                    userSum--;

                    userAdminMenu();
                }

            }
            else
            {
                printf("%s 还有书未还，删除失败\n", pre->name);
                getchar();
                userAdminMenu();
            }

        }

        pre = pre->next;
    }
    printf("没有找到用户,按回车继续\n");
    getchar();
    userAdminMenu();

}

void user_Regist_Batch()//批量注册用户
{
    system("cls");
    int newUserNum, startNum;
    printf("输入要添加的用户数目\n");
    scanf("%d", &newUserNum);
    printf("输入用户名前缀\n");
    getchar();
    char perName[6];
    scanf("%s", perName);
    printf("输入用户名后缀开始数字\n");
    scanf("%d", &startNum);
    file = fopen(userfile, "ab+");
    for(int i = startNum; i < newUserNum + startNum; i++)
    {
        char name[16], last[10];
        sprintf(last, "%06d", i);
        strcpy(name, perName);
        strcat(name, last);   //合成用户名
        if(findUser(name))     //判断是否存在
        {
            system("cls");
            printf("已存在，请更换前缀或后缀");
            getchar();
            getchar();
            break;
        }
        //分配内存，并写入文件
        user *newUser = (user *)malloc(sizeof(user));
        strcpy(newUser->name, name);
        strcpy(newUser->passWord, "1");
        newUser->isAdminstrator = 0;
        newUser->lendNum = 0;
        newUser->next = the_UserLine_Head->next; //配置节点
        the_UserLine_Head->next->before = newUser;
        newUser->before = the_UserLine_Head;
        the_UserLine_Head->next = newUser;
        fwrite(newUser, userDataBlockSize, 1, file); //追加数据

        userSum++;
    }
    fclose(file);
    userAdminMenu();
}
void user_Delete_Batch()
{
    system("cls");
    int endNum, startNum;

    printf("输入删除用户名前缀\n");
    getchar();
    char perName[6];
    scanf("%s", perName);
    printf("输入用户名后缀开始数字\n");
    scanf("%d", &startNum);
    printf("输入用户名后缀结束数字\n");
    scanf("%d", &endNum);
    int i, delNum = 0;
    for(i = startNum; i <= endNum; i++)
    {
        char name[16], last[10];
        sprintf(last, "%06d", i);
        strcpy(name, perName);
        strcat(name, last);
        user *t;//=(user *)malloc( sizeof(user));
        t = findUser(name);

        if(t)
        {
            if(t->lendNum == 0)
            {
                delNum++;
                t->before->next = t->next;
                t->next->before = t->before;
                free(t);
                userSum--;
            }
            else
            {
                printf("%s 还有书未还，删除失败\n", t->name);
            }
        }
        else
        {
            printf("%s 不存在，删除失败\n", name);
            continue;

        }
    }


    printf("共删除 %d 位用户\n", delNum);
    getchar();
    getchar();

    rewriteAll_UserData();
    userAdminMenu();

}

void user_Permit()

{
    char name[16];
    system("cls");
    printf("输入要更改权限的账号\n");
    scanf("%s", name);
    getchar();
    user * theUser = findUser(name);
    if(theUser != NULL)
    {
        theUser->isAdminstrator = !theUser->isAdminstrator;
        rewriteAll_UserData();
        printf("%s权限更改成功，现在是%s\n", theUser->name, theUser->isAdminstrator ? "管理员" : "普通用户");
        getchar();
        userAdminMenu();
    }
    else
    {
        printf("权限更改失败\n");
        getchar();
        userAdminMenu();
    }

}//提权

void showUsersList(user *thehead, int searchNum, bool isTheMianLine)
{
    user* pre = thehead->next; //pre为驱动指针
    int pageNow = 1; //当前页数
    stack<user *> pagestack;//用于存放页数的栈，准确的说是每页的第一条的指针
    pagestack.push(pre );
    while(1)
    {

        int i = itemInPage, pagesNum = ceil(1.0 * searchNum / itemInPage); //item计数器
        user* pagefirtItem = pagestack.top(); //第一个条目
        pre = pagefirtItem;

        system("cls");
        printf("==========================================用户列表============================================\n");
        printf("共计%d人\n", searchNum);
        printf("%-20s%-20s%-20s%-5s\n", "用户名", "密码", "管理员", "共借书");

        while((pre && pre != the_UserLine_Rear) && --i >= 0)
        {
            printf("%-20s%-20s%-20s%-5d\n", pre->name, pre->passWord, pre->isAdminstrator ? "是" : " ", pre->lendNum);
            pre = pre->next;
        }

        /*==============================翻页菜单开始===================

        */
        if(i > 0)             //  |
            while(i--)        //  |
                printf("\n");//控制菜单位置
        printf("\n");
        printf("[1]上一页 [2]下一页 [3]跳转 [4]退出          第 %d / %.0f 页\n", pageNow, ceil(1.0 * searchNum / itemInPage));
        int  cmd;
        scanf("%d", &cmd);
        if(cmd == 1)
        {
            if(pagestack.top() == thehead->next) //用该页的第一项是否是除首节点的第一个节点判断是否已经是第一页
                continue;
            pagestack.pop();//翻页
            pageNow--;
            continue;
        }
        if(cmd == 2)
        {
            if(pre == NULL || pre == the_UserLine_Rear) //判断是否是最后一页
                continue;
            pagestack.push(pre);//翻页
            pageNow++;
            continue;
        }
        if(cmd == 3)   //跳页
        {
            printf("跳转到(1-%d):\n", pagesNum);
            int goalPage;
            scanf("%d", &goalPage);
            if(goalPage <= 0 || goalPage > pagesNum)
            {
                printf("输入错误,回车继续\n");
                getchar();
                getchar();
                continue;
            }
            else
            {

                if(goalPage > pageNow)
                {
                    pre = pagefirtItem;
                    for(int j = pageNow; j < goalPage; j++)
                    {
                        i = itemInPage;

                        while(--i >= 0)
                        {
                            pre = pre->next;
                        }
                        pagestack.push(pre);
                        pageNow++;

                    }
                    continue;
                }
                else if(goalPage < pageNow)
                {
                    for(int j = pageNow; j > goalPage; j--)
                    {
                        pageNow--;
                        pagestack.pop();
                        pre = pagestack.top();

                    }
                    continue;
                }
                else
                    continue;
            }
        }
        if(cmd == 4)
        {
            if(!isTheMianLine)
                freenode_User(thehead);
            userAdminMenu();
        }
        /*================翻页菜单结束==========================*/
    }
}
void freenode_User(user *thehead)
{

    user* pre = thehead->next; //从首节点（空节点）的next开始访问
    free(thehead);
    while(pre)
    {
//        printf("Log FreeNode ");
//        getchar();
        user *t = pre;
        pre = pre->next;
        free(t);
    }
    free(pre);
}


void rewriteAll_UserData()
{
    file = fopen(userfile, "wb+");
    user* pre = the_UserLine_Rear->before; //从尾节点开始，为了下次初始化时，最新添加的书在最上面
    while(pre->before)
    {
        fwrite(pre, userDataBlockSize, 1, file);
        pre = pre->before;
    }
    fclose(file);

}
