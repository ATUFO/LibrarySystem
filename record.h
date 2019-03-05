#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

typedef struct arecord
{
    char userName[16];
    int bookID;
    char time[20];
    int method;
    arecord * next;
} record;
void addrecord(char *userName,int bookid,int method);
void showRecordsList(record *thehead, int searchNum, bool isTheMianLine); //以链的方式显示列表
void freenode_Record(record *thehead);//释放节点
void queryAllRecords();//查询所有用户
void showRecordsingle(record *record,int cnt);
void rewriteAll_RecordData();//将链表中所有数据重写入文件
#endif // RECORD_H_INCLUDED
