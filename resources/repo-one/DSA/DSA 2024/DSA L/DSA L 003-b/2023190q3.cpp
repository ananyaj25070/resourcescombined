#include<iostream>
using namespace std;

void merge(int arr[],int left,int mid,int right){
    int ls=mid-left+1;
    int rs=right-mid;
    int la[ls],ra[rs];

    for(int i = 0; i < ls; i++){
      la[i]=arr[left+i];}
    for(int j=0;j<rs;j++){
      ra[j]=arr[mid+1+j];}

    int i=0,j=0,k=left;
    while(i<ls && j<rs){
      if(la[i]<=ra[j]){
        arr[k]=la[i];
        i++;} 
      else{
        arr[k]=ra[j];
        j++;}
      k++;}

  while(i<ls){
    arr[k]=la[i];
    i++;
    k++;}

  while(j<rs){
    arr[k]=ra[j];
    j++;
    k++;}}

int main(){
    int sz;
    cin>>sz;
    int arr[sz];
    int a=0;int h=0;
    while(h<sz){
        cin>>arr[h];
        h++;}
    int n = sizeof(arr) / sizeof(arr[0]);

    merge(arr, 0, n/2, n - 1);

    for (int i = 0; i < n; i++) {
    cout << arr[i] << " ";}
    return 0;}