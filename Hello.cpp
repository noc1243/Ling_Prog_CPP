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
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <string.h>
#include <cstdlib>


#ifdef _WIN32 || _WIN64
	#define CLEAR "cls"
	#include <conio.h>
#else
	#define CLEAR "clear"
#include <ncurses.h>
#endif

#define ENTER 13


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
					case 4: this->priority = atoi (line.c_str());
							break;
					case 5: this->flags = atoi (line.c_str());
							break;
				}
				i++;
			}
		}
	}

	User (const User& original): path (original.getPath()), serial (original.getSerial()), nick (original.getName()),
			cryptPassword(original.getCryptPassword()), cpf (original.getCpf()), priority(original.getPriority()), flags(original.getFlags()) {}

	User () : path (" ... "), serial (0), nick ("Unknown"), cryptPassword (" ... "), cpf (" ... "), priority (1), flags(0) {}

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

	inline unsigned int getPriority ()
	{
		return priority;
	}

	inline unsigned int getFlags ()
	{
		return flags;
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

	inline unsigned int getPriority () const
	{
		return priority;
	}

	inline unsigned int getFlags () const
	{
		return flags;
	}

	friend ostream& operator<<(ostream& os, const User &user)
	{
		os<< user.nick;
		return os;
	}

	friend ostream& operator<<(ostream& os, const User * user)
	{
		if (user)
		{
			os<< user->nick;
		}
		return os;
	}

	private:
	string path ;
	int serial;
	string nick;
	string cryptPassword;
	string cpf;
	unsigned int priority;
	unsigned int flags;
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

	friend ostream &operator<< (ostream &os, const Post &post)
	{
		os << post.answer << endl;
		os << post.author << endl;
		os << post.text << endl;
		return os;
	}

	friend ostream &operator<< (ostream &os, const Post *post)
	{
		if (post)
		{
			os << post->answer << endl;
			os << post->author << endl;
			os << post->text << endl;
		}
		return os;
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


class Thread
{
public:
	Thread (const char * threadName)
	{
		stringstream ss;
		ss << threadName;
		ss >> this->threadName;

		this->loaded = false;
	}

	~Thread ()
	{
		for (unsigned int i = 0; i<posts.size(); i++)
		{
			delete_pointed_to<Post> (posts [i]);
		}
	}

	void loadPosts ()
	{
		char path [500];
		int postIndex = 1;
		sprintf (path, ".\\%s\\%d.txt", this->threadName.c_str(), postIndex);

		ifstream file (path);

		while (file.is_open())
		{
			posts.push_back(new Post (threadName.c_str(), postIndex));

			postIndex++;
			file.close();

			sprintf (path, ".\\%s\\%d.txt", this->threadName.c_str(), postIndex);
			file.open (path);
		}
		this->loaded = true;
	}

	void showThread ()
	{
		if (!posts.size ())
		{
			cout << "Thread inexistente!" << endl ;
		}
		for (unsigned int i = 0; i< posts.size(); i++)
		{
			cout << i << endl;
			posts[i]->showPost();
			cout << endl << endl ;
		}
	}

	inline string getThreadName ()
	{
		return threadName;
	}

	inline bool getLoaded ()
	{
		return loaded;
	}

	friend ostream &operator<< (ostream &os, Thread &thread)
	{
		os<< thread.threadName << endl;
		if (thread.loaded)
			for (unsigned int i=0; i<thread.posts.size(); i++) os<< thread.posts[i];
		return os;
	}

	friend ostream &operator<< (ostream &os, Thread *thread)
	{
		if (thread)
		{
			os<< thread->threadName << endl;
			if (thread->loaded)
				for (unsigned int i=0; i<thread->posts.size(); i++) os<< thread->posts[i];
		}
		return os;
	}

private:
	string threadName;
	vector <Post *> posts;
	bool loaded;

	template <typename T>
	void delete_pointed_to(T* const ptr)
	{
	    delete ptr;
	}


};

class Interface
{
public:
	Interface()
	{
		loggedUser = new User();

		const char * path = ".\\";

		DIR *dir = opendir (path);

		struct dirent *entry = readdir (dir);

		while (entry != NULL)
		{
			bool validDirectoryName = true;
			if (entry->d_type == DT_DIR)
			{
				for (int i=0; i<7; i++) if (!strcmp(entry->d_name, forbiddenPaths[i])) validDirectoryName = false;
				if (validDirectoryName)
					threads.push_back(new Thread (entry->d_name));
			}

			entry = readdir(dir);
		}
	}

	~Interface ()
	{
		for (unsigned int i = 0; i<threads.size(); i++)
		{
			delete_pointed_to<Thread> (threads [i]);
		}
		delete loggedUser;
	}

	void showThreads ()
	{
		for (unsigned int i=0; i<threads.size(); i++)
		{
			cout << i << endl;
			cout << threads[i]->getThreadName() << endl;
		}
	}

	inline bool getNeedOption()
	{
		return needOption;
	}

	inline void setNeedOption (bool needOption)
	{
		this->needOption = needOption;
	}

	void loadThread (string threadName)
	{
		for (unsigned int i=0; i<threads.size(); i++)
		{
			if (threads[i]->getThreadName() == threadName)
			{
				threads[i]->loadPosts();
				return;
			}
		}
		cout << "Thread nao encontrada" << endl;
	}

	void showLoadedThreads ()
	{
		for (unsigned int i=0; i<threads.size(); i++)
		{
			if (threads[i]->getLoaded())
				cout << threads[i];
		}
	}

	friend ostream &operator<< (ostream &os, Interface &interface)
	{
		for (unsigned int i=0; i<interface.threads.size(); i++) os << interface.threads[i];
		return os;
	}

	void showMenu ()
	{
		switch (menuState)
		{
			case 1: cout << menu1;
					break;
			case 2: this->showLoadedThreads();
					cout << endl;
					cout << menu2;
					break;
			case 3: this->needOption = false;
					break;
			case 4: switch (loggedUser->getPriority())
					{
						case 0 :cout << menu4a;
								break;
						case 1 :cout << menu4b;
								break;
					}
					break;
		}
	}

	void interpreteMenu (unsigned int option)
	{
		string subOption;
		string name;
		char password [20];
		unsigned int index = 0;
		switch (menuState)
		{
			case 1 :switch (option)
					{
						case 1: cout << *this << endl;
								cin >> subOption;
								this->loadThread (subOption);
								this->menuState = 2;
								break;
						case 2: this->menuState = 3;
								break;
						case 3: this->menuState = 4;
								break;
					}
					break;
			case 2:switch (option)
				   {
					case 1: cout << "Postando...\n";
							break;
					case 2: cout << "Respondendo...\n";
							break;
					case 3: menuState = 1;
							break;
				   }
				   break;
			case 3: cout << "Login: ";
					cin >> name;
					cout << "Password: ";
					password[index] = getch ();
					while ((int)password[index] != ENTER && index <20)
					{
						index ++;
						password[index] = getch();
					}
					password[index] = '\0';
					this->menuState = 1;
					cout << password << endl;
					getch();
					break;
			case 4:switch (loggedUser->getPriority())
				   {
						case 0: switch (option)
								{
									case 1: cout << "Trocando... \n";
											break;
									case 2: cout << "Banindo... \n";
											break;
									case 3: cout << "Trocando... \n";
											break;
									case 4: cout << "Criando... \n";
											break;
									case 5: logout();
											menuState = 1;
											break;
									case 6: menuState = 1;
											break;
								}
								break;
						case 1: switch (option)
								{
									case 1: cout << "Trocando\n";
											break;
									case 2: logout();
											menuState = 1;
											break;
									case 3: menuState = 1;
											break;
								}
								break;
				   }
				   break;
		}
	}

private:
	vector <Thread *> threads;
	User *loggedUser;
	unsigned int menuState = 1;
	const char * forbiddenPaths [7] = {"Debug", "Users", "bin", "lib", ".settings", ".", ".."};
	bool needOption = true;

	string menu1 = "Escolha uma entre as opções abaixo:\n\
1 - Ver todas as Threads e escolher uma para visualizar\n\
2 - Fazer Login\n\
3 - Opções de Conta\n\
0 - Sair do Programa\n";

	string menu2 = "1 - Postar Algo\n\
2 - Responder uma postagem \n\
3 - Sair da thread\n";

	string menu4a = "1 - Trocar Senha\n\
2 - Banir Usuário\n\
3 - Trocar Permissão de usuário\n\
4 - Criar Thread\n\
5 - Logout\n\
6 - Sair deste menu\n";

	string menu4b = "1 - Trocar Senha\n\
2 - Logout\n\
3 - Sair deste menu\n";



	template <typename T>
	void delete_pointed_to(T* const ptr)
	{
	    delete ptr;
	}

	void logout ()
	{
		if (this->loggedUser->getName()!= "Unknown")
		{
			delete this->loggedUser;
			this->loggedUser = new User ();
		}
		else
			cout << "Você não está logado!!\n";
	}

};

int main() {
	unsigned int option = 1;
   User  firstUser(2);
//   cout << firstUser;
//   firstUser.showValues();
//   User testUser;
//   testUser.showValues();
//   Post explodiu ("Topico_B",3);
//   cout << explodiu;
//   explodiu.showPost();
//   Thread firstThread ("Topico_B");
//   firstThread.loadPosts();
//   cout << firstThread;
//   firstThread.showThread();
   Interface interface;
//   cout << interface;
//   interface.showThreads();
//   interface.showLoadedThreads();
//   interface.loadThread ("Topico_A");
//   cout << interface;
   interface.showMenu();
   cin >> option;
   while (option)
   {
	   system (CLEAR);
	   interface.interpreteMenu(option);
	   system (CLEAR);
	   interface.showMenu ();
	   if (interface.getNeedOption ())
		   cin >> option;
	   else
		   interface.setNeedOption (true);
   }
//   interface.showLoadedThreads();
   return 0;
}



