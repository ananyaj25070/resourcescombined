#include<iostream>
#include<vector>
using namespace std;

bool help(vector<vector<int>>& arr,int row,int column,int r0,int c0){
    if(arr[row][column]!=0 && arr[0][0]!=0){ //check if entry and exit are open
        return false;}

    if(row==r0 && column==c0){ //check if u've reached exit
        return true;}

    if((row<0||column<0||row>r0||column>c0)){ //check if u're inside the maze
        return false;}
    
    if(arr[row][column]==1){ //check for walls
        return false;}

    arr[row][column] = 2; //mark already visited locations

    //check which directions you can go to
    return (help(arr,row+1,column,r0,c0)||help(arr,row-1,column,r0,c0)||help(arr,row,column+1,r0,c0)||help(arr,row,column-1,r0,c0));

    //repeater
    arr[row][column] = 0; 
    return false;}

int main(){
    //establish no of rows and columns
    int r,c;
    cout<<"Enter number of rows:"<<endl;
    cin>>r;
    cout<<"Enter number of columns:"<<endl;
    cin>>c;

    //take input to establish maze
    vector<vector<int>> arr(r, vector<int>(c));
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            cout<<"Enter value for "<<i+1<<" row and "<<j+1<<" column:"<<endl;
            cin>>arr[i][j];}}
    
    //calling function and sending out the output
    bool ans=help(arr,0,0,r-1,c-1);
    if(ans==1){
        cout<<"true";}
    else{cout<<"false";}
    return 0;}