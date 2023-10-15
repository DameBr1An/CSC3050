#ifndef ABC
#define ABC

#include <vector>
#include <string>
#include <map>
#include<string>
using namespace std;

//void pass1(std::string filename);
//void pass2(std::string filename);
std::string deal_str(std::string str);
std::string delete_maohao(std::string str);
extern std::map<std::string, unsigned int> labelMap;
class LabelTable
{
    public:
    LabelTable(){
        //
    }
    void pass1(std::string infilename);
    void pass2(std::string infilename, std::string outfilename);


};

#endif