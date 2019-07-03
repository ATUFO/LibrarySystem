#ifndef BOOK_H_INCLUDED
#define BOOK_H_INCLUDED
//书本
typedef struct b
{
    char name[100], ISBN[40], authou[100], press[100], time[15], classfiy[15];
    //ISBN 失效   press 出版社   time出版时间  classfiy分类
    int numAll, numInLibrary; //numAll 总共本数   numInlibrary在管图书数目
    int id;//图书id
    double price;//价格

    b *before, *next;
} book;
void book_data_recover();
void importbooks();
void addbooks();//添加图书
void deletebooks();//删除图书
void deletebooks_Batch();
void updatabooks();//跟新图书
void queryAllbooks();//查询所有图书
void querySomebooks();//模糊查询
void showBooksList(book *thehead, int searchNum, bool isTheMainLine); //输出书的列表
book *findBookbyId(int id);//按id寻找图书
void showBooksimple(book *pre);//输出数目详细信息
void freenode_Book(book *thehead);//释放节点，配合模糊搜索
void showBookdetail(book *thebook);
void rewriteAll_BookData();//同上

#endif // BOOK_H_INCLUDED

