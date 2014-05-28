#include "os.hpp"
#include <iostream>
using namespace std;

#if defined(LINUX)
// linux
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#else
// windows
#include <windows.h>
#include <tchar.h>
#endif

namespace os
{

bool listdir(std::string path, vector<std::string> &files)
{

#if defined(LINUX)
	// linux
	struct dirent *entry;
	DIR *dir;
	dir = opendir(path.c_str());
	if ( dir == NULL )
	{
		return false;
	}

	while ( ( entry = readdir(dir) ) )
	{
		files->push_back(entry->d_name);
	}

	closedir(dir);
	return true;

#else
	// windows
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	char dir_path[256];
	sprintf(dir_path, "%s\\*.*", path.c_str());
	hFind = FindFirstFile(dir_path, &findFileData);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	do 
	{
		if (findFileData.cFileName[0] == '.')
			continue;
		files.push_back( findFileData.cFileName );
	} while ( FindNextFile( hFind, &findFileData ) );

	FindClose(hFind);
	return true;
#endif
}

bool isfile(std::string path)
{
#if defined(LINUX)
//linux
	struct stat file_stat;
	if ( lsstat(path, &file_stat) < 0 )
	{
		return false;
	}

	if ( !S_ISDIR(file_stat.st_mode) )
	{
		return true;
	}
	return false;

#else
//windows

	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	hFind = FindFirstFile(path.c_str(), &findFileData);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		return false;
	}
	FindClose(hFind);
	if (findFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
	{
		return true;
	}

	return false;

#endif
}

bool isdir(std::string path)
{
#if defined(LINUX)
	//linux
	struct stat file_stat;
	if ( lsstat(path, &file_stat) < 0 )
	{
		return false;
	}

	if ( S_ISDIR(file_stat.st_mode) )
	{
		return true;
	}
	return false;


#else
	//windows
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	string path1 = "D:\\Pictures\\num_samples\\0";
	int i = path1.compare(path);
	hFind = FindFirstFile(path.c_str(), &findFileData);
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		FindClose(hFind);
		return false;
	}
	
	if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
	{
		FindClose(hFind);
		return true;
	}
	FindClose(hFind);
	return false;
#endif
}

namespace path{
void join(std::string path, std::string file, std::string &join_path)
{
	int len = path.size();
	if (path[len-1] == '\\')
	{
		join_path = path + file;
	}
	else 
	{
		join_path = path + "\\" + file;
	}
	
}

void split(std::string file_path, std::string &path, std::string &file)
{
	int len = file_path.size();
	int idx = file_path.rfind('\\');
	path = file_path.substr(0, idx+1);
	file = file_path.substr(idx+1, len-idx-1);
}

} // namespace path

} // namespace os

/* end of file*/
