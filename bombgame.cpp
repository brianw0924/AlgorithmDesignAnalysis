#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctype.h>
#include <unordered_map>
#include <utility>
#include "helper.h"
// #pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
#define llu unsigned long long 
using namespace std;
using namespace Memory;


/*
Merge sort : 根據 d_i 和 p_i 由大排到小

merge : 
    左半邊 : 找 player 
    右半邊 : 找 bomb

BIT : 給座標回傳傷害
*/


struct event{
    int c,l,r,t; // t : 進場時間
    llu x,k; // player : d, bomb : p
    char T;
};

int n,M;
vector<event> V; // event
vector<int> player_sequence; // <t> 進場順序
unordered_map<int,llu> P; // <t, damage>

void bruteforce(int l, int r){
    if(V[l].T == 'A' || V[r].T == 'P' || V[l].x > V[r].x){ // 左邊是 bomb || 右邊是 player || player 的防禦力 > bomb 的威力
        if(V[l].x < V[r].x){ // SWAP
            event temp = V[l];
            V[l] = V[r];
            V[r] = temp;
        }
        return;
    }
    if(V[r].l <= V[l].c && V[r].r >= V[l].c){ // 在爆炸範圍內
        P[V[l].t] += V[r].k;
    }
    if(V[l].x < V[r].x){ // SWAP
        event temp = V[l];
        V[l] = V[r];
        V[r] = temp;
    }
}

void DQ(int l, int r){
    if(l==r)
        return;
    if(r-l == 1){
        bruteforce(l,r);
        return;
    }
    

    int q = (l+r)/2;
    DQ(l,q);
    DQ(q+1,r);

    vector<event>::iterator L=V.begin()+l, R=V.begin()+q+1, L_end=V.begin()+q+1, R_end=V.begin()+r+1;
    vector<event> VL (L,L_end), VR (R,R_end); // 先保存最後要 Merge
    vector<event> forReset; // 保存等一下有對座標造成傷害的 bomb ，最後要復原(歸零)
    bool notempty=1;
   

   /* 計算傷害 開始 */
    while(notempty){ // 其中一邊掃完就跳出去
        
        while(L != L_end && L->T != 'P'){ // 左半邊找 player
            L++; 
        }
        while(R != R_end && R->T != 'A'){ // 右半邊找 bomb
            R++; 
        }

        if(R == R_end || L == L_end){ // 有一邊掃完了
            notempty = 0;
        }else{
            if(L->x > R->x){ // player 防禦力 > bomb 威力
                P[L->t] += Memory::get(L->c); // 結算傷害 (右側剩下的 bomb 威力只會更小)
                L++;
            }else{ // player 防禦力 <= bomb 威力
                Memory::add(R->l,R->r,R->k); // 對 cell[l~r] 造成傷害
                forReset.push_back(*R);
                R++;
            }
        }

    }

    while(L != L_end){ // 左邊還有 player，傷害要繼續結算
        if(L->T == 'P'){
            P[L->t] += Memory::get(L->c);
        }
        L++;
    }
    /* 計算傷害 結束 */

    /* Merge 開始 */
    int i = l; // 開始 merge event l~r 的部分
    L=VL.begin(),R=VR.begin(); // VL VR 是備份
    vector<event>::iterator VL_end = VL.end(), VR_end = VR.end();
    
    while(L != VL_end && R != VR_end){
        if(L->x > R->x){ // 左邊丟進去
            V[i] = *L;
            L++;
        }else{ // 右邊丟進去
            V[i] = *R;
            R++;
        }
        ++i;
    }
    while(L != VL_end){ // 左邊還有東西
        V[i] = *L;
        L++;
        ++i;
    }
    while(R != VR_end){ // 右邊還有東西
        V[i] = *R;
        R++;
        ++i;
    }
    /* Merge 結束 */

    // printf("\n目前為止傷害 : \n");
    // for(vector<int>::iterator it=player_sequence.begin(),end=player_sequence.end();it!=end;it++){
    //     printf("%llu\n",P[*it]); // <time,damage>
    // }
    // printf("\n");

    /* 歸零傷害 */
    for(vector<event>::iterator it=forReset.begin(),end=forReset.end();it!=end;it++)
        Memory::add(it->l,it->r,-(it->k));
}

/* 
壓常數 : 
1. vector 太慢
2. push_back 太慢
3. 讀取用 getchar 自己拚
4. map 用陣列做就好
*/


int main(){
    Memory::reset(); // 歸零
    event E;
    char type;

    scanf("%d%d",&n,&M);
    
    for(int i=0;i<M;++i){
        scanf("\n%c",&type);
        E.T = type;
        E.t = i;
        if(type == 'P'){ // player
            scanf("%d%lld",&E.c,&E.x);
            V.push_back(E);
            player_sequence.push_back(i);
            P.insert({i,0});
        }else{ // bomb
            scanf("%d%d%lld%lld",&E.l,&E.r,&E.x,&E.k);
            V.push_back(E);
        }
    }

    DQ(0,M-1); // 對 event timeline DQ

    /* 根據進場順序印出傷害 */
    for(vector<int>::iterator it=player_sequence.begin(),end=player_sequence.end();it!=end;it++){
        printf("%llu\n",P[*it]); // <time,damage>
    }
    
  return 0;
}