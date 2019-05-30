#include <iostream>

#include "LL1.h"

using namespace std;

int main()
{   FILE *fpin;//用于打开文件
    string file_name;//文件名
    LL1 ll1 ;
    //cout << "请输入文件名：";
    //cin >> file_name;//获取输入的文件名
    fpin=fopen("C:\\Users\\NASA\\Desktop\\C4.TXT", "r");//根据文件名打开文件，打开方式是读取
    if (!fpin)//如果没有找到这个文件
    {
       cerr << "打开文件" << file_name << "失败" << endl;
        return -1;
    }
    ll1.getResult(fpin); //输入需要分析的文件
    
    return 0;
}

