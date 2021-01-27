#include <iostream>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <ctype.h>
#define ll long long 
#pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
using namespace std;
ll N,K,A,B,s[100002];
int main(){
    scanf("%lld%lld%lld%lld",&N,&K,&A,&B);
    ll *ptr=s,val=A*B-A;ptr++;
    for(int i=1;i<=N;++i,ptr++){scanf("%lld",ptr);}
    ll p_max = s[1], m_1, m_2, res = s[1], cmp, dp[2][100002];
    /* 一個塔 */
    for(int i=1;i<=N;i++){ 
        dp[0][i] = s[i];
        if(dp[0][i] > p_max){
            p_max = s[i];
            res = p_max;
        }
    }
    /* 超過一個塔 */
    for(int k=2;k<=K;++k){
        m_1 = dp[0][k-1];
        m_2 = dp[0][k-1] + val;
        p_max = m_1>m_2? m_1:m_2;
        ptr = &dp[1][k];
        for(int j=k;j<=N;j++,ptr++){
            *ptr = s[j] + p_max;
            if(dp[1][j] > res)
                res = dp[1][j];
            m_1 = m_1>dp[0][j]? m_1:dp[0][j];
            m_2 = (m_2-A)>(dp[0][j]+val)? (m_2-A):(dp[0][j]+val);
            p_max = m_1>m_2? m_1:m_2;
        }
        for(int i=1;i<=N;i++){
            dp[0][i] = dp[1][i];
        }
    }
    printf("%lld\n",max((ll)0,res));
    return 0;
}