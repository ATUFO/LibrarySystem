#ifndef CONF_H_INCLUDED
#define CONF_H_INCLUDED
/* 配置文件  */
FILE *file;//
char bookfile[]="bookinfo.data";//文件存放位置
char userfile[]="userinfo.data";
int booksnum=0,id=0,userSum=0 ; //booksum为图书种数,usersum同  id为在库图书的最大id（方便为新书分配id）
int isAdmin=0;             //isAdmin 当前登陆用户是否是管理员
int itemInPage=20 ;        //每页显示的条目数量
int maxLandBookNum=10;   //每个用户最多借书的本数
#endif // CONF_H_INCLUDED
