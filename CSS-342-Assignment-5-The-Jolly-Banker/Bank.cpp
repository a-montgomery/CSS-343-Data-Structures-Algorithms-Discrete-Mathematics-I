#include "Bank.h"

Bank::Bank()
{
}

//Destructor for Bank
Bank::~Bank()
{
	while (!transList.empty())
	{
		Transaction *trans = &transList.front();
		delete trans;
		trans = nullptr;
		transList.pop();
	}
}

//Function that reads transactions from the files and puts them in the queue
bool Bank::ReadTransactions(string FileName)
{
	//Open the file
	ifstream infile;
	infile.open(FileName);
	string line;	
	//Throw an error if the file is not found
	if (!infile)
	{
		cout << "ERROR: The file could not be opened" << endl;
		return false;
	}
	while (!infile.eof())
	{
		getline(infile, line);
		if (line.empty())
		{
			break;
		}
		istringstream readLine(line);
		char transType;
		readLine >> transType;

		if (transType == 'D' || transType == 'd' || transType == 'W' || transType == 'w')
		{
			int acctID, fundID, amt;
			readLine >> acctID >> amt;
			if (amt <= 0)
			{
				cerr << "ERROR: Deposit/Withdrawel amount must be greater than 0. Transaction refused." << endl;
			}
			else if (acctID > 10000 && acctID < 99999)
			{
				fundID = acctID % 10; //find the remainder of the acctID from the file to get the fund ID. 
				acctID = acctID / 10; //Divide the acctID from the file to give you just the account ID
				Transaction addTrans(transType, acctID, amt, fundID, true);
				transList.push(addTrans);
			}
			else
			{
				fundID = acctID % 10;
				acctID = acctID / 10;
				Transaction addTrans(transType, acctID, amt, fundID, false);
				transList.push(addTrans);
			}
		}
		else if (transType == 'T' || transType == 't')
		{
			int acctID, fundID, amt, trnsfAcctID, trnsfFundID;
			readLine >> acctID >> amt >> trnsfAcctID;
			if (amt <= 0)
			{
				cerr << "ERROR: Transfer amount must be greater than 0. Transaction refused." << endl;
			}
			else if (acctID > 10000 && acctID < 99999 && trnsfAcctID > 10000 && trnsfAcctID < 99999)
			{
				fundID = acctID % 10;
				acctID = acctID / 10;
				trnsfFundID = trnsfAcctID % 10;
				trnsfAcctID = trnsfAcctID / 10;
				if (acctID == trnsfAcctID)
				{
					Transaction newTransaction(transType, acctID, fundID, trnsfFundID, amt, true);
					transList.push(newTransaction);
				}
				else
				{
					Transaction newTransaction(transType, acctID, trnsfAcctID, fundID, trnsfFundID, amt, true);
					transList.push(newTransaction);
				}
			}
			else
			{
				fundID = acctID % 10;
				acctID = acctID / 10;
				trnsfFundID = trnsfAcctID % 10;
				trnsfAcctID = trnsfAcctID / 10;
				if (acctID == trnsfAcctID)
				{
					Transaction newTransaction(transType, acctID, fundID, trnsfFundID, amt, false);
					transList.push(newTransaction);
					cout << transList.front() << endl;
				}
				else
				{
					Transaction newTransaction(transType, acctID, trnsfAcctID, fundID, trnsfFundID, amt, false);
					transList.push(newTransaction);
					cout << transList.front() << endl;
				}
			}
		}
		else if (transType == 'O' || transType == 'o')
		{
			int acctID;
			string fName, lName;
			readLine >> fName >> lName >> acctID;
			if (acctID > 1000 && acctID < 9999)
			{
				Transaction newTransaction(transType, fName, lName, acctID, true);
				transList.push(newTransaction);
			}
			else
			{
				Transaction newTransaction(transType, fName, lName, acctID, false);
				transList.push(newTransaction);
			}
		}
		else if (transType == 'H' || transType == 'h')
		{
			int acctID, fundID;
			readLine >> acctID;
			if (acctID > 10000 && acctID < 99999)
			{
				fundID = acctID % 10;
				acctID = acctID / 10;
				Transaction newTransaction(transType, acctID, fundID, true);
				transList.push(newTransaction);
			}
			else if (acctID > 1000 && acctID < 9999)
			{
				Transaction newTransaction(transType, acctID, true);
				transList.push(newTransaction);
			}
			else
			{
				Transaction newTransaction(transType, acctID, false);
				transList.push(newTransaction);
			}
		}
		else
		{
			cerr << "ERROR: Invalid transaction type" << endl;
		}
	}
	return true;
}

//Function that executes transactions from the queue
void Bank::ExeTransactions()
{
	while (!transList.empty())
	{
		Transaction currentTrans = transList.front();
		Account* acct;
		if (currentTrans.getTransType() == 'O' || currentTrans.getTransType() == 'o')
		{
			if (currentTrans.getValidTrans())
			{
				if (accountList.Retrieve(currentTrans.getAcct(), acct) == true)
				{
					cerr << "ERROR: Account " << currentTrans.getAcct() << " is already open. Transaction refused." << endl;
				}
				else
				{
					Account* acct1 = new Account(currentTrans.getAcct(), currentTrans.getFName(), currentTrans.getLName());
					accountList.Insert(acct1);
				}
			}
		}
		else if (currentTrans.getTransType() == 'D' || currentTrans.getTransType() == 'd')
		{
			if (accountList.Retrieve(currentTrans.getAcct(), acct) == false)
			{
				cerr << "ERROR: Account " << currentTrans.getAcct() << " not found.Deposit refused." << endl;
			}
			else 
			{
				acct->Deposit(currentTrans.getAmt(), currentTrans.getFundID(), currentTrans.getAcct(), currentTrans);
			}
		}
		else if (currentTrans.getTransType() == 'W' || currentTrans.getTransType() == 'w')
		{
			if (accountList.Retrieve(currentTrans.getAcct(), acct) == false)
			{
				cerr << "ERROR: Account " << currentTrans.getAcct() << " not found.Withdrawel refused." << endl;
			}
			else
			{
				acct->Withdraw(currentTrans.getAmt(), currentTrans.getFundID(), currentTrans.getAcct(), currentTrans);
			}
		}
		else if (currentTrans.getTransType() == 'T' || currentTrans.getTransType() == 't')
		{
			Account* trnsfAcct;
			if (accountList.Retrieve(currentTrans.getAcct(), acct) == false)
			{
				cerr << "ERROR: Account " << currentTrans.getAcct() << " not found.Transferal refused." << endl;
			}
			else if (accountList.Retrieve(currentTrans.getTrnsfAcct(), trnsfAcct) == false)
			{
				cerr << "ERROR: Transfer Account " << currentTrans.getAcct() << " not found.Transferal refused." << endl;
			}
			else if (currentTrans.getAcct() == currentTrans.getTrnsfAcct())
			{
				acct->FundTransfer(currentTrans.getAmt(), currentTrans.getFundID(), currentTrans.getTrnsfFundID(), currentTrans.getAcct(), currentTrans);
			}
			else
			{
				Account& refAcct = *trnsfAcct;
				acct->AccountTransfer(currentTrans.getAmt(), currentTrans.getFundID(), currentTrans.getAcct(), currentTrans.getTrnsfAcct(), currentTrans.getTrnsfFundID(), refAcct, currentTrans);
			}
		}
		else if (currentTrans.getTransType() == 'H' || currentTrans.getTransType() == 'h')
		{
			accountList.Retrieve(currentTrans.getAcct(), acct);
			if (currentTrans.getValidTrans() && !currentTrans.hasFundID())
			{
				acct->PrintAcctHist();
			}
			else if (currentTrans.getValidTrans())
			{
				acct->PrintFund(currentTrans.getFundID());
			}
			else
			{
				cerr << "ERROR: Account " << currentTrans.getAcct() << " not found.History cannot be displayed." << endl;
			}
		}
		transList.pop();
	}
}

//Function that displays all of the accounts in the bank
void Bank::Display()
{
	accountList.Display();
}
