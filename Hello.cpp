/*
 * Hello.cpp
 *
 *  Created on: 28 de nov de 2016
 *      Author: nocera
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>

using namespace std;


class User
{
	public:
	User (const int fileName)
	{
		char buffer [100];
		sprintf (buffer, ".\\Users\\%d.txt", fileName);

		int i =0;
		string line;

		stringstream ss;
		ss << buffer;
		ss >> this->path;

		ifstream file (buffer);
		if (file.is_open())
		{
			while (getline (file, line))
			{
				switch (i)
				{
					case 0: this->serial = atoi (line.c_str());
					     	break;
					case 1: this->nick = line;
							break;
					case 2: this->cryptPassword = line;
							break;
					case 3: this->cpf = line;
							break;
				}
				i++;
			}
		}
	}

	User (const User& original): path (original.getPath()), serial (original.getSerial()), nick (original.getName()),
			cryptPassword(original.getCryptPassword()), cpf (original.getCpf()) {}

	User () : path (" ... "), serial (0), nick ("Unknown"), cryptPassword (" ... "), cpf (" ... ") {}

	void showValues ()
	{
		cout << path << endl;
		cout << serial << endl;
		cout << nick << endl;
		cout << cryptPassword << endl;
		cout << cpf << endl;
	}

	inline string getName ()
	{
		return this->nick;
	}

	inline int getSerial ()
	{
		return this->serial;
	}

	inline string getCryptPassword ()
	{
		return this->cryptPassword;
	}

	inline string getCpf()
	{
		return this->cpf;
	}

	inline string getPath ()
	{
		return path;
	}

	inline string getName () const
	{
		return this->nick;
	}

	inline int getSerial () const
	{
		return this->serial;
	}

	inline string getCryptPassword () const
	{
		return this->cryptPassword;
	}

	inline string getCpf() const
	{
		return this->cpf;
	}

	inline string getPath ()const
	{
		return path;
	}

	private:
	string path ;
	int serial;
	string nick;
	string cryptPassword;
	string cpf;
};

class Post
{
public:
	Post (const char * postName, const int postOrder)
	{
		char buffer [100];
		sprintf (buffer, ".\\%s\\%d.txt", postName, postOrder);

		stringstream ss;
		ss << buffer;
		ss >> this->path;

		int i =0;
		string line;

		ifstream file (buffer);
		if (file.is_open())
		{
			while (getline (file, line))
			{
				switch (i)
				{
					case 0: this->serial = atoi (line.c_str());
					     	break;
					case 1: searchUser(line);
							break;
					case 2: (atoi (line.c_str()) > 0?answer= new Post(postName, atoi(line.c_str())) : answer = NULL);
							break;
					case 3: this->text = line;
							break;
				}
				i++;
			}
		}

	}

	~Post ()
	{
		if (answer)
			delete answer;
		if (author)
			delete author;
	}

	void showPost ()
	{
		if (answer)
			answer->showPost();
		cout << path << endl;
		cout << serial << endl;
		author->showValues();
		cout << text << endl;
	}

	inline string getPath ()
	{
		return path;
	}

	inline int getSerial ()
	{
		return serial;
	}

	inline User * getAuthor()
	{
		return author;
	}

	inline Post * getAnswer ()
	{
		return answer;
	}

	inline string getText ()
	{
		return text;
	}



private:
	string path;
	int serial;
	User * author;
	Post * answer;
	string text;

	void searchUser (string userName)
	{
		char buffer [100];
		int userIndex = 1;
		int i = 0;

		sprintf (buffer, ".\\Users\\%d.txt", userIndex);

		ifstream file (buffer);

		while (file.is_open())
		{
			string line;
			i = 0;

			while (getline (file, line))
			{
				if (i == 1)
					if (line == userName)
					{
						this->author = new User (userIndex);
						file.close();
						return;
					}
				i++;
			}

			userIndex++;
			file.close();


			sprintf (buffer, ".\\Users\\%d.txt", userIndex);
			file.open (buffer);
		}
		this->author = new User();
	}
};

int main() {
   User  firstUser(1);
//   firstUser.showValues();
//   User testUser;
//   testUser.showValues();
   Post explodiu ("Topico_B",3);
   explodiu.showPost();
   return 0;
}



