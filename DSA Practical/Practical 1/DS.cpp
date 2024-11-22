#include <bits/stdc++.h>
using namespace std;

void Add(int *A, int &Lindex, int E)
{
    if (Lindex == 0)
    { // edge test case handling
        A[Lindex] = E;
    }
    else
    {
        for (int i = 0; i < Lindex; i++)
        {
            if (E < A[i])
            {
                for (int j = Lindex; j > i; j--)
                {
                    A[j] = A[j - 1]; // shifting of array to right
                }
                A[i] = E; // assigning the array element
                Lindex++; // adjusting Lindex as Lindex represnt number of elements
                return;
            }
        }
        if (E >= A[Lindex - 1])
        { // edge test case handling (when every array element is bigger than number added)
            A[Lindex] = E;
        }
    }
    Lindex++;
}

int findMin(int *arr, int n)
{
    int min = arr[0]; // value intialization
    for (int i = 1; i < n; i++)
    {
        if (arr[i] < min)
        { // iteration which will compare all array elements
            min = arr[i];
        }
    }
    return min;
}
int findMax(int *arr, int n)
{
    int max = arr[0]; // value intialization
    for (int i = 1; i < n; i++)
    {
        if (arr[i] > max)
        { // iteration which will compare all array elements
            max = arr[i];
        }
    }
    return max;
}
void deleteElement(int *arr, int &n, int x)
{
    if (n == 0)
    { // edge test case handling
        cout << "Underflow, Array Is Empty" << endl;
        return;
    }
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == x)
        {
            for (int j = i; j < n - 1; j++)
            {
                arr[j] = arr[j + 1]; // shifting elements to left
            }
            n--; // Reduce the array size
            i--; // Adjust the index to check the shifted elements
        }
    }
}
void updateArray(int *arr, int n, int Lindex, int newVal)
{
    if (n == 0)
    { // edge test case handling
        cout << "Underflow" << endl;
        return;
    }
    for (int i = 0; i < n; i++)
    {
        if (arr[i] == Lindex)
        {
            arr[i] = newVal;
            break; // Break the loop after updating the first occurrence
        }
    }
}
void insertAtPosition(int arr[], int &Lindex, int insertAtPosition, int newVal, int maxCapacity)
{
    if (Lindex >= maxCapacity || insertAtPosition > Lindex)
    { // edge test case handling
        cout << "Array has reached its max capacity or invalid position. Please enter a valid value." << endl;
        return;
    }
    for (int j = Lindex; j > insertAtPosition; j--)
    {
        arr[j] = arr[j - 1]; // array Shifting
    }
    arr[insertAtPosition] = newVal; // assigning value at desired pos
    Lindex++;
}
void printArray(int *arr, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}
int main()
{
    int arr[10] = {10, 20, 30, 40};
    int Lindex = 4; // Logical index representing the number of initialized elements
    int len = sizeof(arr) / sizeof(arr[0]);
    int E = 60;                    // element to be added
    int x = 10;                    // element to be deleted
    int oldVal = 30;               // element to be updated
    int newVal = 99;               // replacement of the old value
    int insertAtPositionIndex = 2; // desired index
    int insertVal = 200;           // Value To Be inserted
    int maxCapacity = 10;          // max array pockets in array that are statically allocated

    cout << "1. Add Element" << endl;
    cout << "2. Find Max Of Element" << endl;
    cout << "3. Find Min Of Element" << endl;
    cout << "4. Delete Element In Array" << endl;
    cout << "5. Update Element In Array" << endl;
    cout << "6. Insert Element In Array" << endl;
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
        cout << "Adding Element In Array"<< endl;
        Add(arr, Lindex, E);
        break;
    case 2:
        cout << "Max element = " << findMax(arr, Lindex) << endl;
        break;
    case 3:
        cout << "Min element = " << findMin(arr, Lindex) << endl;
        break;
    case 4:
        cout << "deleting element from array"<< endl;
        deleteElement(arr, Lindex, x);
        break;
    case 5:
        cout << "updating element in array"<< endl;
        updateArray(arr, Lindex, oldVal, newVal);
        break;
    case 6:
        cout << "insertion at desired index"<< endl;
        insertAtPosition(arr, Lindex, insertAtPositionIndex, insertVal, maxCapacity);
        break;
    default:
        cout << "Invalid choice" << endl;
        return 0;
    }

    cout << "Array after operation: ";
    printArray(arr, Lindex);

    return 0;
}
