#include "LL1.h"


void LL1::CreatLL1Table()
{
    getFirstAndFollow(); //先调用该函数获得first集和follow集
	for (int index = 0; index < T; index++) {  									                    // 对于每个产生式(编号index):A->α
		int row = getNIndex(nonTerminals(productions[index].left));
		int emptyCount = 0;
		for (int i = 0; productions[index].right[i]!= "0"; i++) { // 1) 对FIRST(α)中的每个终结符号a,将index加入(A, a)中
			string tmp = productions[index].right[i];
			
			if (!isNonterminal(tmp) || tmp == "$") { // tmp是终结符
				if (tmp != "$")
					LL1Table[row][getIndex(tmp)] = index;
				if (tmp == "$") {
					emptyCount++;
				}
				break;
			}
			else {  // tmp是非终结符
				set<string>::iterator it;
				int tmpIndex = getNIndex(tmp);
				// 对FIRST(tmp)中的每个终结符号a,将i加入(A, a)中
				for (it = firstSet[tmpIndex].begin(); it != firstSet[tmpIndex].end(); it++) {
					LL1Table[row][getIndex(*it)] = index;
				}
				if (firstSet[tmpIndex].count("$") != 0) {      // 2) 如果空$在FIRST(tmp)中,继续看α中的下一个符号
					emptyCount++;
				}
				else {
					break;
				}
			}
		}
        // cout<< "cahngdu"<<rlen(productions[index].right)<<" ";
		// 2) 如果空$在FIRST(α)中,对FOLLOW(A)中的每个终结符或结束符b,将i加入(A,b)中
		if (emptyCount == rlen(productions[index].right)) {
			set<string>::iterator  it;
			for (it = followSet[row].begin(); it != followSet[row].end(); it++) {
				LL1Table[row][getIndex(*it)] = index;
			}
		}
	}
}

void LL1::analyExpression(struct token s[]) {
	/*int aa[1000];
	for(int i=0;i<1000;i++){
		aa[i]=0;
	} */
	ofstream mycout4("C:\\Users\\NASA\\Desktop\\分析过程.txt");
	vector<int> lineStack;
	
    for (int i = 0; s[i].w.tok!=ENDFILE; i++){ 
        leftExpr.push_back(tterminalNoEmpty(s[i].w.tok));
        lineStack.push_back(s[i].lineShow);
        //aa[i]=s[i].lineShow;
      //  cout<<tterminalNoEmpty(s[i].w.tok)<<endl;
    } 
    leftExpr.push_back("#");

    analyStack.push_back("#");
    analyStack.push_back(nonTerminals(nonterminal[0]));  // 加入开始符号

   while (analyStack.size() > 0) {
        //cout<<"分析栈：";
        string outs = "";
        for (int i = 0; i < analyStack.size(); i++)
            outs += analyStack[i];
        mycout4<<endl;
        mycout4<<"分析栈:"<<endl;
        mycout4<<"+++++++++++++++++++++++++++++++++++++"<<endl;
        mycout4 << setw(15) << outs << endl;
        mycout4<<"+++++++++++++++++++++++++++++++++++++"<<endl;

        //cout<<"剩余输入串：";
        outs = "";
        for (int i = 0; i < leftExpr.size(); i++)
            outs = outs + leftExpr[i] + "  ";
        mycout4<<endl;
        mycout4<<"剩余输入串:"<<endl;
		mycout4<<"*************************************"<<endl; 
        mycout4 << setw(15) << outs << endl;
        mycout4<<"*************************************"<<endl;

        // 匹配
        string string1 = analyStack.back();
        string string2 = leftExpr.front();
        if (string1 == string2 && string1 == "#") {
            mycout4 << setw(15) << "Accepted!" << endl;
            return;
        }
        if (string1 == string2) {
            analyStack.pop_back();
            leftExpr.erase(leftExpr.begin());
            lineStack.erase(lineStack.begin());
            mycout4  << "匹配：" << string1 << endl;
        }
        else if (LL1Table[getNIndex(string1)][getIndex(string2)] != -1) {  // 预测表中有推倒项，可进行推导
            int tg = LL1Table[getNIndex(string1)][getIndex(string2)];
            analyStack.pop_back();

            if (productions[tg].right[0]!= "$") {
                for (int i = rlen(productions[tg].right) - 1; i >= 0; i--){ // 注意这里是反向的，逆序压入
                    analyStack.push_back(productions[tg].right[i]);
                }
            }
            
            string str="";
			 
			for (int i = 0 ; i < rlen(productions[tg].right) ; i++){ // 注意这里是反向的，逆序压入
                str = str + productions[tg].right[i] + "  ";
            }
            mycout4 << "推导：" << nonTerminals(productions[tg].left) << "->" << str <<endl;
        }
        else {  // 错误
            mycout4 << setw(15) << "error!" << endl;
            mycout4 << "出错行数为:" << lineStack.front() << "行";
			return;
        }  	
    }
    mycout4.close();
}

/*void LL1::printPredictTable() {
    // 表头
    cout<<endl;
    cout<<"************************************************"<<endl;
    for (int i = 0; i < nonterminal.size(); i++) {
        cout << nonTerminals(nonterminal[i]) << ":";
        for (int j = 0; j < terminalNoEmpty.size(); j++) {
            if (LL1Table[i][j] == -1)
                cout << setw(10) << "   ";
            else{
			for (int n = 0; productions[LL1Table[i][j]].right[n]!= "0"; n++) {
                cout << productions[LL1Table[i][j]].right[n];
				}
        	}
		}
        cout << endl;
    }
    cout << endl;
}*/

void LL1::getResult(FILE *fpin) {
    Scanner scanner;

    token *s = scanner.getTokenList(fpin); //获取tokenlist
    scanner.PrintTokenList();   //输出tokenlist

    CreatLL1Table();           //创建LL1表
    // printPredictTable();       //输出predict表
    //栈匹配
    cout << endl; 
    //cout << setw(15) << "分析栈" << setw(15) << "剩余输入串" << setw(15) << "推导式" << endl;
    analyExpression(s);        //分析tokenlist

}

