#include "BST.h"

BST::BST()
{
	root = nullptr;
}

BST::BST(const BST& tree)
{
	this->root = nullptr;
	*this = tree;
}

BST::~BST()
{
	EmptyTree();
}

//Function that returns the number of accounts in the BST
int BST::getCount() const
{
	return count;
}

//Function that returns whether the tree is empty or not
bool BST::isEmpty() const
{
	if (root == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool BST::Retrieve(int acctID, Account*& accountPtr) const
{
	bool found = false;
	Node* pNode = root;
	//Returns false if the root is nullptr. The tree is empty
	if (pNode == nullptr)
	{
		return found;
	}
	//Traverse the list until the node is nullptr
	while (pNode != nullptr)
	{
		//if at any point the node account is equal to the account being searched for
		//set the accountPtr to the account and return true
		if (pNode->account->getAcctID() == acctID)
		{
			accountPtr = pNode->account;
			found = true;
			return found;
		}
		//if the account number is greater than that account ID being searched for
		//go to the left node
		else if (pNode->account->getAcctID() > acctID)
		{
			pNode = pNode->left;
		}
		//if the account number is less than the account ID being searched for
		//go to the right node
		else
		{
			pNode = pNode->right;
		}
	}
	return found;
}

//function that inserts an account into the BST in the correct location using recursion
bool BST::Insert(Account* theAccount)
{
	//If the account ID number is invalid, return an error
	if (theAccount->getAcctID() < 1000 || theAccount->getAcctID() > 9999)
	{
		cerr << "Account ID Numbers must be greater than 1000 and less than 9999." << endl;
		return false;
	}
	//If the tree is empty, insert the account at the top of the tree
	if (root == nullptr)
	{
		Node* insNode = new Node();
		insNode->account = theAccount;
		insNode->left = nullptr;
		insNode->right = nullptr;
		root = insNode;
		count++;
		return true;
	}
	//If the tree is not empty, recursively search for the location to be inserted
	else
	{
		Insert(theAccount, root);
	}
	return false;
}

//Function that checks to see if the tree is empty
void BST::EmptyTree()
{
	if (isEmpty())
	{
		return;
	}
	else
	{
		Empty(root);
	}
	
}

//Function that prints all of the accounts in the BST
void BST::Display()
{
	cout << "Processing Done. Final Balances:" << endl;
	if (root == nullptr)
	{
		cout << "There are no accounts currently." << endl;
	}
	else
	{
		Print(root);
	}
}

//Recursively prints tree in order
void BST::Print(Node* node)
{
	if (node != nullptr)
	{
		Print(node->left);
		cout << *(node->account) << endl;
		Print(node->right);
	}
}

//Recursive helper function for insertion
BST::Node* BST::Insert(Account* acct, Node* root)
{
	//if the node is null, insert the account here
	if (root == nullptr)
	{
		Node* insNode = new Node();
		insNode->account = acct;
		insNode->left = insNode->right = nullptr;
		root = insNode;
		count++;
		return insNode;
	}
	//if the account ID being inserted is less than the current node account ID
	//move to the left node, otherwise go to the right node
	else
	{
		if (acct->getAcctID() < root->account->getAcctID())
		{
			root->left = Insert(acct, root->left);
		}
		else
		{
			root->right = Insert(acct, root->right);
		}
	}

	return root;
}

//Function for deleting all the nodes in the tree
void BST::Empty(Node* root)
{
	if (root != nullptr)
	{
		Empty(root->left);
		Empty(root->right);
		delete root->account;
		delete root;
	}
}


