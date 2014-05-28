#ifndef _OS_HPP_
#define _OS_HPP_
#include <vector>
#include <string>
using namespace std;

namespace os
{

bool listdir(std::string path, std::vector<std::string> &files);

bool isfile(std::string path);

bool isdir(std::string path);
namespace path{
void join(std::string path, std::string file, std::string &join_file);

void split(std::string file_path, std::string &path, std::string &file);
}
}


#endif
