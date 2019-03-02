#ifndef BOOK_H_INCLUDED
#define BOOK_H_INCLUDED
//书本
typedef struct b
{
    char name[40],ISBN[15],authou[25],press[20],time[15],classfiy[15],pos[20];
    //ISBN 失效   press 出版社   time出版时间  classfiy分类
    int numAll,numInLibrary;//numAll 总共本数   numInlibrary在管图书数目
    int id;//图书id
    double price;//价格

    b *before,*next;
} book;
#endif // BOOK_H_INCLUDED

