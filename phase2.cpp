#include <iostream>
#include <sstream>
#include <fstream>
#include "LabelTable.h"
#include <string>
#include <vector>
using namespace std;

unsigned int PC = 0x400000;

extern std::map<std::string, unsigned int> labelMap;

std::string tobin(unsigned int num, int bit) {
    std::string result;
    while (num || bit) {
        if (num & 1) {
            result = "1" + result;
        } else {
            result = "0" + result;
        }
        num >>= 1;
        bit--;
        if (bit == 0) {
            break;
        }
    }
    return result;
}

std::string cal_im(unsigned int PC, unsigned int that_address){
    std::string ans;
    unsigned int off_set = (that_address - PC - 4) / 4; // minus 4 is important
    ans = tobin(off_set,16);
    return ans;
}


std::map<std::string, std::string> r_map{

    {"add", "100000"}, 
    {"addu", "100001"}, 
    {"and", "100100"}, 
    {"div", "011010"}, 
    {"divu", "011011"},
    {"jalr", "001001"}, 
    {"jr", "001000"}, 
    {"mfhi", "010000"}, 
    {"mflo", "010010"}, 
    {"mthi", "010001"},
    {"mtlo", "010011"}, 
    {"mult", "011000"}, 
    {"multu", "011001"}, 
    {"nor", "100111"}, 
    {"or", "100101"},
    {"sll", "000000"}, 
    {"sllv", "000100"}, 
    {"slt", "101010"}, 
    {"sltu", "101011"}, 
    {"sra", "000011"},
    {"srav", "000111"}, 
    {"srl", "000010"}, 
    {"srlv", "000110"}, 
    {"sub", "100010"}, 
    {"subu", "100011"},
    {"syscall", "001100"}, 
    {"xor", "100110"}

};

std::map<std::string, std::string> i_map {

    {"addi", "001000"}, 
    {"addiu", "001001"}, 
    {"andi", "001100"}, 
    {"beq", "000100"}, 
    {"bgez", "000001"},
    {"bgtz", "000111"}, 
    {"blez", "000110"}, 
    {"bltz", "000001"}, 
    {"bne", "000101"},
    {"lb", "100000"}, 
    {"lbu", "100100"}, 
    {"lh", "100001"}, 
    {"lhu", "100101"}, 
    {"lui", "001111"},
    {"lw", "100011"}, 
    {"ori", "001101"}, 
    {"sb", "101000"}, 
    {"slti", "001010"}, 
    {"sltiu", "001011"}, 
    {"sh", "101001"}, 
    {"sw", "101011"}, 
    {"xori", "001110"}, 
    {"lwl", "100010"},
    {"lwr", "100110"}, 
    {"swl", "101010"},
    {"swr", "101110"}
};

std::map<std::string, std::string> extendi_map {
    {"bgez", "00001"},
    {"bgtz", "00000"},
    {"blez", "00000"},
    {"bltz", "00000"},
};

std::map<std::string, std::string> j_map{

    {"j", "000010"}, 
    {"jal", "000011"}

};
std::map<std::string, int> rmap {

    {"$zero", 0}, 
    {"$at", 1}, 
    {"$v0", 2}, {"$v1", 3}, 
    {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7},
    {"$t0", 8}, {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13}, {"$t6", 14}, {"$t7", 15},
    {"$s0", 16}, {"$s1", 17}, {"$s2", 18}, {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, 
    {"$k0", 26}, {"$k1", 27}, 
    {"$gp", 28}, 
    {"$sp", 29}, 
    {"$fp", 30}, 
    {"$ra", 31}

};

std::string transR(std::string name,std::vector<std::string> rlist){
    std::string ans ="000000";    //opcode
        //rd rs rt   10ge
    if(name=="add"||name=="addu"||name=="and"||name=="nor"||name=="or"||name=="slt"||name=="sltu"||name=="sub"||name=="subu"||name=="xor"){
        ans=ans + tobin(rmap[rlist[1]],5)+ tobin(rmap[rlist[2]],5)+ tobin(rmap[rlist[0]],5) + "00000"+ r_map[name];
         
    }
    //rs rt    4ge
    else if(name=="div"||name=="divu"||name=="mult"||name=="multu"){
        ans=ans + tobin(rmap[rlist[0]],5)+ tobin(rmap[rlist[1]],5)+ "00000"+ "00000"+ r_map[name];

    }
    //rd rs   1ge
    else if(name=="jalr"){
        ans=ans+ tobin(rmap[rlist[1]],5)+ "00000"+tobin(rmap[rlist[0]],5)+"00000"+ r_map[name];

    }
     //rs 3ge
    else if(name=="jr"||name=="mthi"||name=="mtlo"){
        ans=ans+ tobin(rmap[rlist[0]],5)+ "00000"+ "00000"+"00000"+ r_map[name];

    }
    //rd 2ge
    else if(name=="mfhi"||name=="mflo" ){
        ans=ans+ "00000"+"00000"+tobin(rmap[rlist[0]],5)+"00000"+r_map[name];

    }
    //rd rt sa 3ge
    else if(name=="sll"||name=="sra"||name=="srl"){
        ans=ans+ "00000"+tobin(rmap[rlist[1]],5)+tobin(rmap[rlist[0]],5)+tobin(rmap[rlist[2]],5)+r_map[name];

    }
    //rd rt rs 3ge
    else if(name=="sllv"||name=="srav"||name=="srlv"){
        ans=ans+ tobin(rmap[rlist[2]],5)+ tobin(rmap[rlist[1]],5)+tobin(rmap[rlist[0]],5)+"00000"+r_map[name];

    }
    else if(name=="syscall"){
        ans=ans+ "00000"+ "00000"+ "00000"+ "00000"+ "001100";
    }
    return ans;


}


std::string transI(std::string name,std::vector<std::string> rlist){
    std::string ans="";
    int leftp;
    int rightp;
    //rt rs im 7ge
    if(name=="addi"||name=="addiu"||name=="andi"||name=="ori"||name=="slti"||name=="sltiu"||name=="xori"){
        //leftp = argList[i].find('(');
        //rightp = argList[i].find(')');
        ans=i_map[name]+tobin(rmap[rlist[1]],5)+tobin(rmap[rlist[0]],5)+tobin(std::stoi(rlist[2]),16);
    }
    //rs rt label  2ge
    else if(name=="beq"||name=="bne"){
        ans=i_map[name]+tobin(rmap[rlist[0]],5)+tobin(rmap[rlist[1]],5)+cal_im(PC,labelMap[rlist[2]]);
    }
    //rs label  4ge
    else if(name=="bgez"||name=="bgtz"||name=="blez"||name=="bltz"){
        ans=i_map[name]+tobin(rmap[rlist[0]],5)+extendi_map[name]+cal_im(PC,labelMap[rlist[1]]);
    }
    //rt im(rs)  12ge
    else if(name=="lb"||name=="lbu"||name=="lh"||name=="lhu"||name=="lw"||name=="sb"||name=="sh"||name=="sw"||name=="lwl"||name=="lwr"||name=="swl"||name=="swr"){
        leftp = rlist[1].find('(');
        rightp = rlist[1].find(')');
        ans=i_map[name]+tobin(rmap[rlist[1].substr(leftp+1,rightp-leftp-1)],5)+tobin(rmap[rlist[0]],5)+tobin(std::stoi(rlist[1].substr(0,leftp)),16);

    }
    //rt im 1ge
    else if(name=="lui"){
        ans=i_map[name]+"00000"+tobin(rmap[rlist[0]],5)+tobin(std::stoi(rlist[1]),16);

    }
    return ans;

}



std::string transJ(std::string name,std::vector<std::string> rlist){
    std::string ans="";
    ans=j_map[name]+tobin(labelMap[rlist[0]]/4,26);   // /4 to turn bit to byte
    return ans;

}







std::string trans(std::string str){                  //deal with the instruction at first step
    std::stringstream temp;                   //to change the string object into stream to get seperate string
    std::string ins_name;
    std::string reg_name;
    temp.str(str);                             //use temp to save the input string
    temp >> ins_name;
    std::vector<std::string> reg_list;
    while(temp>>reg_name){
        if(reg_name.back()==','){
            reg_name=reg_name.substr(0,reg_name.length()-1);
        }
        reg_list.push_back(reg_name);
    }
    if(r_map.find(ins_name)!=r_map.end()){
        return transR(ins_name,reg_list);
    }
    if(i_map.find(ins_name)!=i_map.end()){
        return transI(ins_name,reg_list);
    }
    if(j_map.find(ins_name)!=j_map.end()){
        return transJ(ins_name,reg_list);
    }
    /*if(extendi_map.find(ins_name)){
        return transextendI(ins_name,reg_list);
    }*/

}




void LabelTable::pass2(std::string infilename,std::string outfilename) {
    std::ifstream input(infilename);
    std::ofstream output(outfilename);

    int instruction_Num = 0;
    std::string string3;
    std::string string4;
    std::string judge_ifdottext2 = ".text";
    while (getline(input, string3)) {
        if (deal_str(string3).substr(0, 5) == judge_ifdottext2) {
            break;
        }
    }

    while (getline(input, string3)) {
        string4 = deal_str(delete_maohao(string3));
        if(string4==""){continue;}
        if (string4.length()>0) {
            output << trans(string4) << std::endl;
            instruction_Num=instruction_Num+1;
            PC=PC+4;
        }
    }
}