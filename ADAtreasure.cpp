#include <iostream>
#include <string.h>
#include <algorithm>
#include <queue>
#include <math.h>
#include <vector>
#include <utility>
#include <ctype.h>
#define ll long long 
// #pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
using namespace std;
int N, M;
struct City{
    int discover,finish;
    ll treasure, new_treasure;
    int new_num = 0;
    char color_0 = 'W', color_1 = 'W', color_2 = 'W';
    ll max_treasure;
};
struct Road{
    int u,v;
    ll treasure;
};

City city[500002];
Road road[500002];
vector<pair<int,ll>> Town[500002]; // 原圖
vector<pair<int,ll>> Town_reverse[500002]; // 反轉原圖
vector<pair<int,ll>> Town_new[500002]; // 合併過點的圖

void connect(int u, int v, ll w){Town[u].push_back({v,w});} // 原圖
void connect_reverse(int u, int v, ll w){Town_reverse[u].push_back({v,w});} // 反轉原圖
void connect_new(int u, int v, ll w){Town_new[u].push_back({v,w});} // 合併過點的圖

int visit_time=0, T_sort[500002], k=0;
void DFS(int u){ // 原圖的 DFS
    visit_time++;
    city[u].discover = visit_time;
    city[u].color_0 = 'G';
    for(vector<pair<int,ll>>::iterator it = Town[u].begin();it!=Town[u].end();it++){
        if(city[it->first].color_0 == 'W')
            DFS(it->first);
    }
    visit_time++;
    city[u].color_0 = 'B';
    city[u].finish = visit_time;
    T_sort[k++] = u; // for Topological sort
}

void DFS_new(int u){ // 合併過點的圖
    visit_time++;
    city[u].discover = visit_time;
    city[u].color_2 = 'G';
    for(vector<pair<int,ll>>::iterator it = Town_new[u].begin();it!=Town_new[u].end();it++){
        if(city[it->first].color_2 == 'W')
            DFS_new(it->first);
    }
    visit_time++;
    city[u].color_2 = 'B';
    city[u].finish = visit_time;
    T_sort[k++] = u; // for Topological sort
    // printf("%d\n",u);
}

int n; // 新的點的號碼
ll New_treasure;
void DFS_SCC(int u){
    city[u].new_num = n;
    New_treasure += city[u].treasure; // 點上的 treasure
    city[u].color_1 = 'G';
    for(vector<pair<int,ll>>::iterator it = Town_reverse[u].begin();it!=Town_reverse[u].end();it++){
        if(city[it->first].color_1 == 'W'){
            New_treasure += it->second; // 邊上的 treasure
            DFS_SCC(it->first);
        }else if(city[it->first].color_1 == 'G'){
            New_treasure += it->second; // 邊上的 treasure
        }else{ // 'B'
            if(city[it->first].new_num != n)
                connect_new(city[it->first].new_num,n,it->second);
            else
                New_treasure += it->second;
        }
    }
    city[u].color_1 = 'B';
}

void SCC(){
    int i;
    /* Reverse Graph */
    for(i=0;i<M;i++)
        connect_reverse(road[i].v,road[i].u,road[i].treasure);
    /* Reverse Graph */

    n = 1;
    /* Find SCC */
    for(i=N-1;i>=0;i--){
        New_treasure = 0;
        if(city[T_sort[i]].color_1 == 'W'){
            DFS_SCC(T_sort[i]); // 按照結束時間，降序 DFS
            city[n].new_treasure = New_treasure; // 把 SCC 變成一個點
            city[n++].max_treasure = New_treasure;
        }
    }
    n--;
    /* Find SCC */
}

void Relax(int u, int v, int w){
    ll temp = city[u].max_treasure + w + city[v].new_treasure;
    if(city[v].max_treasure < temp)
        city[v].max_treasure = temp;
}


int main(){
    int i,u,v;
    ll t,w;
    scanf("%d%d",&N,&M);

    /* Build Graph */
    for(i=1;i<=N;i++){
        scanf("%lld",&t);
        city[i].treasure = t;
    }
    for(i=0;i<M;i++){
        scanf("%d%d%lld",&u,&v,&w);
        road[i].u = u; road[i].v = v; road[i].treasure = w;
        connect(u,v,w);
    }
    /* Build Graph */

    /* Topolocical sort */
    for(i=1;i<=N;i++){
        if(city[i].color_0 == 'W')
            DFS(i);
    }
    /* Topolocical sort */

    /* SCC and Build new Graph */
    SCC();
    /* SCC and Build new Graph */

    /* Longest path on new Graph */
    visit_time = 0;
    k = 0;
    for(i=1;i<=n;i++){
        if(city[i].color_2 == 'W')
            DFS_new(i);
    }
    for(i=n-1;i>=0;i--){
        u = T_sort[i];
        for(vector<pair<int,ll>>::iterator it = Town_new[u].begin();it!=Town_new[u].end();it++){
            Relax(u,it->first,it->second);
        }
    }
    /* Longest path on new Graph */

    ll max = 0;
    for(i=1;i<=N;i++) max = (city[i].max_treasure>max)? city[i].max_treasure:max;
    printf("%lld\n",max);



    return 0;
}

    // for(i=1;i<=N;i++){
    //     printf("%d : ",i);
    //     for(vector<pair<int,int>>::iterator it = Town_reverse[i].begin();it!=Town_reverse[i].end();it++){
    //         printf("%d ",it->first);
    //     }
    //     printf("\n");
    // }

        
    // for(i=1;i<=n;i++){
    //     printf("%d : ",i);
    //     for(vector<pair<int,int>>::iterator it = Town_new[i].begin();it!=Town_new[i].end();it++){
    //         printf("(%d %d) ",it->first, it->second);
    //     }
    //     printf("\n");
    // }

    // for(i=1;i<=n;i++){printf("%lld ",city[i].new_treasure);}
    // printf("\n");