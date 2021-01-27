#include <iostream>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <utility>
#include <ctype.h>
// #pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
using namespace std;
char back[602][205][205];
pair<int,int> from[205][205];
int N,K,D,a[205],c[205],s[205],dp[602][205][205],score=0;
bool IsADAsequence(int i, int j){
    int d = (a[i]-a[j])>0? (a[i]-a[j]):(a[j]-a[i]);
    return d<=D;
}
int main(){
    int i,j,l;
    scanf("%d%d%d",&N,&K,&D);
    for(i=1;i<=N;i++){scanf("%d",&a[i]);}
    for(i=1;i<=N;i++){scanf("%d",&c[i]);}
    for(i=1;i<=N;i++){scanf("%d",&s[i]); score+=s[i];}
    if(N==1){printf("%d\n%d\n%d\n%d\n",s[1],0,1,1); return 0;}
    
    /* K=0 */
    if(K==0){
    for(i=1;i<=N;++i){for(j=1;j<=N;++j)dp[0][i][j] = 0;} // dp[0] 歸零
    for(i=1;i<=N;++i){dp[0][i][i]=s[i]; from[i][i]={1,0};} // 對角線 = c[]
    for(l=1;l<N;++l){
        for(i=1;i<=(N-l);++i){
                if(IsADAsequence(i,i+l)){ // 頭尾符合 ADAsequence
                    int x,y,z;
                    x = s[i] + s[i+l] + dp[0][i+1][i+l-1]; y = dp[0][i+1][i+l]; z = dp[0][i][i+l-1];
                    if(y>z){
                        if(x>y){
                            dp[0][i][i+l] = x;
                            from[i][i+l] = {i+1,i+l-1};
                        }else{
                            dp[0][i][i+l] = y;
                            from[i][i+l] = {i+1,i+l};
                        }
                    }else{
                        if(x>z){
                            dp[0][i][i+l] = x;
                            from[i][i+l] = {i+1,i+l-1};
                        }else{
                            dp[0][i][i+l] = z;
                            from[i][i+l] = {i,i+l-1};
                        }
                    }
                }else{ // 頭尾不符合 ADAsequence
                    if(dp[0][i+1][i+l]>dp[0][i][i+l-1]){ // 左邊減少一格 比較大
                        from[i][i+l] = {i+1,i+l};
                        dp[0][i][i+l] = dp[0][i+1][i+l];
                    }else{ // 右邊減少一格 比較大
                        from[i][i+l] = {i,i+l-1};
                        dp[0][i][i+l] = dp[0][i][i+l-1];
                    }
                }
        }
    }
    int Left[205], Right[205], L_index = 0, R_index = 0, sequence_len;
    i = 1; j = N; int temp;
    while(i<j){
        if(from[i][j].first == i+1 && from[i][j].second == j-1){ // 走斜對角
            Left[L_index++] = i;
            Right[R_index++] = j;
        }
        temp = from[i][j].second;
        i = from[i][j].first; j = temp;
        if(i == j){ // 走到對角線了
            Left[L_index++] = i;
            break;
        }
    }
    printf("%d\n%d\n%d\n",dp[0][1][N],0,L_index + R_index);
    for(i=0;i<L_index;++i){printf("%d ",Left[i]);} // 先印左半邊
    for(i=R_index-1;i>=0;--i){printf("%d ",Right[i]);} // 再印右半邊
    printf("\n");
    /* K=0 */
    }else{

    /* K>0 */
    int cost,k,temp;
    for(k=0;k<=score;++k){
        for(i=1;i<=N;++i){for(j=1;j<=N;++j){dp[k][i][j] = k==0? 0:(K+1);}} // 初始化
        // printf("k=%d\n",k);
        for(j=1;j<=N;++j){
            if(s[j]==k) {dp[k][j][j]=0;}
            else {dp[k][j][j]= K+1;}
        }

        for(l=1;l<N;++l){
            for(i=1;i<=(N-l);++i){ // j = i + l
                j = i+l;
                cost = K+1;
                temp = s[i]+s[j]; // 頭尾的分數
                // if(k==3 && i==3 && j==4){printf("temp = %lld\n",temp);}
                if(temp <= k){ // 頭尾分數和 <= 這層要求的才計
                    cost = dp[k-temp][i+1][j-1]; // 找裡面一層
                    if(!IsADAsequence(i,j)){cost+=min(c[i],c[j]);} // 如果頭尾不符合 ADA sequence 則加上 cost
                }
                // dp[k][i][j] = min(cost,min(dp[k][i+1][j],dp[k][i][j-1]));

                if(dp[k][i+1][j] < dp[k][i][j-1]){ // 下方小
                    if(cost < dp[k][i+1][j]){
                        back[k][i][j] = 'B'; // back
                        dp[k][i][j] = cost; 
                    }else{
                        back[k][i][j] = 'D'; // down
                        dp[k][i][j] = dp[k][i+1][j];
                    }
                }else{ // 左方小
                    if(cost < dp[k][i][j-1]){
                        back[k][i][j] = 'B'; // back
                        dp[k][i][j] = cost;
                    }else{
                        back[k][i][j] = 'L'; // left
                        dp[k][i][j] = dp[k][i][j-1];
                    }
                }
            }
        }

    }
    for(k=score;k>=0;--k){
        if(dp[k][1][N] <= K)
            break;
    }

    printf("%d\n",k); //分數
    /* 找操作 & Sequence */
    int Left[201], Right[201], L_index = 0, R_index = 0, op_index = 0, sequence_len;
    pair<int,int> op[201];
    i = 1; j = N;
    while(i<j && k!=0){
        // printf("(%d,%d,%d)\n",k,i,j);
        if(back[k][i][j] == 'B'){ // 從上一層來
            Left[L_index++] = i;
            Right[R_index++] = j;
            if(!IsADAsequence(i,j)){ // 不符合 ADAsequence ， 需要改動 a[]
                if(c[i] > c[j])
                    op[op_index++] = {j,a[i]};
                else
                    op[op_index++] = {i,a[j]};
            }
            k-=(s[i]+s[j]);i++;j--;
        }else{
            if(back[k][i][j] == 'L'){
                j--;
            }else{ // 'D'
                i++;
            }
        }
        if(i == j){ // 走到對角線了
            // printf("(%d,%d,%d)\n",k,i,j);
            Left[L_index++] = i;
            break;
        }
    }
    /* 找操作 & Sequence */
    printf("%d\n",op_index);
    for(i=0;i<op_index;++i){printf("%d %d\n",op[i].first,op[i].second);}

    printf("%d\n", R_index + L_index);
    for(i=0;i<L_index;++i){printf("%d ",Left[i]);} // 先印左半邊
    for(i=R_index-1;i>=0;--i){printf("%d ",Right[i]);} // 再印右半邊
    printf("\n");
        
    return 0;
    }
}