#include <string>
#include "ClientData.h"
#include "SIdx.h"
using namespace std;

// default ClientData constructor
SIdx::SIdx(const string &lastName)
{
    setLastName(lastName);
 }

 

// get account-number value

void SIdx::setLastName(const string &lastNameString) {
    // copy at most 15 characters from string to lastName
    // copy at most 15 characters from string to lastName
    int length = lastNameString.size();
    length = (length < 15 ? length : 14);
    lastNameString.copy(lastName, length);
    lastName[ length ] = '\0'; // append null character to lastName
} 

string SIdx::getLastName() const {
    return lastName;
}

vector<int> SIdx::getAccountNumbers() const {
    return accountNumbers;
} 

void SIdx::setAccountNumbers(vector<int> accsNumsArrPointer) {
    accountNumbers = accsNumsArrPointer;
} 
 


