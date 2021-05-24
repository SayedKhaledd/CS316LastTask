#ifndef PIDX_H
#define PIDX_H

#include <string>

class PIdx {
public:
    // default ClientData constructor
    PIdx(int = 0,int = 0);

    // accessor functions for accountNumber
    void setAccountNumber(int);
    int getAccountNumber() const;
    
    void setOffset(int);
    int getOffset() const;

    
private:
    int accountNumber;
    int offset;
   
}; // end class ClientData

#endif