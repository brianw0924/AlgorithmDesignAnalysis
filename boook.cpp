#include <iostream>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <vector>
#include <ctype.h>
#define ll long long 
// #pragma GCC optimize("O3,Ofast,no-stack-protector,unroll-loops,fast-math")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
using namespace std;
/* n=3 的測資會錯 */
int n,c[100002],d[100002],boook_1[10000000],boook_2[10000000],flag;
ll insertion_1=0, insertion_2=0,insertion_3=0,insertion_4=0;
int main(){
    int i,j,k,r,l=1,m,*ptr=&c[1];
    // vector<int> boook_1,boook_2;
    scanf("%d",&n);
    for(int i=1;i<=n;i++,ptr++){
        scanf("%d",ptr);
        d[i]=c[i];
    }
    // for(int i=1;i<=n;i++){printf("%d ",c[i]);}
    // printf("\n");
    // for(int i=1;i<=n;i++){printf("%d ",d[i]);}
    // printf("\n");
    scanf("%d",&flag);

    if(flag==0){

    while(c[l]==0){l++;} r=l+1;
    int temp = l;
    m=n;
    while(c[m]==0){m--;} // m 是最後一個非 0 的格子
    // printf("m=%d l=%d r=%d\n",m,l,r);
    if(r>m){
        insertion_3 = c[l]-1;
    }else{
    while(r<m){
        if(c[l]>c[r]){
            insertion_3+=(c[l]-c[r]);
            c[l]=0;
            c[r]=0;
        }else{
            c[r]-=c[l];
            c[l]=0;
        }
        r++; l++;
        if(r==m) {break;}
        if(c[r]==0){insertion_3++;}
        else{c[r]--;}
    }
    /* r = m ; c[r] > 0 ; c[l] >= 0 */
    /* 下一個要放 c[r] */

    if(c[l]>c[r])
        insertion_3+=c[l]-c[r];
    else if(c[r]>c[l])
        insertion_3+=c[r]-c[l]-1;
    }

    r=m;
    m=temp;
    l=r-1;
    // printf("m=%d l=%d r=%d\n",m,l,r);

    if(l<m){ // 一開始就跑到 m 左邊 , 也就是 d[l] == 0
        insertion_4 = d[r]-1;
    }else{
    
    while(l>m){
        /* 下一個要放 d[r] */
        if(d[r]>d[l]){
            insertion_4+=d[r]-d[l];
            d[r]=0;
            d[l]=0;
        }else{
            d[l]-=d[r];
            d[r]=0;
        }
        /* 最後一個放的是 d[l] */
        l--; r--;
        if(l==m) {break;}
        if(d[l]==0){insertion_4++;}
        else{d[l]--;}
    }

    /* l = m ; d[l] > 0 ; d[r] >= 0 */
    /* 下一個要放 d[l] */
    if(d[r]>d[l])
        insertion_4+=d[r]-d[l];
    else if(d[l]>d[r])
        insertion_4+=d[l]-d[r]-1;
    }

    if(insertion_3<insertion_4){
        // cout << "c1\n";
        printf("%lld\n",insertion_3);
    }
    else{
        // cout << "c2\n";
        printf("%lld\n",insertion_4);
    }
    
    return 0;
    }

    int b_1=0,b_2=0;
    /* ----------正向---------- */
    while(c[l]==0){l++;} r=l+1;
    int temp = l;
    m=n;
    while(c[m]==0){m--;} // m 是最後一個非 0 的格子
    // printf("m=%d l=%d r=%d\n",m,l,r);
    if(r>m){
        while(c[l]>1){
            // boook_1.push_back(l);
            // boook_1.push_back(r);
            boook_1[b_1++] = l;
            boook_1[b_1++] = r;
            c[l]--;
            insertion_1++;
        }
    // boook_1.push_back(l);
    boook_1[b_1++] = l;

    c[l]--;

    }else{

    while(r<m){
        /* 下一個要放 c[l] */
        while(c[l]!=0){ // c[l] 還沒放完
            // boook_1.push_back(l);
            // boook_1.push_back(r);
            boook_1[b_1++] = l;
            boook_1[b_1++] = r;

            c[l]--;
            if(c[r]==0){insertion_1++;}
            else{c[r]--;}
        }
        /* 最後一個放的是 c[r] */
        r++; l++;
        if(r==m) {break;}
        // boook_1.push_back(r);
            boook_1[b_1++] = r;
        if(c[r]==0){insertion_1++;}
        else{c[r]--;}
    }
    /* r = m ; c[r] > 0 ; c[l] >= 0 */
    /* 下一個要放 c[r] */
    while(c[r]>1){
        // boook_1.push_back(r);
        // boook_1.push_back(l);
        boook_1[b_1++] = r;
        boook_1[b_1++] = l;


        c[r]--;
        if(c[l]==0){insertion_1++;}
        else{c[l]--;}
    }
    // boook_1.push_back(r);
    c[r]--;
    /* c[r] = 0 */
    while(c[l]>1){
        // boook_1.push_back(l);
        // boook_1.push_back(r);
        boook_1[b_1++] = l;
        boook_1[b_1++] = r;

        c[l]--;
        if(c[r]==0){insertion_1++;}
        else{c[r]--;}
    }
    if(c[l]==1){
        // boook_1.push_back(l);
        boook_1[b_1++] = l;

        c[l]--;
    }
    }
    /* ----------正向---------- */

    /* ----------反向---------- */
    r=m;
    m=temp;
    l=r-1;
    // printf("m=%d l=%d r=%d\n",m,l,r);

    if(l<m){ // 一開始就跑到 m 左邊 , 也就是 d[l] == 0
        if(l>0){
            while(d[r]>1){
                // boook_2.push_back(r);
                // boook_2.push_back(l);
                boook_2[b_2++] = r;
                boook_2[b_2++] = l;
                d[r]--;
                insertion_2++;
            }
        }else{
            while(d[r]>1){
                // boook_2.push_back(r);
                // boook_2.push_back(r+1);
                boook_2[b_2++] = r;
                boook_2[b_2++] = r+1;

                d[r]--;
                insertion_2++;
            }
        }
    // boook_2.push_back(r);
    boook_2[b_2++] = r;


    d[r]--;

    }else{
    
    while(l>m){
        /* 下一個要放 d[r] */
        while(d[r]!=0){ // d[r] 還沒放完
            // boook_2.push_back(r);
            // boook_2.push_back(l);
            boook_2[b_2++] = r;
            boook_2[b_2++] = l;

            d[r]--;
            if(d[l]==0){insertion_2++;}
            else{d[l]--;}
        }
        /* 最後一個放的是 d[l] */
        l--; r--;
        if(l==m) {break;}
        // boook_2.push_back(l);
        boook_2[b_2++] = l;

        if(d[l]==0){insertion_2++;}
        else{d[l]--;}
    }

    /* l = m ; d[l] > 0 ; d[r] >= 0 */
    /* 下一個要放 d[l] */
    while(d[l]>1){
        // boook_2.push_back(l);
        // boook_2.push_back(r);
        boook_2[b_2++] = l;
        boook_2[b_2++] = r;

        d[l]--;
        if(d[r]==0){insertion_2++;}
        else{d[r]--;}
    }
    // boook_2.push_back(l);
    boook_2[b_2++] = l;

    d[l]--;
    /* d[l] = 0 */
    while(d[r]>1){
        // boook_2.push_back(r);
        // boook_2.push_back(l);
        boook_2[b_2++] = r;
        boook_2[b_2++] = l;

        d[r]--;
        if(d[l]==0){insertion_2++;}
        else{d[l]--;}
    }
    if(d[r]==1){
        // boook_2.push_back(r);
        boook_2[b_2++] = r;
        d[r]--;
    }

    }

    if(insertion_1 < insertion_2){
        // cout << "正向\n";
        printf("%lld\n",insertion_1);
        for(i=0;i<b_1;++i)
            printf("%d ",boook_1[i]);
        // for(vector<int>::iterator it=boook_1.begin(),end=boook_1.end();it!=end;it++)
        //     printf("%d ",*it);
        printf("\n");
        
    }else{
        // cout << "反向\n";
        printf("%lld\n",insertion_2);
        for(i=0;i<b_2;++i)
            printf("%d ",boook_2[i]);
        // for(vector<int>::iterator it=boook_2.begin(),end=boook_2.end();it!=end;it++)
        //     printf("%d ",*it);
        printf("\n");
    }

    return 0;
}