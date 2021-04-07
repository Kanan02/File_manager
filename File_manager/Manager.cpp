#include "Manager.h"
HANDLE hConsole2 = GetStdHandle(STD_OUTPUT_HANDLE);
/**************************************/
/**************************************/
namespace fs = std::filesystem;

#pragma region Additional Functions
	COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
	{
		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
		{
			return cbsi.dwCursorPosition;
		}
		else
		{

			COORD invalid = { 0, 0 };
			return invalid;
		}
	}


	bool isFullDirectory(string dir) {
		bool isFullDir = false;
		for (char a = 'A'; a <= 90; a++)
		{
			if (dir[0] == a && dir[1] == ':') {
				isFullDir = true;
				break;
			}
		}
		return isFullDir;
	}


	void remove_empty_folders(string path) {


		if (_rmdir((path).c_str()) != 0) {

			_finddata_t* fileinfo = new _finddata_t;
			string mask = "*.*";
			path += "\\";
			path += mask;
			long done = _findfirst(path.c_str(), fileinfo);
			path = path.substr(0, path.size() - 3);
			int isReadable = done;

			while (isReadable != -1)
			{
				if (fileinfo->name[0] != '.') {

					remove_empty_folders(path + fileinfo->name);

				}
				isReadable = _findnext(done, fileinfo);
			}
			if (_rmdir((path).c_str()) != 0) {

			}
			_findclose(done);
			delete fileinfo;
		}
		else {
		}

	}


	void remove_inner_files_in_folder(string path) {
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		path += "\\";
		path += mask;
		long done = _findfirst(path.c_str(), fileinfo);
		path = path.substr(0, path.size() - 3);
		int isReadable = done;

		while (isReadable != -1)
		{
			if (fileinfo->name[0] != '.') {
				if (fileinfo->attrib == 16 || fileinfo->attrib == 17) {
					remove_inner_files_in_folder(path + fileinfo->name);
				}
				else {
					remove((path + fileinfo->name).c_str());
				}

			}
			isReadable = _findnext(done, fileinfo);

		}

		_findclose(done);
		delete fileinfo;

	}


	void cpy_dir(string prev_dir, string new_dir) {

		if (_mkdir(new_dir.c_str()) != 0) {
			cout << "Error!\n";
			return;
		}
		else {

		}
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		prev_dir += "\\";
		new_dir += "\\";
		prev_dir += mask;
		long done = _findfirst(prev_dir.c_str(), fileinfo);
		prev_dir = prev_dir.substr(0, prev_dir.size() - 3);
		int isReadable = done;

		while (isReadable != -1)
		{
			if (fileinfo->name[0] != '.') {
				if (fileinfo->attrib == 16 || fileinfo->attrib == 17) {
					cpy_dir(prev_dir + fileinfo->name, new_dir + fileinfo->name);
				}
				else {
					
					fs::path sourceFile = prev_dir + fileinfo->name;
					fs::path targetParent = new_dir;
					auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".
					try // If you want to avoid exception handling, then use the error code overload of the following functions.
					{
						fs::create_directories(targetParent); // Recursively create target directory if not existing.
						fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
						
					}
					catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
					{
						std::cout << e.what();
					}




				}


			}
			isReadable = _findnext(done, fileinfo);

		}

		_findclose(done);
		delete fileinfo;


	}

	int c = 0;
	void count_bytes(string dir){
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		dir += "\\";
		dir += mask;
		long done = _findfirst(dir.c_str(), fileinfo);
		dir = dir.substr(0, dir.size() - 3);
		int isReadable = done;
		while (isReadable != -1)
		{
			if (fileinfo->name[0] != '.') {
				if (fileinfo->attrib == 16 || fileinfo->attrib == 17) {
					count_bytes(dir + fileinfo->name);
				}
				else {
					c += fileinfo->size;
				}


			}
			isReadable = _findnext(done, fileinfo);

		}
		_findclose(done);
		delete fileinfo;
		

	}
	void search_by_mask(string dir, string mask) {
		_finddata_t* fileinfo = new _finddata_t;
		string full_mask = "*.*";
		if (dir[dir.size() - 1] != '\\')dir += "\\";
		dir += full_mask;
		long done = _findfirst(dir.c_str(), fileinfo);
		dir = dir.substr(0, dir.size() - 3);
		int isReadable = done;
		while (isReadable != -1)
		{
			if (fileinfo->name[0] != '.') {
				if (fileinfo->attrib == 16 || fileinfo->attrib == 17) {
					search_by_mask(dir + fileinfo->name, mask);
				}
			}
			isReadable = _findnext(done, fileinfo);

		}
		dir += "*.";
		dir += mask;
		done = _findfirst(dir.c_str(), fileinfo);
		dir = dir.substr(0, dir.size() - (2+mask.size()));
		isReadable = done;
		
		if (done != -1) {
			cout << "In directory \n";
			SetConsoleTextAttribute(hConsole2, 9);
			cout<< dir << endl;
		}
		SetConsoleTextAttribute(hConsole2, 15);
		while (isReadable != -1)
		{
			if (fileinfo->name[0] != '.') {

				cout << fileinfo->name << endl;
				c++;
			}
			isReadable = _findnext(done, fileinfo);

		}
		if (done != -1)
		cout << endl << endl;
		_findclose(done);



		delete fileinfo;



	}
#pragma endregion

/**************************************/
/**************************************/

#pragma region Default Methods, Ls and Goto

	void Manager::SetDefaultDirectory() {
		directory = "C:\\";
		directory_previous = "C:\\";
	}
	void Manager::NewLine() {
		cout << directory << ">> ";
	}


	void Manager::List_Directory()
	{
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		string path = directory + mask;

		long done = _findfirst(path.c_str(), fileinfo);

		int isReadable = done;
		int count_files = 0;
		int count_folders = 0;

		COORD pos = GetConsoleCursorPosition(hConsole2);
		COORD pos_tmp = pos;
		int i = 0;
		int j = 0;
		int table = 1;
		while (isReadable != -1)
		{
			if (fileinfo->name[0] != '.') {
				i++;
				SetConsoleCursorPosition(hConsole2, pos);
				if (fileinfo->attrib == 17 || fileinfo->attrib == 16) {
					SetConsoleTextAttribute(hConsole2, 9);
					cout << "Folder: " << fileinfo->name << endl;
					count_folders++;
				}
				else {
					SetConsoleTextAttribute(hConsole2, 10);
					cout << "File: " << fileinfo->name << endl;
					count_files++;
				}
				pos.Y++;
				SetConsoleTextAttribute(hConsole2, 15);
				SetConsoleCursorPosition(hConsole2, pos);
				cout << " - " << fileinfo->size << "bytes " << endl;
				pos.Y++;
				SetConsoleCursorPosition(hConsole2, pos);
				char time[100];
				ctime_s(time, 100, &fileinfo->time_create);
				cout << " - " << time << endl;
				pos.Y++;

				if (i % 10 == 0) {
					j++;
					pos.X += 30;
					pos.Y = pos_tmp.Y;
					if (j == 4) {
						pos.X = pos_tmp.X;
						j = 0;
						pos.Y += 35;
						pos_tmp.Y += 35;
						table++;

					}

				}


			}
			isReadable = _findnext(done, fileinfo);
		}
		pos.X = pos_tmp.X;
		pos.Y = pos_tmp.Y + 31;

		SetConsoleCursorPosition(hConsole2, pos);
		SetConsoleTextAttribute(hConsole2, 10);
		cout << count_files << " files";
		SetConsoleTextAttribute(hConsole2, 15);
		cout << " and ";
		SetConsoleTextAttribute(hConsole2, 9);
		cout << count_folders << " folders ";
		SetConsoleTextAttribute(hConsole2, 14);
		cout << "were found\nGreen ones are files, Blue ones are folders.\n";
		SetConsoleTextAttribute(hConsole2, 15);
		_findclose(done);
		delete fileinfo;
	}

	void Manager::Change_Directory(string new_dir)
	{
		/*****Current or previous directory*****/
		if (new_dir == ".") {
			return;
		}
		else if (new_dir == "..") {
			directory = directory_previous;
			return;
		}
		/************************************/



		if (isFullDirectory(new_dir)) {
			directory = new_dir;

		}
		else {
			directory_previous = directory;
			directory += new_dir;

		}
		if (directory[directory.size() - 1] != '\\') { directory += "\\"; }


		//to check if this directory exists
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		string path = directory + mask;

		long done = _findfirst(path.c_str(), fileinfo);
		if (done == -1) {
			directory = directory_previous;
			SetConsoleTextAttribute(hConsole2, 12);
			cout << "There is no such directory.\n";
			SetConsoleTextAttribute(hConsole2, 15);
		}

	}
#pragma endregion

/**************************************/
/**************************************/

#pragma region Folder Methods

	void Manager::Create_Folder(string foldername)
	{
		string tmp;

		if (isFullDirectory(foldername)) {
			tmp = foldername;

		}
		else {
			tmp = directory;


			tmp += foldername;
		}


		if (_mkdir(tmp.c_str()) != 0) {
			SetConsoleTextAttribute(hConsole2, 12);
			cout << "Error occured...Something went wrong.\n";
			SetConsoleTextAttribute(hConsole2, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole2, 10);
			cout << "Done!\n";
			SetConsoleTextAttribute(hConsole2, 15);
		}
	}

	void Manager::Remove_Folder(string foldername)
	{
		//to check if this directory exists
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		string path = directory + mask;

		long done = _findfirst(path.c_str(), fileinfo);
		int isReadable = done;
		bool isReal = false;
		while (isReadable != -1) {
			if (fileinfo->name == foldername) {
				isReal = true;
				break;
			}
			isReadable = _findnext(done, fileinfo);
		}
		if (!isReal) {

			SetConsoleTextAttribute(hConsole2, 12);
			cout << "There is no such directory.\n";
			SetConsoleTextAttribute(hConsole2, 15);
			return;
		}

		delete fileinfo;
		//if it exists:
		string tmp = directory;
		tmp += foldername;
		remove_inner_files_in_folder(tmp);
		remove_empty_folders(tmp);
		SetConsoleTextAttribute(hConsole2, 10);
		cout << "Done!\n";
		SetConsoleTextAttribute(hConsole2, 15);
	}

	void Manager::Copy_Folder(string line)
	{
		string foldername, dir;
		if (line[line.size() - 1] != '\\')line += "\\";

		
		
			int space_index = line.find(" ");
			foldername = line.substr(0, space_index);

			dir = line.substr(space_index + 1, line.size() - 1);
		
		cpy_dir(directory + foldername, dir + foldername);
		SetConsoleTextAttribute(hConsole2, 10);
		cout << "Done!\n";
		SetConsoleTextAttribute(hConsole2, 15);
	}

	void Manager::SizeOfFolder(string foldername)
	{
		//to check if this directory exists
		_finddata_t* fileinfo = new _finddata_t;
		string mask = "*.*";
		string path = directory + mask;

		long done = _findfirst(path.c_str(), fileinfo);
		int isReadable = done;
		bool isReal = false;
		while (isReadable != -1) {
			if (fileinfo->name == foldername) {
				isReal = true;
				break;
			}
			isReadable = _findnext(done, fileinfo);
		}
		if (!isReal) {

			SetConsoleTextAttribute(hConsole2, 12);
			cout << "There is no such directory.\n";
			SetConsoleTextAttribute(hConsole2, 15);
			return;
		}
		
		count_bytes(directory + foldername);
		cout << "This folder weights " <<c << " bytes.\n";
		c = 0;
	}

#pragma endregion

/**************************************/
/**************************************/

#pragma region File Methods

	void Manager::Create_File(string filename)
	{
		string tmp;
		bool isF = false;
		if (isFullDirectory(filename)) {
			tmp = filename;
			isF = true;
		}
		else {
			tmp = directory;
			tmp += filename;
		}

		if (filename.size() < 4)filename += ".txt";
		if (filename.substr(filename.size() - 4, 4) == ".txt") {

		}
		else {
			filename += ".txt";
		}
		if ((filename.find(':') != string::npos && !isF) || filename.find('/') != string::npos || filename.find('?') != string::npos || (filename.find('\\') != string::npos && !isF) || filename.find('*') != string::npos || filename.find('\"') != string::npos || filename.find('<') != string::npos || filename.find('>') != string::npos || filename.find('|') != string::npos) {
			SetConsoleTextAttribute(hConsole2, 12);
			cout << "Error occured!\n";
			cout << "A file name can't contain any of these characters:\n";
			cout << ": / \\ ? \" * < > |\n";
			SetConsoleTextAttribute(hConsole2, 15);

		}
		else {
			ofstream File(tmp);
			SetConsoleTextAttribute(hConsole2, 10);
			cout << "Done!\n";
			SetConsoleTextAttribute(hConsole2, 15);

			File << "***********This txt file was created by MyFileManager!***********\n";
			File.close();
		}


	}

	void Manager::Remove_File(string filename)
	{
		string tmp;
		if (isFullDirectory(filename)) {
			tmp = filename;

		}
		else {
			tmp = directory;
			tmp += filename;
		}

		if (remove(tmp.c_str()) != 0) {
			SetConsoleTextAttribute(hConsole2, 12);

			cout << "Error...\nThere is no such file.\n";
			SetConsoleTextAttribute(hConsole2, 15);

		}
		else {
			SetConsoleTextAttribute(hConsole2, 10);

			cout << "Done!\n";
			SetConsoleTextAttribute(hConsole2, 15);

		}

	}

	void Manager::Copy_File(string line)
	{
		
		string filename, dir;
		
		if (line[line.size() - 1] != '\\')line += "\\";
		int space_index = line.find(" ");
		filename = line.substr(0, space_index);

		dir = line.substr(space_index + 1, line.size() - 1);
		

		fs::path sourceFile = directory+filename;
		fs::path targetParent = dir;
		auto target = targetParent / sourceFile.filename(); // sourceFile.filename() returns "sourceFile.ext".
		try // If you want to avoid exception handling, then use the error code overload of the following functions.
		{
			fs::create_directories(targetParent); // Recursively create target directory if not existing.
			fs::copy_file(sourceFile, target, fs::copy_options::overwrite_existing);
			SetConsoleTextAttribute(hConsole2, 10);
			cout << "Done!\n";
			SetConsoleTextAttribute(hConsole2, 15);
		}
		catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
		{
			std::cout << e.what();
		}

	}


	void Manager::SearchByMask(string mask)
	{
		if (mask[0] == '.')mask = mask.substr(1, mask.size() - 1);
		search_by_mask(directory, mask);
		SetConsoleTextAttribute(hConsole2, 10);
		cout << "There are "<<c<<" ."<<mask<<" files in total.\n";
		c = 0;
		SetConsoleTextAttribute(hConsole2, 15);

	}


#pragma endregion

/**************************************/
/**************************************/

#pragma region Rename

	void Manager::Rename(string name)
	{
		int space_index = name.find(" ");
		string oldName = name.substr(0, space_index);

		string newName = name.substr(space_index + 1, name.size() - 1);

		if (rename((directory + oldName).c_str(), (directory + newName).c_str()) != 0) {
			SetConsoleTextAttribute(hConsole2, 12);
			cout << "There is no such file or directory.\n";
			SetConsoleTextAttribute(hConsole2, 15);
		}
		else {
			SetConsoleTextAttribute(hConsole2, 10);
			cout << "Done\n";
			SetConsoleTextAttribute(hConsole2, 15);
		}

	}

#pragma endregion

