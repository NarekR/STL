//2D Array DS only function
int hourglassSum(vector<vector<int>> arr) {
    int max = -100;
    int temp;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp = arr[i][j] + arr[i][j + 1] + arr[i][j + 2] + arr[i + 1][j + 1] +
                arr[i + 2][j] + arr[i + 2][j + 1] + arr[i + 2][j + 2];
            if (max <= temp) {
                max = temp;
            }
        }
    }
    return max;
}

//NewYearChaos 
void minimumBribes(vector<int> q) {
    int swap_num = 0;
    bool chaotic = false;
    for (int i = 0; i < q.size(); i++) {
        if (q[i] - (i + 1) > 2) {
            chaotic = true;
            break;
        }
        for (int j = max(0, q[i] - 2); j < i; j++) {
            if (q[j] > q[i]) {
                swap_num++;
            }
        }
    }

    if (chaotic) {
        cout << "Too chaotic" << endl;
    }
    else {
        cout << swap_num << endl;
    }
}

//MinSwap
int minimumSwaps(vector<int> arr) {
    int swaps = 0;
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        while (arr[i] != i + 1) {
            swap(arr[i], arr[arr[i] - 1]);
            swaps++;
        }
    }

    return swaps;
}
//Manipulation
long arrayManipulation(int n, vector<vector<int>> queries) {
    vector<long> arr(n + 1, 0);

    for (const auto& q : queries) {
        int a = q[0];
        int b = q[1];
        int k = q[2];

        arr[a - 1] += k;
        if (b < n) {
            arr[b] -= k;
        }
    }
    long max_value = 0;
    long current_value = 0;
    for (int i = 0; i < n; i++) {
        current_value += arr[i];
        if (current_value > max_value) {
            max_value = current_value;
        }
    }

    return max_value;
}