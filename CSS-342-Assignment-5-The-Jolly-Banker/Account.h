#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Fund.h"
#include "Transaction.h"
using namespace std;

class Account
{

	friend ostream& operator<<(ostream& outStream, const Account& acct);

public:
	//Constructors
	Account();
	Account(int ID, string fName, string lName);

	//Getters/Setters
	int getAcctID() const;
	string getFirstName() const;
	string getLastName() const;
	void setAcctID(int ID);
	void setFirstName(string fName);
	void setLastName(string lName);

	//Actions
	bool Withdraw(int amt, int fund, int acctID, Transaction& transaction);
	bool Deposit(int amt, int fund, int acctID, Transaction& transaction);
	bool FundTransfer(int amt, int fund, int trnsfFund, int acctID, Transaction& transaction);
	bool AccountTransfer(int amt, int fund, int acctID, int trnsfAcctID, int trnsfFund, Account& transferAcct, Transaction& transaction);
	void PrintAcctHist();
	void PrintFund(int fundID);
	bool RecordAcctTrans(Transaction transaction);
	void PrintAcctTrans();

	//Op overloads
	bool operator==(const Account& rhs);
	bool operator!=(const Account& rhs);
	bool operator<(const Account& rhs);
	bool operator>(const Account& rhs);
	
private:
	int acctID;
	string fName, lName;
	Fund funds[10];
	int LinkedFund(int fundID);
	vector<Transaction> acctHistory;
};

