#include <iostream>
using namespace std;

int part(int A[],int p,int q){
    int random = p+rand()%(q-p+1);
    int x = A[random];
    int i=p;
    swap(A[random], A[p]);
    for(int j=p+1;j<=q;j++){
        if(A[j]<=x){
            i=i+1;
            swap(A[i], A[j]);}}
    swap(A[i], A[p]);
    return i+1;}

void quicksort(int A[],int p,int q){
    if(p<q){
        int r=part(A, p, q);
        cout << "Partition is: ";
        for(int i=0;i<q-p+1;i++) {
        cout<<A[i]<< " ";}
        cout<<"Pivot: "<<A[r]<< endl;
        quicksort(A,p,r-1);
        quicksort(A,r+1,q);}}

int main(){
    int n;
    cout<<"Enter the length of array: ";
    cin>>n;
    int* A=new int[n];

    for(int i=0;i<n;i++){
        cout<<"Enter the element "<<i<<": ";
        cin>>A[i];}

    quicksort(A, 0, n - 1);

    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";}
    cout << endl;}