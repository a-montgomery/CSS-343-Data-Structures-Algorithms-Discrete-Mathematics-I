#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "BST.h"
#include "Transaction.h"
using namespace std;

class Bank
{
public:
	Bank();
	~Bank();

	bool ReadTransactions(string FileName);
	void ExeTransactions();
	void Display();

private:
	BST accountList;
	queue<Transaction> transList;
};

