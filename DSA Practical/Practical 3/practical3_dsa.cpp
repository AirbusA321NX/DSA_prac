#include <bits/stdc++.h>
#include <fstream>
#include <iomanip> 
#include <cctype>  
#include <cstring> 

using namespace std;

const int MAX_NAME_LENGTH = 100;
const int MAX_DATE_LENGTH = 11; // DD MM YYYY or DDMMYYYY
const int MAX_ADDRESS_LENGTH = 200;
const int MAX_PIN_LENGTH = 10;

struct Employee
{
    int id;
    char jindal_name[MAX_NAME_LENGTH];
    char jindal_fathername[MAX_NAME_LENGTH];
    char jindal_mothername[MAX_NAME_LENGTH];
    char jindal_rank[MAX_NAME_LENGTH];
    char jindal_deparment[MAX_NAME_LENGTH];
    char jindal_dateofjoining[MAX_DATE_LENGTH];
    char jindal_homeaddress[MAX_ADDRESS_LENGTH];
    char jindal_streetname[MAX_ADDRESS_LENGTH];
    char jindal_city[MAX_ADDRESS_LENGTH];
    char jindal_state[MAX_ADDRESS_LENGTH];
    char jindal_country[MAX_ADDRESS_LENGTH];
    char jindal_pin[MAX_PIN_LENGTH];
    Employee *next;
};

// Global file stream objects for logging
ofstream searchLog("search_log.txt", ios::app);
ofstream deleteLog("delete_log.txt", ios::app);
ofstream searchByCommonLog("search_by_common_log.txt", ios::app);
ofstream reportLog("report_log_new.txt", ios::app); // Report log file
ofstream logFile("update_log.txt", ios::app);

Employee *head = nullptr;
int currentRecordCount = 0;
void get_input(char *str, int length) // custom input function
{
    if (length <= 0) 
        return; // Invalid length(Underflow)

    cin.get(str, length); // Read up to (length - 1) characters or until newline

    // Check for newline character
    if (cin.peek() == '\n')
    {
        cin.ignore(); // Ignore the newline character
    }
}

void replaceDotsWithSpaces(char *str) // functionality to remove dot with space
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '.')
        {
            str[i] = ' ';
        }
    }
}

void formatDate(const char *date, char *formattedDate)
{
    if (strlen(date) == 8)
    { // Format DDMMYYYY
        sprintf(formattedDate, "%c%c/%c%c/%c%c%c%c", date[0], date[1], date[2], date[3],
        date[4], date[5], date[6], date[7]);
    }
    else if (strlen(date) == 10 && date[2] == ' ' && date[5] == ' ')
    { // Format DD MM YYYY
        sprintf(formattedDate, "%c%c/%c%c/%c%c%c%c", date[0], date[1], date[3], date[4],
        date[6], date[7], date[8], date[9]);
    }
    else
    {
        strcpy(formattedDate, date); // Return as is if the format is not valid
    }
}

bool isValidDate(const char date[]) // function to check date
{
    int day, month, year;

    // Check if the date length is valid for DDMMYYYY or DD MM YYYY
    if (strlen(date) == 8)  // Format DDMMYYYY
    {                           
        char dayStr[3] = {date[0], date[1], '\0'};   // Extract DD
        char monthStr[3] = {date[2], date[3], '\0'}; // Extract MM
        char yearStr[5] = {date[4], date[5], date[6], date[7], '\0'}; // Extract YYYY


        day = atoi(dayStr);      // Convert DD to integer
        month = atoi(monthStr);  // Convert MM to integer
        year = atoi(yearStr);  
    }
    else if (strlen(date) == 10 && date[2] == ' ' && date[5] == ' ')
    {                           // Format DD MM YYYY
        day = atoi(date);       // Convert DD
        month = atoi(date + 3); // Convert MM
        year = atoi(date + 6);  // Convert YYYY
    }
    else
    {
        char dayStr[3] = {date[0], date[1], '\0'};   // Extract DD
        char monthStr[3] = {date[3], date[4], '\0'}; // Extract MM
        char yearStr[5] = {date[6], date[7], date[8], date[9], '\0'}; // Extract YYYY

        day = atoi(dayStr);      // Convert DD to integer
        month = atoi(monthStr);  // Convert MM to integer
        year = atoi(yearStr);    // Convert YYYY to integer
    }

    // Validate month
    if (month < 1 || month > 12)
        return false;

    // Validate day
    if (day < 1 || day > 31)
        return false;

    // Check for February
    if (month == 2)
    {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeap && day > 29)
            return false;
        if (!isLeap && day > 28)
            return false;
    }

    // Check for months with 30 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return false;

    return true; // Date is valid
}

bool containsDigit(const char *str) // function to check digits in input
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isdigit(str[i]))
        {
            return true;
        }
    }
    return false;
}

bool isValidPIN(const char *pin)
{
    if (strlen(pin) == 0 || strlen(pin) > 6 || strlen(pin) < 0)
    {
        cout << "Invalid PIN Length"; // underflow condition
        return false;
    }
    for (int i = 0; pin[i] != '\0'; i++)
    {
        if (!isdigit(pin[i]))
        {
            return false;
        }
    }
    return true;
}

bool isAlphabetic(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isalpha(str[i]) && !isspace(str[i]))
        {
            return false;
        }
    }
    return true;
}

void logSearch(const Employee &emp) // to save searched database in log file
{
    if (searchLog.is_open())
    {
        searchLog << "Employee ID: " << emp.id << endl;
        searchLog << "Name: " << emp.jindal_name << endl;
        searchLog << "Father's Name: " << emp.jindal_fathername << endl;
        searchLog << "Mother's Name: " << emp.jindal_mothername << endl;
        searchLog << "Rank/Position: " << emp.jindal_rank << endl;
        searchLog << "Department: " << emp.jindal_deparment << endl;
        char formattedDate[MAX_DATE_LENGTH];
        formatDate(emp.jindal_dateofjoining, formattedDate);
        searchLog << "Date of Joining: " << formattedDate << endl;
        searchLog << "Address: " << emp.jindal_homeaddress << endl;
        searchLog << "--------------------------" << endl;
    }
    else
    {
        cerr << "Unable to open search log file.\n"
             << endl;
    }
}

void logDelete(const Employee &emp) // to save deleted database in log file 
{
    if (deleteLog.is_open())
    {
        deleteLog << "Employee ID: " << emp.id << endl;
        deleteLog << "Name: " << emp.jindal_name << endl;
        deleteLog << "Father's Name: " << emp.jindal_fathername << endl;
        deleteLog << "Mother's Name: " << emp.jindal_mothername << endl;
        deleteLog << "Rank/Position: " << emp.jindal_rank << endl;
        deleteLog << "Department: " << emp.jindal_deparment << endl;
        char formattedDate[MAX_DATE_LENGTH];
        formatDate(emp.jindal_dateofjoining, formattedDate);
        deleteLog << "Date of Joining: " << formattedDate << endl;
        deleteLog << "Address: " << emp.jindal_homeaddress << endl;
        deleteLog << "--------------------------" << endl;
    }
    else
    {
        cerr << "Unable to open delete log file.\n"<< endl;
    }
}

void logSearchByCommon(const Employee &emp)
{
    if (searchByCommonLog.is_open())
    {
        searchByCommonLog << "Employee ID: " << emp.id << endl;
        searchByCommonLog << "Name: " << emp.jindal_name << endl;
        searchByCommonLog << "Father's Name: " << emp.jindal_fathername << endl;
        searchByCommonLog << "Mother's Name: " << emp.jindal_mothername << endl;
        searchByCommonLog << "Rank/Position: " << emp.jindal_rank << endl;
        searchByCommonLog << "Department: " << emp.jindal_deparment << endl;
        char formattedDate[MAX_DATE_LENGTH];
        formatDate(emp.jindal_dateofjoining, formattedDate);
        searchByCommonLog << "Date of Joining: " << formattedDate << endl;
        searchByCommonLog << "Address: " << emp.jindal_homeaddress << endl;
        searchByCommonLog << "City: " << emp.jindal_city << endl;
        searchByCommonLog << "State: " << emp.jindal_state << endl;
        searchByCommonLog << "Country: " << emp.jindal_country << endl;
        searchByCommonLog << "PIN Code: " << emp.jindal_pin << endl;
        searchByCommonLog << "--------------------------" << endl;
    }
    else
    {
        cerr << "Unable to open search by common log file.\n"<< endl;
    }
}

void insertRecord() // to insert database at begining of list
{
    Employee *newEmployee = new Employee();

    // Input Employee ID
    do
{
    cout << "Enter Employee ID: " << endl;
    cin >> newEmployee->id;

    if (cin.fail())
    {
        cout << "Error: ID can have only integers.\n" << endl;
        cin.clear();                                         // Clear the error state
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    }
    else
    {
        stringstream ss;
        ss << setw(6) << setfill('0') << newEmployee->id;

        string id_str = ss.str();
        if (id_str.length() != 6)
        {
            cout << "Error: Employee ID must be exactly 6 digits.\n" << endl;
            cin.clear();                                         // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
        }
        else
        {
            break; // Valid 6-digit ID entered
        }
    }
} while (true);
    cin.ignore(); // Clear the newline character left in the buffer

    // Check for duplicate ID
    Employee *temp = head;
    while (temp != nullptr)
    {
        if (temp->id == newEmployee->id)
        {
            cout << "Error: Employee ID already exists.\n"<< endl;
            delete newEmployee;
            return;
        }
        temp = temp->next;
    }

    // Input Name
    do
    {
        cout << "Enter Employee Name: \n"<<endl;
        get_input(newEmployee->jindal_name, MAX_NAME_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_name);
        if (containsDigit(newEmployee->jindal_name))
        {
            cout << "Error: Name cannot contain digits.\n"<< endl;
        }
    } while (containsDigit(newEmployee->jindal_name));

    // Input Father's Name
    do
    {
        cout << "Enter Employee's Father Name: \n"<<endl;
        get_input(newEmployee->jindal_fathername, MAX_NAME_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_fathername);
        if (containsDigit(newEmployee->jindal_fathername))
        {
            cout << "Error: Father's Name cannot contain digits. \n"<< endl;
        }
    } while (containsDigit(newEmployee->jindal_fathername));

    // Input Mother's Name
    do
    {
        cout << "Enter Employee Mother's Name:\n "<<endl;
        get_input(newEmployee->jindal_mothername, MAX_NAME_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_mothername);
        if (containsDigit(newEmployee->jindal_mothername))
        {
            cout << "Error: Mother's Name cannot contain digits. \n"<< endl;
        }
    } while (containsDigit(newEmployee->jindal_mothername));

    // Input Rank/Position
    do{
    cout << "Enter Rank/Position: \n"<<endl;
    get_input(newEmployee->jindal_rank, MAX_NAME_LENGTH);
    replaceDotsWithSpaces(newEmployee->jindal_rank);

    }while(false);
    // Input Department
 do {
    cout << "Enter Department: " << endl;
    get_input(newEmployee->jindal_deparment, MAX_NAME_LENGTH);
    if (containsDigit(newEmployee->jindal_deparment)) {
        cout << "Error: Department name should not contain digits.\n" << endl;
    } 
} while (containsDigit(newEmployee->jindal_deparment));

    // Input Date of Joining
    do
    {
        cout << "Enter Date of Joining (DDMMYYYY or DD MM YYYY): ";
        get_input(newEmployee->jindal_dateofjoining, MAX_DATE_LENGTH);
        if (!isValidDate(newEmployee->jindal_dateofjoining))
        {
            cout << "Error: Invalid date format. \n"<< endl;
        }
    } while (!isValidDate(newEmployee->jindal_dateofjoining));

    // Input Home Address
    cout << "Enter Home Address: \n"<<endl;
    get_input(newEmployee->jindal_homeaddress, MAX_ADDRESS_LENGTH);

    // Input Street Name
    cout << "Enter Street Name: \n"<<endl;
    get_input(newEmployee->jindal_streetname, MAX_ADDRESS_LENGTH);

    do // Input City
    {
        cout << "Enter City: \n"<<endl;
        get_input(newEmployee->jindal_city, MAX_ADDRESS_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_city);
        if (!isAlphabetic(newEmployee->jindal_city))
        {
            cout << "Error: City should contain only alphabets.\n"<< endl;
        }
    } while (!isAlphabetic(newEmployee->jindal_city));

    do  // Input State
    {
        cout << "Enter State: \n"<<endl;
        get_input(newEmployee->jindal_state, MAX_ADDRESS_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_state);
        if (!isAlphabetic(newEmployee->jindal_state))
        {
            cout << "Error: State should contain only alphabets.\n"<< endl;
        }
    } while (!isAlphabetic(newEmployee->jindal_state));

    do  // Input Country
    {
        cout << "Enter Country: \n"<<endl;
        get_input(newEmployee->jindal_country, MAX_ADDRESS_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_country);
        if (!isAlphabetic(newEmployee->jindal_country))
        {
            cout << "Error: Country should contain only alphabets.\n"<< endl;
        }
    } while (!isAlphabetic(newEmployee->jindal_country));

    
    do  // Input PIN Code
    {
        cout << "Enter PIN Code(6 Digits): \n"<<endl;
        get_input(newEmployee->jindal_pin, MAX_PIN_LENGTH);
        replaceDotsWithSpaces(newEmployee->jindal_pin);
        if (!isValidPIN(newEmployee->jindal_pin))
        {
            cout << "Error: PIN Code should contain only digits.\n"<< endl;
        }
    } while (!isValidPIN(newEmployee->jindal_pin));

    // Insert the new employee at the beginning of the list
    newEmployee->next = head;
    head = newEmployee;
    currentRecordCount++;
    cout << "Record added successfully.\n"<< endl;
}

void deleteRecord()
{
    if (head == nullptr)
    {
        cout << "Database is empty.\n"<< endl;
        return;
    }

    int idToDelete;
    cout << "Enter Employee ID to delete:\n"<< endl;
    cin >> idToDelete;
    cin.ignore(); // Clear the newline character left in the buffer

    Employee *temp = head;
    Employee *prev = nullptr;

    while (temp != nullptr && temp->id != idToDelete)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == nullptr)
    {
        cout << "Record not found.\n"<<endl;
        return;
    }

    if (prev != nullptr)
    {
        prev->next = temp->next;
    }
    else
    {
        head = temp->next;
    }

    logDelete(*temp);
    delete temp;
    currentRecordCount--;
    cout << "Record deleted successfully.\n"<< endl;
}

bool isNumeric(const string &str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

void updateDatabase(Employee* head) {
    int empID;
    cout << "Enter the Employee ID of the record you want to update: \n";
    cin >> empID;
    cin.ignore(); // Clear the newline character left in the buffer

    Employee* emp = head;
    while (emp != nullptr) {
        if (emp->id == empID) {
            // Open the log file in append mode
            ofstream logFile("update_log.txt", ios::app);

            if (!logFile) {
                cerr << "Error opening log file.\n";
                return;
            }

            // Store old details
            char oldDetails[1000];
            snprintf(oldDetails, sizeof(oldDetails),
                     "Employee ID: %d\n"
                     "Name: %s\n"
                     "Father's Name: %s\n"
                     "Mother's Name: %s\n"
                     "Rank/Position: %s\n"
                     "Department: %s\n"
                     "Date of Joining: %s\n"
                     "City: %s\n"
                     "State: %s\n"
                     "Country: %s\n"
                     "Street Name: %s\n"
                     "House Number: %s\n"
                     "PIN Code: %s\n\n",
                     empID, emp->jindal_name, emp->jindal_fathername, emp->jindal_mothername,
                     emp->jindal_rank, emp->jindal_deparment, emp->jindal_dateofjoining,
                     emp->jindal_city, emp->jindal_state, emp->jindal_country,
                     emp->jindal_streetname, emp->jindal_homeaddress, emp->jindal_pin);

            cout << "Enter new details for Employee ID " << empID << ":\n";

            char tempInput[MAX_NAME_LENGTH];

            // Get and validate employee name
            while (true) {
                cout << "Enter new employee name: \n";
                get_input(tempInput, MAX_NAME_LENGTH);

                // Remove periods from the input
                char cleanedInput[MAX_NAME_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; ++i) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string

                // Validate cleaned input
                if (containsDigit(cleanedInput)) {
                    cout << "Error: Name cannot contain digits. Please enter a valid name.\n";
                } else {
                    strcpy(emp->jindal_name, cleanedInput);
                    break;
                }
            }

            // Get and validate father name
            while (true) {
                cout << "Enter new father's name: \n";
                get_input(tempInput, MAX_NAME_LENGTH);

                // Remove periods from the input
                char cleanedInput[MAX_NAME_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; ++i) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string

                // Validate cleaned input
                if (containsDigit(cleanedInput)) {
                    cout << "Error: Father's Name cannot contain digits. Please enter a valid name.\n";
                } else {
                    strcpy(emp->jindal_fathername, cleanedInput);
                    break;
                }
            }

            // Get and validate mother name
            while (true) {
                cout << "Enter new mother's name: \n";
                get_input(tempInput, MAX_NAME_LENGTH);

                // Remove periods from the input
                char cleanedInput[MAX_NAME_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string

                // Validate cleaned input
                if (containsDigit(cleanedInput)) {
                    cout << "Error: Mother's Name cannot contain digits. Please enter a valid name.\n";
                } else {
                    strcpy(emp->jindal_mothername, cleanedInput);
                    break;
                }
            }

       // Get rank
while (true) {
    cout << "Enter new rank/position: \n";
    get_input(tempInput, MAX_NAME_LENGTH);

    // Remove periods from the input
    char cleanedInput[MAX_NAME_LENGTH];
    int j = 0;
    for (int i = 0; tempInput[i] != '\0'; i++) {
        if (tempInput[i] != '.') {
            cleanedInput[j++] = tempInput[i];
        }
    }
    cleanedInput[j] = '\0'; // Null-terminate the cleaned string

    // No digit check, just copy the cleaned input to emp->jindal_rank
    strcpy(emp->jindal_rank, cleanedInput);
    break; // Exit the loop after processing
}
            // Get department
            while (true) {
                cout << "Enter new department: \n";
                get_input(tempInput, MAX_NAME_LENGTH);
                char cleanedInput[MAX_NAME_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string
                if (containsDigit(cleanedInput)) {
                    cout << "Error: Department cannot contain digits. Please enter a valid input.\n";
                } else {
                    strcpy(emp->jindal_deparment, cleanedInput);
                    break;
                }
            }

            // Get and validate date of joining
            while (true) {
                cout << "Enter new date of joining (DDMMYYYY or DD MM YYYY): ";
                get_input(emp->jindal_dateofjoining, MAX_DATE_LENGTH);
                if (!isValidDate(emp->jindal_dateofjoining)) {
                    cout << "Error: Invalid date format. Please enter a valid date.\n";
                } 
                else{
                    break;
                }
            }

            // Get and validate city
            while (true) {
                cout << "Enter new city: \n";
                get_input(tempInput, MAX_ADDRESS_LENGTH);
                char cleanedInput[MAX_ADDRESS_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string
                if (containsDigit(cleanedInput)) {
                    cout << "Error: City cannot contain digits. Please enter a valid input.\n";
                } else {
                    strcpy(emp->jindal_city, cleanedInput);
                    break;
                }
            }

            // Get and validate state
            while (true) {
                cout << "Enter new state: \n";
                get_input(tempInput, MAX_ADDRESS_LENGTH);
                char cleanedInput[MAX_ADDRESS_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string
                if (containsDigit(cleanedInput)) {
                    cout << "Error: State cannot contain digits. Please enter a valid input.\n";
                } else {
                    strcpy(emp->jindal_state, cleanedInput);
                    break;
                }
            }

            // Get and validate country
            while (true) {
                cout << "Enter new country: \n";
                get_input(tempInput, MAX_ADDRESS_LENGTH);
                char cleanedInput[MAX_ADDRESS_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string
                if (containsDigit(cleanedInput)) {
                    cout << "Error: Country cannot contain digits. Please enter a valid input.\n";
                } else {
                    strcpy(emp->jindal_country, cleanedInput);
                    break;
                }
            }

            // Get and validate street name
            while (true) {
                cout << "Enter new street name: \n";
                get_input(tempInput, MAX_ADDRESS_LENGTH);
                char cleanedInput[MAX_ADDRESS_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string
                if (containsDigit(cleanedInput)) {
                    cout << "Error: Street name cannot contain digits. Please enter a valid input.\n";
                } else {
                    strcpy(emp->jindal_streetname, cleanedInput);
                    break;
                }
            }

            // Get and validate house number
            while (true) {
                cout << "Enter new house number: \n";
                get_input(tempInput, MAX_ADDRESS_LENGTH);
                char cleanedInput[MAX_ADDRESS_LENGTH];
                int j = 0;
                for (int i = 0; tempInput[i] != '\0'; i++) {
                    if (tempInput[i] != '.') {
                        cleanedInput[j++] = tempInput[i];
                    }
                }
                cleanedInput[j] = '\0'; // Null-terminate the cleaned string
                    strcpy(emp->jindal_homeaddress, cleanedInput);
                    break;
                
            }

            // Get and validate PIN code
            while (true) {
                cout << "Enter new PIN code: \n";
                get_input(tempInput, MAX_PIN_LENGTH);
                if (!isValidPIN(tempInput)) {
                    cout << "Error: Invalid PIN code. Please enter a valid PIN code.\n";
                } else {
                    strcpy(emp->jindal_pin, tempInput);
                    break;
                }
            }

            // Write old details to log file
            logFile << oldDetails;

            // Close the log file
            logFile.close();

            cout << "Employee record updated successfully.\n";
            return; // Return to main function after updating
        }
        emp = emp->next;
    }

    cout << "Employee ID not found.\n";
}


void searchByCommon()
{
    if (head == nullptr)
    {
        cout << "Database is empty." << endl;
        return;
    }

    int choice;
    cout << "Search by: \n"<< endl;
    cout << "1. ID \n" << endl;
    cout << "2. Name \n" << endl;
    cout << "3. Department \n" << endl;
    cout << "4. City \n" << endl;
    cout << "5. State \n" << endl;
    cout << "6. Country \n" << endl;
    cout << "7. PIN Code \n" << endl;
    cout << "Enter your choice: \n" << endl;
    cin >> choice;
    cin.ignore(); // Clear the newline character left in the buffer

    char searchTerm[MAX_NAME_LENGTH];
    Employee *temp = head;
    bool found = false;

    switch (choice)
    {
    case 1:
    {
        int idToSearch;
        cout << "Enter Employee ID: \n"<< endl;
        cin >> idToSearch;
        cin.ignore(); // Clear the newline character left in the buffer

        while (temp != nullptr)
        {
            if (temp->id == idToSearch)
            {
                logSearchByCommon(*temp);
                cout << "Record found: \n"<<endl;
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: \n" << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
                break;
            }
            temp = temp->next;
        }
        break;
    }
    case 2:
        cout << "Enter Name: \n"<<endl;
        cin.getline(searchTerm, MAX_NAME_LENGTH);
        while (temp != nullptr)
        {
            if (strcmp(temp->jindal_name, searchTerm) == 0)
            {
                logSearchByCommon(*temp);
                cout << "Record found:\n"<<endl;
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: \n" << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
            }
            temp = temp->next;
        }
        break;
    case 3:
        cout << "Enter Department: \n"<<endl;
        get_input(searchTerm, MAX_NAME_LENGTH);

        while (temp != nullptr)
        {
            if (strcmp(temp->jindal_deparment, searchTerm) == 0)
            {
                logSearchByCommon(*temp);
                cout << "Record found:\n";
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: \n" << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
            }
            temp = temp->next;
        }
        break;
    case 4:
        cout << "Enter City: \n"<<endl;
        get_input(searchTerm, MAX_NAME_LENGTH);

        while (temp != nullptr)
        {
            if (strcmp(temp->jindal_city, searchTerm) == 0)
            {
                logSearchByCommon(*temp);
                cout << "Record found:\n";
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: \n" << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
            }
            temp = temp->next;
        }
        break;
    case 5:
        cout << "Enter State: \n"<<endl;
        get_input(searchTerm, MAX_NAME_LENGTH);

        while (temp != nullptr)
        {
            if (strcmp(temp->jindal_state, searchTerm) == 0)
            {
                logSearchByCommon(*temp);
                cout << "Record found:\n";
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: \n" << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
            }
            temp = temp->next;
        }
        break;
    case 6:
        cout << "Enter Country: ";
        get_input(searchTerm, MAX_NAME_LENGTH);

        while (temp != nullptr)
        {
            if (strcmp(temp->jindal_country, searchTerm) == 0)
            {
                logSearchByCommon(*temp);
                cout << "Record found:\n";
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: \n" << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
            }
            temp = temp->next;
        }
        break;
    case 7:
        cout << "Enter PIN Code: ";
        get_input(searchTerm, MAX_PIN_LENGTH);

        while (temp != nullptr)
        {
            if (strcmp(temp->jindal_pin, searchTerm) == 0)
            {
                logSearchByCommon(*temp);
                cout << "Record found:\n";
                cout << "Employee ID: \n" << temp->id << endl;
                cout << "Name: " << temp->jindal_name << endl;
                cout << "Father's Name: \n" << temp->jindal_fathername << endl;
                cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
                cout << "Rank/Position: \n" << temp->jindal_rank << endl;
                cout << "Department: \n" << temp->jindal_deparment << endl;
                char formattedDate[MAX_DATE_LENGTH];
                formatDate(temp->jindal_dateofjoining, formattedDate);
                cout << "Date of Joining: \n" << formattedDate << endl;
                cout << "Address: \n" << temp->jindal_homeaddress << endl;
                cout << "street name: \n" << temp->jindal_streetname <<endl;
                cout << "City: \n" << temp->jindal_city << endl;
                cout << "State: \n" << temp->jindal_state << endl;
                cout << "Country: \n" << temp->jindal_country << endl;
                cout << "PIN Code: \n" << temp->jindal_pin << endl;
                found = true;
            }
            temp = temp->next;
        }
        break;
    default:
        cout << "Invalid choice.\n"<<endl;
        break;
    }

    if (!found)
    {
        cout << "Record not found.\n"<<endl;
    }
}

void generateReport()
{
    ifstream searchLogFile("search_log.txt");
    ifstream deleteLogFile("delete_log.txt");

    if (!searchLogFile.is_open() || !deleteLogFile.is_open())
    {
        cerr << "Error opening log files.\n"<<endl;
        return;
    }

    reportLog << "Search Log:\n"<<endl;
    string line;
    while (getline(searchLogFile, line))
    {
        reportLog << line << endl;
    }
    reportLog << "--------------------------\n";
    reportLog << "Delete Log:\n"<<endl;
    while (getline(deleteLogFile, line))
    {
        reportLog << line << endl;
    }

    searchLogFile.close();
    deleteLogFile.close();
    reportLog.close();
    cout << "Report generated successfully.\n"<<endl;
}

void displayAllDatabase()
{
    if (head == nullptr)
    {
        cout << "Database is empty.\n"<<endl;
        return;
    }

    Employee *temp = head;
    while (temp != nullptr)
    {
        cout << "Employee ID: \n" << temp->id << endl;
        cout << "Name: \n" << temp->jindal_name << endl;
        cout << "Father's Name: \n" << temp->jindal_fathername << endl;
        cout << "Mother's Name: \n" << temp->jindal_mothername << endl;
        cout << "Rank/Position: \n" << temp->jindal_rank << endl;
        cout << "Department: \n" << temp->jindal_deparment << endl;
        char formattedDate[MAX_DATE_LENGTH];
        formatDate(temp->jindal_dateofjoining, formattedDate);
        cout << "Date of Joining: \n" << formattedDate << endl;
        cout << "Address: \n" << temp->jindal_homeaddress << endl;
        cout << "street name: \n" << temp->jindal_streetname <<endl;
        cout << "City: \n" << temp->jindal_city << endl;
        cout << "State: \n" << temp->jindal_state << endl;
        cout << "Country: \n" << temp->jindal_country << endl;
        cout << "PIN Code: \n" << temp->jindal_pin << endl;
        cout << "--------------------------\n" << endl;
        temp = temp->next;
    }
}

void cleanup()
{
    Employee *current = head;
    while (current != nullptr)
    {
        Employee *next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

int main()
{
    int choice;
    while(true)
    {
        cout << "Menu: \n" << endl;
        cout << "1. Insert Record \n" << endl;
        cout << "2. Delete Record \n" << endl;
        cout << "3. Search by Common Criteria \n" << endl;
        cout << "4. Generate Report \n" << endl;
        cout << "5. Display All Database \n" << endl;
        cout << "6. Update Database \n" << endl;
        cout << "7. Exit \n" << endl;
        cout << "Enter your choice: \n" << endl;
        cin >> choice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the newline character left in the buffer

        switch (choice)
        {
        case 1:
            insertRecord();
            break;
        case 2:
            deleteRecord();
            break;
        case 3:
            searchByCommon();
            break;
        case 4:
            generateReport();
            break;
        case 5:
            displayAllDatabase();
            break;
        case 6:
            updateDatabase(head);
            break;
        case 7:
            cleanup();
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
            break;
        }
    } 
    return 0;
}