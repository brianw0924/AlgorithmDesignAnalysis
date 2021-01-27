#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <stack>
#include <math.h>
#include <ctype.h>
#include <utility>
#pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
#define llu unsigned long long
using namespace std;
llu *bag, *table;
stack<llu> S,T;
int N,K;
llu bruteforce(int l, int r){
    if(r == l)
        return (llu)0;
    if(r-l == 1)
        return (llu)1;
    llu c = 2, sum = bag[l]+bag[l+1]+bag[r];
    sum-=max(bag[l],max(bag[l+1],bag[r]));
    sum-=min(bag[l],min(bag[l+1],bag[r]));
    if(sum%K==0)
        c++;
    return c;
}
llu DQ(int l, int r){
    if(r-l<3)
        return bruteforce(l,r);
    int q = (l+r)/2;
    llu count, res;
    count = DQ(l,q) + DQ(q+1,r);
    /* 找橫跨中間的 */

    /* ---------------max min 都在右側--------------- */
    int L = q, R = q+1;
    llu R_max, R_min, R_sum = 0, L_sum = 0, R_max_index, R_min_index;
    /* 先找右側最大最小 */
    if(bag[R]> bag[R+1]){
        R_max = bag[R];
        R_min = bag[R+1];
    }else{
        R_max = bag[R+1];
        R_min = bag[R];
    }
    R++;
    while(1){
        /* 往左找直到違反其中一個 */
        while(L>=l && bag[L]<=R_max && bag[L]>R_min){ // max 一樣一律選右邊 ; min 一樣一律選左邊
            L_sum+=bag[L];
            res = L_sum%K;
            table[res]++;
            S.push(res);
            L--;
        }
        /* 結算 */
        count+=table[(K-(R_sum%K))%K];
        /* 右側移動一格 */
        R++;
        if(R>r)
            break;
        else{
            if(bag[R]>R_max){
                R_sum+=R_max;
                R_max = bag[R];
            }else if(bag[R]<R_min){
                R_sum+=R_min;
                R_min = bag[R];
            }else
                R_sum+=bag[R];
        }
    }
    while(!S.empty()){
        table[S.top()] = 0;
        S.pop();
    }
    /* ---------------max min 都在左側--------------- */
    L = q, R = q+1, R_sum = 0, L_sum = 0;
    llu L_max, L_min, L_max_index, L_min_index;
    /* 先找左側最大最小 */
    if(bag[L-1]> bag[L]){
        L_max = bag[L-1];
        L_min = bag[L];
    }else{
        L_max = bag[L];
        L_min = bag[L-1];
    }
    L--;
    while(1){
        /* 往右找直到違反其中一個 */
        while(R<=r && bag[R]<L_max && bag[R]>=L_min){
            R_sum+=bag[R];
            res = R_sum%K;
            table[res]++;
            S.push(res);
            R++;
        }
        /* 結算 */
        count+=table[(K-(L_sum%K))%K];
        /* 左側移動一格 */
        L--;
        if(L<l)
            break;
        else{
            if(bag[L]>L_max){
                L_sum+=L_max;
                L_max = bag[L];
            }else if(bag[L]<L_min){
                L_sum+=L_min;
                L_min = bag[L];
            }else
                L_sum+=bag[L];
        }
    }
    while(!S.empty()){
        table[S.top()] = 0;
        S.pop();
    }
    /* ---------------min 左 , max 右--------------- */
    L = q, R = q+1, L_sum = 0, L_min = bag[L], L_max = bag[L];
    llu leftbound_max = bag[R], rightbound_max = bag[R], leftbound_sum = 0, rightbound_sum = 0;
    int leftbound = q+1, rightbound = q+1; //
    /* leftbound : 至少要選到他才滿足 max 右(inclusive), rightbound : 不能到他才滿足 min 左(exclusive) */
    /* 固定左邊動右邊 */
    while(1){
        /* 先擴增右界 */
        while(rightbound<=r && (bag[rightbound] >= L_min)){
            if(rightbound != q+1){
                if(bag[rightbound]>rightbound_max){
                    rightbound_sum+=rightbound_max;
                    rightbound_max = bag[rightbound];
                }else
                    rightbound_sum+=bag[rightbound];
            }
            res = rightbound_sum%K;
            table[res]++;
            S.push(res);
            rightbound++;
        }
        /* 再擴增左界 */
        while(leftbound<=r && (bag[leftbound] < L_max)){
            if(leftbound != q+1){
                if(bag[leftbound]>leftbound_max){
                    leftbound_sum+=leftbound_max;
                    leftbound_max = bag[leftbound];
                }else
                    leftbound_sum+=bag[leftbound];
            }
            res = leftbound_sum%K;
            table[res]--;
            S.push(res);
            leftbound++;
        }

        if(rightbound > leftbound)
            count+=table[(K-(L_sum%K))%K];
        if(leftbound>r)
            break;
        L--;
        if(L<l)
            break;
        else{
            if(bag[L]>L_max)
                L_max = bag[L];
            if(bag[L]<L_min){
                L_sum+=L_min;
                L_min = bag[L];
            }else
                L_sum+= bag[L];
        }

    }
    while(!S.empty()){
        table[S.top()] = 0;
        S.pop();
    }
    /* ---------------max 左 , min 右--------------- */
    L = q, R = q+1, R_sum = 0, R_min = bag[R], R_max = bag[R];
    leftbound_max = bag[L], rightbound_max = bag[L], leftbound_sum = 0, rightbound_sum = 0;
    leftbound = q, rightbound = q; 
    /* 固定右邊動左邊 */
    while(1){

        while(leftbound>=l && (bag[leftbound] > R_min)){
            if(leftbound != q){
                if(bag[leftbound]>leftbound_max){
                    leftbound_sum+=leftbound_max;
                    leftbound_max = bag[leftbound];
                }else{
                    leftbound_sum+=bag[leftbound];
                }
            }
            res = leftbound_sum%K;
            table[res]++;
            S.push(res);
            leftbound--;
        }

        while(rightbound>=l && (bag[rightbound] <= R_max)){
            if(rightbound != q){
                if(bag[rightbound]>rightbound_max){
                    rightbound_sum+=rightbound_max;
                    rightbound_max = bag[rightbound];
                }else
                    rightbound_sum+=bag[rightbound];
            }
            res = rightbound_sum%K;
            table[res]--;
            S.push(res);
            rightbound--;
        }

        if(rightbound > leftbound)
            count+=table[(K-(R_sum%K))%K];
        if(rightbound<l)
            break;
        R++;
        if(R>r)
            break;
        else{
            if(bag[R]>R_max)
                R_max = bag[R];
            if(bag[R]<R_min){
                R_sum+=R_min;
                R_min = bag[R];
            }else
                R_sum+= bag[R];
        }

    }
    while(!S.empty()){
        table[S.top()] = 0;
        S.pop();
    }
    return count;
    
}

int main(){
    
    scanf("%d%d",&N,&K);
    bag = new llu[N];
    table = new llu[K];
    for(int i=0;i<K;++i)
        table[i] = 0;
    for(int i=0;i<N;i++){
        scanf("%lld",&bag[i]);
    }
    printf("%llu\n",DQ(0,N-1));
    return 0;
}