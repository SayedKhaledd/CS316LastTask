#include <string>
#include "ClientData.h"
using namespace std;

// default ClientData constructor

ClientData::ClientData(int accountNumberValue, int branchIDValue,const string &lastName, const string &firstName, double balanceValue)
: accountNumber(accountNumberValue), balance(balanceValue),branchID(branchIDValue) {
    setLastName(lastName);
    setFirstName(firstName);
} // end ClientData constructor

// get account-number value

int ClientData::getAccountNumber() const {
    return accountNumber;
} // end function getAccountNumber

// set account-number value

void ClientData::setAccountNumber(int accountNumberValue) {
    accountNumber = accountNumberValue; // should validate
} // end function setAccountNumber

int ClientData::getBranchID() const {
    return branchID;
} // end function getAccountNumber

// set account-number value

void ClientData::setBranchID(int branchIDValue) {
    branchID = branchIDValue; // should validate
} // end function setAccountNumber




// get last-name value

string ClientData::getLastName() const {
    return lastName;
} // end function getLastName

// set last-name value

void ClientData::setLastName(const string &lastNameString) {
    // copy at most 15 characters from string to lastName
    // copy at most 15 characters from string to lastName
    int length = lastNameString.size();
    length = (length < 15 ? length : 14);
    lastNameString.copy(lastName, length);
    lastName[ length ] = '\0'; // append null character to lastName
} // end function setLastName

// get first-name value

string ClientData::getFirstName() const {
    return firstName;
} // end function getFirstName

// set first-name value

void ClientData::setFirstName(const string &firstNameString) {
    // copy at most 10 characters from string to firstName
    int length = firstNameString.size();
    length = (length < 10 ? length : 9);
    firstNameString.copy( firstName, length );
    firstName[ length ] = '\0'; // append null character to firstName
} // end function setFirstName
// get balance value

double ClientData::getBalance() const {
    return balance;
} // end function getBalance
// set balance value

void ClientData::setBalance(double balanceValue) {
    balance = balanceValue;
} // end function setBalance
 