#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<string>
#include<sstream>
#include"functions.h"

using namespace std;

map<pair<string,string>,int> allRegisters;
map<pair<int,string>,string> virtualMemory;
int programCounter;

void checkMemory()
{
    auto it = virtualMemory.begin();
    while(it!=virtualMemory.end())
    {
        cout<<"Address: "<<it->first.first <<" label: "<<it->first.second <<" instruction/data: "<< it->second<<endl;
        it++;
    }
}

void checkReg()
{
    auto it = allRegisters.begin();
    while(it!=allRegisters.end())
    {
        cout<< it->first.first << " "<< it->first.second<<" " <<" "<<it->second<<endl;
        it++;
    }
}

void parser(string line, bool *flag, string *label)
{
    string temp;
    char space = ' ';
    for(int i=0; i<line.size(); i++) //loop to remove any commas from the instruction
    {
        if(line[i]==',')
        {
            line[i] = space;
        }
    }
    istringstream split(line);
    while(split>>temp) //add all cases here, depending on first word
    {
        if(temp == "add")
        {
            add(line,&allRegisters);
        }
        else if (temp == "sub")
        {
            sub(line,&allRegisters);
        }
        else if(temp == "xor")
        {
            xoring(line,&allRegisters);
        }
        else if(temp == "or")
        {
            oring(line,&allRegisters);
        }
        else if(temp == "and")
        {
            anding(line,&allRegisters);
        }
        else if(temp == "sll")
        {
            sll(line,&allRegisters);
        }
        else if(temp == "srl")
        {
            srl(line,&allRegisters);
        }
        else if(temp == "sra")
        {
            sra(line,&allRegisters);
        }
        else if(temp == "slt")
        {
            slt(line,&allRegisters);
        }
        else if(temp == "sltu")
        {
            sltu(line,&allRegisters);
        }
        else if(temp == "addi" /*&& !temp.find("sp")*/)
        {
            addi(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "xori")
        {
            xori(line,&allRegisters);
        }
        else if(temp == "ori")
        {
            ori(line,&allRegisters);
        }
        else if(temp == "andi")
        {
            andi(line,&allRegisters);
        }
        else if(temp == "slli")
        {
            slli(line,&allRegisters);
        }
        else if(temp == "srli")
        {
            srli(line,&allRegisters);
        }
        else if(temp == "srai")
        {
            srai(line,&allRegisters);
        }
        else if(temp == "slti")
        {
            slti(line,&allRegisters);
        }
        else if(temp == "sltui")
        {
            sltui(line,&allRegisters);
        }
        else if(temp == "bge")
        {
            *flag = bge(line, label, &allRegisters);
        }
        else if(temp == "bne")
        {
            *flag = bne(line, label, &allRegisters);
        }
        else if(temp == "beq")
        {
            *flag = beq(line, label, &allRegisters);
        }
        else if(temp == "blt")
        {
            *flag = blt(line, label, &allRegisters);
        }
        else if(temp == "bltu")
        {
            *flag = bltu(line, label, &allRegisters);
        }
        else if(temp == "bgeu")
        {
            *flag = bgeu(line, label, &allRegisters);
        }
        else if(temp == "sw")
        {
            sw(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "sh")
        {
            sh(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "sb")
        {
            sb(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "lw")
        {
            lw(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "lh")
        {
            lh(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "lb")
        {
            lb(line,&allRegisters, &virtualMemory);
        }
        else if(temp == "lui")
        {
            lui(line,&allRegisters);
        }
        else if(temp == "auipc")
        {
            auipc(line,&allRegisters, programCounter+1);
        }
        else if (temp == "jal")
        {
           *flag = jal(line, &allRegisters, label, programCounter);
        }
        else if(temp == "jalr")
        {
            checkMemory();
            checkReg();
            jalr(line,&allRegisters, programCounter);
        }
    }
}

void readInstructions()
{
    bool branch;
    string label="NONE";
    string labelReturned;
    auto it = virtualMemory.begin();
    programCounter = it->first.first + 1;
    while(it!=virtualMemory.end())
    {
        parser(it->second, &branch, &label);
        if(label!="None")
        {
            label+=":";
        }
        if(branch)
        {
            auto it2=virtualMemory.begin();
            while(it2!=virtualMemory.end())
            {
                if(it2->first.second == label)
                {
                    programCounter = it2->first.first; //changes the value of programCounter
                    programCounter++;
                    it = it2; //jumps to label here
                    parser(it->second, &branch, &label);
                }
                it2++;
            }
            it++;
            branch=false;
        }
        else
        {
            programCounter = programCounter + 1;
            //cout<<programCounter<<endl;
            it++;
        }
    }
}

void storeInstructions(int start_address)
{
    int counter = 0;
    string temp;
    string currLine;
    ifstream reader;
    reader.open("instructions.txt");
    while(getline(reader,currLine))
    {
        stringstream ss(currLine);
        while(ss>>temp)
        {
            if(temp[temp.size()-1] == ':')
            {
                currLine.erase(0, temp.size()+1);
                virtualMemory.insert({{counter,temp},currLine});
                counter++;
                break;
            }
            else
            {
                virtualMemory.insert({{counter,"NONE"},currLine});
                counter++;
                break;
            }
        }
    }
    reader.close();
}

void setData(string reg, int val)
{
    auto it = allRegisters.begin();
    while(it!=allRegisters.end())
    {
        if(it->first.first == reg || it->first.second == reg)
        {
            it->second = val;
        }
        it++;
    }
}

void readInputs()
{
    string reg;
    string value;
    int val;
    ifstream reader;
    reader.open("inputs.txt");
    while(!reader.eof())
    {
        reader >> reg;
        reader >> value;
        if(reg == "x0" || reg == "zero") //condition to make sure that register x0 stays always 0
            value="0";
        val = stoi(value);
        setData(reg,val);
    }
    reader.close();
}

void initializeRegisters()
{
    string temp1;
    string temp2;
    ifstream reader;
    reader.open("registers.txt");
    while(!reader.eof())
    {
        reader>>temp1;
        reader>>temp2;
        allRegisters.insert({{temp1,temp2},NULL});
    }
    reader.close();
}

int main()
{
    string address;
    ifstream read;
    read.open("start address.txt");
    read>>address;
    read.close();
    int startAddress = stoi(address);
    initializeRegisters(); //create registers and all equivalent to 0
    readInputs(); //read all inputs provided by user in "inputs.txt"
    storeInstructions(startAddress); //read all instructions to store them in memory from "instructions.txt"
    readInstructions(); //read instructions from memory
    checkMemory();
    checkReg();
}