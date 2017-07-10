// Shiva Sandesh
// 4/22/2016
// Final Project
# ifndef Directory_H
# define Directory_H
# include<string>
# include<cstring>
# include<iostream>
using namespace std;
struct TreeNode
{
	string name;				// Name of the file or direcory
	TreeNode* parent;			// Parent pointer
	TreeNode* prevSib;			// PrevSib and nextSib to make a doubly linked list 
	TreeNode* nextSib;
	TreeNode* firstChild;		// first and last child for the link list
	TreeNode* lastChild;
	bool isFile;				// is it file or directory
};
class Directory
{
private:
	TreeNode* root;
	TreeNode* cwd;
	TreeNode* createNode(string, bool);
	string tokenizor(string s, string d, bool first);
	void clearHelper(TreeNode*);
	TreeNode* findIt(TreeNode*, string);
	void lsHelper(ostream&);
	void pwdHelper(ostream&, TreeNode*);
	void lsrHelper(TreeNode*, ostream&, int);
	void insert(TreeNode*, ostream&);
public:
	Directory();
	~Directory();
	void mkdir(string, ostream&);
	void mkfile(string, ostream&);
	TreeNode* getRoot();
	// print the contents of the current working directory
	void ls(ostream &);
	// print full path from root to current working directory
	void pwd(ostream&);
	void chdir(string, ostream&);
	void rmfile(string, ostream&);
	void rmdir(string, ostream&);
	void clear(TreeNode*);
	void lsr(ostream&);
	void HandleQ(ostream&);
};
#endif