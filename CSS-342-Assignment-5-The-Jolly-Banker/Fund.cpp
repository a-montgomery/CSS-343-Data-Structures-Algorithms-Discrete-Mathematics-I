#include "Fund.h"

Fund::Fund()
{
	name = "";
	linked = false;
	balance = 0;
}

Fund::Fund(string name, int amount, bool linked)
{
	this->name = name;
	balance = amount;
	this->linked = linked;
}

int Fund::getBalance() const
{
	return balance;
}

string Fund::getName() const
{
	return name;
}

string Fund::getLinked() const
{
	if (linked == true)
	{
		return "Linked";
	}
	else
	{
		return "Not Linked";
	}
}

void Fund::setName(string newName)
{
	name = newName;
}

void Fund::setBalance(int newBalance)
{
	balance = newBalance;
}

//Function that deposit money into a fund
bool Fund::Deposit(int amt, Transaction transaction)
{
	//Checks to make sure the deposit amount is valid
	if (amt < 0)
	{
		cerr << "Deposit amount must be greater than 0." << endl;
		return false;
	}
	else
	{
		balance += amt;
		return true;
	}
}

//Function that withdraws money from a fund
bool Fund::Withdraw(int amt, Transaction transaction)
{
	//Checks to make sure enough money is in the fund
	if (balance - amt >= 0)
	{
		balance -= amt;
		return true;
	}
	else
	{
		return false;
	}
}

//Function that records a transaction for a fund by adding the transaction
//to the history<Transaction> vector
bool Fund::RecordTransaction(Transaction transaction)
{
	history.push_back(transaction);
	return true;
}

//Function that prints the fund history by iterating through the 
//history<Transaction> vector
void Fund::PrintFundHistory()
{
	if (history.size() == 0)
	{
		cout << "  " << "No transactions." << endl;
		return;
	}
	else
	{
		for (int i = 0; i < history.size(); i++)
		{
			cout << " " << history[i] << endl;
		}
	}
}
