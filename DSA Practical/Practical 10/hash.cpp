#include <bits/stdc++.h>
#define M 100 // Size of hash table
using namespace std;

struct Employee
{
    int key; // Unique 4-digit key
    char name[100];
    int age;
    double salary;

    Employee() : key(-1), age(0), salary(0.0) {
        name[0]='\0';
    }
};

// Hash table for employee records
Employee *hashTable[M];

// Hash function
int hashFunction(int key)
{
    return key % M;
}

// Insert record in hash table with linear probing
bool insertRecord(const Employee &emp)
{
    int index = hashFunction(emp.key);
    int originalIndex = index;
    int i = 1;

    // Linear probing for collision resolution
    while (hashTable[index] != nullptr && hashTable[index]->key != -1)
    {
        index = (originalIndex + i) % M;

        i++;

        if (index == originalIndex)
        {
            cout << "Hash Table Overfow" << endl;

            return false;
        }
    }

    // Insert the employee record at the found index
    hashTable[index] = new Employee(emp);

    return true;
}

// Load employees from file and populate hash table
void loadEmployeesFromFile(const std::string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error Opening File , Could not open file: " << filename << std::endl;
        return;
    }

    Employee emp;
    while (file >> emp.key >> emp.name >> emp.age >> emp.salary)
    {
        if (!insertRecord(emp))
        {
            cerr << "Failed to insert employee with key " << emp.key << endl;
        }
    }

    file.close();
}

// Display the hash table
void displayHashTable()
{
    cout << "Index\tKey\tName\tAge\tSalary\n";

    for (int i = 0; i < M; i++)
    {
        if (hashTable[i] != nullptr && hashTable[i]->key != -1)
        {
            cout << i << "\t" << hashTable[i]->key << "\t"
                      << hashTable[i]->name << "\t"
                      << hashTable[i]->age << "\t"
                      << hashTable[i]->salary << endl;
        }
    }
}

int main()
{
    // Initialize hash table
    for (int i = 0; i < M; i++)
    {
        hashTable[i] = nullptr;
    }

    // Load employees from the file
    loadEmployeesFromFile("EMP.dat");

    // Display the loaded hash table
    displayHashTable();

    // Free allocated memory
    for (int i = 0; i < M; i++)
    {
        delete hashTable[i];
    }

    return 0;
}