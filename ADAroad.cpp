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

int N, M, S, T;
struct City{
    int number,pi=-1;
    ll distance = numeric_limits<ll>::max();
    char color='W';
    bool visit = false;
};
struct Edge{
    int u,v,w;
};
City city[300002];
Edge edge[300002];

vector< pair<int,int> > Graph[300002], MST[300002]; // <city, weight>
void connect_Graph(int u, int v, int w){
    Graph[u].push_back({v,w});
    Graph[v].push_back({u,w});
}
void connect_MST(int u, int v, int w){
    MST[u].push_back({v,w});
    MST[v].push_back({u,w});
}

struct DisjointSet{
    int *parent,*rank,n;
    DisjointSet(int n){// initialization
        this->n = n;
        rank = new int[n+1]; // vertex no. is 1~n
        parent = new int[n+1];
        for(int i=1;i<=n;i++){
            parent[i] = i; // root points to itself
            rank[i] = 0; // we use height as rank
        }
    }
    int Find_Halving(int x) { // similar to path compression
    while(parent[x] != x)
        x = parent[x] = parent[parent[x]];
    return x;
    }

    void Union_by_rank(int x, int y){ // rank : tree height
        int rx = Find_Halving(x), ry = Find_Halving(y);
        if(rx == ry)
            return;
        else{
            if(rank[rx] > rank[ry]){
                parent[ry] = rx;
            }else if(rank[rx] == rank[ry]){
                parent[ry] = rx;
                rank[rx]++;
            }else
                parent[rx] = ry;
            }
        }
};

bool cmp(const Edge &a, const Edge &b) {
    return a.w < b.w;
}

class mycomparison{
    bool reverse;
    public:
    mycomparison(const bool& revparam=false){reverse=revparam;}
    bool operator() (const City& lhs, const City&rhs) const{
        if (reverse) return (lhs.distance > rhs.distance);
        else return (lhs.distance < rhs.distance);
    }
};
typedef priority_queue<City,vector<City>,mycomparison> mypq_type;
void Dijkstra(int start){
    mypq_type Q(mycomparison(true));
    city[start].distance = 0;
    for(int j=1;j<=N;j++){Q.push(city[j]);}
    while(!Q.empty()){
        int u = Q.top().number; // 目前 distance 最小的點
        if(city[u].visit){ // lazy deletion
            Q.pop();
            continue;
        }else{
            city[u].visit = true; // 該點已經被 visit，不能再被更新
            Q.pop();
        }
        for(vector<pair<int,int>>::iterator it=MST[u].begin(), end=MST[u].end();it!=end;it++){
            int v = it->first, w = it->second;
            if(city[v].distance > city[u].distance + w){
                city[v].distance = city[u].distance + w;
                Q.push(city[v]);
            }
        }
    }
}

void Kruskal(){
    int edgeCount = 0, i = 0, j = 0, k, x, y;
    sort(edge,&edge[M],cmp); // sort the edge by the weight
    DisjointSet S(N); // to check if the coming edge will form cycle or not
    int curr_w = edge[0].w;
    while(i<M){
        // cout << i << endl;
        curr_w = edge[j].w;
        for(j=i;j<M && edge[j].w == curr_w;j++){
            x = S.Find_Halving(edge[j].u), y = S.Find_Halving(edge[j].v);
            if(x!=y)
                connect_MST(edge[j].u,edge[j].v,edge[j].w);
        }
        /* j == M or edge[j].w != curr.w */

        for(k=i;k<j;k++){
            x = S.Find_Halving(edge[k].u), y = S.Find_Halving(edge[k].v);
            if(x!=y)
                S.Union_by_rank(x,y);
        }
        i = j;
    }
}


int main(){
    int i;
    scanf("%d%d%d%d",&N,&M,&S,&T);
    for(i=1;i<=N;i++){city[i].number=i;}
    int u,v,w;
    for(i=0;i<M;i++){
        scanf("%d%d%d",&u,&v,&w);
        connect_Graph(u,v,w);
        edge[i].u = u;
        edge[i].v = v;
        edge[i].w = w;
    }
    Kruskal();
    Dijkstra(S);
    printf("%lld\n",city[T].distance);


    return 0;
}