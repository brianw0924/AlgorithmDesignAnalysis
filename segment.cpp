#include <iostream>
#include <string.h>
#include <algorithm>
#include <queue>
#include <math.h>
#include <vector>
#include <ctype.h>
#define llu unsigned long long 
// #pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
using namespace std;
int N,M;
llu K, arr[200002],res[200002];
struct line{
    int L,R,index;
    llu W;
};

struct compare{
    bool operator()(const line& a,const line& b){
         return (a.R < b.R);
    }
};

bool cmp(const line &a, const line &b) {
    return a.L < b.L;
}

line Line[200002];
int main(){
    int i,j,k;
    scanf("%d%d%llu",&N,&M,&K);
    for(i=0;i<N;i++){
        scanf("%d%d%llu",&Line[i].L,&Line[i].R,&Line[i].W);
        Line[i].index=i;
    }
    sort(Line,&Line[N],cmp);
    // for(i=0;i<N;i++){printf("%d %d %lld\n",Line[i].L,Line[i].R,Line[i].W);}

    /* 0 沒被蓋到，直接 return */
    if(Line[0].L != 0){
        printf("%d\n",0);
        for(i=0;i<N;i++){
            printf("%d ",0);
        }
        printf("\n");
        return 0;
    }
    /* 0 沒被蓋到，直接 return */

    llu l=0, r=K, w, sum, chosen;
    int next_line;
    line temp;
    priority_queue<line,vector<line>,compare> h;
    // for(i=0;i<N;i++){h.push(Line[i]);}
    // for(i=0;i<N;i++){
    //     line temp = h.top();
    //     h.pop();
    //     printf("%d %d %lld\n",temp.L,temp.R,temp.W);
    // }
    

    while(r-l >1){ // 二分搜尋

        /* 目標 f(x) */
        w = (l+r)/2;
        /* 目標 f(x) */

        /* 歸零 */
        while(!h.empty()){h.pop();} // heap 清空
        for(j=0;j<=M;++j){arr[j]=(llu)0;}
        next_line = 0;
        sum = 0;
        chosen = 0;
        /* 歸零 */

        // printf("l=%lld r=%lld w=%lld\n",l,r,w);

        for(i=0;i<=M;++i){
            // printf("i = %d\n",i);

            /* 左界 == 目前座標則 enqueue */
            while(next_line<=N && Line[next_line].L == i){
                // printf("Enqueue %d %d\n",Line[next_line].L,Line[next_line].R);
                h.push(Line[next_line]);
                next_line++;    
            }
            /* 左界 == 目前座標則 enqueue */
            
            /* 線段不足的時候從 heap 拿出右界最大的 */
            if(i<M){
                sum-=arr[i]; // 檢查有沒有線段在這格開始無法蓋到
                arr[i]=0;
            }
            // printf("sum = %lld\n",sum);
            while((!h.empty()) && sum < w){ // 一直 pop 直到這個點被蓋到 w 次
                temp = h.top();
                // printf("Dequeue %d %d\n",temp.L,temp.R);
                h.pop();
                if(temp.R<i)
                    break;
                if(i>0 && arr[i-1]>0 && temp.L>=i)
                    break;
                if(temp.W > (w-sum)){ // 拿出來的線段比我需要的還"多"
                    chosen+=(w-sum);
                    temp.W-=(w-sum);
                    arr[temp.R] += (w-sum); // 從 temp.R 開始，他會蓋不到
                    sum = w; // sum = sum + w - sum = w
                    h.push(temp); // 還沒用完，push 回去
                }else{ // 拿出來的線段比我需要的還"少"
                    sum+=temp.W;
                    chosen+=temp.W;
                    arr[temp.R] += temp.W; // 從 temp.R 開始，他會蓋不到
                }
            }
            /* 線段不足的時候從 heap 拿出右界最大的 */

            // printf("chosen = %lld\n",chosen);
            if(chosen>K || sum<w){ // 這個 w 不可能達成
                r = w-1;
                break;
            }
            if(i<M){ // 有可能剛剛剛拿出來的 line 右界剛好只到他自己
                sum-=arr[i]; // 檢查有沒有線段在這格開始無法蓋到
            }
        }

        if(i>M){ // for 跑完，代表這個 w 可以達成
            l = w;
        }
        
    }

    if(l!=r){
        w = r; // 檢查 w=r 可不可行
        /* 歸零 */
        while(!h.empty()){h.pop();}
        for(j=0;j<=M;++j){arr[j]=(llu)0;}
        next_line = 0;
        sum = 0;
        chosen = 0;
        /* 歸零 */
        for(i=0;i<=M;++i){
            // printf("i = %d\n",i);
            /* 左界 == 目前座標則 enqueue */
            while(next_line<=N && Line[next_line].L == i){
                h.push(Line[next_line]);
                next_line++;    
            }
            /* 左界 == 目前座標則 enqueue */
            
            /* 線段不足的時候從 heap 拿出右界最大的 */
            if(i<M){
                sum-=arr[i]; // 檢查有沒有線段在這格開始無法蓋到
                arr[i]=0;
            }
            // printf("sum = %lld\n",sum);
            while((!h.empty()) && sum < w){ // 一直 pop 直到這個點被蓋到 w 次
                temp = h.top();
                // printf("Dequeue %d %d\n",temp.L,temp.R);
                h.pop();
                if(temp.R<i)
                    break;
                if(i>0 && arr[i-1]>0 && temp.L>=i)
                    break;
                if(temp.W > (w-sum)){ // 線段比我要的還多
                    chosen+=(w-sum);
                    temp.W-=(w-sum);
                    arr[temp.R] += (w-sum); // 從 temp.R 開始，他會蓋不到
                    sum = w; // sum = sum + w - sum = w
                    h.push(temp);
                }else{ // 線段還不夠
                    sum+=temp.W;
                    chosen+=temp.W;
                    arr[temp.R] += temp.W; // 從 temp.R 開始，他會蓋不到
                }
                
            }
            /* 線段不足的時候從 heap 拿出右界最大的 */
            if(chosen>K || sum<w){ // 這個 w 不可能達成
                r = l;
                break;
            }
            if(i<M){
                sum-=arr[i]; // 檢查有沒有線段在這格開始無法蓋到
            }
        }

        if(l!=r){ // for 跑完，代表這個 w 可以達成
            l = r;
        }
    }

/* 找答案 */

        w = l;
        /* 歸零 */
        while(!h.empty()){h.pop();}
        for(j=0;j<=M;++j){arr[j]=(llu)0;}
        for(j=0;j<=N;++j){res[j]=(llu)0;}
        next_line = 0;
        sum = 0;
        chosen = 0;
        /* 歸零 */
        for(i=0;i<=M;++i){
            // printf("i = %d\n",i);
            /* 左界 == 目前座標則 enqueue */
            while(next_line<=N && Line[next_line].L == i){
                h.push(Line[next_line]);
                next_line++;    
            }
            /* 左界 == 目前座標則 enqueue */
            
            /* 線段不足的時候從 heap 拿出右界最大的 */
            if(i<M){
                sum-=arr[i]; // 檢查有沒有線段在這格開始無法蓋到
                arr[i]=0;
            }
            while((!h.empty()) && sum < w){ // 一直 pop 直到這個點被蓋到 w 次
                temp = h.top();
                h.pop();
                if(temp.W > (w-sum)){ // 線段比我要的還多
                    res[temp.index]+=(w-sum);
                    chosen+=(w-sum);
                    temp.W-=(w-sum);
                    arr[temp.R] += (w-sum); // 從 temp.R 開始，他會蓋不到
                    sum = w; // sum = sum + w - sum = w
                    h.push(temp);
                }else{ // 線段還不夠
                    res[temp.index]+=temp.W;
                    sum+=temp.W;
                    chosen+=temp.W;
                    arr[temp.R] += temp.W; // 從 temp.R 開始，他會蓋不到
                }
            }
            if(i<M){
                sum-=arr[i]; // 檢查有沒有線段在這格開始無法蓋到
                arr[i]=0;
            }
        }
    
    printf("%llu\n",l);
    for(i=0;i<N;++i){
        printf("%llu ",res[i]);
    }
    printf("\n");

    return 0;
}