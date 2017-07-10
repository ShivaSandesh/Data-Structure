# include "Directory.h"
# include<iostream>
# include<string>
# include<fstream>
using namespace std;
string tokens(string, string, bool);
int main(int argc, char * argv[])
{
	if (argc < 3)
	{
		cout << "Insufficient arguments";
		exit(-1);
	}
	ifstream input(argv[1]);
	if (!input)
	{
		cout << "Cannot find input file";
		exit(-1);
	}
	ofstream out;
	out.open(argv[2]);
	if (!out)
	{
		cout << "Insufficient space";
		exit(-1);
	}
	Directory Unix;
	string allCommnands[] = { "mkdir", "mkfile", "chdir", "rmdir",
		"rmfile", "ls", "pwd", "lsr", "Q" };
	string commnand;
	string temp;
	string line;
	string delimiter = " ";
	while (!input.eof())
	{
		getline(input, line, '\n');
		out << "*" << line << endl;
		commnand = tokens(line, delimiter, true);
		temp = tokens(line, delimiter, false);
		int index = -1;
		for (int i = 0; i < 9; i++)
		{
			if (allCommnands[i] == commnand)
				index = i;
		}
		switch (index)
		{
			//  case 0 -> make a directory
		case 0:
			Unix.mkdir(temp, out);
			break;
			//  case 1 -> make a file 
		case 1:
			Unix.mkfile(temp, out);
			break;
			// case 2 -> change the current working directory
		case 2:
			Unix.chdir(temp, out);
			break;
			// case 3-> remove a directory from file system
		case 3:
			Unix.rmdir(temp, out);
			break;
			// case 4-> remove a file from file system
		case 4:
			Unix.rmfile(temp, out);
			break;
			// case 5-> print all the contents of the current working directory
		case 5:
			Unix.ls(out);
			break;
			// case 6 -> print full path from root to current working directory
		case 6:
			Unix.pwd(out);
			break;
			// case 7->recursively list all file in this and all subdirectories
		case 7:Unix.lsr(out);
			break;
			// case 8-> change directory to the root and execute a pwd command 
			// and a lsr command
		case 8:
			Unix.HandleQ(out);
			break;
			// none of the other commands are recognized by the file system
		default:
			out << "Invalid Command !" << endl;
			break;
		}
		out << endl;
	}
	return 0;
}
string tokens(string s, string d, bool first)
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
