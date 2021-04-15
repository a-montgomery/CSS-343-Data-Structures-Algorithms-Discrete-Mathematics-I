#include "Account.h"

Account::Account()
{
	acctID = 0;
	fName = "";
	lName = "";
}

Account::Account(int ID, string fName, string lName)
{
	acctID = ID;
	this->fName = fName;
	this->lName = lName;
	//Array of strings that hold the names of the different funds
	string FundNames[] = { "Money Market", "Prime Money Market", "Long-Term Bond", "Short-Term Bond", "500 Index Fund",
						"Capital Value Fund", "Growth Equity Fund", "Growth Index Fund", "Value Fund", "Value Stock Index" };
	//Initizes the funds in each account with the given name
	for (int i = 0; i < 10; i++)
	{
		if (i < 4)
		{
			funds[i] = Fund(FundNames[i], 0, true);
		}
		else
		{
			funds[i] = Fund(FundNames[i], 0, false);
		}
	}
}

int Account::getAcctID() const
{
	return acctID;
}

string Account::getFirstName() const
{
	return fName;
}

string Account::getLastName() const
{
	return lName;
}

void Account::setAcctID(int ID)
{
	acctID = ID;
}

void Account::setFirstName(string fName)
{
	this->fName = fName;
}

void Account::setLastName(string lName)
{
	this->lName = lName;
}

//Function that deposits an amount into a given accounts fund. Also passes a transaction
//reference so that transaction can be updated if it fails
bool Account::Deposit(int amt, int fund, int acctID, Transaction& transaction)
{
	//If the account ID is invalid, prints an error and sets the transaction to failed
	if (acctID < 1000 || acctID > 9999)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << acctID << " is not a valid account number." << endl;
		return false;
	}
	//If the fund ID is invalid, prints an error and sets the transaction to failed
	if (fund < 0 || fund > 9)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << fund << " is not a valid fund." << endl;
		return false;
	}
	//If the amount is invalid, prints an error and sets the transaction to failed
	else if (amt < 0)
	{
		transaction.setValid(false);
		funds[fund].RecordTransaction(transaction);
		cerr << "ERROR: " << "Deposit amount must be greater than 0." << endl;
		return false;
	}
	//Account ID, Fund ID and Amount being deposited are all valid so process the transaction and record
	//the transaction
	else
	{
		funds[fund].Deposit(amt, transaction);
		funds[fund].RecordTransaction(transaction);
		return true;
	}
}

//Function for transfering money between funds in the same account
bool Account::FundTransfer(int amt, int fund, int trnsfFund, int acctID, Transaction& transaction)
{
	//If the account ID is invalid, prints an error and sets the transaction to failed
	if (acctID < 1000 || acctID > 9999)
	{
		transaction.setValid(false);
		cerr <<"ERROR: " << acctID << " is not a valid account number." << endl;
		return false;
	}
	//If the fund ID is invalid, prints an error and sets the transaction to failed
	if (fund < 0 || fund > 9 || trnsfFund < 0 || trnsfFund > 9)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << fund << " is not a valid fund." << endl;
		return false;
	}
	//If the amount is invalid, prints an error and sets the transaction to failed
	if (amt < 0)
	{
		transaction.setValid(false);
		funds[fund].RecordTransaction(transaction);
		cerr << "ERROR: " << "Transfer amount must be greater than 0" << endl;
		return false;
	}
	//If the fund and transfer fund are linked, then process the transaction as normal between funds
	if ((fund == 0 && trnsfFund == 1) || (fund == 1 && trnsfFund == 0) || (fund == 2 && trnsfFund == 3) || (fund == 3 && trnsfFund == 2))
	{
		//Checks to see if there is enough money in the fund to process the transaction
		if (funds[fund].getBalance() < amt)
		{
			//Not enough funds available, sets the transaction to failed, records it in the fund history, and prints an error
			transaction.setValid(false);
			funds[fund].RecordTransaction(transaction);
			cerr << "ERROR: Not enough funds to withdraw " << amt << " from " << getFirstName() << " " << getLastName() << " "
				<< funds[fund].getName() << " Fund." << endl;
			return false;
		}
		else
		{
			//Enough funds available, withdraw the amount from the fund, deposit the amount into the transfer fund, and then
			//record the transaction in both funds history
			funds[fund].Withdraw(amt, transaction);
			funds[trnsfFund].Deposit(amt, transaction);
			funds[fund].RecordTransaction(transaction);
			funds[trnsfFund].RecordTransaction(transaction);
			return true;
		}
	}
	int linkedFund = LinkedFund(fund);
	if (linkedFund != -1)
	{
		//Not enough funds in the linked funds combined, set the transaction to false and record the transaction in the fund history
		if (funds[fund].getBalance() + funds[linkedFund].getBalance() < amt)
		{
			transaction.setValid(false);
			funds[fund].RecordTransaction(transaction);
			cerr << "ERROR: Not enough available funds in " << funds[fund].getName() << " and " << funds[linkedFund].getName() << ". Transaction refused." << endl;
			return false;
		}
		else
		{
			//If there is enough money in the fund, then transfer the money without using funds from the linked fund
			if (funds[fund].getBalance() - amt >= 0)
			{
				funds[fund].Withdraw(amt, transaction);
				funds[trnsfFund].Deposit(amt, transaction);
				funds[fund].RecordTransaction(transaction);
				funds[trnsfFund].RecordTransaction(transaction);
				return true;
			}
			//If there is not enough money in the origin fund, but there is enough money to cover the transaction in the linked fund
			//withdraw all the money from the origin fund, withdraw the remainder from the linked fund, then deposit the money into
			//the transfer fund. Record the transactions in each fund for the amount the was transacted
			else
			{
				int tempAmt = amt;
				int initBalance = funds[fund].getBalance();
				tempAmt -= funds[fund].getBalance();
				funds[fund].Withdraw(funds[fund].getBalance(), transaction);
				funds[linkedFund].Withdraw(tempAmt, transaction);
				funds[trnsfFund].Deposit(amt, transaction);
				funds[trnsfFund].RecordTransaction(transaction);
				transaction.setAmt(initBalance);
				funds[fund].RecordTransaction(transaction);
				transaction.setAmt(tempAmt);
				transaction.setFundID(linkedFund);
				funds[linkedFund].RecordTransaction(transaction);
				return true;
			}
		}
	}
	//Origin fund is not linked
	else
	{
		//If there is enough money in the fund, process the transaction and record the transaction
		if (funds[fund].getBalance() - amt >= 0)
		{
			funds[fund].Withdraw(amt, transaction);
			funds[trnsfFund].Deposit(amt, transaction);
			funds[fund].RecordTransaction(transaction);
			funds[trnsfFund].RecordTransaction(transaction);
			return true;
		}
		//Not enough money in the fund, set the transaction to failed and record the transaction
		else
		{
			transaction.setValid(false);
			funds[fund].RecordTransaction(transaction);
			cerr << "ERROR: Not enough funds to withdraw " << amt << " from " << getFirstName() << " " << getLastName() << " "
				<< funds[fund].getName() << " Fund." << endl;
			return false;
		}
	}
}

//Function for transferring money between funds in seperate accounts
bool Account::AccountTransfer(int amt, int fund, int acctID, int trnsfAcct, int trnsfFund, Account& transferAcct, Transaction& transaction)
{
	//If the account ID is invalid, prints an error and sets the transaction to failed
	if (acctID < 1000 || acctID > 9999)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << acctID << " is not a valid account number." << endl;
		return false;
	}
	//If the fund ID is invalid, prints an error and sets the transaction to failed
	if (fund < 0 || fund > 9 || trnsfFund < 0 || trnsfFund > 9)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << fund << " is not a valid fund." << endl;
		return false;
	}
	//If the amount is invalid, prints an error and sets the transaction to failed
	if (amt < 0)
	{
		transaction.setValid(false);
		cerr << "ERROR: Transfer amount must be greater than 0" << endl;
		return false;
	}
	//If the origin fund is linked (fund 0 or fund 1), then process the transition.
	int linkedFund = LinkedFund(fund);
	if (linkedFund != -1)
	{
		//Not enough money in the linked funds combined, set transaction to failed and print an error
		if (funds[fund].getBalance() + funds[linkedFund].getBalance() < amt)
		{
			transaction.setValid(false);
			cerr << "ERROR: Not enough available funds in " << funds[fund].getName() << " and " << funds[linkedFund].getName() << ". Transaction refused." << endl;
			return false;
		}
		//There is enough money in the linked funds combined
		else
		{
			//There is enough money in the origin fund, use the money in the origin funds and transfer it to the other
			//account's fund and record the transaction in both fund's history
			if (funds[fund].getBalance() - amt >= 0)
			{
				funds[fund].Withdraw(amt, transaction);
				transferAcct.funds[fund].Deposit(amt, transaction);
				funds[fund].RecordTransaction(transaction);
				transferAcct.funds[trnsfFund].RecordTransaction(transaction);
				return true;
			}
			//There is not enough money in the origin fund to process the transfer, then we use money from the linked
			//fund to finish the transaction. Record the transaction in all three funds with the amount that was transacted
			//from each fund
			else
			{
				int tempAmt = amt; //tempory variable to hold the amount needed to be taken out of the linked fund
				int initBalance = funds[fund].getBalance(); //temp variable to hold the original balance of the origin fund
				tempAmt -= funds[fund].getBalance(); //update the temp amount based on how much money was taken from the origin fund
				funds[fund].Withdraw(funds[fund].getBalance(), transaction);
				funds[linkedFund].Withdraw(tempAmt, transaction);
				transferAcct.funds[trnsfFund].Deposit(amt, transaction);
				transferAcct.funds[trnsfFund].RecordTransaction(transaction);
				transaction.setAmt(initBalance);
				funds[fund].RecordTransaction(transaction);
				transaction.setAmt(tempAmt);
				transaction.setFundID(linkedFund);
				funds[linkedFund].RecordTransaction(transaction);
				return true;
			}
		}
	}
	//If the origin fund is not linked, process the transaction only taking into account the
	//money available in the origin fund
	else
	{
		//If there is enough money in the fund, process the transaction and record the transaction in
		//the origin fund history and the transfer fund history
		if (funds[fund].getBalance() - amt >= 0)
		{
			funds[fund].Withdraw(amt, transaction);
			transferAcct.funds[fund].Deposit(amt, transaction);
			funds[fund].RecordTransaction(transaction);
			transferAcct.funds[trnsfFund].RecordTransaction(transaction);
			return true;
		}
		//There is not enough money in the fund, set the transaction to failed and only record it in the
		//origin fund history
		else
		{
			transaction.setValid(false);
			cerr << "ERROR: Not enough funds to withdraw " << amt << " from " << getFirstName() << " " << getLastName() << " "
				<< funds[fund].getName() << " Fund." << endl;
			return false;
		}
	}
}

//Function that prints the entire history of each fund in the account
void Account::PrintAcctHist()
{
	cout << "Transaction History for " << getFirstName() << " " << getLastName() << " " << "by fund." << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << funds[i].getName() << ": $" << funds[i].getBalance() << endl;
		funds[i].PrintFundHistory();
	}
}

//Function that prints the history of a designated fund from an account
void Account::PrintFund(int fundID)
{
	cout << "Transaction History for " << getFirstName() << " " << getLastName() << " " << funds[fundID].getName() << ": $" << 
		funds[fundID].getBalance() << endl;
	funds[fundID].PrintFundHistory();
}

//Function that records an account transaction. This would be if an account is opened or the history
//of an account or fund is viewed
bool Account::RecordAcctTrans(Transaction transaction)
{
	acctHistory.push_back(transaction);
	return true;
}

//Function that prints each transaction for an account
void Account::PrintAcctTrans()
{
	if (acctHistory.size() == 0)
	{
		return;
	}
	else
	{
		for (int i = 0; i < acctHistory.size(); i++)
		{
			cout << " " << acctHistory[i];
		}
		cout << endl;
	}
}

//Op overload that checks the equality of two accounts based on the account number
bool Account::operator==(const Account& rhs)
{
	if (getAcctID() == rhs.getAcctID())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Op overload that checks the inequality of two accounts based on the account number
bool Account::operator!=(const Account& rhs)
{
	if (getAcctID() == rhs.getAcctID())
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Op overload that checks to see if an account is less than another account based on account number
bool Account::operator<(const Account& rhs)
{
	if (getAcctID() < rhs.getAcctID())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Op overload that checks to see if an account is greater than another account based on account number
bool Account::operator>(const Account& rhs)
{
	if (getAcctID() > rhs.getAcctID())
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Function to withdraw an amount from a fund in a given account. Refrence to a transaction is passed
//to update the validity of the transaction
bool Account::Withdraw(int amt, int fund, int acctID, Transaction& transaction)
{
	//If the account ID is invalid, prints an error and sets the transaction to failed
	if (acctID < 1000 || acctID > 9999)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << acctID << " is not a valid account number." << endl;
		return false;
	}
	//If the fund ID is invalid, prints an error and sets the transaction to failed
	if (fund < 0 || fund > 9)
	{
		transaction.setValid(false);
		cerr << "ERROR: " << fund << " is not a valid fund." << endl;
		return false;
	}
	//If the amount is invalid, prints an error and sets the transaction to failed
	if (amt < 0)
	{
		transaction.setValid(false);
		funds[fund].RecordTransaction(transaction);
		cerr << "ERROR: Deposit amount must be greater than 0" << endl;
		return false;
	}
	//Checks to see if the funds are linked
	int linkedFund = LinkedFund(fund);
	if (linkedFund != -1)
	{
		//Checks to see if the linked funds have enought total money to process transaction
		if (funds[fund].getBalance() + funds[linkedFund].getBalance() < amt)
		{
			transaction.setValid(false);
			funds[fund].RecordTransaction(transaction);
			cerr << "ERROR: Not enough funds to withdraw " << amt << " from " << getFirstName() << " " << getLastName() << " "
				<< funds[fund].getName() << " Fund. Transaction refused." << endl;
			return false;
		}
		else
		{
			//Checks to see if there is enough money in the origin fund to process withdrawel
			if (funds[fund].getBalance() - amt > 0)
			{
				funds[fund].Withdraw(amt, transaction);
				funds[fund].RecordTransaction(transaction);
				return true;
			}
			//If there isn't enough money in origin fund, takes the remainder out of the linked fund
			else
			{
				amt -= funds[fund].getBalance();
				transaction.setAmt(funds[fund].getBalance());
				funds[fund].RecordTransaction(transaction);
				funds[fund].Withdraw(funds[fund].getBalance(), transaction);
				funds[linkedFund].Withdraw(amt, transaction);
				transaction.setAmt(amt);
				funds[linkedFund].RecordTransaction(transaction);
				return true;
			}
		}
	}
	//If the funds aren't linked, processes transaction accordingly
	else
	{
		if (funds[fund].getBalance() - amt > 0)
		{
			funds[fund].Withdraw(amt, transaction);
			funds[fund].RecordTransaction(transaction);
			return true;
		}
		else
		{
			transaction.setValid(false);
			funds[fund].RecordTransaction(transaction);
			cerr << "ERROR: Not enough funds to withdraw " << amt << " from " << getFirstName() << " " << getLastName() << " "
				<< funds[fund].getName() << " Fund." << endl;
			return false;
		}
	}
}

//Op overload to print an account
ostream& operator<<(ostream& outStream, const Account& acct)
{
	outStream << acct.getLastName() << " " << acct.getFirstName() << " Account ID: " << acct.getAcctID() << endl;
	for (int i = 0; i < 10; i++)
	{
		outStream << "    " << acct.funds[i].getName() << ": $" << acct.funds[i].getBalance() << endl;
	}

	return outStream;
}

int Account::LinkedFund(int fundID)
{
	if (fundID == 0)
	{
		return 1;
	}
	else if (fundID == 1)
	{
		return 0;
	}
	else if (fundID == 2)
	{
		return 3;
	}
	else if (fundID == 3)
	{
		return 2;
	}
	else
	{
		return -1;
	}
}