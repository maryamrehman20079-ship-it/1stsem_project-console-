#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <regex>
#include <conio.h>
#include <windows.h>
using namespace std;

// GLOBALS
string currentUser;
const int MAX = 50;

string medName[MAX], medType[MAX];
int    qty[MAX];
double price[MAX];
int    doseCount[MAX];
int    doseTime[MAX][3];     
int    doseStatus[MAX][3];   
int    totalMeds = 0;

//FOR GOTOXY
void gotoxy(int x, int y);

//FOR CENTER-ALIGNED PRINTING
void printCentered(string text, int y);

// FUNCTION DECLARATIONS
void signUp(string role);
void login(string role);

//VALIDATION FUNCTIONS
bool isValidPassword(string pass);
bool isValidEmail(string email);
bool isUsernameUnique(string role, string username);
int getInt();
double getDouble();


//FILE OPERATIONS
void loadMedicines();
void saveMedicines();

//ADMIN DASHBOARD
void adminDashboard();

//ADMIN DASHBOARD FEATURES
void addMedicine();
void deleteMedicine();
void updateMedicine();
void viewAllMedicines();
void sortByName();
void viewLowStock();

//USER DASHBOARD
void userdashboard();

//USER DASHBOARD FEATURES
void viewDoseHistory();
void viewTodaySchedule();
void MarkDose();
void checkMedicineStock();
void getRefillRecommendation();
void setColor(WORD color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// MAIN
// MAIN
int main() 
{
    char ch;
    int choice;
    
    // MAIN MENU LOOP
    do 
    {
        system("cls");

        // Title ASCII banner — bright magenta
        setColor(13);
        printCentered("===============================================================================", 1);
        printCentered("                     WELCOME TO                                 ", 2);
        printCentered("===============================================================================", 3);

        // MEDIALERT ASCII Banner (made of stars)
      setColor(11); // cyan
    cout << R"(
     **      **  *******  *****    ****    ****   **       *******  *****    ******
     ***    ***  **       **  **    **    **  **  **       **       **  **     **  
     ** ** ** ** **       **   **   **    **  **  **       **       **  **     **  
     **  **  **  *****    **   **   **    ******  **       *****    *****      **  
     **      **  **       **   **   **    **  **  **       **       ** **      **   
     **      **  *******  *****    ****   **  **  *******  *******  **  **     **  
    )"
     << endl;

        setColor(13);
        printCentered("===============================================================================", 10);
        printCentered("                     MEDIALERT MAIN MENU                        ", 11);
        printCentered("===============================================================================", 12);

        // Menu options — bright yellow
        setColor(14);
        gotoxy(30, 14); cout << "1. Admin Sign Up";
        gotoxy(30, 15); cout << "2. Admin Login";
        gotoxy(30, 16); cout << "3. Patient Sign Up";
        gotoxy(30, 17); cout << "4. Patient Login";
        gotoxy(30, 18); cout << "5. Exit";

        // Separator line
        setColor(7);
        printCentered("============================================================", 20);

        // Input prompt — bright green
        setColor(10);
        gotoxy(40, 22);
        cout << "Enter your choice (1-5): ";

        // Input
        setColor(7);
        cin >> ch;
        cin.ignore(1000, '\n');

        // Validation
        if (ch < '1' || ch > '5') 
        {
            setColor(12); // Red for error
            cout << "\nInvalid choice. Please enter a number between 1 and 5.\n";
            setColor(7);
            system("pause");
            continue;
        }

        choice = ch - '0';

        switch (choice) 
        {
            case 1: signUp("admin");   break;
            case 2: login("admin");    break;
            case 3: signUp("patient"); break;
            case 4: login("patient");  break;
            case 5:
                cout << "\nExiting MediAlert... Goodbye!\n";
                exit(0);
        }

    } while (true);

    return 0;
}
//GOTOXY TO GET CONSOLE COORDINATES
void gotoxy(int x, int y) 
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//FOR CEBTER ALIGN PRINTING OF MENU
void printCentered(string text, int y) 
{
    int consoleWidth = 80;   // Change if your console is wider
    int x = (consoleWidth - text.length()) / 2;
    gotoxy(x, y);
    cout << text;
}

// SIGNUP
void signUp(string role) {
    ofstream file;

    if (role == "admin")
        file.open("admin_data.txt", ios::app);
    else
        file.open("patient_data.txt", ios::app);

    if (!file) {
        cout << "Error opening user data file for " << role << ".\n";
        return;
    }

    string username, password, email;
    system("cls");

    cout << "\n------------- SIGN UP (" << role << ") -------------\n";
    cout << "Enter username: ";
    cin >> username;

    if (!isUsernameUnique(role, username)) 
    {
        cout << "Username already exists. Please try another.\n";
        return;
    }

    cout << "Enter password (max 7 chars, must contain 1 digit and 1 of * # $ @): ";
    cin >> password;

    while (!isValidPassword(password)) 
    {
        cout << "Invalid password! Re-enter: ";
        cin >> password;
    }

    cout << "Enter email: ";
    cin >> email;
    
   while (!isValidEmail(email)) 
   {
    cout << "Invalid email format! Enter again: ";
    cin >> email;
    }

    // (Optional) use isValidEmail(email) if you want to enforce valid email format

    file << "Username:" << username
         << "\nPassword:" << password
         << "\nEmail:" << email
         << "\n----------------------\n";

    file.close();
    cout << "Sign-up successful!\n";
}

// USERNAME UNIQUENESS CHECK
bool isUsernameUnique(string role, string username) 
{
    ifstream file;

    if (role == "admin")
        file.open("admin_data.txt");
    else
        file.open("patient_data.txt");

    if (!file)
        return true;   // If file does not exist, username is unique by default

    string line;
    while (getline(file, line)) 
    {
        if (line.substr(0, 9) == "Username:") 
        {
            if (line.substr(10) == username) 
            {
                
                file.close();
                return false;
            }
        }
    }

    file.close();
    return true;
}

// EMAIL VALIDATION
bool isValidEmail(string email) 
{
    regex pattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return regex_match(email, pattern);
}

// PASSWORD VALIDATION
bool isValidPassword(string pass) 
{
    if (pass.length() > 7)
        return false;

    bool hasDigit   = false;
    bool hasSpecial = false;
    char specials[] = { '*', '#', '$', '@' 
    };

    for (int i = 0; i < (int)pass.length(); i++) 
    {
        if (pass[i] >= '0' && pass[i] <= '9')
            hasDigit = true;

        for (int j = 0; j < 4; j++) {
            if (pass[i] == specials[j])
                hasSpecial = true;
        }
    }
    return (hasDigit && hasSpecial);
}

//INTEGER VALIDATION
int getInt() {
    int x;
    
    while (!(cin >> x)) 
    {
        cout << "Invalid input! Please enter a valid number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    return x;
}

//DOUBLE VALIDATION
double getDouble() {
    double x;
    
    while (!(cin >> x)) 
    {
        cout << "Invalid input! Please enter a valid number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    return x;
}

// LOGIN
void login(string role) {
    ifstream file;

    if (role == "admin")
        file.open("admin_data.txt");
    else
        file.open("patient_data.txt");

    if (!file) {
        cout << "No data file found for " << role << ". Please sign up first.\n";
        return;
    }
    system("cls");

    cout << "\n---------------- LOGIN (" << role << ") ----------------\n";
    string input, pass;
    cout << "Username or Email: ";
    cin >> input;
    cout << "Password: ";
    cin >> pass;

    string user, password, email, line;
    bool ok = false;

    while (getline(file, line)) 
    {
        if (line.substr(0, 9) == "Username:")
            user = line.substr(9);
        else if (line.substr(0, 9) == "Password:")
            password = line.substr(9);
        else if (line.substr(0, 6) == "Email:")
            email = line.substr(7);
        else if (!line.empty() && line[0] == '-') {
            // End of one user's block
            if ((input == user || input == email) && pass == password) {
                ok = true;
                currentUser = user;
                break;
            }
        }
    }

    file.close();

    if (ok) {
        cout << "Login successful. Welcome, " << currentUser << "!\n";
        if (role == "admin")
            adminDashboard();
        else
            userdashboard();
    } else {
        cout << "Wrong credentials.Either user name or password is wrong. Please try again.\n";
    }
}

// ADMIN DASHBOARD
void adminDashboard() {
    int  choice;
    char ch;

    do 
    {
        system("cls");

        // Heading in bright cyan (11)
        setColor(11);
        printCentered("===========================================================", 2);
        printCentered("ADMIN DASHBOARD", 3);
        printCentered("===========================================================", 4);

        // Menu options in bright yellow (14)
        setColor(14);
        gotoxy(25, 6);
        cout << "1. Add New Medicine\n";
        gotoxy(25, 7);
        cout << "2. Delete Medicine\n";
        gotoxy(25, 8);
        cout << "3. Update Medicine Info\n";
        gotoxy(25, 9);
        cout << "4. Display All Medicines\n";
        gotoxy(25, 10);
        cout << "5. Sort Medicines By Name\n";
        gotoxy(25, 11);
        cout << "6. View Low Stock\n";
        gotoxy(25, 12);
        cout << "7. View Dose History\n";
        gotoxy(25, 13);
        cout << "8. Logout\n";

        // Bottom line in default gray (7)
        setColor(7);
        printCentered("===========================================================", 16);

        // Prompt in bright green (10)
        setColor(10);
        gotoxy(40, 18);
        cout << "Enter choice (1-8): ";

        // Back to normal before input
        setColor(7);
        cin >> ch;
        cin.ignore(1000, '\n');

        if (ch < '1' || ch > '8') {
            // Error message in red (12)
            setColor(12);
            cout << "\nInvalid choice. Try again.\n";
            setColor(7);
            system("pause");   // optional, so user can see the message
            continue;
        }

        choice = ch - '0';

        switch (choice) 
        {
            case 1: addMedicine();      
                    break;
            case 2: deleteMedicine();   
                    break;
            case 3: updateMedicine();   break;
            case 4: viewAllMedicines(); break;
            case 5: sortByName();       break;
            case 6: viewLowStock();     break;
            case 7: viewDoseHistory();  break;
            case 8:
                setColor(10); // green message on logout
                cout << "\nLogging out of admin dashboard...\n";
                setColor(7);
                break;
        }

    } while (choice != 8);
}


// ADD MEDICINE
void addMedicine() {
    if (totalMeds >= MAX) 
    {
        cout << "Medicine list is full!\n";
        return;
    }

    cout << "\n---------------- ADD MEDICINE ----------------\n";
    cout << "Enter name: ";
    cin >> medName[totalMeds];
    
    //Medicine Type Input With Validation
    while (true) 
    {
        cout << "Enter type (tablet/syrup): ";
        cin >> medType[totalMeds];

        if (medType[totalMeds] == "Tablet" || medType[totalMeds] == "Syrup" || medType[totalMeds] == "tablet" || medType[totalMeds] == "syrup") 
        {
            break;   
        }
        cout << "!!!!!!!!!! Invalid type! Only 'tablet' or 'syrup' allowed.\n";
    }
    cout << "Enter quantity: ";
    qty[totalMeds] = getInt() ;
    cout << "Enter price: ";
    price[totalMeds] = getDouble();

    while (true)
    {
        cout << "Enter dose count (1-3): ";
        cin >> doseCount[totalMeds];
        if (doseCount[totalMeds]>=1 && doseCount[totalMeds]<=3)
            break;
        cout << "!!!!!!!!!! Invalid dose count (1-3):\n";
    }
    
    for (int i = 0; i < doseCount[totalMeds]; i++) {
        while (true)
        {
            cout << "Enter dose time in 24 hour template" << (i + 1) << ": ";
            cin >> doseTime[totalMeds][i];
            if (doseTime[totalMeds][i]>=0 && doseTime[totalMeds][i])
                break;
            cout << "!!!!!!!!!! Invalid dose time. Should be between (0-24):\n";
        }
        doseStatus[totalMeds][i] = 0;   // pending
    }

    // Fill unused dose slots with 0
    for (int i = doseCount[totalMeds]; i < 3; i++) 
    {
        doseTime[totalMeds][i]   = 0;
        doseStatus[totalMeds][i] = 0;
    }

    totalMeds++;
    saveMedicines();

    cout << "Medicine added successfully.\n";
}

// VIEW ALL MEDICINES
void viewAllMedicines() 
{
    loadMedicines();

    cout << "\n================== ALL MEDICINES ==================\n";
    cout << left << setw(12) << "Name"
         << setw(10) << "Type"
         << setw(8)  << "Qty"
         << setw(8)  << "Price"
         << setw(12) << "DoseCnt"
         << setw(18) << "Times"
         << "\n";
    cout << "---------------------------------------------------\n";

    for (int i = 0; i < totalMeds; i++) 
    {
        cout << left << setw(12) << medName[i]
             << setw(10) << medType[i]
             << setw(8)  << qty[i]
             << setw(8)  << price[i]
             << setw(12) << doseCount[i];

        for (int j = 0; j < doseCount[i]; j++) 
        {
            cout << setw(6) << doseTime[i][j];
        }
        cout << "\n";
    }
    getch();
}

// DELETE MEDICINE
void deleteMedicine() 
{
    loadMedicines();

    string target;
    cout << "\nEnter medicine name to delete: ";
    cin >> target;

    int index = -1;
    for (int i = 0; i < totalMeds; i++) 
    {
        if (medName[i] == target) 
        {
            index = i;
            break;
        }
    }

    if (index == -1) 
    {
        cout << "Medicine not found.\n";
        return;
    }

    for (int i = index; i < totalMeds - 1; i++) 
    {
        medName[i]   = medName[i + 1];
        medType[i]   = medType[i + 1];
        qty[i]       = qty[i + 1];
        price[i]     = price[i + 1];
        doseCount[i] = doseCount[i + 1];

        for (int j = 0; j < 3; j++) {
            doseTime[i][j]   = doseTime[i + 1][j];
            doseStatus[i][j] = doseStatus[i + 1][j];
        }
    }

    totalMeds--;
    saveMedicines();

    cout << "Medicine deleted successfully.\n";
    getch();
}

// UPDATE MEDICINE
void updateMedicine() 
{
    loadMedicines();

    string target;
    cout << "\nEnter medicine name to update: ";
    cin >> target;

    for (int i = 0; i < totalMeds; i++) 
    {
        if (medName[i] == target) 
        {
            cout << "Enter new quantity: ";
            qty[i] =getInt();

            cout << "Enter new price: ";
            price[i] = getDouble();

            while (true)
            {
                cout << "Enter new dose count (1-3): ";
                cin >> doseCount[i];
                if (doseCount[i]>=0 && doseCount[i]<=3)
                    break;
            }
    
            for (int j = 0; j < doseCount[totalMeds]; j++) 
            {
                while (true)
                {
                    cout << "Enter dose time in 24 hour template" << (j + 1) << ": ";
                    cin >> doseTime[i][j];
                    if (doseTime[i][j]>=0 && doseTime[i][j])
                        break;
                }
            }
            for (int j = doseCount[i]; j < 3; j++) 
            {
                doseTime[i][j]   = 0;
                doseStatus[i][j] = 0;
            }

            saveMedicines();
            cout << "Medicine updated successfully.\n";
            return;
        }
    }

    cout << "Medicine not found.\n";
}

// SORT BY NAME
void sortByName() 
{
    loadMedicines();

    for (int i = 0; i < totalMeds - 1; i++) 
    {
        for (int j = i + 1; j < totalMeds; j++) 
        {
            if (medName[i] > medName[j]) 
            {
                // Swap simple fields
                swap(medName[i],   medName[j]);
                swap(medType[i],   medType[j]);
                swap(qty[i],       qty[j]);
                swap(price[i],     price[j]);
                swap(doseCount[i], doseCount[j]);

                // Swap dose arrays row-wise
                for (int k = 0; k < 3; k++) {
                    swap(doseTime[i][k],   doseTime[j][k]);
                    swap(doseStatus[i][k], doseStatus[j][k]);
                }
            }
        }
    }

    cout << "Medicines sorted by name.\n";
    saveMedicines();
    getch();
}

// VIEW LOW STOCK
void viewLowStock() 
{
    loadMedicines();

    cout << "\n========== LOW STOCK MEDICINES (Qty < 2) ==========\n";
    bool found = false;

    for (int i = 0; i < totalMeds; i++) 
    {
        if (qty[i] < 2) {
            cout << "- " << medName[i] << " (" << qty[i] << " left)\n";
            found = true;
        }
    }

    if (!found)
        cout << "All stocks are sufficient.\n";
    getch();
}

// LOAD MEDICINES
void loadMedicines() 
{
    ifstream file("medicine_data.txt");

    if (!file.is_open()) 
    {
        cout << "\n[INFO] medicine_data.txt not found. Starting with empty list.\n";
        ofstream newFile("medicine_data.txt");
        newFile.close();
        totalMeds = 0;
        return;
    }

    totalMeds = 0;

    while (file >> medName[totalMeds]
                >> medType[totalMeds]
                >> qty[totalMeds]
                >> price[totalMeds]
                >> doseCount[totalMeds]
                >> doseTime[totalMeds][0]
                >> doseTime[totalMeds][1]
                >> doseTime[totalMeds][2]
                >> doseStatus[totalMeds][0]
                >> doseStatus[totalMeds][1]
                >> doseStatus[totalMeds][2]) 
    {
        totalMeds++;
        if (totalMeds >= MAX) break;
    }

    file.close();
}

// SAVE MEDICINES
void saveMedicines() 
{
    ofstream file("medicine_data.txt");
    if (!file.is_open()) 
    {
        cout << "[ERROR] Unable to open medicine_data.txt for writing.\n";
        return;
    }

    for (int i = 0; i < totalMeds; i++) 
    {
        file << medName[i]   << " "
             << medType[i]   << " "
             << qty[i]       << " "
             << price[i]     << " "
             << doseCount[i] << " "
             << doseTime[i][0]   << " "
             << doseTime[i][1]   << " "
             << doseTime[i][2]   << " "
             << doseStatus[i][0] << " "
             << doseStatus[i][1] << " "
             << doseStatus[i][2] << "\n";
    }

    file.close();
}

// USER DASHBOARD (PATIENT)
void userdashboard() 
{
    int  choice;
    char ch;

    do 
    {
        system("cls");

        // Heading in bright cyan (11)
        setColor(11);
        printCentered("============================================", 2);
        printCentered(" PATIENT DASHBOARD ", 3);
        printCentered("============================================", 4);

        // Menu options in bright yellow (14)
        setColor(14);
        gotoxy(25, 6);
        cout << "1. View Today's Schedule";
        gotoxy(25, 7);
        cout << "2. Mark Dose (Taken/Missed)";
        gotoxy(25, 8);
        cout << "3. Check Medicine Stock";
        gotoxy(25, 9);
        cout << "4. Get Refill Recommendation";
        gotoxy(25, 10);
        cout << "5. View Dose History";
        gotoxy(25, 11);
        cout << "6. Logout";

        // Bottom line in default gray (7)
        setColor(7);
        printCentered("============================================", 14);

        // Prompt in bright green (10)
        setColor(10);
        gotoxy(40, 16);
        cout << "Enter choice (1-6): ";

        // Back to normal before taking input
        setColor(7);
        cin >> ch;
        cin.ignore(1000, '\n');

        if (ch < '1' || ch > '6') 
        {
            // Error in red (12)
            setColor(12);
            cout << "\nInvalid choice. Try again.\n";
            setColor(7);
            system("pause");   // so patient can see the message
            continue;
        }

        choice = ch - '0';

        switch (choice) 
        {
            case 1: viewTodaySchedule();       break;
            case 2: MarkDose();                break;
            case 3: checkMedicineStock();      break;
            case 4: getRefillRecommendation(); break;
            case 5: viewDoseHistory();         break;
            case 6:
                setColor(10); // green logout message
                cout << "\nLogging out of patient dashboard...\n";
                setColor(7);
                break;
        }

    } while (choice != 6);
}
// VIEW TODAY'S SCHEDULE
void viewTodaySchedule() 
{
    loadMedicines();

    cout << "\n---------------- TODAY'S SCHEDULE ----------------\n";

    if (totalMeds == 0) 
    {
        cout << "No medicines available.\n";
        return;
    }

    for (int i = 0; i < totalMeds; i++) 
    {
        cout << medName[i] << " (" << doseCount[i] << "x/day): ";

        for (int j = 0; j < doseCount[i]; j++) 
        {
            cout << doseTime[i][j] << " ";
        }

        cout << "| Stock: " << qty[i] << "\n";
    }
    getch();
}

// MARK DOSE
void MarkDose() 
{
    loadMedicines();

    if (totalMeds == 0) 
    {
        cout << "No medicines available.\n";
        return;
    }

    string target;
    cout << "\n---------------- MARK DOSE ----------------\n";
    cout << "Enter medicine name: ";
    cin >> target;

    for (int i = 0; i < totalMeds; i++) 
    {
        if (medName[i] == target) {
            for (int j = 0; j < doseCount[i]; j++) 
            {
                cout << "Dose " << j + 1
                     << " at " << doseTime[i][j] <<":00"
                     << " (1 = Taken, 0 = Missed): ";
                int status;
                while (1)
                {
                    cout << "Enter status: ";
                    status = getInt();
                    if (status >=0 && status <=1)
                        break;
                }
                if (status ==1)
                {
                    doseStatus[i][j] = 1;
                    qty[i]--;
                }
                else
                    doseStatus[i][j] = 2;
            }

            saveMedicines();
            cout << "Dose information updated.\n";
            return;
        }
    }

    cout << "Medicine not found.\n";
}

// VIEW DOSE HISTORY
void viewDoseHistory() 
{
    loadMedicines();

    cout << "\n---------------- DOSE HISTORY ----------------\n";

    if (totalMeds == 0) 
    {
        cout << "No medicines available.\n";
        return;
    }

    for (int i = 0; i < totalMeds; i++) 
    {
        cout << medName[i] << ": ";

        for (int j = 0; j < doseCount[i]; j++) 
        {
            if (doseStatus[i][j] == 1)
                cout << doseTime[i][j] << ":00 Taken ";
            else if (doseStatus[i][j] == 2)
                cout << doseTime[i][j] << ":00 Missed ";
            else
                cout << "Pending ";
        }

        cout << "\n";
    }
    getch();
}

// GET REFILL RECOMMENDATION

void getRefillRecommendation() 
{
    loadMedicines();

    cout << "\n---------------- REFILL ALERTS ----------------\n";
    bool low = false;

    for (int i = 0; i < totalMeds; i++) 
    {
        if (qty[i] < 2) {
            low = true;
            cout << "!!! " << medName[i]
                 << " is LOW (Qty = " << qty[i] << ")\n";
        }
    }

    if (!low)
        cout << "All medicines have sufficient stock.\n";
    getch();
}

// CHECK MEDICINE STOCK
void checkMedicineStock() 
{
    loadMedicines();

    if (totalMeds == 0) 
    {
        cout << "\nNo medicines available in the system.\n";
        return;
    }

    cout << "\n----------- MEDICINE STOCK STATUS -----------\n";
    cout << left << setw(15) << "Name"
         << setw(10) << "Type"
         << setw(8)  << "Qty"
         << "Status\n";
    cout << "--------------------------------------------\n";

    for (int i = 0; i < totalMeds; i++) {
        cout << left << setw(15) << medName[i]
             << setw(10) << medType[i]
             << setw(8)  << qty[i];

        if (qty[i] == 0)
            cout << "OUT OF STOCK";
        else if (qty[i] < 2)
            cout << "LOW";
        else
            cout << "OK";

        cout << "\n";
    }
    getch();
}


