#include "Manager.h"
#include <string>
void getInfo();
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void main() {
	#pragma region Font
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 21;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
	#pragma endregion

	#pragma region Main

	cout << "\t\tWelcome to MyFileManager!\n";
	cout << "Here you can easily work with all files and folders on your pc!\n";
	cout << "There are just some commands that you need to know before using this file manager.\n\n";
	getInfo();
	string command;
	Manager m;
	m.SetDefaultDirectory();
	while (command != "exit") {
		m.NewLine();
		getline(cin, command);
		if (command == "ls") {
			m.List_Directory();
		}
		else if (command == "info") {
			getInfo();
		}
		else if (command == "clear") {
			system("cls");
		}
		else if (command.substr(0, 4) == "goto") {
			m.Change_Directory(command.substr(5, command.size() - 4)); //substring is only a directory, we cut the word "goto "
		}
		else if (command.substr(0, 5) == "mkdir") {
			m.Create_Folder(command.substr(6, command.size() - 5)); //substring is only a foldername, we cut the word "mkdir "
		}
		else if (command.substr(0, 6) == "mkfile") {
			m.Create_File(command.substr(7, command.size() - 6)); //substring is only a filename, we cut the word "mkfile "
		}
		else if (command.substr(0, 5) == "rmdir") {
			m.Remove_Folder(command.substr(6, command.size() - 5)); //substring is only a foldername, we cut the word "mkdir "
		}
		else if (command.substr(0, 6) == "rmfile") {
			m.Remove_File(command.substr(7, command.size() - 6)); //substring is only a filename, we cut the word "mkfile "
		}
		else if (command.substr(0, 6) == "rename") {
			m.Rename(command.substr(7, command.size() - 6)); 
		}
		else if (command.substr(0, 8) == "cpydirto") {
			m.Copy_Folder(command.substr(9, command.size() - 8));
		}
		else if (command.substr(0, 9) == "cpyfileto") {
			m.Copy_File(command.substr(10, command.size() - 9));
		}
		else if (command.substr(0, 6) == "sizeof") {
			m.SizeOfFolder(command.substr(7, command.size() - 6));
		}
		else if (command.substr(0, 12) == "searchbymask") {
			m.SearchByMask(command.substr(13, command.size() - 12));
		}

		else if (command == "exit") {
			SetConsoleTextAttribute(hConsole, 10);
			cout << "Goodbye!\n";
			SetConsoleTextAttribute(hConsole, 15);
			break;
		}
		else if (command=="") {

		}
		else {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Error, there is no such command.\n";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Use ";
			SetConsoleTextAttribute(hConsole, 9);
			cout << "info";
			SetConsoleTextAttribute(hConsole, 15);
			cout<<" keyword to list the commands.\n";

		}
		

	}
	#pragma endregion

}

#pragma region getInfo

void getInfo()
{
	cout << "\t\t\tInstrustions\n";
	
	/************************************************/
	
	cout << "\tDirectories.\n";
	cout << "To list current folder, use command ";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "ls";
	SetConsoleTextAttribute(hConsole, 15);
	cout << ".\n";
	/************************************************/
	cout << "To go to another folder, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "goto";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " directory_name'.('goto .' gives you current directory, 'goto ..' gives you directory that you wrote previously)\n";
	/************************************************/
	cout << "To make folder, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "mkdir";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " folder_name'.\n";
	/************************************************/
	cout << "To remove folder, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "rmdir";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " folder_name'.\n";
	/************************************************/
	
	cout << "To copy folder to another directory, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "cpydirto";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " folder_name directory'.(Foldername should not have space)\n";
	/************************************************/
	cout << "To rename file or folder, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "rename ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "old_name new_name'.\n\n";
	/************************************************/
	cout << "To search file or folder by mask, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "searchbymask ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "mask'.\n\n";
	/************************************************/
	cout << "To get the size of folder in bytes, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "sizeof ";
	SetConsoleTextAttribute(hConsole, 15);
	cout << "foldername'.\n\n";
	/************************************************/
	cout << "\tFiles.\n";
	cout << "To create a file, use command ";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "mkfile";
	SetConsoleTextAttribute(hConsole, 15);
	cout << ".\n";
	/************************************************/
	cout << "To delete a file, use command ";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "rmfile";
	SetConsoleTextAttribute(hConsole, 15);
	cout << ".\n";
	
	/************************************************/
	cout << "To copy file to another directory, use command '";
	SetConsoleTextAttribute(hConsole, 10);
	cout << "cpyfileto";
	SetConsoleTextAttribute(hConsole, 15);
	cout << " filename directory_name'.\n\n";
	/************************************************/
	cout << "\tOther.\n";
	cout << "To display this instruction again, use command ";
	SetConsoleTextAttribute(hConsole, 9);
	cout << "info";
	SetConsoleTextAttribute(hConsole, 15);
	cout << ".\n";
	/************************************************/
	cout << "To clear the window use command ";
	SetConsoleTextAttribute(hConsole, 9);
	cout << "clear";
	SetConsoleTextAttribute(hConsole, 15);
	cout << ".\n";
	/************************************************/
	cout << "To exit, use command ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << "exit";
	SetConsoleTextAttribute(hConsole, 15);
	cout << ".\n";
	/************************************************/

}

#pragma endregion
