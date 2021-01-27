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

int n;

struct City{
    int num, pi = -1, max_child = -1;
    bool visit = false;
    ll d = 0, parent_edge = 0;
};
struct Road {
    int u,v,d;
};

City city[500002];

vector<pair<int,ll>> Kingdom[500002];

void connect(int u, int v, int d){
    Kingdom[u].push_back({v,d});
    Kingdom[v].push_back({u,d});
}

ll ans[500002], k = 1;

ll DFS_visit(int u){
    city[u].visit = true;
    ll temp;
    for(vector<pair<int,ll>>::iterator it=Kingdom[u].begin(), end=Kingdom[u].end();it!=end;it++){
        if(!city[it->first].visit){
            city[it->first].pi = u;
            city[it->first].parent_edge = it->second;
            temp = it->second + DFS_visit(it->first);
            if(temp > city[u].d){
                city[u].d = temp;
                city[u].max_child = it->first;
            }
        }
    }
    // printf("%d,%lld\n",u,city[u].d);
    return city[u].d;
}

void DFS(int start){
    int u;
    for(int i=1;i<=n;i++){
        city[i].visit = false;
        city[i].pi = -1;
        city[i].max_child = -1;
        city[i].d = 0;
    }
    DFS_visit(start);
}

void BFS(int u){
    city[u].visit = true;
    int v;
    queue<int> Q;
    Q.push(u);
    while(!Q.empty()){
        v = Q.front();
        Q.pop();
        for(vector<pair<int,ll>>::iterator it=Kingdom[v].begin(), end=Kingdom[v].end();it!=end;it++){
            if(!city[it->first].visit){
                Q.push(it->first);
                city[it->first].visit = true;
                city[it->first].d = city[v].d + it->second;
            }
        }
    }
}

class mycomparison{
    bool reverse;
    public:
    mycomparison(const bool& revparam=false){reverse=revparam;}
    bool operator() (const City& lhs, const City&rhs) const{
        if (reverse) return (lhs.d + lhs.parent_edge > rhs.d + rhs.parent_edge);
        else return (lhs.d + lhs.parent_edge < rhs.d + rhs.parent_edge);
    }
};

void DFS_find(int u){
    typedef priority_queue<City,vector<City>,mycomparison> mypq_type;
    mypq_type Q(mycomparison(false));
    Q.push(city[u]);
    City v;
    while(!Q.empty()){
        v = Q.top();
        // printf("Pop : %d\n",v.num);
        Q.pop();
        ans[k] = ans[k-1] + v.parent_edge + v.d ; // still bug
        k++;
        u = v.num;
        while(city[u].max_child != -1){
            // printf("now : %d\n",u);
            for(vector<pair<int,ll>>::iterator it=Kingdom[u].begin(), end=Kingdom[u].end();it!=end;it++){
                if(it->first != city[u].pi && it->first != city[city[u].max_child].num){
                    // printf("Push : %d\n",it->first);
                    Q.push(city[it->first]);
                }
            }
            u = city[u].max_child;
        }
        // printf("end at= %d\n",u);
    }
    while(k<=n){
        ans[k] = ans[k-1];
        k++;
    }
}


int main(){
    scanf("%d",&n);
    int u,v,d,i;
    for(i=1;i<=n;i++){
        city[i].num = i;
    }
    for(i=1;i<n;i++){
        scanf("%d%d%d",&u,&v,&d);
        connect(u,v,d);
    }
    /* 先 Diameter 的其中一端點 */
    ans[k++] = 0;
    BFS(1);
    ll max=city[1].d; int max_id = 1;
    for(i=1;i<=n;i++){
        if(city[i].d > max){
            max = city[i].d;
            max_id = i;
        }
    }
    int root = max_id;
    /* 先 Diameter 的其中一端點 */
    
    /* 計算 root 到其他端點 */
    DFS(root);
    /* 計算 root 到其他端點 */

    /* find answer */
    DFS_find(root);
    /* find answer */

    /* print answer */
    for(i=1;i<=n;i++){printf("%lld ",ans[i]);}
    printf("\n");
    /* print answer */



    return 0;
}