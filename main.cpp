#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

// Structure to store patient information
struct Patient {
    string id, name, surname, city, date, vaccine;
    int age, dose;
};

vector<Patient> patients; // Vector to store patient records
const string DATABASE_FILE = "database.txt";

// Load patient records from file
void loadDatabase() {
    ifstream file(DATABASE_FILE);
    if (!file) return;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Patient p;
        iss >> p.id >> p.name >> p.surname >> p.age >> p.city >> p.date >> p.dose >> p.vaccine;
        patients.push_back(p);
    }
}

// Save patient records to file
void saveDatabase() {
    ofstream file(DATABASE_FILE);
    for (const auto& p : patients) {
        file << p.id << " " << p.name << " " << p.surname << " "
            << p.age << " " << p.city << " " << p.date << " "
            << p.dose << " " << p.vaccine << endl;
    }
}

// Check if the given ID is valid (HHNN format)
bool isValidID(const string& id) {
    return id.size() == 4 && isalpha(id[0]) && isalpha(id[1]) && isdigit(id[2]) && isdigit(id[3]);
}

// Check if the given ID is unique
bool isUniqueID(const string& id) {
    for (const auto& p : patients) {
        if (p.id == id) return false;
    }
    return true;
}

// Find a patient by ID
Patient* findPatient(const string& id) {
    for (auto& p : patients) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

// Register a new patient
void signUp() {
    Patient p;
    do {
        cout << "Create an ID (HHNN format): ";
        cin >> p.id;
    } while (!isValidID(p.id) || !isUniqueID(p.id));

    cout << "Enter your name: "; cin >> p.name;
    cout << "Enter your surname: "; cin >> p.surname;
    do {
        cout << "Enter your age: "; cin >> p.age;
    } while (p.age < 18);

    cout << "Enter your city: "; cin >> p.city;
    p.date = "01.06.2020";
    p.dose = 0;
    p.vaccine = "NULL";
    patients.push_back(p);
    saveDatabase();
    cout << "Registration successful!\n";
}

// Display patient record
void showRecord(const string& id) {
    Patient* p = findPatient(id);
    if (p) {
        cout << "ID: " << p->id << "\nName: " << p->name << "\nSurname: " << p->surname
            << "\nAge: " << p->age << "\nCity: " << p->city << "\nVaccine: "
            << p->vaccine << "\nDate: " << p->date << "\n";
    }
    else {
        cout << "Record not found!\n";
    }
}

// Register a vaccine dose
void registerVaccine(const string& id) {
    Patient* p = findPatient(id);
    if (!p || p->dose >= 2) {
        cout << "You cannot receive more vaccines!\n";
        return;
    }
    string vaccineType;
    do {
        cout << "Choose vaccine type (X or Y): ";
        cin >> vaccineType;
    } while (vaccineType != "X" && vaccineType != "Y");

    if (p->vaccine == "NULL" || p->vaccine == vaccineType) {
        p->vaccine = vaccineType;
        p->dose++;
        cout << "Enter today's date: ";
        cin >> p->date;
        saveDatabase();
        cout << "Vaccine registered successfully!\n";
    }
    else {
        cout << "You cannot switch vaccine types!\n";
    }
}

// Main menu function
void mainMenu() {
    loadDatabase();
    int choice;
    do {
        cout << "1. Sign Up\n2. Login\n3. Exit\nChoose an option: ";
        cin >> choice;
        if (choice == 1) signUp();
        else if (choice == 2) {
            string id;
            cout << "Enter ID: "; cin >> id;
            if (findPatient(id)) {
                int opt;
                cout << "1. Show Record\n2. Register Vaccine\nChoose an option: ";
                cin >> opt;
                if (opt == 1) showRecord(id);
                else if (opt == 2) registerVaccine(id);
            }
            else {
                cout << "ID not found!\n";
            }
        }
    } while (choice != 3);
}

int main() {
    mainMenu();
    return 0;
}
