# include"Directory.h"
#include<string>
#include<cstring>
#include <stdexcept>
using namespace std;

// normal constructor 
Directory::Directory()
{
	root = new TreeNode;
	root->name = "/";
	root->parent = root;
	root->isFile = false;
	root->prevSib = nullptr;
	root->nextSib = nullptr;
	root->firstChild = nullptr;
	root->lastChild = nullptr;
	cwd = root;
}

// destrucor for the class
Directory::~Directory()
{
	// clear everything hanging of the root
	clear(root);
}

// clear evertyhing hanging of the parameter to this node
void Directory::clear(TreeNode* toRemove)
{
	if (toRemove == nullptr)
		return;
	// call the recursive function which will actually delete nodes
	clearHelper(toRemove->firstChild);
	return;
}

// recursive traversal function that will delete the nodes 
void Directory::clearHelper(TreeNode* toRemove)
{
	if (toRemove == nullptr)
		return;
	clearHelper(toRemove->firstChild);
	clearHelper(toRemove->nextSib);
	delete toRemove;
	toRemove = nullptr;
	return;
}

// creates the nodes
TreeNode* Directory::createNode(string Name, bool IsFile)
{
	TreeNode* newNode = new TreeNode;
	newNode->name = Name;
	newNode->isFile = IsFile;
	newNode->firstChild = nullptr;
	newNode->lastChild = nullptr;
	newNode->parent = nullptr;
	newNode->prevSib = nullptr;
	newNode->nextSib = nullptr;
	return newNode;
}

// in the tester call this to make a new Directory
void Directory::mkdir(string name, ostream& out)
{
	TreeNode* newNode = createNode(name, false);
	insert(newNode, out);
}

// in the tester call this to make a new file
void Directory::mkfile(string name, ostream& out)
{
	TreeNode* newNode = createNode(name, true);
	insert(newNode, out);
}

// insert the node in the current working directory - cwd
void Directory::insert(TreeNode *toInsert, ostream& out)
{
	toInsert->parent = cwd;
	if (cwd->firstChild == nullptr && cwd->lastChild == nullptr)
	{
		cwd->firstChild = toInsert;
		cwd->lastChild = toInsert;
	}
	else if (toInsert->name < cwd->firstChild->name)
	{
		TreeNode* temp = cwd->firstChild;
		cwd->firstChild = toInsert;
		toInsert->nextSib = temp;
		temp->prevSib = toInsert;
	}
	else if (toInsert->name > cwd->lastChild->name)
	{
		TreeNode* temp = cwd->lastChild;
		cwd->lastChild = toInsert;
		toInsert->prevSib = temp;
		temp->nextSib = toInsert;
	}
	else
	{
		TreeNode* next = cwd->firstChild;
		while (next->name < toInsert->name)
		{
			next = next->nextSib;
		}
		if (next->name == toInsert->name)
		{

			out << " Can't create" <<
				(toInsert->isFile ? " file" : " directory ") << " named "
				<< toInsert->name << ". Duplicate names not allowed !!\n ";
			// delete it to avoid the memory leaks.
			delete toInsert;
			return;
		}
		else
		{
			TreeNode* pre = next->prevSib;
			pre->nextSib = toInsert;
			toInsert->prevSib = pre;
			toInsert->nextSib = next;
			next->prevSib = toInsert;
		}
	}
	out << " Successfully created " <<
		(toInsert->isFile ? " file" : " directory ")
		<< " named " << toInsert->name << " to file system. \n ";
}

// this function just gives back the root of the file system
TreeNode* Directory::getRoot()
{
	return root;
}

// print the contents of the current working directory
void Directory::lsHelper(ostream& out)
{
	TreeNode* p = cwd->firstChild;
	while (p != nullptr)
	{
		out << p->name << 
			(p->isFile ? " ":"/")<<endl;
		p = p->nextSib;
	}
	return;
}
void Directory::ls(ostream& out)
{
	if (!cwd->firstChild)
	{
		out << " No contents ! Current Working Directory is empty!" << endl;
		return;
	}
	lsHelper(out);
	return;
}
void Directory::pwd(ostream& out)
{
	if (cwd == root)
		out << "/" << endl;
	else
	{
		pwdHelper(out, cwd);
		out << endl;
	}
	return;
}
// print the entire path from the  root to current working directory
void Directory::pwdHelper(ostream& out, TreeNode* temp)
{
	if (temp != root)
	{
		pwdHelper(out, temp->parent);
		out << "/";
		out << temp->name;
	}
	return;
}

// helper function to parse the string into tokens
string Directory::tokenizor(string s, string d, bool first)
{
	static string tempStr = "";
	string token;
	static int startStr, endStr;
	bool strStart = false, strEnd = false;
	//string::npos is the value returned when something is not found.
	if (first)
	{
		tempStr = s;
	}
	startStr = tempStr.find_first_not_of(d);
	if (startStr == string::npos) //The delimiter was never found
	{
		return "";
	}
	endStr = tempStr.find_first_of(d, startStr);
	if (endStr == string::npos)
	{
		endStr = tempStr.length();
	}
	token = tempStr.substr(startStr, endStr - startStr);
	tempStr = tempStr.substr(endStr);
	return token;
}
//supports the change drectory via absolute and relative path
void  Directory::chdir(string path, ostream& out)
{
	// if path is like of form /user/sys/bin then relative path is true
	// i.e path starting from the root onwards
	bool absolute_path = false;
	// this will help us to navigate and get to the desired directory
	// if the path provided is correct
	TreeNode* temp = cwd;
	if (path[0] == '/')
	{
		if (path.length() == 1)
		{
			cwd = root;
			out <<"Successfully changed directory to " << cwd->name <<"."<< endl;
			return;
		}
		temp = root;
		absolute_path = true;
		path = path.substr(1, path.length() - 1);
	}
	static string delim = "/";
	string word = tokenizor(path, delim, true);
	while (word != "")
	{
		temp = findIt(temp, word);
		if (temp == nullptr)
		{
			out << "Failed to change Directory !" << endl;
			return;
		}
		word = tokenizor(path, delim, false);
	}
	cwd = temp;
	out << "Successfully changed directory to " << cwd->name <<"."<< endl;
	return;
}
// it will return pointer to the node if it exists
TreeNode* Directory::findIt(TreeNode* temp, string name)
{
	if (name == ".")
		return temp;
	else if (name == "..")
		return temp->parent;
	else
	{
		temp = temp->firstChild;
		while (temp)
		{
			if (temp->name == name)
			{
				if (temp->isFile)
				{
					temp = nullptr;
					return temp;
				}
				else
				{
					return temp;
				}
			}
			temp = temp->nextSib;
		}
		return temp;
	}
}
// find the file and remove that file from the file system
void Directory::rmfile(string toRemove, ostream& out)
{
	// if the current working directory is empty
	if (cwd->firstChild == nullptr)
	{
		out << " Can't remove file named " << toRemove
			<< ". Current Directory is empty !\n";
		return;
	}

	TreeNode* temp = cwd->firstChild;
	while (temp && temp->name != toRemove)
	{
		temp = temp->nextSib;
	}
	// temp points to the end of the list of sibblings
	if (temp == nullptr)
	{
		out << " Can't remove file named " << toRemove
			<< ". No such file exists !\n";
		return;
	}

	// if temp points to the correct node but it is a directory not a file
	if (temp->isFile == false)
	{
		out << " Can't remove named " << toRemove
			<< ". It is a directory not a file!\n";
		return;
	}
	// if temp points to the correct node but it is the only node in the cwd
	if (temp->prevSib == nullptr&& temp->nextSib == nullptr)
	{
		cwd->firstChild = nullptr;
		cwd->lastChild = nullptr;
		delete temp;
	}
	// if temp points to the correct node but it is the first child
	else if (temp == cwd->firstChild)
	{
		cwd->firstChild = temp->nextSib;
		temp->nextSib->prevSib = nullptr;
		delete temp;
	}
	//  if temp points to the correct node but it is the last child
	else if (temp == cwd->lastChild)
	{
		cwd->lastChild = temp->prevSib;
		temp->prevSib->nextSib = nullptr;
		delete temp;
	}
	else
	{
		temp->nextSib->prevSib = temp->prevSib;
		temp->prevSib->nextSib = temp->nextSib;
		delete temp;
	}
	out << "Successfully removed file named " << toRemove
		<< " from the file system\n";
	return;
}

// remove the directory with name "toRemove" in cwd
void Directory::rmdir(string toRemove, ostream& out)
{
	// this means the current working directory is empty
	if (cwd->firstChild == nullptr)
	{
		out << " Can't remove directory named " << toRemove
			<< ". Current Directory is empty !\n";
		return;
	}
	TreeNode* temp = cwd->firstChild;
	// loop through the list untill you find the location if it exist
	while (temp && temp->name != toRemove)
	{
		temp = temp->nextSib;
	}
	// temp points to the end of the list of sibblings
	if (temp == nullptr)
	{
		out << " Can't remove directory named " << toRemove
			<< ". No such directory exists !\n";
		return;
	}
	// now we know it pointing to the actaul node 
	// now check if it is file or directory
	if (temp->isFile == true)
	{
		out << " Can't remove directory named " << toRemove
			<< ". It is not directory but a file!\n";
		return;
	}
	// now we know that we have a node which needs to deleted
	// along with it's descendants
	// call clear() to delete all the decendants of the node temp
	clear(temp);

	// if the node is the only child 
	if (temp->prevSib == nullptr&& temp->nextSib == nullptr)
	{
		cwd->firstChild = nullptr;
		cwd->lastChild = nullptr;
		delete temp;
	}
	// if temp points to the correct node but it is the first child
	else if (temp == cwd->firstChild)
	{
		cwd->firstChild = temp->nextSib;
		temp->nextSib->prevSib = nullptr;
		delete temp;
	}
	//  if temp points to the correct node but it is the last child
	else if (temp == cwd->lastChild)
	{
		cwd->lastChild = temp->prevSib;
		temp->prevSib->nextSib = nullptr;
		delete temp;
	}
	else
	{
		temp->nextSib->prevSib = temp->prevSib;
		temp->prevSib->nextSib = temp->nextSib;
		delete temp;
	}
	out << "Successfully removed directory named " << toRemove
		<< " from the file system\n";
	return;
}
void Directory::lsrHelper(TreeNode* temp, ostream& out, int space)
{
	if (temp != nullptr)
	{
		for (int i = 0; i < space * 3; i++)
		{
			out << " ";
		}
		out << temp->name << (temp->isFile ? " " : "/") << endl;
		lsrHelper(temp->firstChild, out, space + 1);
		lsrHelper(temp->nextSib, out, space);
	}
}
void Directory::lsr(ostream& out)
{
	lsrHelper(cwd->firstChild, out, 0);
	return;
}
// 'Q', change directory to the root and execute a pwd command and an
void Directory::HandleQ(ostream& out)
{  
	out << " *chdir /" << endl;
	cwd = root;
	out << "Successfully changed directoty to /" << endl;
	out << " *pwd" << endl;
	pwd(out);
	out << " *lsr" << endl;
	lsr(out);
	return;
}