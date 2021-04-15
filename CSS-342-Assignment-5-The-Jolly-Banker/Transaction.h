#pragma once
#include <string>
#include <iostream>
using namespace std;

class Transaction
{

	friend ostream& operator<<(ostream& outStream, const Transaction& trans);

public:
	//Constructors
	Transaction();
	Transaction(char type, int acctID, int amt, int fundID, bool valid);
	Transaction(char type, int acctID, int trnsfAcctID, int fundID, int trnsfFundID, int amt, bool valid);
	Transaction(char type, int acctID, int fundID, int trnsfFundID, int amt, bool valid);
	Transaction(char type, string fName, string lName, int acctID, bool valid);
	Transaction(char type, int acctID, bool valid);
	Transaction(char type, int acctID, int fundID, bool valid);

	//getters/setters
	char getTransType() const;
	bool getValidTrans() const;
	int getAcct() const;
	int getTrnsfAcct() const;
	int getFundID() const;
	int getTrnsfFundID() const;
	int getAmt() const;
	string getFName();
	string getLName();
	void setValid(bool valid);
	void setAmt(int amt);
	void setFundID(int fundID);

	bool hasFundID();

private:
	char type;
	string fName, lName;
	int fundID, trnsfFundID, acctID, trnsfAcctID, amt;
	bool valid;
};

