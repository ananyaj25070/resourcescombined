#include<iostream>
using namespace std;

int ins(){
    int sz;
    cin>>sz;
    int lin1[sz];
    int a=0;int h=0;
    while(h<sz){
        cin>>lin1[h];
        h++;}
    for(int i=0;i<sz;i++){
        int x=lin1[i];
    for(int j=i-1;j>=0;j--){
        if (lin1[j]>x){
            lin1[j + 1]=lin1[j];
            a+=1;}
        else{
            break;}
        lin1[j + 1] =x;}}
    return a;}

/*int mers(int lin1[]){
    int b=0;
    int p;int q;int r;int n1;int n2;
    int i=0,j=0,k=p;
    int n1 = q - p + 1;
    int n2 = r - q;
    int L[n1], M[n2];
    for (int i = 0; i < n1; i++){
        L[i] = lin1[p + i];}
    for (int j = 0; j < n2; j++){
        M[j] = lin1[q + 1 + j];}
    while (i < n1 && j < n2) {
    if (L[i] <= M[j]) {
      arr[k] = L[i];
      i++;} 
      else {
      arr[k] = M[j];
      j++;}
    k++;}
    
    return b;}
*/

int main(){
    int a,b;
    a=ins();
    //b=mers();
    cout<<a<<" and "<<"missing";
    return 0;
}

/*
Highest number of inversions possible is (n*(n-1))/2

The time complexity of mergesort remains O(nlogn) in best case, worst case AND average case......which means that 
the time complexity is independant of no inversions and remains same in the case of maximum inversions and no inversions at all. 
*/
