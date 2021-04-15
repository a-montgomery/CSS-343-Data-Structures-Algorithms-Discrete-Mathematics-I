#include <iostream>
#include "Account.h"
#include "BST.h"
#include "Bank.h"
using namespace std;


int main(int argc, char* argv[])
{
	string file_name = "";
	if (argc != 2)
	{
		cerr << "Invalid number of command line arguments." << endl;
	}
	else
	{
		file_name = string(argv[1]);
		Bank testBank = Bank();
		testBank.ReadTransactions(file_name);
		testBank.ExeTransactions();
		testBank.Display();
	}
}