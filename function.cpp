#include <iostream>
#include<cstdio>
#include<windows.h>
#include<stdlib.h>
#include<stack>
#include<math.h>
#include"book.h"
#include"function.h"
#include"user.h"
#include"conf.h"
using namespace std;
//===================初始化===============================================
book *the_BookLine_Head = (book *)malloc(sizeof(book)); //首节点
book *the_BookLine_Rear = (book *)malloc(sizeof(book)); //尾节点，实现双向链表
user *the_UserLine_Head = (user *)malloc(sizeof(user));
user *the_UserLine_Rear = (user *)malloc(sizeof(user));


user *userLogin = (user *)malloc(sizeof(user)); //已登陆的用户的地址


int bookDataBlockSize;//保存的单个图书数据块大小
int userDataBlockSize;//同上
//===================初 始化结束==============

//======================系统函数开始=====================================================================================
void welcome()//欢迎界面
{
    system("cls");
    isAdmin = 0;
    printf("===============================LibrarySystem Menu==========================================================\n");
    printf("                               [1] 用户登录\n");
    printf("                               [2] 管理员登录\n");
    printf("                               [3] 注册\n");
    int cmd;
    scanf("%d", &cmd);
    char name[16], pwd[16];
    switch(cmd)
    {
    case 1:
        system("cls");
        printf("===============================LibrarySystem Menu======================================================\n");
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
        printf("===============================LibrarySystem Menu===================================\n");
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



void userMenu()//用户菜单
{
    system("cls");
    printf("===============================LibrarySystem Menu===================================\n");
    printf("当前共 %d 种书在馆\n", booksnum);
    printf("[1] 浏览图书\n");
    printf("[2] 搜索图书\n");
    printf("[3] 借书\n");
    printf("[4] 还书\n");
    printf("[5] 借书记录\n");
    printf("[6] 密码更改\n");
    printf("[0] 登出\n");
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
    printf("===============================LibrarySystem Menu===================================\n");
    printf("当前共 %d 种书在馆，%d位用户\n", booksnum, userSum);
    printf("[1] 图书管理\n");
    printf("[2] 用户管理\n");
    printf("[0] 登出\n");
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
    printf("===============================LibrarySystem Menu===================================\n");
    printf("当前共 %d 种书在馆\n", booksnum);
    printf("[1] 浏览图书\n");
    printf("[2] 搜索图书\n");
    printf("[3] 增加单本图书\n");
    printf("[4] 下架单本图书\n");//方法，整个链表重写
    printf("[5] 按时间批量下架图书\n");
    printf("[6] 修改图书信息\n");
    printf("[7] 导入图书数据\n");
    printf("[8] 导入失败，恢复数据\n");
    printf("[0] 返回\n");
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
    printf("===============================LibrarySystem Menu===================================\n");
    printf("共 %d 位用户\n", userSum);
    printf("[1] 浏览用户\n");
    printf("[2] 搜索用户\n");
    printf("[3] 用户借书记录\n");
    printf("[4] 更改用户密码\n");
    printf("[5] 注册用户\n");
    printf("[6] 删除用户\n");
    printf("[7] 批量注册用户\n");
    printf("[8] 批量删除用户\n");
    printf("[9] 用户权限更改\n");
    printf("[0] 退出\n");
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

//======================系统函数结束===================================================================================


//======================书本函数开始========================================================================================

void updatabooks()//更新书
{
    system("cls");

    printf("输入待修改图书的id：\n");

    int theid;
    scanf("%d", &theid);
    getchar();
    book* pre = the_BookLine_Head->next; //从首节点开始
    while(pre)   //访问下下节点，因为当时是单链表，方便操作
    {
        if(pre->id == theid)
        {
            printf("========================图书信息================================\n");
            printf("%-8s%-20s%-20s%-20s\n", "ID", "书名", "作者", "ISBN");
            printf("%-8d%-20s%-20s%-20s\n", pre->id, pre->name, pre->authou, pre->ISBN);
            char q;
            printf("是否更改 [Y]/[N] ?\n");
            scanf("%c", &q);
            if(q == 'Y')
            {
                printf("输入书名：\n");
                cin >> pre->name;
                printf("输入作者：\n");
                cin >> pre->authou;
                printf("输入价格：\n");
                cin >> pre->price;
                printf("分类：\n");
                cin >> pre->classfiy;
                printf("输入出版社：\n");
                cin >> pre->press;
                printf("输入出版时间：\n");
                cin >> pre->time;
                rewriteAll_BookData();//重新写入所有节点
            }
            bookAdminMenu();//返回
        }
        pre = pre->next;
    }


}
void addbooks()//添加书
{
    system("cls");

    book *t = (book *)malloc(sizeof(book));

    printf("输入书名：\n");
    cin >> t->name;
    printf("输入作者：\n");
    cin >> t->authou;
    printf("输入价格：\n");
    cin >> t->price;
    printf("分类：\n");
    cin >> t->classfiy;
    printf("输入出版社：\n");
    cin >> t->press;
    printf("输入出版时间：\n");
    cin >> t->time;
    printf("输入ISBN：\n");
    cin >> t->ISBN;
    printf("输入图书位置：\n");
    cin >> t->pos;
    printf("输入库存：\n");
    cin >> t->numAll;
    t->numInLibrary = t->numAll;
    t->id = ++id; //分配id

    booksnum++;

    file = fopen(bookfile, "ab+");
    fwrite(t, bookDataBlockSize, 1, file); //追加数据

    t->next = the_BookLine_Head->next; //配置节点
    the_BookLine_Head->next->before = t;
    t->before = the_BookLine_Head;
    the_BookLine_Head->next = t;
    // rewriteAll_BookData();
    fclose(file);

    printf("添加成功，1s返回");
    Sleep(1000);
    bookAdminMenu();
}
void user_LendBook()//借书
{
    int id;
    system("cls");
    printf("输入要借的书的ID：\n");
    scanf("%d", &id);
    getchar();
    book *goal = findBookbyId(id);

    if(goal == NULL)
    {
        printf("没有该书\n");
        getchar();
        getchar();
        userMenu();
    }
    else
    {
        if(goal->numInLibrary == 0)
        {
            printf("已经借完了\n");
            getchar();
            getchar();
            userMenu();
        }
        else
        {
            showBookDetails(goal);
            printf("是否借阅？[Y]/[N]\n");
            char t;
            scanf("%c", &t);
            if(t == 'Y')
            {
                if(userLogin->lendNum >= maxLandBookNum )   //用户最多借10本书
                {
                    printf("你不能再借更多的书了\n");
                    getchar();
                    getchar();
                    userMenu();
                }
                user *theUser = findUser(userLogin->name);
                theUser->lendedBookId[theUser->lendNum++] = id;

                goal->numInLibrary--;
                printf("《%s》成功借出\n", goal->name);
                rewriteAll_BookData();
                rewriteAll_UserData();
                getchar();
                getchar();
                userMenu();
            }
            userMenu();
        }
    }


}

void user_ReturnBook()//还书
{
    int id;
    system("cls");
    printf("输入要还的书的ID：\n");
    scanf("%d", &id);
    book *goal = findBookbyId(id);

    if(goal == NULL)
    {
        printf("输入错误，或该书已下架\n");
        getchar();
        getchar();
        isAdmin ? adminMenu() : userMenu();
    }
    else
    {
        int i = 1;
        user *theUser = findUser(userLogin->name);
        for( i = 0; i < 10; i++)
        {
            if(theUser->lendedBookId[i] == id)
            {
                theUser->lendedBookId[i] = 0;

                for(int j = i; j < 9; j++)
                {
                    theUser->lendedBookId[j] = theUser->lendedBookId[j + 1];
                }//数组移位  去 0 ；

                theUser->lendNum--;
                break;
            }
        }
        if(i == 10)   //i==10时 意为没有找到该用户借过该书
        {
            printf("还书失败\n");
            getchar();
            getchar();
        }
        else
        {
            goal->numInLibrary++;
            printf("《%s》成功归还\n", goal->name);
            rewriteAll_BookData();
            rewriteAll_UserData();
            getchar();
            getchar();

        }

    }
    userMenu();
}
void deletebooks()//删除图书
{
    system("cls");
    printf("输入待删除图书的id：\n");
    int theid;//目标图书Id
    scanf("%d", &theid);
    getchar();

    book* pre = the_BookLine_Head; //从首节点开始
    while(pre->next->next)   //访问下下节点，因为当时是单链表，方便操作
    {
        if(pre->next->id == theid)   //找到图书
        {
            showBookDetails(pre->next);
            printf("确认删除%s？[Y]/[N]\n", pre->next->name);
            char t;
            scanf("%c", &t);
            if(t == 'Y')
            {
                if(pre->next->numAll - pre->next->numInLibrary > 0)
                {
                    printf("有人未还书，无法删除\n");
                    getchar();
                    getchar();
                    bookAdminMenu();
                }
                book *e = pre->next;
                pre->next = pre->next->next;
                pre->next->before = pre;
                free(e);//释放被删除的图书的内存

                rewriteAll_BookData();//重新写入所有节点
                booksnum--;

                bookAdminMenu();
            }

        }
        pre = pre->next;
    }
    printf("没有找到该书,按回车继续\n");
    getchar();
    bookAdminMenu();

}
void deletebooks_Batch()
{
    system("cls");
    printf("下架某时间之前的图书\n输入时间 如1950/4/3\n");
    char time[15];
    int cnt = 0;
    scanf("%s", time);
    book *pre = the_BookLine_Head->next;
    while(pre->next)
    {
        book *nxt = pre->next;
        if(strcmp(pre->time, time) <= 0)
        {
            book *t = pre;
            pre->before->next = pre->next;
            pre->next->before = pre->before;
            booksnum--;
            cnt++;
            free(t);
        }
        pre = nxt;
        rewriteAll_BookData();
    }
    printf("成功删除 %d 本书\n", cnt);
    getchar();
    getchar();
    bookAdminMenu();

}
void showBookDetails(book* thebook)//输出书目详细信息
{
    printf("==================================================================图书信息==================================================================\n");
    printf("%-8s%-20s%-16s%-25s%-8s%-13s%-12s%-15s%-15s%-10s\n", "ID", "书名", "作者", "出版社", "价格", "出版时间", "分类", "ISBN", "位置", "在馆/总计");
    printf("%-8d%-20s%-16s%-25s%-8.2f%-13s%-12s%-15s%-15s%-d/%d\n", thebook->id, thebook->name, thebook->authou, thebook->press, thebook->price, thebook->time, thebook->classfiy, thebook->ISBN, thebook->pos, thebook->numInLibrary, thebook->numAll);

}
void rewriteAll_BookData()//数据文本重写
{

    file = fopen(bookfile, "wb+");
    book* pre = the_BookLine_Rear->before; //从尾节点开始，为了下次初始化时，最新添加的书在最上面
    while(pre->before)
    {
        fwrite(pre, bookDataBlockSize, 1, file);
        pre = pre->before;
    }
    fclose(file);
}
void queryAllbooks()//查询所有图书
{
    showBooksList(the_BookLine_Head, booksnum, true);
}
void importbooks()
{
    system("cls");
    getchar();
    printf("导入图书说明:\n");
    printf(" 1.图书入库信息文件(bookimport.prn)通过 Excel 导出。导出方式为 < 带格式文本文件(空格分隔) .prn  >  \n");
    printf(" 2.数据格式为[书名] [作者] [出版社] [价格] [出版日期] [分类] [入库数目] [ISBN] [位置]\n");
    printf(" 3.将图书信息文件放在程序运行目录下\n");
    printf("\n\n是否导入？[Y]/[N]\n");
    char cmd;
    scanf("%c", &cmd);
    getchar();

    if(cmd == 'Y')
    {
        FILE *importfile = fopen("bookimport.prn", "r");
        if(importfile == NULL)
        {
            printf("打开文件失败\n");
            getchar();
            bookAdminMenu();
        }//判断导入文件是否存在
//备份
        void *buf = malloc(1);//缓冲区
        FILE *data = fopen("bookinfo.data", "rb");
        FILE *backup = fopen("bookinfo.bak", "wb");
        while(fread(buf,  1, 1, data) != NULL)
        {
            fwrite(buf, 1, 1, backup);
        }
        free(buf);
        fclose(data);
        fclose(backup);

//开始导入
        data = fopen("bookinfo.data", "ab+");
        int cnt = 0;
        book bk;
        double p;
        while(!feof(importfile))
        {
            cnt++;
            fscanf(importfile, "%s%s%s%lf%s%s%d%s%s", bk.name, bk.authou, bk.press, &bk.price, bk.time, bk.classfiy, &bk.numAll, bk.ISBN, bk.pos);
            bk.numInLibrary = bk.numAll ;
            bk.id = ++id;
            fwrite(&bk, bookDataBlockSize, 1, data);
        }
        fclose(importfile);
        fclose(data);
        printf("成功导入 %d 本书\n", cnt);
//导入结束

        getchar();
        booksnum = 0;
        bookline_Init();
        bookAdminMenu();
    }
    else
    {
        bookAdminMenu();
    }
}
void book_data_recover()
{
    FILE *backup = fopen("bookinfo.bak", "rb");
    if(backup == NULL)
    {
        printf("打开文件失败\n");
        getchar();
        getchar();
        bookAdminMenu();
    }
    //开始恢复
    void *buf = malloc(1);
    FILE *data = fopen("bookinfo.data", "wb");
    while(fread(buf,  1, 1, backup) != NULL)
    {
        fwrite(buf, 1, 1, data);
    }

    fclose(data);
    fclose(backup);
    free(buf);//释放资源

    booksnum = 0;
    bookline_Init();
    printf("恢复完成\n");
    getchar();
    getchar();
    bookAdminMenu();

}

void querySomebooks()//模糊查询
{
    //   查询方式 [0]id [1]书名 [2]作者 [3]出版社 [4]分类 [5]时间
    system("cls");
    printf("搜索方式：\n [1] ID\n [2] 书名\n [3] 作者\n [4] 出版社\n [5] 分类\n [6] 时间\n\n[0] 返回\n");
    int method ;
    scanf("%d", &method);

    getchar();
    int goal, isSearch = 0, searchedNum = 0;
    char con[20];
    printf("输入关键词\n");
    book *queryHead = (book *)malloc(sizeof(book));
    queryHead->next = NULL;
    if(method == 0)
        isAdmin ? bookAdminMenu() : userMenu();
    if(method == 1)
        scanf("%d", &goal);
    else
        scanf("%s", con); //int 和 string两种方式

    book *pre = the_BookLine_Rear->before;

    while(pre->before)
    {
        switch(method)
        {
        case 1:
            if(goal == pre->id)
            {
                searchedNum++;
                book *t = (book *)malloc(sizeof(book));
                isSearch = 1;
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;
        case 2:
//            printf("%s  %s\n",pre->name,con);
            if(strstr(pre->name, con))
            {
                isSearch = 1;
                searchedNum++;
                book *t = (book *)malloc(sizeof(book));
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;
        case 3:
            if(strstr(pre->authou, con))
            {
                isSearch = 1;
                searchedNum++;
                book *t = (book *)malloc(sizeof(book));
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;

        case 4:
            if(strstr(pre->press, con))
            {
                isSearch = 1;
                searchedNum++;
                book *t = (book *)malloc(sizeof(book));
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;
        case 5:
            if(strstr(pre->classfiy, con))
            {
                isSearch = 1;
                searchedNum++;
                book *t = (book *)malloc(sizeof(book));
                *t = *pre;
                t->next = queryHead->next;
                queryHead->next = t;
            }
            break;
        case 6:
            if(strstr(pre->time, con))
            {
                isSearch = 1;
                searchedNum++;
                book *t = (book *)malloc(sizeof(book));
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
        showBooksList(queryHead, searchedNum, false);
    }

    else
        printf("没有搜到\n");
    getchar();
    getchar();
    isAdmin ? bookAdminMenu() : userMenu();

}
void showBooksList(book *thehead, int searchNum, bool isTheMianLine) //输出书的列表
/*
参数分别为  链表的头节点   列表的数目    是否是主链表，如果不是则在最后释放所有节点；
*/
{

    book* pre = thehead->next; //pre为驱动指针
    int pageNow = 1; //当前页数
    stack<book *> pagestack;//用于存放页数的栈，准确的说是每页的第一条的指针
    pagestack.push(pre );
    while(1)
    {

        int i = itemInPage, pagesNum = ceil(1.0 * searchNum / itemInPage); //item计数器
        book* pagefirtItem = pagestack.top(); //当前页第一个条目
        pre = pagefirtItem;

        system("cls");
        printf("==================================================================图书信息===============================================================\n");
        printf("共计%d种图书\n", searchNum);
        printf("%-8s%-20s%-16s%-25s%-8s%-13s%-12s%-15s%-15s%-10s\n", "ID", "书名", "作者", "出版社", "价格", "出版时间", "分类", "ISBN", "位置", "在馆/总计");

        while((pre && pre != the_BookLine_Rear) && --i >= 0)
        {
            printf("%-8d%-20s%-16s%-25s%-8.2f%-13s%-12s%-15s%-15s%-d/%d\n", pre->id, pre->name, pre->authou, pre->press, pre->price, pre->time, pre->classfiy, pre->ISBN, pre->pos, pre->numInLibrary, pre->numAll);
            pre = pre->next;

        }

        //==============================翻页菜单
        if(i > 0)
            while(i--    )        //  |
                printf("\n");//控制菜单位置
        printf("\n");
        printf("[1]上一页 [2]下一页 [3]跳转 [4]退出          第 %d / %.0f 页\n", pageNow, ceil(1.0 * searchNum / itemInPage));
        int cmd;
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

            printf("%x", pre);

            if(pre == NULL || pre == the_BookLine_Rear) //判断是否是最后一页
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

//================翻页菜单结束

        }
        if(cmd == 4)
        {
            if(!isTheMianLine)
                freenode_Book(thehead);
            isAdmin ? bookAdminMenu() : userMenu();
        }

    }
}
book *findBookbyId(int id)//按id寻找图书
{
    book *pre = the_BookLine_Head->next;
    while(pre != the_BookLine_Rear)
    {
        if(pre->id == id)
            return pre;
        pre = pre->next;
    }

    return NULL;

}


void freenode_Book(book *thehead)//释放节点
{

    book* pre = thehead->next; //从首节点（空节点）的next开始访问
    free(thehead);
    while(pre )
    {
//        printf("Log FreeNode ");
//        getchar();
        book *t = pre;
        pre = pre->next;
        free(t);
    }
    free(pre);
}



//  ======================书本函数结束================================================================================


//  ======================用户函数开始===================================================================================

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
    printf("搜索方式：\n [1] 用户名 \n [2] 管理员[Y]/[N] \n [3] 已借书的ID\n [0] 返回\n");
    int method ;
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
//======================用户函数结束=================================

