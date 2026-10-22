#include<iostream>
#include<vector>
using namespace std;

class nod{
    public:
    int x;
    nod* next;

    nod(int val):x(val),next(nullptr){}};

class llist{
    public:
    nod* head;

    llist():head(nullptr){}
    void add(int val){
        nod* nnod=new nod(val);
        if(head==nullptr){
            head=nnod;}
        else{
            nod* temp=head;
            while(temp->next!=nullptr){
                temp=temp->next;}
            temp->next=nnod;}}

    void prin(){
        nod* temp=head;
        while(temp!=nullptr){
            cout<<temp->x<<" ";
            temp=temp->next;}
            cout<<endl;}

    void rot(int x){
        if(x==0||head==nullptr||head->next==nullptr){
            return;}

        int length=1;
        nod* temp=head;
        while(temp->next!=nullptr){
            length++;
            temp = temp->next;}

        x=x%length;
        x=length-x;

        nod* kthNode=head;
        for(int i=0;i<x-1;i++){
            kthNode=kthNode->next;}
        nod* newHead=kthNode->next;

        kthNode->next = nullptr;
        temp->next = head;
        head = newHead;}};

int main(){
    llist list;
    int sz;
    cout<<"Enter size of the linked list: ";
    cin>>sz;

    cout <<"Enter space seperated elements: ";
    int arr[sz];
    int inp;
    for(int i=0;i<sz;i++){
        cin>>inp;
        arr[i]=inp;
        list.add(inp);}
    
    int rc;
    cout<<"Enter the rotation constant: ";
    cin>>rc;
    list.rot(rc);

    cout<<"The rotated list is: ";
    if(rc%sz==0){
        for(int i=0;i<sz;i++){
            cout<<arr[i]<<" ";}}
    else{
        list.prin();}
    return 0;}