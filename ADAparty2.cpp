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

/* 
deque
N types
Q groups
in i-th group : 
    c_i people
    t_i type
    join at s_i end(front or back)
    persuade k_i people

C_i,j : people of type j in group 1~i 


*/
ll N, Q, ans[200002];

int main(){
    scanf("%lld%lld",&N,&Q);
    for(int i=1;i<=N;i++){ans[i]=0;}
    ll c,t,k;
    char s[7];
    deque <pair<ll,ll>> DQ; // <type, 人數>
    pair<ll,ll> temp, next;
    for(int i=1;i<=Q;i++){
        scanf("%s%lld%lld%lld",s,&c,&t,&k);
        // printf("%s %lld %lld %lld\n",s,c,t,k);
        next = {t,c};
        if(s[0] == 'f'){
            while(!DQ.empty()){
                temp = DQ.front();
                if(k >= temp.second){ // 說服人數 >= 末端 Group 人數
                    next.second+=temp.second;
                    k-=temp.second;
                    ans[temp.first] -= (temp.second * (Q-i+1));
                    DQ.pop_front();
                }else{ // 說服人數 < 末端 Group 人數
                    next.second+=k;
                    temp.second-=k;
                    ans[temp.first] -= (k * (Q-i+1));
                    DQ.pop_front();
                    DQ.push_front(temp);
                    break;
                }
            }
            DQ.push_front({next});
            ans[next.first] += (next.second * (Q-i+1));
        }else{ // back
            while(!DQ.empty()){
                temp = DQ.back();
                if(k >= temp.second){ // 說服人數 >= 末端 Group 人數
                    next.second+=temp.second;
                    k-=temp.second;
                    ans[temp.first] -= (temp.second * (Q-i+1));
                    DQ.pop_back();
                }else{ // 說服人數 < 末端 Group 人數
                    next.second+=k;
                    temp.second-=k;
                    ans[temp.first] -= (k * (Q-i+1));
                    DQ.pop_back();
                    DQ.push_back(temp);
                    break;
                }
            }
            DQ.push_back({next});
            ans[next.first] += (next.second * (Q-i+1));

        }
    //     for(int i=1;i<=N;i++){printf("%lld ",ans[i]);}
    // printf("\n");
    }

    for(int i=1;i<=N;i++){printf("%lld ",ans[i]);}
    printf("\n");
    
    return 0;
}