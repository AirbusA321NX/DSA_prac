#include <bits/stdc++.h>
using namespace std;

template <typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// Structure to hold mixed data (as string) and related functions
template<typename T>
struct MixedData {
    vector<string> arr;  // Array to store input data as strings
    int size;

    // Function to input data from console
    void inputFromConsole() {
        cout << "Enter number of elements: ";
        cin >> size;
        arr.resize(size);
        cout << "Enter elements (mix of int, char, string allowed): ";
        for (int i = 0; i < size; ++i) {
            cin >> arr[i]; // All inputs will be stored as strings
        }
    }

    // Function to input data from file
    void inputFromFile() {
        ifstream file("input.txt");
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }
        
        // Read elements directly without first reading the size
        string temp;
        arr.clear(); // Clear any previous data
        
        while (file >> temp) {  // Read until end of file
            arr.push_back(temp);  // Add elements one by one
        }
        size = arr.size();  // Set size based on number of elements
        file.close();
    }

    // Function to display data
    void display() {
        for (int i = 0; i < size; ++i) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

// Insertion Sort for mixed data (string-based)
template <typename T>
void insertionSort(MixedData<T>& data) {
    for (int i = 1; i < data.size; ++i) {
        string key = data.arr[i];
        int j = i - 1;
        while (j >= 0 && data.arr[j] > key) {
            data.arr[j + 1] = data.arr[j];
            j--;
        }
        data.arr[j + 1] = key;
    }
}

// Selection Sort for mixed data (string-based)
template <typename T>
void selectionSort(MixedData<T>& data) {
    for (int i = 0; i < data.size - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < data.size; ++j) {
            if (data.arr[j] < data.arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(data.arr[i], data.arr[minIndex]);
    }
}

// Bubble Sort for mixed data (string-based)
template <typename T>
void bubbleSort(MixedData<T>& data) {
    for (int i = 0; i < data.size - 1; ++i) {
        for (int j = 0; j < data.size - i - 1; ++j) {
            if (data.arr[j] > data.arr[j + 1]) {
                swap(data.arr[j], data.arr[j + 1]);
            }
        }
    }
}

// Quick Sort helper function for mixed data (string-based)
template <typename T>
int partition(MixedData<T>& data, int low, int high) {
    string pivot = data.arr[high];
    int i = (low - 1);
    for (int j = low; j < high; ++j) {
        if (data.arr[j] <= pivot) {
            i++;
            swap(data.arr[i], data.arr[j]);
        }
    }
    swap(data.arr[i + 1], data.arr[high]);
    return (i + 1);
}

// Quick Sort for mixed data (string-based)
template <typename T>
void quickSort(MixedData<T>& data, int low, int high) {
    if (low < high) {
        int pi = partition(data, low, high);
        quickSort(data, low, pi - 1);
        quickSort(data, pi + 1, high);
    }
}

// Linear Search for mixed data
template <typename T>
int linearSearch(MixedData<T>& data, string key) {
    for (int i = 0; i < data.size; ++i) {
        if (data.arr[i] == key) {
            return i;
        }
    }
    return -1;
}

// Binary Search for mixed data (array must be sorted lexicographically)
template <typename T>
int binarySearch(MixedData<T>& data, string key) {
    int left = 0, right = data.size - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (data.arr[mid] == key) {
            return mid;
        } else if (data.arr[mid] < key) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Function to perform counting sort for Radix Sort
void countingSort(vector<int>& arr, int size, int exp) {
    vector<int> output(size); // Output array
    int count[10] = {0};      // Count array

    // Count occurrences
    for (int i = 0; i < size; ++i) {
        count[(arr[i] / exp) % 10]++;
    }

    // Change count[i] so that it contains the position of this digit in output[]
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    // Build the output array
    for (int i = size - 1; i >= 0; --i) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now contains sorted numbers
    for (int i = 0; i < size; ++i) {
        arr[i] = output[i];
    }
}

// Radix Sort for integers
template <typename T>
void radixSort(MixedData<T>& data) {
    vector<int> intData;
    for (const auto& str : data.arr) {
        try {
            intData.push_back(stoi(str)); // Convert to integer
        } catch (invalid_argument&) {
            cout << "Ignoring non-integer value: " << str << endl;
        }
    }

    int size = intData.size();
    if (size == 0) return; // Avoid processing if no integers were found

    // Find the maximum number to know the number of digits
    int maxNum = *max_element(intData.begin(), intData.end());

    // Perform counting sort for every digit
    for (int exp = 1; maxNum / exp > 0; exp *= 10) {
        countingSort(intData, size, exp);
    }

    // Update the MixedData array with sorted integers converted back to strings
    data.arr.clear();
    for (const auto& num : intData) {
        data.arr.push_back(to_string(num));
    }
    data.size = data.arr.size(); // Update size
}

// Main program to choose input method and perform operations
int main() {
    int choice, operation, algorithm;
    bool exit = false;

    while (!exit) {
        cout << "Select Input Method:\n1. Console Input\n2. File Input (input.txt)\n3. Exit\n";
        cin >> choice;

        if (choice == 3) {
            exit = true;
            break;
        }

        // Create an object for mixed data (all data stored as strings)
        MixedData<string> mixedData;

        // Get input based on user's choice (console or file)
        if (choice == 1) mixedData.inputFromConsole();
        else if (choice == 2) mixedData.inputFromFile();

        cout << "Choose the operation:\n1. Searching\n2. Sorting\n";
        cin >> operation;

        if (operation == 1) {  // Searching
            cout << "Choose the searching algorithm:\n1. Linear Search\n2. Binary Search\n";
            cin >> algorithm;
            string key;
            cout << "Enter key to search: ";
            cin >> key;
            if (algorithm == 1)
                cout << "Found at index: " << linearSearch(mixedData, key) << endl;
            else {
                insertionSort(mixedData); // Ensure the array is sorted for binary search
                cout << "Found at index: " << binarySearch(mixedData, key) << endl;
            }
        } else if (operation == 2) { // Sorting
            cout << "Choose the sorting algorithm:\n1. Insertion Sort\n2. Selection Sort\n3. Bubble Sort\n4. Quick Sort\n5. Radix Sort (for integers)\n";
            cin >> algorithm;
            switch (algorithm) {
                case 1: insertionSort(mixedData); break;
                case 2: selectionSort(mixedData); break;
                case 3: bubbleSort(mixedData); break;
                case 4: quickSort(mixedData, 0, mixedData.size - 1); break;
                case 5: radixSort(mixedData); break;
                default: cout << "Invalid option!" << endl; continue;
            }
            cout << "Sorted data: ";
            mixedData.display();
        }
    }

    return 0;
}