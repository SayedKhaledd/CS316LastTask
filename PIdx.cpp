#include <string>
#include "ClientData.h"
#include "PIdx.h"
using namespace std;

// default ClientData constructor

PIdx::PIdx(int accountNumberValue, int offsetValue)
: accountNumber(accountNumberValue), offset(offsetValue) {

} // end ClientData constructor

// get account-number value

int PIdx::getAccountNumber() const {
    return accountNumber;
} // end function getAccountNumber

// set account-number value

void PIdx::setAccountNumber(int accountNumberValue) {
    accountNumber = accountNumberValue; // should validate
} // end function setAccountNumber

int PIdx::getOffset() const {
    return offset;
} // end function getAccountNumber

// set account-number value

void PIdx::setOffset(int offsetValue) {
    offset = offsetValue; // should validate
} // end function setAccountNumber



 