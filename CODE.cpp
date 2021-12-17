#include <iostream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
using namespace std;
//數獨長寬大小
#define Maxsize 9  	
vector<int> Row_Allow_Num[Maxsize];
vector<int> Col_Allow_Num[Maxsize];
vector<int> Block_Allow_Num[Maxsize];

int Matrix [Maxsize][Maxsize];

//判斷是在哪個9宮格
int Find_which_block(int row, int col)
{
	row = row/3*3;
	col = col/3;
	return row+col;
}

//初始化所有9宮格的數字都允許
void init_Block_Allow()
{
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=1;j<=Maxsize;j++)
		{
			Block_Allow_Num[i].push_back(j);
		}
	}
}

// 預處理每列每行每個9宮格允許填入的數字
void Check_allownum()
{
	// **ROW PART**
	bool check[Maxsize+1]; // 記錄各列數字有無出現
	for(int i=0;i<Maxsize;i++) // 掃描各列的值
	{
		memset(check,0,sizeof(check));
		for(int j=0;j<Maxsize;j++) 
		{
			int val = Matrix[i][j];
			check[val] = true; // 紀錄數字出現
		}	
		for(int k=1;k<=Maxsize;k++) 
		{
			if(!check[k])
			{
				// 將第i列允許填入的值加入 Row_Allow_Num[i]
				Row_Allow_Num[i].push_back(k);
			}
		}
	}

	// **COLUMN PART**
	memset(check,0,sizeof(check)); 
	for(int j=0;j<Maxsize;j++) // 掃描各行的值
	{
		memset(check,0,sizeof(check)); // 記錄各行數字有無出現
		for(int i=0;i<Maxsize;i++)
		{
			int val = Matrix[i][j];
			check[val] = true; // 紀錄數字出現
		}
		for(int k=1;k<=Maxsize;k++) 
		{
			if(!check[k])
			{
				// 將第j列允許填入的值加入 Col_Allow_Num[j]
				Col_Allow_Num[j].push_back(k);
			}
		}
	}

	// **BLOCK PART**
	init_Block_Allow(); //一次將所有1~9的值全部加入(代表暫時允許)
	
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=0;j<Maxsize;j++)
		{
			int index = Find_which_block(i,j); //判斷在第幾個9宮格
			int val = Matrix[i][j];
			if(val == 0) continue;
			vector<int>::iterator it;
			it =  find(Block_Allow_Num[index].begin(),Block_Allow_Num[index].end(),val);
			Block_Allow_Num[index].erase(it); //不允許的再刪除
		}
	}
}

//將數獨結果印出來
void Print_it_out()
{
	cout<<".......\n";
	Sleep(500);
	cout<<"......\n";
	Sleep(700);
	cout<<"Mission completed-->\n";
	Sleep(400);
	cout<<"\n";
	cout<<"*****************\n";
		for(int i=0;i<Maxsize;i++)
		{
			for(int j=0;j<Maxsize;j++)
			{
				cout<<Matrix[i][j]<<" ";
			}
			cout<<"\n";
		}
	cout<<"*****************\n";
}

// 讓使用者輸入欲破解之數獨題目
void init_Matrix()
{
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=0;j<Maxsize;j++)
		{
			cin>>Matrix[i][j];
		}
	}
}

// 計算待處理空格數&座標(row,col) 
int Find_empty_block(vector<int> &Empty_row, vector<int> &Empty_col)
{
	int pendnum = 0;
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=0;j<Maxsize;j++)
		{
			if( !Matrix[i][j] )
			{
				pendnum++;
				Empty_row.push_back(i);
				Empty_col.push_back(j); 
			}
		}
	}
	return pendnum;
}

//判斷列、行、9宮格中允許填入的數字的交集
vector<int> Find_intersection (int row, int col ,int block )
{
	bool R[Maxsize+1];
	bool C[Maxsize+1];
	bool B[Maxsize+1];
	memset(R,0,sizeof(R));
	memset(C,0,sizeof(C));
	memset(B,0,sizeof(B));
	vector<int> tmp;
	
	for(int i=0;i<Row_Allow_Num[row].size();i++)
		R[Row_Allow_Num[row][i]] = true;
/*	for(auto val:R)
		cout<<val<<" ";
	cout<<"\n";*/
	for(int i=0;i<Col_Allow_Num[col].size();i++)
		C[Col_Allow_Num[col][i]] = true;
/*	for(auto val:C)
		cout<<val<<" ";
	cout<<"\n";*/
	for(int i=0;i<Block_Allow_Num[block].size();i++)
		B[Block_Allow_Num[block][i]] = true;
/*	for(auto val:B)
		cout<<val<<" ";
	cout<<"\n";*/
	for(int i=1;i<=Maxsize;i++)
	{
		if(R[i] && C[i] && B[i])
			tmp.push_back(i);
	}
	return tmp;
}

// 最重要的遞迴函式 
void DFS(int currnum, int Pendnum, vector<int> & Rowlist, vector<int> & Collist)
{
	if(currnum == Pendnum) // 遞迴終止條件
	{
		Print_it_out(); // 印出解答
		return;
	}
	int row = Rowlist[currnum]; //取得要處理空格之列
	int col = Collist[currnum]; //取得要處理空格之行
	int block = Find_which_block(row,col); //取得要處理空格之9宮格
	vector<int> Possible_value = Find_intersection(row,col,block); //尋找列、行、9宮格的交集

	// 遞迴的開始
	for(int i=0;i<Possible_value.size();i++)
	{
		int val = Possible_value[i]; // 設定值

		// 值填入後 該值的列、行、9宮格都設為不允許
		vector<int>:: iterator it;
		it = find(Row_Allow_Num[row].begin(),Row_Allow_Num[row].end(),val);
		Row_Allow_Num[row].erase(it);
		it = find(Col_Allow_Num[col].begin(),Col_Allow_Num[col].end(),val);
		Col_Allow_Num[col].erase(it);
		it = find(Block_Allow_Num[block].begin(),Block_Allow_Num[block].end(),val);
		Block_Allow_Num[block].erase(it);

		Matrix[row][col] = val; // 設定空格的值
		
		//遞迴下個空格
		DFS(currnum+1,Pendnum,Rowlist,Collist);
		
		//回溯法的部分(回復到上一回的狀態)
		Matrix[row][col] = 0;
		Row_Allow_Num[row].push_back(val);
		Col_Allow_Num[col].push_back(val);
		Block_Allow_Num[block].push_back(val);
	}                                                           
}                                                        
                                                                                  //
//主程式
int main()
{
	memset(Matrix,0,sizeof(Matrix)); //初始化數獨矩陣為空
	cout<<"****Please input your Sudoku problems--->\n"; 

	init_Matrix(); //輸入數獨題目來初始化9*9的矩陣
	
	Check_allownum(); //預處理每個Row,COL,BLOCK中允許填入的數字
	
	int pending_dataNum; //儲存待處理空格的數量
	vector<int> Empty_row , Empty_col; //儲存待處理空格的座標(row,col)
	pending_dataNum = Find_empty_block(Empty_row,Empty_col); //取得待處理空格的數量
	
	DFS(0,pending_dataNum,Empty_row,Empty_col); //遞迴求出答案
	
	// ****************This Sudoku solver is contributed by Nathan850****************
	
	//Bug測試
/*	
	cout<<"ROW\n";
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=0;j<Row_Allow_Num[i].size();j++)
		{
			cout<<Row_Allow_Num[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"COL\n";
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=0;j<Col_Allow_Num[i].size();j++)
		{
			cout<<Col_Allow_Num[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"BLOCK\n";
	for(int i=0;i<Maxsize;i++)
	{
		for(int j=0;j<Block_Allow_Num[i].size();j++)
		{
			cout<<Block_Allow_Num[i][j]<<" ";
		}
		cout<<"\n";
	}
*/
	return 0;
} 
