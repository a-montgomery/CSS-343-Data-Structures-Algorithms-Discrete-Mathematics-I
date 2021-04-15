#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Account.h"
using namespace std;

class BST
{
public:
	//constructors
	BST();
	BST(const BST& tree);
	~BST();

	//getters/setters
	int getCount() const;

	//Action functions
	bool isEmpty() const;
	bool Retrieve(int acctID, Account*& accountPtr) const;
	bool Insert(Account* theAccount);
	void EmptyTree();
	void Display();

private:
	struct Node
	{
		Account* account;
		Node* right;
		Node* left;
	};
	int count = 0;
	Node* root = nullptr;
	void Print(Node* printNode);
	Node* Insert(Account* acct, Node* root);
	void Empty(Node* root);
};

