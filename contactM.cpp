#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Contact {
private:
    string firstName, lastName, address, email;
    long long phoneNumber;

public:
    void createContact() {
        cout << "Enter First Name: ";
        cin >> firstName;

        cout << "Enter Last Name: ";
        cin >> lastName;

        cout << "Enter Phone Number: ";
        cin >> phoneNumber;

        cout << "Enter Address: ";
        cin.ignore();
        getline(cin, address);

        cout << "Enter Email: ";
        cin >> email;
    }

    void showContact() const {
        cout << "\nName: " << firstName << " " << lastName << endl;
        cout << "Phone: " << phoneNumber << endl;
        cout << "Address: " << address << endl;
        cout << "Email: " << email << endl;
    }

    void saveContact() {
        char choice;
        ofstream outFile("contacts.dat", ios::binary | ios::app);

        do {
            createContact();
            outFile.write(reinterpret_cast<char*>(this), sizeof(*this));
            cout << "\nDo you want to add another contact? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');

        outFile.close();
        cout << "\nContact(s) saved successfully!\n";
    }

    void displayContacts() {
        ifstream inFile("contacts.dat", ios::binary);
        if (!inFile) {
            cout << "\nNo contacts found.\n";
            return;
        }

        cout << "\n======= Contact List =======\n";

        while (inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            showContact();
            cout << "\n-----------------------------\n";
        }

        inFile.close();
    }

    void searchContact() {
        ifstream inFile("contacts.dat", ios::binary);
        long long searchPhone;
        bool found = false;

        cout << "Enter Phone Number to Search: ";
        cin >> searchPhone;

        while (inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            if (phoneNumber == searchPhone) {
                cout << "\nContact Found:\n";
                showContact();
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nContact not found.\n";

        inFile.close();
    }

    void deleteContact() {
        long long deletePhone;
        bool found = false;

        cout << "Enter Phone Number to Delete: ";
        cin >> deletePhone;

        ifstream inFile("contacts.dat", ios::binary);
        ofstream tempFile("temp.dat", ios::binary);

        while (inFile.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            if (phoneNumber != deletePhone) {
                tempFile.write(reinterpret_cast<char*>(this), sizeof(*this));
            } else {
                found = true;
            }
        }

        inFile.close();
        tempFile.close();

        remove("contacts.dat");
        rename("temp.dat", "contacts.dat");

        if (found)
            cout << "\nContact deleted successfully.\n";
        else
            cout << "\nContact not found.\n";
    }

    void editContact() {
        long long editPhone;
        bool found = false;

        cout << "Enter Phone Number to Edit: ";
        cin >> editPhone;

        fstream file("contacts.dat", ios::binary | ios::in | ios::out);

        while (file.read(reinterpret_cast<char*>(this), sizeof(*this))) {
            if (phoneNumber == editPhone) {
                cout << "\nCurrent Contact Details:\n";
                showContact();

                cout << "\nEnter New Details:\n";
                createContact();

                int position = static_cast<int>(file.tellg()) - sizeof(*this);
                file.seekp(position);
                file.write(reinterpret_cast<char*>(this), sizeof(*this));

                cout << "\nContact updated successfully.\n";
                found = true;
                break;
            }
        }

        if (!found)
            cout << "\nContact not found.\n";

        file.close();
    }
};

int main() {
    Contact c;
    int option;

    while (true) {
        cout << "\n====== Contact Management System ======\n";
        cout << "1. Add New Contact\n";
        cout << "2. View All Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Edit Contact\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> option;

        switch (option) {
            case 1:
                c.saveContact();
                break;
            case 2:
                c.displayContacts();
                break;
            case 3:
                c.searchContact();
                break;
            case 4:
                c.deleteContact();
                break;
            case 5:
                c.editContact();
                break;
            case 0:
                cout << "\nThank you for using the Contact Management System.\n";
                return 0;
            default:
                cout << "\nInvalid option. Please try again.\n";
        }
    }

    return 0;
}
