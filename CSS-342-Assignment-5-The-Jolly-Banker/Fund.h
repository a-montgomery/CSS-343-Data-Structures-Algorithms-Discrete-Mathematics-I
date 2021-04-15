#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Transaction.h"
using namespace std;

class Fund
{
public:
	//Constructors
	Fund();
	Fund(string name, int amount, bool linked);

	//Getters/Setters
	int getBalance() const;
	string getName() const;
	string getLinked() const;
	void setName(string newName);
	void setBalance(int newBalance);

	//Action functions
	bool Deposit(int amt, Transaction transaction);
	bool Withdraw(int amt, Transaction transaction);
	bool RecordTransaction(Transaction transaction);
	void PrintFundHistory();

private:
	string name;
	int balance = 0;
	vector<Transaction> history;
	bool linked;
};

