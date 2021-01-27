#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <math.h>
#include <ctype.h>
#include <utility>
#pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
# define ll long long 
using namespace std;

// n*m grids
// goal : max sweetness
// unlimited nuts each grid, same grid <-> same sweetness
// only move right or down
// must take 1 nut in the current grid
// k space machine, use it -> decrease 'c' sweetness for all nuts


int main(){
    int n,m,k;
    ll c;
    ll Grid[500][500], DP[500][500]; // DP : 走到這一格可以拿多少 sweetness
    pair<int,int> from[500][500];

    scanf("%d%d%d%lld",&n,&m,&k,&c);

    /* 讀地圖 */
    for(int i=0;i<n;++i){
      for(int j=0;j<m;++j){
        scanf("%lld",&Grid[i][j]);
      }
    }
    /* 第一列 */
    DP[0][0] = Grid[0][0];
    for(int i=1;i<m;++i){
      DP[0][i] = DP[0][i-1] + Grid[0][i];
      from[0][i] = {0,i-1};
      // printf("%d %d\n",from[0][i].first,from[0][i].second);
    }
    /* 第一行 */
    for(int i=1;i<n;++i){
      DP[i][0] = DP[i-1][0] + Grid[i][0]; 
      from[i][0] = {i-1,0};
    }
    for(int i=1;i<n;++i){
      // printf("i=%d\n",i);
      for(int j=1;j<m;++j){
        // printf("j=%d\n",j);
        if(DP[i-1][j] > DP[i][j-1]){ // 從上面走過來比較好
          DP[i][j] = DP[i-1][j] + Grid[i][j];
          from[i][j] = {i-1,j};
          // printf("%d %d\n",i-1,j);
        }else{ // 從左邊走過來比較好
          DP[i][j] = DP[i][j-1] + Grid[i][j];
          from[i][j] = {i,j-1};
          // printf("%d %d\n",i,j-1);
        }
      }
    }
    // for(int i=0;i<n;i++){
    //   for(int j=0;j<m;j++){
    //     printf("(%d,%d) ",from[i][j].first,from[i][j].second);
    //   }
    //     printf("\n");
    // }
    vector<pair<int,int>> path;
    int i=n-1, j=m-1, count=0, temp;
    path.push_back({i,j});
    while(i!=0 || j!=0){
      path.push_back(from[i][j]);
      temp = from[i][j].second;
      i = from[i][j].first;
      j = temp;
      count++;
    }
    path.pop_back();
    printf("%lld\n%d\n",DP[n-1][m-1],count);
    for(vector<pair<int,int>>::reverse_iterator it=path.rbegin(),end=path.rend();it!=end;it++){
      printf("Move %d %d\n",it->first,it->second);
    }

  return 0;
}




