#include "Transaction.h"

Transaction::Transaction()
{

}

//Transaction constructor for a withdrawel or deposit
Transaction::Transaction(char type, int acctID, int amt, int fundID, bool valid)
{
	this->type = type;
	this->acctID = acctID;
	this->amt = amt;
	this->fundID = fundID;
	this->valid = valid;
}

//Transaction constructor for an account transfer
Transaction::Transaction(char type, int acctID, int trnsfAcctID, int fundID, int trnsfFundID, int amt, bool valid)
{
	this->type = type;
	this->acctID = acctID;
	this->trnsfAcctID = trnsfAcctID;
	this->amt = amt;
	this->fundID = fundID;
	this->trnsfFundID = trnsfFundID;
	this->valid = valid;
}

//Transaction constructor for a fund transfer
Transaction::Transaction(char type, int acctID, int fundID, int trnsfFundID, int amt, bool valid)
{
	this->type = type;
	this->acctID = acctID;
	this->trnsfAcctID = acctID;
	this->amt = amt;
	this->fundID = fundID;
	this->trnsfFundID = trnsfFundID;
	this->valid = valid;
}

//Transaction constructor for opening a new account
Transaction::Transaction(char type, string fName, string lName, int acctID, bool valid)
{
	this->type = type;
	this->acctID = acctID;
	this->fName = fName;
	this->lName = lName;
	this->valid = valid;
}

//Transaction constructor for checking the history of an account
Transaction::Transaction(char type, int acctID, bool valid)
{
	this->type = type;
	this->acctID = acctID;
	this->valid = valid;
	fundID = -1;
}

//Transaction constructor for checking the history of a fund in an account
Transaction::Transaction(char type, int acctID, int fundID, bool valid)
{
	this->type = type;
	this->acctID = acctID;
	this->fundID = fundID;
	this->valid = valid;
}

//Returns the type of transaction
char Transaction::getTransType() const
{
	return type;
}

//Returns whether the transaction is valid
bool Transaction::getValidTrans() const
{
	return valid;
}

//Returns the account ID
int Transaction::getAcct() const
{
	return acctID;
}

//Returns the transfer account ID
int Transaction::getTrnsfAcct() const
{
	return trnsfAcctID;
}

//Returns the fund ID
int Transaction::getFundID() const
{
	return fundID;
}

//Returns the transfer fund ID
int Transaction::getTrnsfFundID() const
{
	return trnsfFundID;
}

//returns the amount of the transaction
int Transaction::getAmt() const
{
	return amt;
}

//Returns the first name of the transaction (only used for opening accounts)
string Transaction::getFName()
{
	return fName;
}

//Returns the last name of the transaction (only used for opening accounts)
string Transaction::getLName()
{
	return lName;
}

//Sets the transactions validity
void Transaction::setValid(bool valid)
{
	this->valid = valid;
}

//Checks to see if the transaction has a fund ID
bool Transaction::hasFundID()
{
	if (fundID == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Sets the amount of the transaction
void Transaction::setAmt(int amt)
{
	this->amt = amt;
}

//Sets the fund ID of the transaction
void Transaction::setFundID(int fundID)
{
	this->fundID = fundID;
}

ostream& operator<<(ostream& outStream, const Transaction& trans)
{
	if (trans.getValidTrans() == true)
	{
		if (trans.getTransType() == 'D' || trans.getTransType() == 'W')
		{
			outStream << " " << trans.getTransType() << " " << trans.getAcct() << trans.getFundID() << " " << trans.getAmt();
		}
		else if (trans.getTransType() == 'T')
		{
			outStream << " " << trans.getTransType() << " " << trans.getAcct() << trans.getFundID() << " " << trans.getAmt() << " " << trans.getTrnsfAcct() << trans.getTrnsfFundID();
		}
		else
		{
			outStream << " " << trans.getTransType() << " " << trans.getAcct() << trans.getFundID();
		}
	}
	else
	{
		if (trans.getTransType() == 'D' || trans.getTransType() == 'W')
		{
			outStream << " " << trans.getTransType() << " " << trans.getAcct() << trans.getFundID() << " " << trans.getAmt() << " " << "Failed";
		}
		else if (trans.getTransType() == 'T')
		{
			outStream << " " << trans.getTransType() << " " << trans.getAcct() << trans.getFundID() << " " << trans.getAmt() << " " << trans.getTrnsfAcct() << trans.getTrnsfFundID()  << " " << "Failed";
		}
		else
		{
			outStream << " " << trans.getTransType() << " " << trans.getAcct() << trans.getFundID();
		}
	}
	return outStream;
}
