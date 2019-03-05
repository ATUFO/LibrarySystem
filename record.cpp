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
void addrecord(char *userName, int bookid, int method)
{
    record *t = (record *)malloc(sizeof(record));
    t->next = the_RecordLine_Head->next;
    the_RecordLine_Head->next = t;
    strcpy(t->userName, userName);
    t->bookID = bookid;
    t->method = method;
    strcpy(t->time, gettime());
    recordNum++;
    FILE *rfile = fopen(recordfile, "ab+");
    fwrite(t, recordDataBlockSize, 1, rfile);
    fclose(rfile);
}
void showRecordsingle(record *record, int cnt)
{
    char bookname[100];
    book *book = findBookbyId(record->bookID);
    strcpy(bookname, book ->name);
    char tem = bookname[showlen];
    bookname[showlen] = '\0';
    printf("%-6d%-25s%-25s%-26s%-10d%-23s\n", cnt, record->userName, record->method ? "还" : "借", bookname, book->id, record->time);
    bookname[showlen] = tem;

}
void showRecordsList(record *thehead, int searchNum, bool isTheMianLine) //以链的方式显示列表
{
    system("cls");
    printf("==============================================================LibrarySystem Menu============================================================================\n");
    printf("共%d 条记录\n\n", searchNum);
    printf("%-6s%-25s%-25s%-25s%-10s%-25s\n", "序号", "用户", "借/还", "书名", "书ID", "时间");
    record *pre = the_RecordLine_Head->next;
    int cnt = 1;
    while(pre)
    {
        showRecordsingle(pre, cnt);
        pre = pre->next;
    }
    getchar();
    getchar();
    adminMenu();
}
//void freenode_Record(record *thehead)//释放节点
//{
//
//}
//
//void queryAllRecords()//查询所有
//{
//
//}
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
