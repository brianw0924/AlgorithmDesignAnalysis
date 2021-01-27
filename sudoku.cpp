#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "helper.h"
#pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
#define ll long long 

struct node{
    char color;         // 什麼顏色
    int cells=0, sum;   // 出現幾格, 總和多少
    vector<pair<int,int>> pos;  // 出現在第幾 row , col
};

struct ans_node{
    int number,row,col;    
    ans_node(int a, int b, int c){
        this->number = a;
        this->row = b;
        this->col = c;
    }
};

vector<ans_node> table;
int next_index = 0;
void PARTITION(int n, int prev, int cell, vector<vector<int>> *partition, vector<int> res){
    if(n>0){
        if(prev+1<=n){
            for(int i=prev+1;i<=n && i<=9;i++){
                res.push_back(i);
                PARTITION(n-i, i, cell, partition, res); 
                res.pop_back();
            }
        }
    }else{
        if(res.size() == cell)
            partition->push_back(res);
    }
}

int main(void){
    char sudoku[9][9];
    node Node[127];

    /* 讀數獨 */
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            scanf("%c",&sudoku[i][j]);
            Node[sudoku[i][j]].cells++;
            Node[sudoku[i][j]].pos.push_back({i,j});
        }
        scanf("\n");
    }
    /* 讀數獨 */

    /* 什麼顏色、有幾格、總和多少 */
    int target, c=0;
    char colors[127];
    while(scanf("%c",&colors[c])!=EOF){
        scanf("%d\n",&target);
        Node[colors[c]].color = colors[c];
        Node[colors[c]].sum   = target;
        c++;
    }
    /* 什麼顏色、有幾格、總和多少 */

    /* 初始化 */
    int s = 324+c*9;
    DLX::Init(s); // 1~324+c*9
    /* 初始化 */

    /* 丟東西進去 DLX */
    int cells, sum; char color; vector<int> tmp;
    for(int i=0;i<c;i++){ // 依顏色順序一個一個填數字
        color = Node[colors[i]].color; // 顏色
        cells = Node[colors[i]].cells; // 有幾格
        sum   = Node[colors[i]].sum;   // 總和
        vector<vector<int>> partition; // 儲存分割結果
        PARTITION(sum,0,cells,&partition,tmp); // 分割

        /* 把分割的 Complement 塞進 DLX */ // bugged
        bool numbers[10]; // 紀錄哪些數字有可能出現在這個顏色
        if(cells == 9){
            for(int j=1;j<10;j++){numbers[j]=true;} // 初始化為 true
        }else{
        for(int j=1;j<10;j++){numbers[j]=false;} // 初始化為 false
        for(vector<vector<int>>::iterator it=partition.begin();it!=partition.end();it++){
            vector<int>::iterator it2 = it->begin();
            vector<int> row; // 到時候要 DLX::Addrow();
            for(int j=1;j<10;j++){ // 從 1~9 檢查分割數字是哪些，把不是分割的數字 push 進去
                if(it2!=it->end()){ // 還有分割數字
                    numbers[*it2] = true;
                    if(j!=*it2){
                        // printf("PUSH : %d \n",j);
                        row.push_back(324 + i*9 + j);
                    } // j 不是此分割的數字就要 push
                    else{
                        // printf("%d ",*it2);
                        it2++;
                    } // 是的話就換下一個分割數字，然後不要 push
                }else{row.push_back(324 + i*9 + j);} // 分割數字已經跑完就直接 push
            }
            // printf("\n");
            if(!row.empty()){
                table.push_back(ans_node(-1,-1,-1));
                DLX::AddRow(next_index++,row); // 丟進 DLX
            }
        }
        }
        /* 把分割的 Complement 塞進 DLX */


        /* 該顏色的每一格塞入所有可能出現的數字 */
        for(vector<pair<int,int>>::iterator it=Node[color].pos.begin();it!=Node[color].pos.end();it++){
            int ROW   = it->first, COL = it->second; // 該顏色的其中一個格子座標
            int BLOCK = 3*(ROW/3) + COL/3;           // 九宮格 = 0~8 (左上到右下)
            int CELL  = ROW*9 + COL + 1;
            for(int j=1;j<10;j++){                   // 從 1~9 檢查該顏色的分割有沒有這個數字出現
                vector<int> row;
                if(numbers[j]){                      // 有這個數字出現
                    row.push_back(CELL);             // 第幾格 (1~81)，但是 ROW,COL 從 0 開始數
                    row.push_back(81+ROW*9+j);       // 第幾個 ROW   (0~8)  的什麼數字
                    row.push_back(162+COL*9+j);      // 第幾個 COL   (0~8)  的什麼數字
                    row.push_back(243+BLOCK*9+j);    // 第幾個 BLOCK (0~8)  的什麼數字
                    row.push_back(324+i*9+j);        // 第幾個 顏色          的什麼數字
                    table.push_back(ans_node(j,ROW,COL));
                    DLX::AddRow(next_index++,row); // 丟進 DLX
                }
            }
        }
        /* 該顏色的每一格塞入所有可能出現的數字 */
    }
    /* 丟東西進去 DLX */

    vector<int> FINAL_ANS = DLX::Solver();
    // if(FINAL_ANS.empty()){printf("EMPTY\n");}
    assert(!FINAL_ANS.empty());
    int ans[9][9];
    for(vector<int>::iterator it=FINAL_ANS.begin();it!=FINAL_ANS.end();it++){
        if(table[*it].number >= 0){
            ans[table[*it].row][table[*it].col] = table[*it].number;
        }
    }
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            printf("%d",ans[i][j]);
        }
        printf("\n");
    }
    return 0;
}