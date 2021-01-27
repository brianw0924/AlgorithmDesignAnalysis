#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctype.h>
#include <utility>
#include <limits>
#pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
# define llu unsigned long long 
using namespace std;

int N;
vector<pair<llu,llu>> X,Y;

bool sortbyY(const pair<llu,llu> &a, const pair<llu,llu> &b) {
    return (a.second < b.second); 
} 

bool sortbyX(const pair<llu,llu> &a, const pair<llu,llu> &b) {
    if(a.first<b.first)
        return true;
    else if(a.first == b.first)
        return a.second < b.second;
    else
        return false;
} 


llu dist(pair<llu,llu> a, pair<llu,llu> b){
    llu dx = (a.first-b.first), dy = (a.second-b.second);
    return dx*dx+dy*dy;
}


llu bruteforce(vector<pair<llu,llu>> Px, vector<pair<llu,llu>> Py, int n){
    if(n == 0){
        return numeric_limits<llu>::max();
    }else if(n == 1){
        return dist(Px[0],Px[1]);
    }else{
        return min(dist(Px[0],Px[1]),min(dist(Px[1],Px[2]),dist(Px[0],Px[2])));
    }
}

llu DQ(vector<pair<llu,llu>> Px, vector<pair<llu,llu>> Py, int n){
    if(n<3){
        return bruteforce(Px,Py,n);
    }
    int L = n/2,R, size = n+1;
    if(size%2==0)
        R = n/2;
    else
        R = n/2 - 1;
    llu midpoint = Px[L].first, midY = Px[L].second, delta;
    vector<pair<llu,llu>> Lx(Px.begin(),Px.begin()+L+1), Ly;
    vector<pair<llu,llu>> Rx(Px.begin()+L+1,Px.begin()+n+1),Ry;
    
    for(int i=0;i<size;++i){
        if(Py[i].first > midpoint){
            Ry.push_back(Py[i]);
        }else if(Py[i].first == midpoint && Py[i].second > midY){
            Ry.push_back(Py[i]);
        }else
            Ly.push_back(Py[i]);
    }

    delta = min(DQ(Lx,Ly,L),DQ(Rx,Ry,R));


    vector<pair<llu,llu>> middle;
    for(int i=0;i<size;++i){
        if((Py[i].first - midpoint)*(Py[i].first - midpoint) < delta)
            middle.push_back(Py[i]);
    }
    llu d;
    for(vector<pair<llu,llu>>::iterator it=middle.begin(),end=middle.end();it!=end;it++){
        vector<pair<llu,llu>>::iterator next7 = it+1;
        for(int i=0;i<7 && next7!=end;i++,next7++){
            d = dist(*it,*next7);
            if(d<delta)
                delta = d;
        }
    }

    vector<pair<llu,llu>> ().swap(Rx);
    vector<pair<llu,llu>> ().swap(Ry);
    vector<pair<llu,llu>> ().swap(Lx);
    vector<pair<llu,llu>> ().swap(Ly);
    vector<pair<llu,llu>> ().swap(middle);
    return delta;

}

int main(){ // 重疊點非連續會有問題
    scanf("%d",&N);
    llu x,y,lastx=-1,lasty=-1;

    for(int i=0;i<N;++i){
        scanf("%lld%lld",&x,&y);
        if(x==lastx && y==lasty){
            printf("%d\n",0);
            return 0;
        }
        X.push_back({x,y});
        Y.push_back({x,y});
        lastx = x;
        lasty = y;
    }
    sort(X.begin(),X.end(),sortbyX);
    sort(Y.begin(),Y.end(),sortbyY);
    llu d = DQ(X,Y,N-1);
    if(d==0)
        printf("%d\n",0);
    else
        printf("-%llu\n",d);
    return 0;
}