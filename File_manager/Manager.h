#pragma once
#ifndef manager_h
#define manager_h

#include <iostream>
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <time.h>
#include <Windows.h>
#include <winbase.h>
#include <fstream>
#include <exception>
#include <filesystem>
using namespace std;
/*
	*****************************METHODS OF MANAGER************************
	ls = lists all files and folders in current directory in 4 columns, 10 elements each
		if more than 40 elements , it will make one more such 40 element's table
		it shows the size of each file (not folder), file/folder name and date of creation
		at the end it gives you number of folders and files in directory (doesn't count '.' and '..' folders)


	goto 'directory' = changes current directory. 
		You can change it completely by writing full directory, or add just folder name of current directory
		(\ is not neccesarry, program will work either way)

	mkdir 'foldername' = creates folder in any directory or current directory, as you want
	mkfile 'filename' = creates file in full directory or current directory, as you want (file will have default sentence in it)

	rmdir 'foldername'= recursively deletes the folder, firsly deletes all files then all folders
	rmfile 'filename' = deletes a file in full directory or current directory, as you want

	rename 'oldname new name' = renames any file or folder in current directory

	cpyfileto 'filename directory'= copies any file from current directory to the directory that you
		choose as second argument

	cpydirto 'foldername directory'= recursively copies all  files and folders of a folder from current directory to the directory that you
		choose as second argument

	sizeof 'foldername' - recursively counts the size of a folder and all subfolders and files
	searchbymask 'mask' - recursively prints all files(including subfolders) with given mask, shows the total number of files with such mask, * prints all files and folders

	exit = exits the program
	clear= clears window and prints newline
	info = prints instuctions


*/

class Manager
	{
		string directory_previous;
		string directory;
	public:
		Manager() {

		}
		~Manager() {

		}
		void SetDefaultDirectory();
		void NewLine(); 
		void List_Directory();
		void Change_Directory(string new_dir);
		void Create_Folder(string foldername);
		void Remove_Folder(string foldername);
		void Create_File(string filename);
		void Remove_File(string filename);
		void Copy_File(string line);
		void SearchByMask(string mask);
		void Copy_Folder(string line);
		
		void SizeOfFolder(string foldername);
		void Rename(string name);
	};

#endif // !
