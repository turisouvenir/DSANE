#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

struct Item
{
    string itemId;
    string itemName;
    string quantity;
    string regDate;
};

bool compareItems(const Item &item1, const Item &item2)
{
    return item1.itemName < item2.itemName;
}

bool isValidDateFormat(const string &dateString)
{
    stringstream ss(dateString);
    int year, month, day;
    char separator1, separator2;
    if (ss >> year >> separator1 >> month >> separator2 >> day &&
        separator1 == '-' && separator2 == '-')
    {
        // Check if the date is valid
        if (year >= 0 && month >= 1 && month <= 12 && day >= 1 && day <= 31)
        {
            return true;
        }
    }
    return false;
}

bool isDateAfterToday(const string &dateString)
{
    stringstream ss(dateString);
    int year, month, day;
    char separator1, separator2;
    ss >> year >> separator1 >> month >> separator2 >> day;

    // Get the current date
    time_t currentTime = time(nullptr);
    tm *localTime = localtime(&currentTime);
    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    // Compare the dates
    if (year > currentYear)
    {
        return true;
    }
    else if (year == currentYear)
    {
        if (month > currentMonth)
        {
            return true;
        }
        else if (month == currentMonth)
        {
            if (day >= currentDay)
            {
                return true;
            }
        }
    }

    return false;
}

void listItems()
{
    ifstream file("database.csv");
    string line;
    vector<Item> items;

    while (getline(file, line))
    {
        istringstream iss(line);
        string itemId, itemName, quantity, regDate;

        getline(iss, itemId, ',');
        getline(iss, itemName, ',');
        getline(iss, quantity, ',');
        getline(iss, regDate, ',');

        Item item{itemId, itemName, quantity, regDate};
        items.push_back(item);
    }

    file.close();

    // Sort the items alphabetically by item name
    sort(items.begin(), items.end(), compareItems);

    // Print the sorted items
    for (const auto &item : items)
    {
        cout << "Item ID: " << item.itemId << "\t\tItem Name: " << item.itemName
             << "\t\tQuantity: " << item.quantity << "\t\tReg Date: " << item.regDate << endl;
    }
}

bool itemExists(const string &itemId, const string &itemName)
{
    ifstream file("database.csv");
    string line;

    while (getline(file, line))
    {
        istringstream iss(line);
        string currItemId, currItemName;
        getline(iss, currItemId, ',');
        getline(iss, currItemName, ',');

        if (currItemId == itemId || currItemName == itemName)
        {
            file.close();
            return true;
        }
    }

    file.close();

    return false;
}


string toPascalCase(const string& itemName)
{
    string pascalCaseName = itemName;
    bool capitalizeNext = true;

    for (char& c : pascalCaseName) {
        if (isalpha(c)) {
            if (capitalizeNext) {
                c = toupper(c);
                capitalizeNext = false;
            } else {
                c = tolower(c);
            }
        } else {
            capitalizeNext = true;
        }
    }

    return pascalCaseName;
}


void addItem(const string &itemId, const string &itemName, const string &quantity, const string &regDate)
{
    // Validate date format
    if (!isValidDateFormat(regDate))
    {
        cout << "Invalid date format! Expected format: YYYY-MM-DD" << endl;
        return;
    }

    // Validate date is after today
    if (!isDateAfterToday(regDate))
    {
        cout << "Invalid registration date! Date should be after today." << endl;
        return;
    }

    try
    {
        int id = stoi(itemId); // Convert itemId to an integer
        // Check if id is a valid positive integer
        if (id <= 0)
        {
            cout << "Id should be a valid positive integer." << endl;
            return;
        }
    }
    catch (const invalid_argument &e)
    {
        cout << "Id should be a valid integer." << endl;
        return;
    }

    try
    {
        int qty = stoi(quantity); // Convert quantity to an integer
        // Check if qty is a valid non-negative integer
        if (qty < 0)
        {
            cout << "Quantity should be a valid non-negative integer." << endl;
            return;
        }
    }
    catch (const invalid_argument &e)
    {
        cout << "Quantity should be a valid integer." << endl;
        return;
    }

    if (itemExists(itemId, itemName))
    {
        cout << "The item with the same id or name already exists." << endl;
        return;
    }

    // Add item to the database
    ofstream file("database.csv", ios_base::app); // Open file in append mode
    if (file.is_open())
    {
    	string pascalCaseName = toPascalCase(itemName);
        file << itemId << "," << pascalCaseName << "," << quantity << "," << regDate << endl;
        file.close();
        cout << "Item added successfully!" << endl;
    }
    else
    {
        cout << "Failed to open the file." << endl;
    }
}

void returnInvalidCommandWarning(const string &command)
{
    cout << "Oops, command '" << command << "' is not supported! Type 'help' for support." << endl;
}

void help()
{
    cout << "Need help? Type 'help' command then press Enter key." << endl;
    string command;
    do
    {
        cout << "\nConsole > ";
        getline(cin, command);
        transform(command.begin(), command.end(), command.begin(), ::tolower);
        // Create a stringstream from the input string
        stringstream ss(command);
        // Vector to store the split tokens
        vector<string> tokens;
        // Temporary string to store each token
        string token;
        // Split the string by space and store the tokens in the vector
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        if (tokens.size() == 1 && tokens.at(0).compare("help") == 0)
        {
            cout << "================================================================" << endl;
            cout << "*\t\t\t"
                 << "Command syntaxes"
                 << "\t\t\t" << endl;
            cout << "================================================================" << endl;
            cout << "itemadd <item_id> <item_name> <quantity> <registration_date>" << endl;
            cout << "itemslist" << endl;
            cout << "help" << endl;
            cout << "Exit" << endl;
        }
        else if (tokens.size() == 1 && tokens.at(0).compare("itemslist") == 0)
        {
            listItems();
        }
        else if (tokens.size() == 5 && tokens.at(0).compare("itemadd") == 0)
        {
            addItem(tokens.at(1), tokens.at(2), tokens.at(3), tokens.at(4));
        }
        else if (command.compare("exit") == 0)
        {
            cout << "BYE!" << endl;
            break;
        }
        else
        {
            returnInvalidCommandWarning(command);
        }
    } while (command.compare("exit") != 0);
}

int main()
{
    cout << "=============================================" << endl;
    cout << "*\t"
         << "Welcome to RCA inventory management System!"
         << "\t*" << endl;
    cout << "* "
         << "*****************************************"
         << " *" << endl;
    cout << "\t\t\t\t" << endl;
    cout << "* "
         << "It is developed by TURINUMUGISHA Souvenir as DSA practical"
         << " *" << endl;
    cout << "* "
         << "Evaluation for the end of Year 3."
         << "\t\t*" << endl;
    cout << "=============================================" << endl;
    help();
    return 0;
}
