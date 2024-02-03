#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;

struct User {
    string id;
    string name;
    string password;
    int enterCount;
    int leaveCount;
    queue<string> complaints;
};

struct Laundry {
    string userId;
    string laundryNumber;
};

struct DayScholar {
    string rollNo;
    string name;
    string department;
    int enterCount;
    int leaveCount;
};

stack<string> userOperations;
queue<string> laundryRequests;
list<User> userRecords;
list<DayScholar> dayScholarRecords;

void saveComplaintsToFile(const string& userId, const string& complaint) {
    ofstream complaintFile("complaint.txt", ios::app);
    if (complaintFile.is_open()) {
        complaintFile << "User ID: " << userId << "\n";
        complaintFile << "Complaint: " << complaint << "\n\n";
        complaintFile.close();
    } else {
        cout << "Unable to open complaint file." << endl;
    }
}

void quickSort(vector<User>& arr, int low, int high) {
    if (low < high) {
        int pivot = high;
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j].id < arr[pivot].id) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[pivot]);
        pivot = i + 1;

        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

void sortUserRecordsByID() {
    vector<User> temp(userRecords.begin(), userRecords.end());
    quickSort(temp, 0, temp.size() - 1);
    userRecords.clear();
    for (const auto& user : temp) {
        userRecords.push_back(user);
    }
}

void registerUserComplaint(const string& userId) {
    cout << "Please enter your complaint: ";
    string complaint;
    cin.ignore();
    getline(cin, complaint);

    for (auto& user : userRecords) {
        if (user.id == userId) {
            user.complaints.push(complaint);
            saveComplaintsToFile(userId, complaint);
            cout << "Thank you for your complaint. It has been registered." << endl;
            break;
        }
    }
}
void registerUser() {
    string adminUsername, adminPassword;
    cout << "Enter admin username: ";
    cin >> adminUsername;
    cout << "Enter admin password: ";
    cin >> adminPassword;

    if (adminUsername == "menal" && adminPassword == "1234") {
        string name;
        cout << "Enter user name: ";
        cin >> name;

        string id = "U" + to_string(rand() % 1000);
        string password = to_string(rand() % 10000);

        User newUser;
        newUser.id = id;
        newUser.name = name;
        newUser.password = password;
        newUser.enterCount = 0;
        newUser.leaveCount = 0;

        ofstream file("users.csv", ios::app);
        file << newUser.id << "," << newUser.name << "," << newUser.password << "," << newUser.enterCount << "," << newUser.leaveCount << endl;
        file.close();

        cout << "User registered successfully!" << endl;
        cout << "User ID: " << newUser.id << endl;
        cout << "Password: " << newUser.password << endl;

        userRecords.push_back(newUser);
        sortUserRecordsByID();
    } else {
        cout << "Invalid admin login details!" << endl;
    }
}

void doLaundry(const string& userId, vector<string>& lines, list<User>& users) {
    string laundryType;
    int clothesCount;
    string dateOfEntry;

    cout << "What clothes do you want to give for laundry? (T-shirts/Trousers/Jeans/Shoes): ";
    cin >> laundryType;
    cout << "Enter the total number of " << laundryType << ": ";
    cin >> clothesCount;
    cout << "Enter the date of entry (DD/MM/YYYY): ";
    cin >> dateOfEntry;

    int laundryNumber = rand() % 1000;

    int daysAfter;
    cout << "Enter the number of days after which you want to collect your clothes: ";
    cin >> daysAfter;

    int day, month, year;
    sscanf(dateOfEntry.c_str(), "%d/%d/%d", &day, &month, &year);

    tm timeStruct = {0};
    timeStruct.tm_mday = day + daysAfter;
    timeStruct.tm_mon = month - 1;
    timeStruct.tm_year = year - 1900;

    time_t pickupTime = mktime(&timeStruct);
    char pickupDate[11];
    strftime(pickupDate, sizeof(pickupDate), "%d/%m/%Y", localtime(&pickupTime));

    lines.back() = userId + "," + lines.back() + "," + to_string(laundryNumber) + "," + laundryType + "," + to_string(clothesCount) + "," + dateOfEntry + "," + pickupDate;

    cout << "Laundry registered successfully!" << endl;
    cout << "Laundry Number: " << laundryNumber << endl;
    cout << "Please collect your clothes on: " << pickupDate << endl;

    cout << "Do you have any laundry-related complaints? (Y/N): ";
    char complaintChoice;
    cin >> complaintChoice;

    if (toupper(complaintChoice) == 'Y') {
        string complaint;
        cout << "Please enter your complaint: ";
        cin.ignore();
        getline(cin, complaint);

        for (auto& user : users) {
            if (user.id == userId) {
                user.complaints.push(complaint);
                saveComplaintsToFile(userId, complaint);
                break;
            }
        }

        cout << "Thank you for your complaint. It has been registered." << endl;
    }

    ofstream outFile("users.csv");
    for (const string& updatedLine : lines) {
        outFile << updatedLine << endl;
    }
    outFile.close();

    laundryRequests.push(userId + "," + to_string(laundryNumber));
}

void userLogin() {
    string id, password;
    cout << "Enter user ID: ";
    cin >> id;
    cout << "Enter password: ";
    cin >> password;

    ifstream file("users.csv");
    string line;
    vector<string> lines;

    bool userFound = false;
    while (getline(file, line)) {
        lines.push_back(line);

        string userId, username, userPassword, enterCountStr, leaveCountStr;
        size_t pos = line.find(",");
        userId = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        username = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        userPassword = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        enterCountStr = line.substr(0, pos);
        line.erase(0, pos + 1);
        leaveCountStr = line;

        if (userId == id) {
            userFound = true;

            if (password == userPassword) {
                cout << "User logged in successfully!" << endl;

                cout << "Choose your action: \n"
                     << "E - Enter the hostel\n"
                     << "L - Leave the hostel\n"
                     << "D - Do laundry\n"
                     << "C - Register a complaint\n"
                     << "Enter your choice: ";

                char choice;
                cin >> choice;

                if (toupper(choice) == 'E') {
                    int enterCount = stoi(enterCountStr);
                    enterCount++;
                    lines.back() = userId + "," + username + "," + userPassword + "," + to_string(enterCount) + "," + leaveCountStr;
                    cout << "You have entered the hostel." << endl;
                    userOperations.push("User " + userId + " entered the hostel");
                    registerUserComplaint(userId);
                } else if (toupper(choice) == 'L') {
                    int leaveCount = stoi(leaveCountStr);
                    leaveCount++;
                    lines.back() = userId + "," + username + "," + userPassword + "," + enterCountStr + "," + to_string(leaveCount);
                    cout << "You have left the hostel." << endl;
                    userOperations.push("User " + userId + " left the hostel");
                    registerUserComplaint(userId);
                } else if (toupper(choice) == 'D') {
                    doLaundry(userId, lines, userRecords);
                } else if (toupper(choice) == 'C') {
                    registerUserComplaint(userId);
                } else {
                    cout << "Invalid choice! No action taken." << endl;
                }

                ofstream outFile("users.csv");
                for (const string& updatedLine : lines) {
                    outFile << updatedLine << endl;
                }
                outFile.close();
            } else {
                cout << "Invalid password!" << endl;
            }
        }
    }

    if (!userFound) {
        cout << "User ID not found!" << endl;
    }

    if (!userFound) {
        cout << "User ID not found!" << endl;
    }
}

void displayMenu() {
    cout << "Hostel Entry and Exit Management System" << endl;
    cout << "1. Register as a user" << endl;
    cout << "2. Log in as a user" << endl;
    cout << "3. Register as a day scholar" << endl;
    cout << "4. Log in as a day scholar" << endl;
    cout << "5. Admin Login" << endl;
    cout << "0. Exit" << endl;
    cout << "Enter your choice: ";
}

void quickSortDayScholars(vector<DayScholar>& arr, int low, int high) {
    if (low < high) {
        int pivot = high;
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j].rollNo < arr[pivot].rollNo) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[pivot]);
        pivot = i + 1;

        quickSortDayScholars(arr, low, pivot - 1);
        quickSortDayScholars(arr, pivot + 1, high);
    }
}

void sortDayScholarRecordsByRollNo() {
    vector<DayScholar> temp(dayScholarRecords.begin(), dayScholarRecords.end());
    quickSortDayScholars(temp, 0, temp.size() - 1);
    dayScholarRecords.clear();
    for (const auto& scholar : temp) {
        dayScholarRecords.push_back(scholar);
    }
}

void registerDayScholar() {
    string rollNo, name, department;

    cout << "Enter day scholar's roll number: ";
    cin >> rollNo;

    for (const auto& scholar : dayScholarRecords) {
        if (scholar.rollNo == rollNo) {
            cout << "Day scholar with this roll number already registered!" << endl;
            return;
        }
    }

    cout << "Enter day scholar's name: ";
    cin >> name;
    cout << "Enter day scholar's department: ";
    cin >> department;

    DayScholar newDayScholar;
    newDayScholar.rollNo = rollNo;
    newDayScholar.name = name;
    newDayScholar.department = department;
    newDayScholar.enterCount = 0;
    newDayScholar.leaveCount = 0;

    ofstream file("day_scholars.csv", ios::app);
    file << newDayScholar.rollNo << "," << newDayScholar.name << "," << newDayScholar.department << "," << newDayScholar.enterCount << "," << newDayScholar.leaveCount << endl;
    file.close();

    cout << "Day scholar registered successfully!" << endl;

    dayScholarRecords.push_back(newDayScholar);
    sortDayScholarRecordsByRollNo();
}

void dayScholarLogin() {
    string rollNo;
    cout << "Enter day scholar's roll number: ";
    cin >> rollNo;

    ifstream file("day_scholars.csv");
    string line;
    vector<string> lines;

    bool scholarFound = false;
    while (getline(file, line)) {
        lines.push_back(line);

        string scholarRollNo, scholarName, scholarDepartment, enterCountStr, leaveCountStr;
        size_t pos = line.find(",");
        scholarRollNo = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        scholarName = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        scholarDepartment = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(",");
        enterCountStr = line.substr(0, pos);
        line.erase(0, pos + 1);
        leaveCountStr = line;

        if (scholarRollNo == rollNo) {
            scholarFound = true;

            cout << "Day scholar found!" << endl;

            cout << "Choose your action: \n"
                 << "E - Enter the Hostel\n"
                 << "L - Leave the Hostel\n"
                 << "Enter your choice: ";

        char choice;
        cin >> choice;

            if (toupper(choice) == 'E') {
                int enterCount = stoi(enterCountStr);
                enterCount++;
                lines.back() = scholarRollNo + "," + scholarName + "," + scholarDepartment + "," + to_string(enterCount) + "," + leaveCountStr;
                cout << "You have entered the campus." << endl;
            } else if (toupper(choice) == 'L') {
                int leaveCount = stoi(leaveCountStr);
                leaveCount++;
                lines.back() = scholarRollNo + "," + scholarName + "," + scholarDepartment + "," + enterCountStr + "," + to_string(leaveCount);
                cout << "You have left the campus." << endl;
            } else {
                cout << "Invalid choice! No action taken." << endl;
            }

            ofstream outFile("day_scholars.csv");
            for (const string& updatedLine : lines) {
                outFile << updatedLine << endl;
            }
            outFile.close();
        }
    }

    if (!scholarFound) {
        cout << "Day scholar with roll number not found!" << endl;
    }
}

void adminActions() {
    string adminUsername, adminPassword;
    cout << "Enter admin username: ";
    cin >> adminUsername;
    cout << "Enter admin password: ";
    cin >> adminPassword;

    if (adminUsername == "menal" && adminPassword == "1234") {
        cout << "Admin logged in successfully!" << endl;

        int adminChoice;
        do {
            cout << "\nAdmin Actions:\n";
            cout << "2. View Laundry Requests\n";
            cout << "3. View User Complaints\n";
            cout << "4. View Day Scholars' Status\n";
            cout << "5. View Hostel Residents' Status\n";
            cout << "6. Exit Admin Menu\n";
            cout << "Enter your choice: ";
            cin >> adminChoice;

            switch (adminChoice) {
                
                case 2:
                    // View Laundry Requests
                    while (!laundryRequests.empty()) {
                        cout << laundryRequests.front() << endl;
                        laundryRequests.pop();
                    }
                    break;
                case 3:
    // View User Complaints
                   cout << "\nUser Complaints:\n";
                    for (auto& user : userRecords) {
                        cout << "User ID: " << user.id << "\n";
                        queue<string>& complaints = user.complaints;  // Non-const reference
                    while (!complaints.empty()) {
                     cout << "Complaint: " << complaints.front() << "\n";
                     complaints.pop();
        }
                     cout << "\n";
    }
                     break;

                case 4:
                    // View Day Scholars' Status
                    cout << "\nDay Scholars' Status:\n";
                    for (const auto& scholar : dayScholarRecords) {
                        cout << "Roll No: " << scholar.rollNo << ", Name: " << scholar.name << "\n";
                        cout << "Enter Count: " << scholar.enterCount << ", Leave Count: " << scholar.leaveCount << "\n";
                    }
                    break;
                case 5:
                    // View Hostel Residents' Status
                    cout << "\nHostel Residents' Status:\n";
                    for (const auto& user : userRecords) {
                        cout << "User ID: " << user.id << ", Name: " << user.name << "\n";
                        cout << "Enter Count: " << user.enterCount << ", Leave Count: " << user.leaveCount << "\n";
                    }
                    break;
                case 6:
                    cout << "Exiting Admin Menu\n";
                    break;
                default:
                    cout << "Invalid choice! Please try again.\n";
                    break;
            }

        } while (adminChoice != 6);
    } else {
        cout << "Invalid admin login details!" << endl;
    }
}


int main() {
    srand(time(0));

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                userLogin();
                break;
            case 3:
                registerDayScholar();
                break;
            case 4:
                dayScholarLogin();
                break;
            case 5:
                adminActions();
                break;
            case 0:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }

        cout << endl;
    } while (choice != 0);

    return 0;
}