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
#include"record.h"
using namespace std;
void addrecord(char *userName, int bookid, int method)//增加记录
{
    record *t = (record *)malloc(sizeof(record));
    t->next = the_RecordLine_Head->next;
    the_RecordLine_Head->next = t;
    strcpy(t->userName, userName);
    t->bookID = bookid;
    t->method = method;
    t->id = ++recordNum;
    strcpy(t->time, getTime());
    FILE *rfile = fopen(recordfile, "ab+");
    fwrite(t, recordDataBlockSize, 1, rfile);
    fclose(rfile);
}
void showRecordsingle(record *record, int cnt)//单条显示
{
    char bookname[100];
    book *book = findBookbyId(record->bookID);
    strcpy(bookname, book ->name);
    char tem = bookname[showlen];
    bookname[showlen] = '\0';
    printf("%-14d%-18s%-18s%-35s%-18d%-25s\n", cnt, record->userName, record->method ? "   还" : "借  ", bookname, book->id, record->time);
    bookname[showlen] = tem;

}
void showRecordsList(record *thehead, int searchNum, bool isTheMianLine) //以链的方式显示列表
{


    record* pre = thehead->next; //pre为驱动指针
    int pageNow = 1; //当前页数
    stack<record *> pagestack;//用于存放页数的栈，准确的说是每页的第一条的指针
    pagestack.push(pre );
    while(1)
    {

        int i = itemInPage, pagesNum = ceil(1.0 * searchNum / itemInPage); //item计数器
        record* pagefirtItem = pagestack.top(); //当前页第一个条目
        pre = pagefirtItem;
        system("cls");
        printf("==========================================================================================================================================================================\n");
        printf("                                                          图书管理系统-借书记录(总览)                                                                                                             \n");
        printf("==========================================================================================================================================================================\n\n");
        printf("共%d 条记录\n\n", searchNum);
        printf("%-14s%-18s%-18s%-35s%-18s%-25s\n\n", "序号", "用户", "借/还", "书名", "书ID", "时间");
        while(pre && --i >= 0)
        {
            showRecordsingle(pre, pre->id);
            pre = pre->next;
        }

        //==============================翻页菜单
        if(i > 0)
            while(i--    )        //  |
                printf("\n");//控制菜单位置
        printf("\n");

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

            if(pre == NULL ) //判断是否是最后一页
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

        if(cmd == 0)
        {
            //if(!isTheMianLine)
            //freenode_Book(thehead);
            adminMenu();
        }

    }
}

void rewriteAll_RecordData()//将链表中所有数据重写入文件
{

    FILE *rfile = fopen(recordfile, "wb+");
    record* pre = the_RecordLine_Head->next; //从尾节点开始，为了下次初始化时，最新添加的书在最上面
    while(pre)
    {
        fwrite(pre, recordDataBlockSize, 1, rfile);
        pre = pre->next;
    }
    fclose(rfile);
}
