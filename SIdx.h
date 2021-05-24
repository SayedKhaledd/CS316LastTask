#ifndef SIDX_H
#define SIDX_H

#include <string>
#include<bits/stdc++.h>
using namespace std;

class SIdx {
public:
    // default ClientData constructor
    SIdx(const string & = "");

    // accessor functions for accountNumber
    void setAccountNumbers(vector<int>);
    vector<int> getAccountNumbers() const;

    void setLastName(const string &);
    string getLastName() const;



private:
   char lastName[ 15 ];
    vector<int> accountNumbers;

};

#endif