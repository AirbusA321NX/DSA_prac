#include <bits/stdc++.h>
#include <fstream>
#include <cstring>
#include <cstdlib>

#define SIZE 80
using namespace std;

struct Address
{
    char houseno[100];
    char street[100];
    char city[100];
    char state[100];
    char country[100];
};

struct Record
{
    char roll_no[50];
    char name[200];
    char f_name[200];
    char m_name[200];
    char Class[50];
    int semester;
    char branch[50];
    Address addr;
};

typedef struct Address ADD;
typedef struct Record RD;

RD S_DB[SIZE];
int Lindex = 0; // to keep track of the number of records

RD search_logfile[SIZE]; // Log for search operations
int search_count = 0;

RD delete_logfile[SIZE]; // Log for delete operations
int delete_count = 0;

void inputRecord(RD &rd);
void addRecord(RD S_DB[], int &count);
int deleteRecord(RD S_DB[], int &count);
void displayRecords(const RD S_DB[], int count);
RD search(RD S_DB[], int count);
void generateReport();
void logToFile(const string &operation, const RD &record);

string getInput(char *str, int size)
{
    int i = 0;
    char c;
    while (i < size - 1 && (c = getchar()) != '\n' && c != EOF)
    {
        str[i++] = c;
    }
    str[i] = '\0';
    return str;
}

int main()
{
    int choice;
    while (true)
    {
        cout << "\nMenu\n";
        cout << "1. Add Record\n";
        cout << "2. Delete Record\n";
        cout << "3. Display Records\n";
        cout << "4. Search\n";
        cout << "5. Generate Report\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To ignore the newline character left in the buffer

        switch (choice)
        {
        case 1:
            addRecord(S_DB, Lindex);
            break;
        case 2:
            if (deleteRecord(S_DB, Lindex) == 0)
            {
                cout << "Record deleted successfully.\n";
            }
            else
            {
                cout << "Record not found.\n";
            }
            break;
        case 3:
            displayRecords(S_DB, Lindex);
            break;
        case 4:
            search(S_DB, Lindex);
            break;
        case 5:
            generateReport();
            break;
        case 6:
            return 0; // Exit case
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}

void inputRecord(RD &rd)
{
    cout << "Input Student Roll Number: ";
    getInput(rd.roll_no, 50);
    cout << "Enter Student Name: ";
    getInput(rd.name, 200);
    cout << "Enter Father's Name: ";
    getInput(rd.f_name, 200);
    cout << "Enter Mother's Name: ";
    getInput(rd.m_name, 200);
    cout << "Enter Class: ";
    getInput(rd.Class, 50);
    cout << "Enter Semester: ";
    cin >> rd.semester;
    cin.ignore();
    cout << "Enter Branch: ";
    getInput(rd.branch, 50);
    cout << "Enter House No: ";
    getInput(rd.addr.houseno, 100);
    cout << "Enter Street: ";
    getInput(rd.addr.street, 100);
    cout << "Enter City: ";
    getInput(rd.addr.city, 100);
    cout << "Enter State: ";
    getInput(rd.addr.state, 100);
    cout << "Enter Country: ";
    getInput(rd.addr.country, 100);
}

void addRecord(RD S_DB[], int &count)
{
    if (count >= SIZE) // checking for overflow
    {
        cout << "Database is full. Cannot add more records.\n";
        return;
    }
    RD rd;
    inputRecord(rd);
    S_DB[count++] = rd;
    logToFile("ADD", rd); // Log the added record to add_log.txt
    cout << "Record added successfully.\n";
}

void logToFile(const string &operation, const RD &record)
{
    ofstream logFile;
    if (operation == "ADD")
    {
        logFile.open("Add_log.txt", ios::app); // Open file in append mode
    }
    else if (operation == "DELETE")
    {
        logFile.open("Delete_log.txt", ios::app); // Open file in append mode
    }
    else if (operation == "SEARCH")
    {
        logFile.open("Search_log.txt", ios::app); // Open file in append mode
    }
    else if(operation == "reportFile"){
         logFile.open("report_log.txt", ios::app); // Open file in append mode
    }
    if (logFile.is_open())
    {
        logFile << "Operation: " << operation << "\n";
        logFile << "Roll Number: " << record.roll_no << "\n";
        logFile << "Name: " << record.name << "\n";
        logFile << "Father's Name: " << record.f_name << "\n";
        logFile << "Mother's Name: " << record.m_name << "\n";
        logFile << "Class: " << record.Class << "\n";
        logFile << "Semester: " << record.semester << "\n";
        logFile << "Branch: " << record.branch << "\n";
logFile << "Address: " << record.addr.houseno << ", " << record.addr.street << ", " << record.addr.city << ", " << record.addr.state << ", " << record.addr.country << "\n";
        logFile << "--------------------------------------\n";
    }
    else
    {
        cout << "Unable to open log file.\n";
    }

    logFile.close();
}

int deleteRecord(RD S_DB[], int &count)
{
    if (count == 0)
    {
        cout << "No records to delete. The database is empty.\n"; // underflow
        return -1;
    }
    char roll_no[50];
    cout << "Enter Roll Number to delete: ";
    getInput(roll_no, 50);

    int index = -1;
    for (int i = 0; i < count; ++i)
    {
        if (strcmp(S_DB[i].roll_no, roll_no) == 0)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        // Log the deleted record
        delete_logfile[delete_count++] = S_DB[index];
        logToFile("DELETE", S_DB[index]);

        // Shift records to fill the gap
        for (int i = index; i < count - 1; ++i)
        {
            S_DB[i] = S_DB[i + 1];
        }
        --count;
        return 0;
    }
    else
    {
        return -1;
    }
}

void displayRecords(const RD S_DB[], int count)
{
    if (count == 0)
    {
        cout << "No records to display.\n";
        return;
    }
    for (int i = 0; i < count; ++i)
    {
        const RD &rd = S_DB[i];
        cout << "\nRoll Number: " << rd.roll_no << "\n";
        cout << "Name: " << rd.name << "\n";
        cout << "Father's Name: " << rd.f_name << "\n";
        cout << "Mother's Name: " << rd.m_name << "\n";
        cout << "Class: " << rd.Class << "\n";
        cout << "Semester: " << rd.semester << "\n";
        cout << "Branch: " << rd.branch << "\n";
        cout << "Address: " << rd.addr.houseno << ", " << rd.addr.street << ", " << rd.addr.city << ", " << rd.addr.state << ", " << rd.addr.country << "\n";
    }
}

RD search(RD S_DB[], int count)
{
    char location[100];
    cout << "Starting search\n";
    cout << "1. City\n";
    cout << "2. State\n";
    cout << "3. Country\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    cin.ignore();

    RD result;
    bool found = false;

    switch (choice)
    {
    case 1:
        cout << "Enter City: ";
        getInput(location, 100);
        for (int i = 0; i < count; ++i)
        {
            if (strcmp(S_DB[i].addr.city, location) == 0)
            {
                result = S_DB[i];
                found = true;
                break;
            }
        }
        break;
    case 2:
        cout << "Enter State: ";
        getInput(location, 100);
        for (int i = 0; i < count; ++i)
        {
            if (strcmp(S_DB[i].addr.state, location) == 0)
            {
                result = S_DB[i];
                found = true;
                break;
            }
        }
        break;
    case 3:
        cout << "Enter Country: ";
        getInput(location, 100);
        for (int i = 0; i < count; ++i)
        {
            if (strcmp(S_DB[i].addr.country, location) == 0)
            {
                result = S_DB[i];
                found = true;
                break;
            }
        }
        break;
    default:
        cout << "Invalid choice.\n";
    }

    if (found)
    {
        // Log the search result
        search_logfile[search_count++] = result;
        logToFile("SEARCH", result);

        // Display the found record
        cout << "\nRoll Number: " << result.roll_no << "\n";
        cout << "Name: " << result.name << "\n";
        cout << "Father's Name: " << result.f_name << "\n";
        cout << "Mother's Name: " << result.m_name << "\n";
        cout << "Class: " << result.Class << "\n";
        cout << "Semester: " << result.semester << "\n";
        cout << "Branch: " << result.branch << "\n";
        cout << "Address: " << result.addr.houseno << ", " << result.addr.street << ", " << result.addr.city << ", " << result.addr.state << ", " << result.addr.country << "\n";
    }
    else
    {
        cout << "No record found.\n";
    }

    return result;
}

void generateReport()
{
    cout << "\nGenerating Report...\n";

    if (search_count > 0)
    {
        cout << "\nSearch Log:\n";
        for (int i = 0; i < search_count; ++i)
        {
            const RD &rd = search_logfile[i];
            cout << "\nRoll Number: " << rd.roll_no << "\n";
            cout << "Name: " << rd.name << "\n";
            cout << "Father's Name: " << rd.f_name << "\n";
            cout << "Mother's Name: " << rd.m_name << "\n";
            cout << "Class: " << rd.Class << "\n";
            cout << "Semester: " << rd.semester << "\n";
            cout << "Branch: " << rd.branch << "\n";
            cout << "Address: " << rd.addr.houseno << ", " << rd.addr.street << ", " << rd.addr.city << ", " << rd.addr.state << ", " << rd.addr.country << "\n";
        }
    }
    else
    {
        cout << "\nNo search operations to report.\n";
    }

    if (delete_count > 0)
    {
        cout << "\nDelete Log:\n";
        for (int i = 0; i < delete_count; ++i)
        {
            const RD &rd = delete_logfile[i];
            cout << "\nRoll Number: " << rd.roll_no << "\n";
            cout << "Name: " << rd.name << "\n";
            cout << "Father's Name: " << rd.f_name << "\n";
            cout << "Mother's Name: " << rd.m_name << "\n";
            cout << "Class: " << rd.Class << "\n";
            cout << "Semester: " << rd.semester << "\n";
            cout << "Branch: " << rd.branch << "\n";
            cout << "Address: " << rd.addr.houseno << ", " << rd.addr.street << ", " << rd.addr.city << ", " << rd.addr.state << ", " << rd.addr.country << "\n";
        }
    }
    else
    {
        cout << "\nNo delete operations to report.\n";
    }

    cout << "\nReport generated successfully.\n";
}
void saveReportToFile()
{
    ofstream reportFile("report_log.txt",ios::app);

    if (reportFile.is_open())
    {
        reportFile << "Report Log\n\n";

        if (search_count > 0)
        {
            reportFile << "Search Log:\n";
            for (int i = 0; i < search_count; ++i)
            {
                const RD &rd = search_logfile[i];
                reportFile << "\nRoll Number: " << rd.roll_no << "\n";
                reportFile << "Name: " << rd.name << "\n";
                reportFile << "Father's Name: " << rd.f_name << "\n";
                reportFile << "Mother's Name: " << rd.m_name << "\n";
                reportFile << "Class: " << rd.Class << "\n";
                reportFile << "Semester: " << rd.semester << "\n";
                reportFile << "Branch: " << rd.branch << "\n";
                reportFile << "Address: " << rd.addr.houseno << ", " << rd.addr.street << ", " << rd.addr.city << ", " << rd.addr.state << ", " << rd.addr.country << "\n";
            }
        }
        else
        {
            reportFile << "No search operations to report.\n";
        }

        if (delete_count > 0)
        {
            reportFile << "\nDelete Log:\n";
            for (int i = 0; i < delete_count; ++i)
            {
                const RD &rd = delete_logfile[i];
                reportFile << "\nRoll Number: " << rd.roll_no << "\n";
                reportFile << "Name: " << rd.name << "\n";
                reportFile << "Father's Name: " << rd.f_name << "\n";
                reportFile << "Mother's Name: " << rd.m_name << "\n";
                reportFile << "Class: " << rd.Class << "\n";
                reportFile << "Semester: " << rd.semester << "\n";
                reportFile << "Branch: " << rd.branch << "\n";
                reportFile << "Address: " << rd.addr.houseno << ", " << rd.addr.street << ", " << rd.addr.city << ", " << rd.addr.state << ", " << rd.addr.country << "\n";
            }
        }
        else
        {
            reportFile << "No delete operations to report.\n";
        }

        reportFile.close();
        cout << "\nReport saved to report_log.txt successfully.\n";
    }
    else
    {
        cout << "\nUnable to open file to save the report.\n";
    }
}