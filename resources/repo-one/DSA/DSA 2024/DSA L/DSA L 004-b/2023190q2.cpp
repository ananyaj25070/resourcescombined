#include <iostream>
#include <vector>

using namespace std;

int mss(int arr[], int low, int high) {
    if (low == high) {
        return arr[low];
    }


    int mid = (low + high) / 2;
    int lm = mid, rm = mid;
    int sum_left = 0, sum_right = 0;

    for (int i = mid; i >= low; --i) {
        sum_left += arr[i];
        lm = max(lm, sum_left);
    }

    for (int i = mid + 1; i <= high; ++i) {
        sum_right += arr[i];
        rm = max(rm, sum_right);
    }

    int mlr = max(mss(arr, low, mid), mss(arr, mid + 1, high));
    return max(mlr, lm + rm);
}

int main() {
    // Taking input into the array
    int sz;
    cout << "Enter array size:" << endl;
    cin >> sz;

    int arr[sz];
    int h = 0;
    while (h < sz) {
        cout << "Enter element " << h << ":" << endl;
        cin >> arr[h];
        h++;
    }

    int n = sizeof(arr) / sizeof(arr[0]);

    //calling functin anf giving output
    int max_sum = mss(arr, 0, n - 1);
    cout << "Maximum subarray sum: " << max_sum << endl;

    return 0;
}
