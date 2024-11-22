#include <bits/stdc++.h>
#include <fstream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX_LEN 100
#define DATE_FORMAT_LEN 11

// Global log files
const char *deleteLogFile = "delete_log_double.txt";
const char *searchLogFile = "search_log_double.txt";
const char *reportLogFile = "report_log_double.txt";

// Structure for storing record
struct Record
{
    char aadhar[13];
    char name[MAX_LEN];
    char gender[10];
    char fatherName[MAX_LEN];
    char motherName[MAX_LEN];
    char dob[DATE_FORMAT_LEN];
};

// Structure for doubly linked list node
struct Node
{
    Record data;
    Node *next;
    Node *prev;
};

// Function prototypes
void get_input_custom(char *str, int length);
bool isValidAadhar(const char *aadhar);
bool isValidDate(char *dob);
void toUpper(char *str);
bool containsDigit(const char *str);
void replaceDots(char *str);
void logToFile(const char *filename, const char *logEntry);
Node *createNode(const Record &record);
Node *insertNode(Node *head, const Record &record);
Node *deleteNode(Node *head, const char *aadhar);
Node *updateNode(Node *head, const char *aadhar);
void searchNode(Node *head, int searchBy);
void generateReport(Node *head);
void displayAllRecords(Node *head);
bool doesAadharExist(Node *head, const char *aadhar);

// Function implementations
void get_input_custom(char *str, int length) // Custom input function
{
    int index = 0;
    char ch;
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;

    // Get current console mode
    GetConsoleMode(hstdin, &mode);

    // Disable echo input
    SetConsoleMode(hstdin, mode & ~(ENABLE_ECHO_INPUT));

    while (index < length - 1)
    {
        ch = _getch(); // Read one character at a time without displaying it

        if (ch == '\r' || ch == '\n')
        { // Break on Enter key
            break;
        }

        if (!isdigit(ch))
        {                 // Check if the character is not a digit
            str[index] = ch; // Store non-digit character
            cout << ch;      // Display the character
            index++;
        }
        // Digits are ignored
    }

    str[index] = '\0'; // Null-terminate the string

    // Restore console mode
    SetConsoleMode(hstdin, mode);
    cout << endl; // Move to the next line after input is complete
}
void customInput(char *buffer, int length)
{
    cin.getline(buffer, length);
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
}
bool isValidAadhar(const char *aadhar)
{
    if (strlen(aadhar) != 12)
        return false;
    for (int i = 0; i < 12; i++)
    {
        if (!isdigit(aadhar[i]))
            return false;
    }
    return true;
}

bool doesAadharExist(Node *head, const char *aadhar)
{
    Node *temp = head;
    while (temp)
    {
        if (strcmp(temp->data.aadhar, aadhar) == 0)
        {
            cout << "Duplicate Aadhar found!\n"
                 << endl; // Debug output
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool isValidDate(char *dob)
{
    int day, month, year;
    char tempDob[DATE_FORMAT_LEN];

    // Check if the input is in ddmmyyyy format
    if (strlen(dob) == 8 && sscanf(dob, "%2d%2d%4d", &day, &month, &year) == 3)
    {
        // Convert to dd/mm/yyyy format
        snprintf(tempDob, DATE_FORMAT_LEN, "%02d/%02d/%04d", day, month, year);
        strncpy(dob, tempDob, DATE_FORMAT_LEN);
    }
    else if (strlen(dob) == 10 && sscanf(dob, "%2d/%2d/%4d", &day, &month, &year) != 3)
    {
        return false;
    }
    else
    {
        return false;
    }

    // Validate the date
    if (year < 1900 || year > 2099)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;
    if (month == 2)
    {
        bool leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (leap ? 29 : 28))
            return false;
    }

    return true;
}

void toUpper(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }
}

bool containsDigit(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (isdigit(str[i]))
            return true;
    }
    return false;
}

void replaceDots(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '.')
        {
            str[i] = ' ';
        }
    }
}

void logToFile(const char *filename, const char *logEntry)
{
    ofstream logFile;
    logFile.open(filename, ios::app);
    if (logFile.is_open())
    {
        logFile << logEntry << endl;
        logFile.close();
    }
}

Node *createNode(const Record &record)
{
    Node *newNode = new Node();
    newNode->data = record;
    newNode->next = nullptr;
    newNode->prev = nullptr;
    return newNode;
}

Node *insertNode(Node *head, const Record &record)
{
    // Check for duplicate Aadhar number
    if (doesAadharExist(head, record.aadhar))
    {
        cout << "Error: Aadhar number " << record.aadhar << " already exists in the database." << endl;
        return head; // Abort insertion
    }

    Node *newNode = createNode(record);
    if (!head)
        return newNode;

    Node *temp = head;
    Node *prev = nullptr;

    // Find the correct position to insert the new node
    while (temp && strcmp(temp->data.aadhar, record.aadhar) < 0)
    {
        prev = temp;
        temp = temp->next;
    }

    // Insert the new node at the correct position
    if (prev)
    {
        prev->next = newNode;
        newNode->prev = prev;
        newNode->next = temp;
        if (temp)
            temp->prev = newNode;
    }
    else
    {
        newNode->next = head;
        if (head)
            head->prev = newNode;
        head = newNode;
    }

    cout << "Record inserted successfully!" << endl; // Confirmation message
    return head;
}

Node *deleteNode(Node *head, const char *aadhar)
{
    Node *temp = head;
    while (temp)
    {
        if (strcmp(temp->data.aadhar, aadhar) == 0)
        {
            if (temp->prev)
                temp->prev->next = temp->next;
            if (temp->next)
                temp->next->prev = temp->prev;
            if (temp == head)
                head = temp->next;
            char logEntry[MAX_LEN];
            snprintf(logEntry, MAX_LEN, "Deleted record with Aadhar: %s", aadhar);
            logToFile(deleteLogFile, logEntry);
            delete temp;
            return head;
        }
        temp = temp->next;
    }
    cout << "Record not found!" << endl;
    return head;
}

Node *updateNode(Node *head, const char *aadhar)
{
    Node *temp = head;
    while (temp)
    {
        if (strcmp(temp->data.aadhar, aadhar) == 0)
        {
            Record newRecord;
            cout << "Enter new details for Aadhar: " << aadhar << endl;

            while (true)
            {
                cout << "Enter Name: ";
                get_input_custom(newRecord.name, MAX_LEN);
                replaceDots(newRecord.name);
                if (containsDigit(newRecord.name))
                {
                    cout << "Name should not have digits. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Enter Gender (1 for Male, 2 for Female , 3 for Others): ";
                char genderOption[10];
                get_input_custom(genderOption, 10);
                toUpper(genderOption);
                if (strcmp(genderOption, "1") == 0)
                    strcpy(newRecord.gender, "Male");
                else if (strcmp(genderOption, "2") == 0)
                    strcpy(newRecord.gender, "Female");
                else if (strcmp(genderOption, "3") == 0)
                    strcpy(newRecord.gender, "Others");
                else
                {
                    cout << "Invalid gender option. Please re-enter." << endl;
                    continue;
                }
                break;
            }

            while (true)
            {
                cout << "Enter Father's Name: ";
                get_input_custom(newRecord.fatherName, MAX_LEN);
                replaceDots(newRecord.fatherName);
                if (containsDigit(newRecord.fatherName))
                {
                    cout << "Father's Name should not have digits. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Enter Mother's Name: ";
                get_input_custom(newRecord.motherName, MAX_LEN);
                replaceDots(newRecord.motherName);
                if (containsDigit(newRecord.motherName))
                {
                    cout << "Mother's Name should not have digits. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Enter Date of Birth (ddmmyyyy or dd/mm/yyyy): ";
                customInput(newRecord.dob, DATE_FORMAT_LEN);
                if (!isValidDate(newRecord.dob))
                {
                    cout << "Invalid date format. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            temp->data = newRecord;
            cout << "Record updated successfully!" << endl;
            return head;
        }
        temp = temp->next;
    }
    cout << "Record not found!" << endl;
    return head;
}

void searchNode(Node *head, int searchBy)
{
    char searchValue[MAX_LEN];
    cout << "Enter search value: ";
    customInput(searchValue, MAX_LEN);
    replaceDots(searchValue);

    Node *temp = head;
    bool found = false;
    while (temp)
    {
        bool match = false;
        switch (searchBy)
        {
        case 1: // Search by Aadhar
            match = (strcmp(temp->data.aadhar, searchValue) == 0);
            break;
        case 2: // Search by Name
            match = (strcasecmp(temp->data.name, searchValue) == 0);
            break;
        case 3: // Search by Father's Name
            match = (strcasecmp(temp->data.fatherName, searchValue) == 0);
            break;
        case 4: // Search by Mother's Name
            match = (strcasecmp(temp->data.motherName, searchValue) == 0);
            break;
        }
        if (match)
        {
            cout << "Aadhar: " << temp->data.aadhar << "\n";
            cout << "Name: " << temp->data.name << "\n";
            cout << "Gender: " << temp->data.gender << "\n";
            cout << "Father's Name: " << temp->data.fatherName << "\n";
            cout << "Mother's Name: " << temp->data.motherName << "\n";
            cout << "DOB: " << temp->data.dob << "\n";
            found = true;
            break; // Optional: stop after first match
        }
        temp = temp->next;
    }

    if (!found)
    {
        cout << "Record not found!" << endl;
    }
    else
    {
        char logEntry[MAX_LEN];
        snprintf(logEntry, MAX_LEN, "Searched for value: %s", searchValue);
        logToFile(searchLogFile, logEntry);
    }
}

void generateReport(Node *head)
{
    Node *temp = head;
    ofstream reportFile(reportLogFile);

    if (!reportFile.is_open())
    {
        cout << "Failed to open report file." << endl;
        return;
    }

    reportFile << "Aadhar, Name, Gender, Father's Name, Mother's Name, DOB\n";
    while (temp)
    {
        reportFile << temp->data.aadhar << ", "
                   << temp->data.name << ", "
                   << temp->data.gender << ", "
                   << temp->data.fatherName << ", "
                   << temp->data.motherName << ", "
                   << temp->data.dob << "\n";
        temp = temp->next;
    }

    reportFile.close();
    cout << "Report generated successfully!" << endl;
}

void displayAllRecords(Node *head)
{
    if (!head)
    {
        cout << "No records found!" << endl;
        return;
    }

    Node *temp = head;
    while (temp)
    {
        cout << "Aadhar: " << temp->data.aadhar << "\n";
        cout << "Name: " << temp->data.name << "\n";
        cout << "Gender: " << temp->data.gender << "\n";
        cout << "Father's Name: " << temp->data.fatherName << "\n";
        cout << "Mother's Name: " << temp->data.motherName << "\n";
        cout << "DOB: " << temp->data.dob << "\n";
        cout << "------------------------" << endl;
        temp = temp->next;
    }
}
int main()
{
    Node *head = nullptr;
    int choice;
    do
    {
        cout << "Menu:\n";
        cout << "1. Insert Record\n";
        cout << "2. Delete Record\n";
        cout << "3. Update Record\n";
        cout << "4. Search Record\n";
        cout << "5. Generate Report\n";
        cout << "6. Display All Records\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear the input buffer

        switch (choice)
        {
        case 1:
        {
            Record record;
            while (true)
            {
                cout << "Enter Aadhar: ";
                customInput(record.aadhar, 13);
                if (!isValidAadhar(record.aadhar))
                {
                    cout << "Invalid Aadhar number. Aadhar must be 12 digits long." << endl;
                    continue;
                }
                
                if (doesAadharExist(head, record.aadhar))
                {
                    cout << "Error: Aadhar number " << record.aadhar << " already exists in the database." << endl;
                    continue;
                }
                break;
            }

            while (true)
            {
                cout << "Enter Name: ";
                get_input_custom(record.name, MAX_LEN);
                replaceDots(record.name);
                if (containsDigit(record.name))
                {
                    cout << "Name should not have digits. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Enter Gender (1 for Male, 2 for Female, 3 For Others): ";
                char genderOption[10];
                customInput(genderOption, 10);
                toUpper(genderOption);
                if (strcmp(genderOption, "1") == 0)
                    strcpy(record.gender, "Male");
                else if (strcmp(genderOption, "2") == 0)
                    strcpy(record.gender, "Female");
                else if (strcmp(genderOption, "3") == 0)
                    strcpy(record.gender, "Others");
                else
                {
                    cout << "Invalid gender option. Please re-enter." << endl;
                    continue;
                }
                break;
            }

            while (true)
            {
                cout << "Enter Father's Name: ";
                get_input_custom(record.fatherName, MAX_LEN);
                replaceDots(record.fatherName);
                if (containsDigit(record.fatherName))
                {
                    cout << "Father's Name should not have digits. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Enter Mother's Name: ";
                get_input_custom(record.motherName, MAX_LEN);
                replaceDots(record.motherName);
                if (containsDigit(record.motherName))
                {
                    cout << "Mother's Name should not have digits. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Enter Date of Birth (ddmmyyyy or dd/mm/yyyy): ";
                customInput(record.dob, DATE_FORMAT_LEN);
                if (!isValidDate(record.dob))
                {
                    cout << "Invalid date format. Please re-enter." << endl;
                }
                else
                {
                    break;
                }
            }

            head = insertNode(head, record);
            break;
        }

        case 2:
        {
            char aadhar[13];
            cout << "Enter Aadhar to delete: ";
            get_input_custom(aadhar, 13);
            head = deleteNode(head, aadhar);
            break;
        }

        case 3:
        {
            char aadhar[13];
            cout << "Enter Aadhar to update: ";
            get_input_custom(aadhar, 13);
            head = updateNode(head, aadhar);
            break;
        }

        case 4:
        {
            int searchBy;
            cout << "Search by:\n";
            cout << "1. Aadhar\n";
            cout << "2. Name\n";
            cout << "3. Father's Name\n";
            cout << "4. Mother's Name\n";
            cout << "Enter your choice: ";
            cin >> searchBy;
            cin.ignore(); // Clear the input buffer
            searchNode(head, searchBy);
            break;
        }

        case 5:
            generateReport(head);
            break;

        case 6:
            displayAllRecords(head);
            break;

        case 7:
            cout << "Exiting program." << endl;
            break;

        default:
            cout << "Invalid choice. Please enter a number between 1 and 7." << endl;
        }
    } while (choice != 7);

    return 0;
}
