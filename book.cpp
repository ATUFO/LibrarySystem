#include <iostream>
#include<cstdio>
#include<windows.h>
#include<stdlib.h>
#include<stack>
#include<math.h>
#include"user.h"
#include"book.h"
#include"init.h"
#include"record.h"
#include"menu.h"
using namespace std;
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
             printf("==========================================================================================================================================================================\n");
    printf("                                                          图书管理系统 - 更新书本信息                                                                                                             \n");
    printf("==========================================================================================================================================================================\n\n");
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
void user_LendBook(book *thebook)//借书
{

    book *goal = thebook;

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
            getchar();

    printf("=====================================================图书信息=============================================================================================\n\n");
               printf("%-8s%-25s%-25s%-25s%-8s%-13s%-12s%-15s%-10s\n", "ID", "书名", "作者", "出版社", "价格", "出版时间", "分类", "ISBN", "在馆/总计");

            showBooksimple(goal);
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
                theUser->lendedBookId[theUser->lendNum++] = goal->id;
                goal->numInLibrary--;
                printf("《%s》成功借出\n", goal->name);
                addrecord(userLogin->name,goal->id,0);
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

void user_ReturnBook(book *thebook)//还书
{

    book *goal = thebook;

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
            if(theUser->lendedBookId[i] == goal-> id)
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
            addrecord(userLogin->name,goal->id,1);
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
            showBooksimple(pre->next);
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
void showBooksimple(book* pre)//输出书目简略信息
{

    char nametem = pre->name[showlen];
    pre->name[showlen] = '\0';
    char authortem = pre->authou[showlen];
    pre->authou[showlen] = '\0';
    char presstem = pre->press[showlen];
    pre->press[showlen] = '\0';
    printf("%-8d%-25s%-25s%-25s%-8.2f%-13s%-12s%-18s%-d/%d\n", pre->id, pre->name, pre->authou, pre->press, pre->price, pre->time, pre->classfiy, pre->ISBN, pre->numInLibrary, pre->numAll);
    pre->name[showlen] = nametem;
    pre->authou[showlen] = authortem;
    pre->press[showlen] = presstem;
}
void showBookdetail(book *thebook)//输出书目详细信息
{
    system("cls");
     printf("==========================================================================================================================================================================\n");
    printf("                                                          图书管理系统 - 图书详细信息                                                                                                             \n");
    printf("==========================================================================================================================================================================\n\n");
       printf("                                                        书名：      《%s》\n\n", thebook->name);
    printf("                                                        作者：      %s\n\n", thebook->authou);
    printf("                                                        分类:       %s\n\n", thebook->classfiy);
    printf("                                                        出版社：    %s\n\n", thebook->press);
    printf("                                                        出版时间：  %s\n\n", thebook->time);
    printf("                                                        ISBN:       %s\n\n", thebook->ISBN);
    printf("                                                        价格：      %.2lf\n\n", thebook->price);
    printf("                                                        在馆：  %d       馆藏数目：  %d\n", thebook->numInLibrary, thebook->numAll);
    printf("回车返回\n");
    getchar();
    getchar();
    return ;
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
void importbooks()//导入图书
{
    system("cls");
    getchar();
    printf("导入图书说明:\n");
    printf(" 1.图书入库信息文件(bookimport.prn)通过 Excel 导出。\n");
    printf(" 2.数据格式为[书名] [作者] [出版社] [价格] [出版日期] [分类] [ISBN][入库数目]   以空格分隔 \n");
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
            fscanf(importfile, "%s%s%s%lf%s%s%s%d", bk.name, bk.authou, bk.press, &bk.price, bk.time, bk.classfiy, bk.ISBN, &bk.numAll);
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
void book_data_recover()//恢复图书
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

void querySomebooks()//搜索图书
{
    //   查询方式 [0]id [1]书名 [2]作者 [3]出版社 [4]分类 [5]时间
    system("cls");
     printf("==========================================================================================================================================================================\n");
    printf("                                                          图书管理系统 - 图书搜索                                                                                                             \n");
    printf("==========================================================================================================================================================================\n\n");
       printf("                                                      搜索方式：\n\n");
    printf("                                                                [1] ID\n\n");
    printf("                                                                [2] 书名\n\n");
    printf("                                                                [3] 作者\n\n");
    printf("                                                                [4] 出版社\n\n");
    printf("                                                                [5] 分类\n\n");
    printf("                                                                [6] 时间\n\n");
    printf("                                                                [7] 已借出图书\n\n");
    printf("                                                                [0] 返回\n\n");
    int method ;
    scanf("%d", &method);

    getchar();
    int goal, isSearch = 0, searchedNum = 0;
    char con[20];
    if(method == 0)
        isAdmin ? bookAdminMenu() : userMenu();
    if(method != 7)
    {
        printf("输入关键词\n");
        if(method == 1)
            scanf("%d", &goal);
        else
            scanf("%s", con); //int 和 string两种方式
    }


    book *queryHead = (book *)malloc(sizeof(book));
    queryHead->next = NULL;


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
        case 7:
            if(pre->numAll - pre->numInLibrary > 0)
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
         printf("==========================================================================================================================================================================\n");
    printf("                                                          图书管理系统 - 图书信息                                                                                                             \n");
    printf("==========================================================================================================================================================================\n\n");
          printf("\n\n共计%d种图书\n\n", searchNum);
        printf("%-8s%-25s%-25s%-25s%-8s%-13s%-12s%-15s%-10s\n\n", "ID", "书名", "作者", "出版社", "价格", "出版时间", "分类", "ISBN", "在馆/总计");

        while((pre && pre != the_BookLine_Rear) && --i >= 0)
        {
            showBooksimple(pre);

            pre = pre->next;

        }

        //==============================翻页菜单
        if(i > 0)
            while(i--    )        //  |
                printf("\n");//控制菜单位置
        printf("\n");
        if(!isAdmin)
        {
            printf("[1]上一页 [2]下一页 [3]跳转 [4]书本详细信息 [5] 借书 [6] 还书  [0]退出          第 %d / %.0f 页\n", pageNow, ceil(1.0 * searchNum / itemInPage));

        }
        else
            printf("[1]上一页 [2]下一页 [3]跳转 [4]书本详细信息   [0]退出          第 %d / %.0f 页\n", pageNow, ceil(1.0 * searchNum / itemInPage));
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
            int id;
            printf("输入要查看的书本 ID :\n");
            scanf("%d", &id);
            book *t = findBookbyId(id);
            if(t != NULL)
            {
                showBookdetail(t);
            }
            else
            {
                printf("查询失败,回车继续\n");
                getchar();
            }
        }
        if(!isAdmin){
             if(cmd == 5)
        {

            int id;
            printf("输入要借的书本 ID :\n");
            scanf("%d", &id);
            book* tt = findBookbyId(id);
            user_LendBook(tt);
        }
        if(cmd == 6)
        {
            int id;
            printf("输入要归还的书本 ID :\n");
            scanf("%d", &id);
            book* tt = findBookbyId(id);
            user_ReturnBook(tt);
        }
        }


        if(cmd == 0)
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
