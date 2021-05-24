#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include "ClientData.h"
#include "ClientData.cpp"
#include "PIdx.h"
#include "PIdx.cpp"
#include "SIdx.h"
#include "SIdx.cpp"
#include <map>
#include <vector>

using namespace std;

int enterChoice();

void createTextFile(fstream &);

void updateRecord(fstream &);

void newRecord(fstream &);

void deleteRecord(fstream &);

void outputLine(ostream &, const ClientData &);

int getAccount(const char *const);

void createBackUp(fstream &);

void restoreBackUp();

void createIdxsFiles(fstream &);

void createTextIndexFile();

enum Choices {
    PRINT = 1, UPDATE, NEW, DELETE, BACKUP, RESTORE, CREATEIDXS, CREATETXTIDXSFILES, END
};

int main(int argc, char **argv) {
    ifstream templ("credit.dat", ios::in | ios::binary);
    if (!templ) {
        ofstream outCredit("credit.dat", ios::out | ios::binary);
        // exit program if ofstream could not open file
        if (!outCredit) {
            cerr << "File could not be opened." << endl;
            exit(EXIT_FAILURE);
        } // end if
        ClientData blankClient; // constructor zeros out each data member
        // output 100 blank records to file
        for (int i = 0; i < 100; ++i)
            outCredit.write(reinterpret_cast<const char *> (&blankClient), sizeof(ClientData));

        outCredit.close();
    };
    templ.close();
    // open file for reading and writing
    fstream inOutCredit("credit.dat", ios::in | ios::out | ios::binary);

    // exit program if fstream cannot open file
    if (!inOutCredit) {
        cerr << "File could not be opened." << endl;
        exit(EXIT_FAILURE);
    }

    int choice; // store user choice

    while ((choice = enterChoice()) != END) {
        switch (choice) {
            case PRINT: // create text file from record file
                createTextFile(inOutCredit);
                break;
            case UPDATE: // update record
                updateRecord(inOutCredit);
                break;
            case NEW: // create record
                newRecord(inOutCredit);
                break;
            case DELETE: // delete existing record
                deleteRecord(inOutCredit);
                break;
            case BACKUP: // store data into  backup file
                createBackUp(inOutCredit);
                break;
            case RESTORE: // restore data from backup file
                restoreBackUp();
                break;
            case CREATEIDXS: // delete existing record
                createIdxsFiles(inOutCredit);
                break;
            case CREATETXTIDXSFILES: // delete existing record
                createTextIndexFile();
                break;
            default: // display error if user does not select valid choice
                cerr << "Incorrect choice" << endl;
                break;
        } // end switch
        inOutCredit.clear();
    }
}

int enterChoice() {
    // display available options
    cout << "\nEnter your choice" << endl
         << "1 - store a formatted text file of accounts" << endl
         << " called \"print.txt\" for printing" << endl
         << "2 - update an account" << endl
         << "3 - add a new account" << endl
         << "4 - delete an account" << endl
         << "5 - Backup Data" << endl
         << "6 - Restore Data" << endl
         << "7 - Create Index Files" << endl
         << "8 - Create text files for Index Files" << endl
         << "9 - end program\n? ";
    int menuChoice;
    cin >> menuChoice; // input menu selection from user
    return menuChoice;
}

void createTextFile(fstream &readFromFile) {
    // create text file
    ofstream outPrintFile("print.txt", ios::out);

    // exit program if ofstream cannot create file
    if (!outPrintFile) {
        cerr << "File could not be created." << endl;
        exit(EXIT_FAILURE);
    } // end if

    // output column heads
    outPrintFile << left << setw(10) << "Account" << setw(16)
                 << "Last Name" << setw(11) << "First Name" << right
                 << setw(10) << "Balance" << setw(10) << "BranchID" << endl;

    // set file-position pointer to beginning of readFromFile
    readFromFile.seekg(0);

    // read first record from record file
    ClientData client;

    readFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
    // copy all records from record file into text file
    while (!readFromFile.eof()) {
        // write single record to text file
        if (client.getAccountNumber() != 0) // skip empty records
            outputLine(outPrintFile, client);
        // read next record from record file
        readFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
    } // end while
} // end function createTextFile

void updateRecord(fstream &updateFile) {
    // obtain number of account to update
    int accountNumber = getAccount("Enter account to update");

    // move file-position pointer to correct record in file
    updateFile.seekg((accountNumber - 1) * sizeof(ClientData));

    // read first record from file
    ClientData client;
    updateFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));

    // update record
    if (client.getAccountNumber() != 0) {
        outputLine(cout, client); // display the record
        // request user to specify transaction
        cout << "\nEnter charge (+) or payment (-): ";
        double transaction; // charge or payment
        cin >> transaction;
        // update record balance
        double oldBalance = client.getBalance();
        client.setBalance(oldBalance + transaction);
        outputLine(cout, client); // display the record
        // move file-position pointer to correct record in file
        updateFile.seekp((accountNumber - 1) * sizeof(ClientData));
        // write updated record over old record in file
        updateFile.write(reinterpret_cast<const char *> (&client), sizeof(ClientData));
    } else { // display error if account does not exist
        cerr << "Account #" << accountNumber << " has no information." << endl;
    }
}

void newRecord(fstream &insertInFile) {
    // obtain number of account to create
    int accountNumber = getAccount("Enter new account number");

    // move file-position pointer to correct record in file
    insertInFile.seekg((accountNumber - 1) * sizeof(ClientData));
    // read record from file
    ClientData client;
    insertInFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));

    // create record, if record does not previously
    if (client.getAccountNumber() == 0) {
        string lastName;
        string firstName;
        double balance;
        int branchId;
        // user enters last name, first name and balance
        cout << "Enter lastname, firstname, balance, branchID \n? ";
        cin >> setw(15) >> lastName;
        cin >> setw(10) >> firstName;
        cin >> setw(10) >> balance;
        cin >> branchId;
        // use values to populate account values
        client.setLastName(lastName);
        client.setFirstName(firstName);
        client.setBalance(balance);
        client.setBranchID(branchId);
        client.setAccountNumber(accountNumber);
        // move file-position pointer to correct record in file
        insertInFile.seekp((accountNumber - 1) * sizeof(ClientData));
        // insert record in file
        insertInFile.write(reinterpret_cast<const char *> (&client), sizeof(ClientData));
    } else { // display error if account already exists
        cerr << "Account #" << accountNumber << " already contains information." << endl;
    }

}

void deleteRecord(fstream &deleteFromFile) {
    // obtain number of account to delete
    int accountNumber = getAccount("Enter account to delete");

    // move file-position pointer to correct record in file
    deleteFromFile.seekg((accountNumber - 1) * sizeof(ClientData));

    // read record from file
    ClientData client;
    deleteFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));

    // delete record, if record exists in file
    if (client.getAccountNumber() != 0) {
        ClientData blankClient; // create blank record

        // move file-position pointer to correct record in file
        deleteFromFile.seekp((accountNumber - 1) * sizeof(ClientData));
        // replace existing record with blank record
        deleteFromFile.write(reinterpret_cast<const char *> (&blankClient), sizeof(ClientData));
        cout << "Account #" << accountNumber << " deleted.\n";
    } else { // display error if record does not exist
        cerr << "Account #" << accountNumber << " is empty.\n";
    }
}

void outputLine(ostream &output, const ClientData &record) {
    output << left << setw(10) << record.getAccountNumber()
           << setw(16) << record.getLastName()
           << setw(11) << record.getFirstName()
           << setw(10) << setprecision(2) << right << fixed
           << showpoint << record.getBalance()
           << setw(10) << record.getBranchID() << endl;
}

int getAccount(const char *const prompt) {
    int accountNumber;
    // obtain account-number value
    do {
        cout << prompt << " (1 - 100): ";
        cin >> accountNumber;
    } while (accountNumber < 1 || accountNumber > 100);
    return accountNumber;
}


void createBackUp(fstream &readFromFile) {
    ofstream backupFile("backup.dat", ios::out | ios::binary);
    if (!backupFile) {
        cerr << "backupFile could not be opened." << endl;
        exit(EXIT_FAILURE);
    }

    readFromFile.seekg(0);

    ClientData client;
    ClientData blankClient;
    readFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));

    while (!readFromFile.eof()) {
        /*  if (client.getAccountNumber() != 0) {*/
        backupFile.write(reinterpret_cast<const char *> (&client), sizeof(ClientData));
        /*}else {
           backupFile.write(reinterpret_cast<const char *> (&blankClient), sizeof ( ClientData));
       }*/
        readFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
    }

    backupFile.close();
    printf("Backup created successfully  \n");

}

void restoreBackUp() {
    ifstream backupFile("backup.dat", ios::in | ios::binary);
    if (!backupFile) {
        printf(" ERROR: backup.dat file doesn't exist");

    } else {
        ofstream creditFile("credit.dat", ios::out | ios::binary);
        if (!creditFile) {
            cerr << "creditFile could not be opened." << endl;
            exit(EXIT_FAILURE);
        }

        backupFile.seekg(0);
        ClientData client;
        ClientData blankClient;
        backupFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));

        while (!backupFile.eof()) {
            /*  if (client.getAccountNumber() != 0) {*/
            creditFile.write(reinterpret_cast<const char *> (&client), sizeof(ClientData));
            /*} else {*/
            //   creditFile.write(reinterpret_cast<const char *> (&blankClient), sizeof ( ClientData));
            /* }*/

            backupFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
        }

        creditFile.close();
        printf("Data restored successfully \n");
    }


}

void createIdxsFiles(fstream &readFromFile) {
    ofstream primaryIdxFile("primary_index.dat", ios::out | ios::binary);
    if (!primaryIdxFile) {
        cerr << "primaryIdxFile could not be opened." << endl;
        exit(EXIT_FAILURE);
    }

    readFromFile.seekg(0);
    ClientData client;
    ClientData blankClient;
    readFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
    PIdx currRecord;

    // store data in primary_index.dat, each entry as pair: client account number  & byteoffset field of the corresponding record

    map<string, vector<int >> m;

    while (!readFromFile.eof()) {
        int accountNumber = client.getAccountNumber();
        int byteOffset = (accountNumber - 1) * sizeof(ClientData);
        currRecord.setAccountNumber(accountNumber);
        currRecord.setOffset(byteOffset);
        primaryIdxFile.write(reinterpret_cast<const char *> (&currRecord), sizeof(PIdx));

        if (accountNumber != 0) {
            string lastNm = client.getLastName();
            auto itr = m.find(lastNm);
            if (itr != m.end()) {
                //    printf(" founded before %d \n", accountNumber);
                itr->second.push_back(accountNumber);
            } else {
                vector<int> accNums;
                accNums.push_back(accountNumber);
                // printf("not founded before %d \n", accNums.at(0));
                m.insert(pair<string, vector<int >>(lastNm, accNums));
            }
        }
        readFromFile.read(reinterpret_cast<char *> (&client), sizeof(ClientData));
    }

    primaryIdxFile.close();

    ofstream outPrintFile("secondary_index.dat", ios::out | ios::binary);
    if (!outPrintFile) {
        cerr << "secondaryIdxFile could not be opened." << endl;
        exit(EXIT_FAILURE);
    }
    auto it = m.begin();

    while (it != m.end()) {
        string lastN = it->first;
        vector<int> acNums = it->second;

        int *p = reinterpret_cast<int *> (&lastN);
        string *c = reinterpret_cast<string *> (p);

        outPrintFile << setw(15) << left << *c;
        vector<int>::iterator itt;
        int counter = 0;
        for (itt = acNums.begin(); itt < acNums.end(); itt++) {
            outPrintFile << acNums.at(counter++) << ' ';
        }
        outPrintFile << "  \n";
        it++;
    }
    outPrintFile.close();
    printf("Primary and Secondary indexes dat files are created successfully  \n");

}

void createTextIndexFile() {
    ifstream readFromFile("primary_index.dat", ios::in | ios::binary);
    if (!readFromFile) {
        cerr << "File could not be created." << endl;
        exit(EXIT_FAILURE);
    }

    ofstream outPrintFile("primary_index.txt", ios::out);

    if (!outPrintFile) {
        cerr << "File could not be created." << endl;
        exit(EXIT_FAILURE);
    }


    readFromFile.seekg(0);

    PIdx pIdx;
    readFromFile.read(reinterpret_cast<char *> (&pIdx), sizeof(PIdx));
    // copy all records from record file into text file
    while (!readFromFile.eof()) {
        // write single record to text file

        outPrintFile << left << setw(10) << pIdx.getAccountNumber()
                     << setw(16) << pIdx.getOffset() << endl;
        // read next record from record file
        readFromFile.read(reinterpret_cast<char *> (&pIdx), sizeof(PIdx));
    }


    ifstream readFromFileS("secondary_index.dat", ios::in | ios::binary);
    if (!readFromFileS) {
        cerr << "File could not be created." << endl;
        exit(EXIT_FAILURE);
    }

    ofstream outPrintFileS("secondary_index.txt", ios::out);
    if (!outPrintFileS) {
        cerr << "File could not be created." << endl;
        exit(EXIT_FAILURE);
    }

    for (string line; getline(readFromFileS, line);) {
        outPrintFileS << line << endl;
    }
    outPrintFileS.close();
    printf("Primary and Secondary indexes txt files are created successfully  \n");


}

void createTextSecIndexFile() {

} // end function createTextFile
