#include <iostream>
using namespace std;

void swap(int &a, int &b) 
    {
        int temp = a;
        a = b;
        b = temp;
    }


struct minheap 
{
    int *heap;
    int capacity;
    int size;

    minheap(int capacity) : capacity(capacity), size(0) 
    {
        heap = new int[capacity];
    }

    ~minheap() 
    {
        delete[] heap;
    }

    int parent(int i) 
    {
        return (i - 1) / 2;
    }

    int leftchild(int i) 
    {
        return 2 * i + 1;
    }

    int rightchild(int i) 
    {
        return 2 * i + 2;
    }

    void insert(int value) 
    {
        if (size == capacity) 
        {
            cout << "Heap overflow" << endl;
            return;
        }
        size++;
        int i = size - 1;
        heap[i] = value;
        heapifyup(i);
    }

    int deletef() 
    {
        if (size == 0) return -1; // or any appropriate handling for empty heap

        int deletedValue = heap[0];
        heap[0] = heap[size - 1];
        size--;
        heapifydown(0);
        return deletedValue;
    }

    void heapifyup(int i) 
    {
        while (i > 0 && heap[parent(i)] > heap[i]) 
        {
            swap(heap[parent(i)], heap[i]);
            i = parent(i);
        }
    }

    void heapifydown(int i) 
    {
        int minindex = i;
        int left = leftchild(i);
        int right = rightchild(i);

        if (left < size && heap[left] < heap[minindex]) 
        {
            minindex = left;
        }
        if (right < size && heap[right] < heap[minindex]) 
        {
            minindex = right;
        }

        if (minindex != i) 
        {
            swap(heap[i], heap[minindex]);
            heapifydown(minindex);
        }
    }

    int extractmin() 
    {
        return deletef();
    }

    void heapSort() 
    {
        for (int i = size / 2 - 1; i >= 0; i--)
        {
            heapifydown(i);
        }
        cout << "Sorted array: ";
        for (int i = size - 1; i >= 0; i--) 
        {
            swap(heap[0], heap[i]);
            heapifydown(0);
        }
        for (int i = 0; i < size; i++) 
        {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};

int main() 
{   
    //establish min heap
    minheap heap(10);

    //insertion
    heap.insert(3);
    heap.insert(0);
    heap.insert(1);
    heap.insert(7);
    heap.insert(2);

    //display heap
    cout << "Heap after insertion: ";
    heap.heapSort();

    //deletion
    cout << "Deleted element: " << heap.deletef() << endl;
    cout << "Heap after deletion: ";
    heap.heapSort();

    //extract min
    cout << "Extracted min element: " << heap.extractmin() << endl;
    cout << "Heap after extraction: ";
    heap.heapSort();    

    return 0;
}
