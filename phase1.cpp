#include <iostream>
#include <sstream>
#include <fstream>
#include "LabelTable.h"
#include <string>
using namespace std;
/*std::string regname [32] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", 
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};  */

std::map<std::string, unsigned int> labelMap;

std::string deal_str(std::string str){
    //if(str.empty()){return str;}
     
    if(str.find("#")!=str.npos){
        str=str.substr(0,str.find('#'));
    }
    if(!str.empty()){
        str.erase(0,str.find_first_not_of(" "));
        str.erase(str.find_last_not_of(" ")+1); 
        if(str.substr(0,2)==" "){str="";}
    }
    return str;
}



std::string delete_maohao(std::string str){
    if (str.find(':') != str.npos) {
        str = str.substr(str.find(':') + 1, str.length());
    }
    return deal_str(str);
}



void LabelTable::pass1(std::string filename){
    unsigned int PC=0x400000;
    int current_address=PC;
    std::ifstream input(filename);
    if(!input){
        std::cout<<filename<<"fail to open"<<std::endl;
        return;
    }
    int instruction_Num=0;
    std::string string1;
    std::string string2;
    std::string judge_ifdottext=".text";
    while(getline(input,string1)){
        if(deal_str((string1).substr(0,5))==judge_ifdottext){
            break;                                                //to judge if it is .text
        }

    }
    //std::stringstream 
    while(getline(input,string1)){
      string2=deal_str(string1);
      if(string2==""){continue;}
            

        
        if(string2.find(':')!=std::string::npos &&string2.back()!=':'){
            labelMap[string2.substr(0,string2.find(':'))]=PC;
            PC=PC+4;
            instruction_Num=instruction_Num+1;

        }
        else if(string2.back()==':'){
            labelMap[string2.substr(0,string2.length()-1)]=PC;              //save the address of instruction
        }
        else if(delete_maohao(string1).length()>0){                     //save the amount of instruction
            PC=PC+4;
            instruction_Num=instruction_Num+1;
        }
     
    }
    input.close();




}