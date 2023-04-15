#ifndef FUNCTIONS
#define FUNCTIONS
#include<iostream>
#include<string>
#include<sstream>
#include<vector>
#include<bitset>
#include<map>
#include<cmath>

using namespace std;


string decimalToBinary(int number)
{
    string binary = bitset<32>(number).to_string();
    return binary;
}

string decimalToBinary16Bits(int number)
{
    string binary = bitset<16>(number).to_string();
    return binary;
}

string decimalToBinary8Bits(int number)
{
    string binary = bitset<8>(number).to_string();
    return binary;
}

int binaryToDecimalUsingString(string text)
{
    int i,N,decimal_number=0,p=0;
    N=text.size(); // storing the size of string in N
    for(i=N-1;i>=0;i--) // starting the loop from LSB
    {
        if(text[i]=='1') // Checking if string element is 1
        {
            decimal_number+=pow(2,p); // if yes, adding the pth power of 2 to the decimal_number
        }
        p++; // incrementing the value of p as we move towards MSB
    }
    return decimal_number;
}

string decimalToBinary20Bits(int num)
{
    string binary = bitset<20>(num).to_string();
    return binary;
}

int binaryToDecimal(int number)
{
    int num = number;
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    int base = 1;
 
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
 
        dec_value += last_digit * base;
 
        base = base * 2;
    }
 
    return dec_value;
}

void add(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int sum=0;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'add' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                sum+=it->second;
            }
            it++;
        }
        it = reg->begin();
    }
    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = sum;
        }
        it++;
    }
}

void addi(string line, map<pair<string,string>,int> *reg, map<pair<int,string>,string> *memory)
{
    int originalSize;
    int imm;
    string rd;
    int sum=0;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    if(line.find("sp") != std::string::npos)
    {
       while(splitter>>temp) //read word by word
        {
            words.push_back(temp);
        }
        words.erase(words.begin()); //remove the word 'addi' from the vector

        originalSize = stoi(words[2]);
        if(originalSize<0)
        {
                originalSize = originalSize/-4;
                int lastLine = memory->end()->first.first;
                lastLine--;
                auto it = memory->begin();
                for(int i=0; i<originalSize; i++)
                {
                    while(it!=memory->end())
                    {
                        if(it->first.first == lastLine)
                        {
                            lastLine++;
                            memory->insert({{lastLine,"sp"},""});
                            break;
                        }
                        it++;
                    }
                    it=memory->begin();
            }
        }
        else if(originalSize>0)
        {
            originalSize = originalSize/4;
            auto it = memory->begin();
            for(int i=0; i<originalSize; i++)
            {
                while(it!=memory->end())
                {
                    if(it->first.second == "sp")
                    {
                        memory->erase(it);
                        it++;
                        it++;
                    }
                    it++;
                }
                it=memory->begin();
            }
        }
    }
    else
    {
        while(splitter>>temp) //read word by word
        {
            words.push_back(temp);
        }
        words.erase(words.begin()); //remove the word 'addi' from the vector
        rd = words[0];
        if(rd=="x0")
        {
            cout<<"cannot edit x0"<<endl;
            return;
        }
        words.erase(words.begin()); //remove rd
        imm = stoi(words[1]);
        auto it = reg->begin();
        while(it!=reg->end())
        {
            if(it->first.first == words[0] || it->first.second == words[0])
            {
                sum+=it->second;
            }
            it++;
        }
        sum += imm;
        it = reg->begin();
        while(it!=reg->end())
        {
            if(it->first.first == rd || it->first.second == rd)
            {
                it->second = sum;
            }
            it++;
        }
    }
}

void sub(string line, map<pair<string,string>, int> *reg)
{
    string rd;
    int diff;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin()); //remove rd from line
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }
    diff = values[0] - values[1];
    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = diff;
        }
        it++;
    }
}

void xoring(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
     auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }

    src1_int = values[0];
    src1_str = decimalToBinary(src1_int);
    src2_int = values[1];
    src2_str = decimalToBinary(src2_int);

    for(int i=0; i<src1_str.size();i++)
    {
        if(src1_str[i] == '1' && src2_str[i] == '0' || src1_str[i] == '0' && src2_str[i] == '1' )
        val_str[i] = '1';
        else
        val_str[i] = '0';
    }

    val_int = stoi(val_str);
    int result;
    result = binaryToDecimal(val_int);

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = result;
        }
        it++;
    }
}

void xori(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
        it++;
    }

    src1_int = values[0];
    src1_str = decimalToBinary(src1_int);
    imm = stoi(words[1]);
    cout<<imm<<endl;
    imm_str = decimalToBinary(imm);
    cout<<imm_str<<endl;

    for(int i=0; i<src1_str.size();i++)
    {
        if(src1_str[i] == '1' && imm_str[i] == '0' || src1_str[i] == '0' && imm_str[i] == '1' )
        val_str[i] = '1';
        else
        val_str[i] = '0';
    }

    val_int = stoi(val_str);
    int result;
    result = binaryToDecimal(val_int);

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = result;
        }
        it++;
    }
}

void oring(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
     auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }

    src1_int = values[0];
    src1_str = decimalToBinary(src1_int);
    src2_int = values[1];
    src2_str = decimalToBinary(src2_int);

    for(int i=0; i<src1_str.size();i++)
    {
        if(src1_str[i] == '0' && src2_str[i] == '0')
        val_str[i] = '0';
        else
        val_str[i] = '1';
    }

    val_int = stoi(val_str);
    int result;
    result = binaryToDecimal(val_int);

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = result;
        }
        it++;
    } 
}

void ori(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
    auto it = reg->begin();
   
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    
    src1_int = values[0];
    src1_str = decimalToBinary(src1_int);
    imm = stoi(words[1]);
    imm_str = decimalToBinary(imm);

    for(int i=0; i<src1_str.size();i++)
    {
        if(src1_str[i] == '0' && imm_str[i] == '0')
        val_str[i] = '0';
        else
        val_str[i] = '1';
    }

    val_int = stoi(val_str);
    int result;
    result = binaryToDecimal(val_int);

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = result;
        }
        it++;
    } 
}

void anding(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
     auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }

    src1_int = values[0];
    src1_str = decimalToBinary(src1_int);
    src2_int = values[1];
    src2_str = decimalToBinary(src2_int);

    for(int i=0; i<src1_str.size();i++)
    {
        if(src1_str[i] == '1' && src2_str[i] == '1')
        val_str[i] = '1';
        else
        val_str[i] = '0';
    }

    val_int = stoi(val_str);
    int result;
    result = binaryToDecimal(val_int);

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = result;
        }
        it++;
    } 
}

void andi(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    if(rd=="x0" || rd == "zero")
    {
        cout<<"cannot edit x0"<<endl;
        return;
    }
    words.erase(words.begin());
    auto it = reg->begin();
  
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }

    src1_int = values[0];
    src1_str = decimalToBinary(src1_int);
    imm = stoi(words[1]);
    imm_str = decimalToBinary(imm);

    for(int i=0; i<src1_str.size();i++)
    {
        if(src1_str[i] == '1' && imm_str[i] == '1')
        val_str[i] = '1';
        else
        val_str[i] = '0';
    }

    val_int = stoi(val_str);
    int result;
    result = binaryToDecimal(val_int);

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = result;
        }
        it++;
    } 
}

void sll(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }
    src1_int = values[0];
    src2_int = values[1];

    src1_int = src1_int << src2_int;

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}

void slli(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    src1_int = values[0];
    imm = stoi(words[1]);

    src1_int = src1_int << imm;

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}

void srl(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }
    src1_int = values[0];
    src2_int = values[1];

    src1_int = src1_int >> src2_int;

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}

void srli(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    
    src1_int = values[0];
    imm = stoi(words[1]);

    src1_int = src1_int >> imm;

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}

void sra(string line, map<pair<string,string>,int> *reg)
{
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }
    src1_int = values[0];
    src2_int = values[1];

    //src1_int = src1_int >>= src2_int;
    src1_int = (src1_int >> src2_int)|(src1_int << (32 - src2_int));

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}

void srai(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    string rd;
    int val_int;
    string val_str = "00000000000000000000000000000000";
    string src1_str;
    int src1_int;
    string src2_str;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();

    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    src1_int = values[0];
    imm = stoi(words[1]);

    src1_int = (src1_int >> imm)|(src1_int << (32 - imm));

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}

void slt(string line, map<pair<string,string>,int> *reg)
{
    int finalResult;
    string rd;
    int src1_int;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }
    src1_int = values[0];
    src2_int = values[1];

    if(src1_int < src2_int)
    {
        finalResult=1;
    }
    else
    {
        finalResult=0;
    }

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = finalResult;
        }
        it++;
    } 
}

void slti(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string imm_str;
    int finalResult;
    string rd;
    int src1_int;
    int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();

    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    src1_int = values[0];
    imm = stoi(words[1]);

    if(src1_int < imm)
    {
        finalResult=1;
    }
    else
    {
        finalResult=0;
    }

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = finalResult;
        }
        it++;
    } 
}

void sltu(string line, map<pair<string,string>,int> *reg)
{
    int finalResult;
    string rd;
    unsigned int src1_int;
    unsigned int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
            }
            it++;
        }
        it = reg->begin();
    }
    src1_int = values[0];
    src2_int = values[1];
    

    if(src1_int < src2_int)
    {
        finalResult=1;
    }
    else
    {
        finalResult=0;
    }

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = finalResult;
        }
        it++;
    } 
}

void sltui(string line, map<pair<string,string>,int> *reg)
{
    unsigned int imm;
    string imm_str;
    int finalResult;
    string rd;
    unsigned int src1_int;
    unsigned int src2_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'sub' from the vector
    rd = words[0];
    words.erase(words.begin());
    auto it = reg->begin();

    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }

    src1_int = values[0];
    imm = stoi(words[1]);
    

    if(src1_int < imm)
    {
        finalResult=1;
    }
    else
    {
        finalResult=0;
    }

    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = finalResult;
        }
        it++;
    } 
}

bool bge(string line, string *labelToSend,  map<pair<string,string>,int> *reg)
{
    vector<string> words;
    int counter=0;
    vector<int> values;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'bge' from the vector
    *labelToSend = words[2]; //label of line to be executed
    words.erase(words.end()); //remove label from words
    words.resize(words.size());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
                counter++;
            }
            it++;
        }
        it = reg->begin();
    }
    if(values[0]>=values[1]) //comparison is done here and it returns result
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool bne(string line, string *labelToSend,  map<pair<string,string>,int> *reg)
{
    vector<string> words;
    int counter=0;
    vector<int> values;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'bge' from the vector
    *labelToSend = words[2]; //label of line to be executed
    words.erase(words.end()); //remove label from words
    words.resize(words.size());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
                counter++;
            }
            it++;
        }
        it = reg->begin();
    }
    if(values[0]!=values[1]) //comparison is done here and it returns result
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool beq(string line, string *labelToSend,  map<pair<string,string>,int> *reg)
{
    vector<string> words;
    int counter=0;
    vector<int> values;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'bge' from the vector
    *labelToSend = words[2]; //label of line to be executed
    words.erase(words.end()); //remove label from words
    words.resize(words.size());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
                counter++;
            }
            it++;
        }
        it = reg->begin();
    }
    if(values[0]==values[1]) //comparison is done here and it returns result
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool blt(string line, string *labelToSend,  map<pair<string,string>,int> *reg)
{
    vector<string> words;
    int counter=0;
    vector<int> values;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'bge' from the vector
    *labelToSend = words[2]; //label of line to be executed
    words.erase(words.end()); //remove label from words
    words.resize(words.size());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
                counter++;
            }
            it++;
        }
        it = reg->begin();
    }
    if(values[0]<values[1]) //comparison is done here and it returns result
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool bltu(string line, string *labelToSend,  map<pair<string,string>,int> *reg)
{
    vector<string> words;
    int counter=0;
    vector<int> values;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'bge' from the vector
    *labelToSend = words[2]; //label of line to be executed
    words.erase(words.end()); //remove label from words
    words.resize(words.size());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
                counter++;
            }
            it++;
        }
        it = reg->begin();
    }
    unsigned int num1Unsigned = values[0];
    unsigned int num2Unsigned = values[1];
    values[0] = num1Unsigned;
    values[1] = num2Unsigned;
    if(values[0]<values[1]) //comparison is done here and it returns result
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool bgeu(string line, string *labelToSend,  map<pair<string,string>,int> *reg)
{
    vector<string> words;
    int counter=0;
    vector<int> values;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove the word 'bge' from the vector
    *labelToSend = words[2]; //label of line to be executed
    words.erase(words.end()); //remove label from words
    words.resize(words.size());
    auto it = reg->begin();
    for(int i=0; i<words.size(); i++)
    {
        while(it!=reg->end())
        {
            if(it->first.first == words[i] || it->first.second == words[i])
            {
                values.push_back(it->second);
                counter++;
            }
            it++;
        }
        it = reg->begin();
    }
    unsigned int num1Unsigned = values[0];
    unsigned int num2Unsigned = values[1];
    values[0] = num1Unsigned;
    values[1] = num2Unsigned;
    if(values[0]>=values[1]) //comparison is done here and it returns result
    {
        return true;
    }
    else
    {
        return false;
    }
}

void sw(string line, map<pair<string,string>,int> *reg, map<pair<int,string>, string> *memory)
{
    int counter = 0;
    char space = ' ';
    string source;
    string dest;
    int offset;
    vector<string> words;
    vector<int> values;
    string temp;
    for(int i=0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove 'sw' word
    if(words.size() == 3)
    {
        source = words[0];
        offset = stoi(words[1]);
        dest = words[2];
        offset = (offset/4)+1;
        auto itReg = reg->begin();
        while(itReg != reg->end())
        {
            if(itReg->first.first == source || itReg->first.second == source)
            {
                break;
            }
            else
            {
                itReg++;
            }
        }
        string word32bit = decimalToBinary(itReg->second);
        auto itMem = memory->begin();
        while(itMem!=memory->end())
        {
            if(itMem->first.second == "sp")
            {
                counter++;
            }
            if(counter == offset)
            {
                itMem->second = word32bit; 
            }
            itMem++;
        }
    }
}

void sh(string line, map<pair<string,string>,int> *reg, map<pair<int,string>, string> *memory)
{
    int counter = 0;
    char space = ' ';
    string source;
    string dest;
    int offset;
    vector<string> words;
    vector<int> values;
    string temp;
    for(int i=0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove 'sw' word
    if(words.size() == 3)
    {
        source = words[0];
        offset = stoi(words[1]);
        dest = words[2];
        offset = (offset/4)+1;
        auto itReg = reg->begin();
        while(itReg != reg->end())
        {
            if(itReg->first.first == source || itReg->first.second == source)
            {
                break;
            }
            else
            {
                itReg++;
            }
        }
        string word32bit = decimalToBinary16Bits(itReg->second);
        auto itMem = memory->begin();
        while(itMem!=memory->end())
        {
            if(itMem->first.second == "sp")
            {
                counter++;
            }
            if(counter == offset)
            {
                itMem->second = word32bit; 
            }
            itMem++;
        }
    }
}

void sb(string line, map<pair<string,string>,int> *reg, map<pair<int,string>, string> *memory)
{
    int counter = 0;
    char space = ' ';
    string source;
    string dest;
    int offset;
    vector<string> words;
    vector<int> values;
    string temp;
    for(int i=0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove 'sw' word
    if(words.size() == 3)
    {
        source = words[0];
        offset = stoi(words[1]);
        dest = words[2];
        offset = (offset/4)+1;
        auto itReg = reg->begin();
        while(itReg != reg->end())
        {
            if(itReg->first.first == source || itReg->first.second == source)
            {
                break;
            }
            else
            {
                itReg++;
            }
        }
        string word32bit = decimalToBinary8Bits(itReg->second);
        auto itMem = memory->begin();
        while(itMem!=memory->end())
        {
            if(itMem->first.second == "sp")
            {
                counter++;
            }
            if(counter == offset)
            {
                itMem->second = word32bit; 
            }
            itMem++;
        }
    }
}

void lw(string line, map<pair<string,string>,int> *reg, map<pair<int,string>, string> *memory)
{
    int val = 0;
    int counter = 0;
    char space = ' ';
    string source;
    string dest;
    int offset;
    vector<string> words;
    string temp;
    for(int i=0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove 'lw' word
    if(words.size() == 3)
    {
        int counter = 0;
        cout<<offset<<endl;
        offset = binaryToDecimalUsingString(words[0]);
        cout<<offset<<endl;
        source = words[1];
        dest = words[2];
        if(offset == 0)
        {
            offset = (offset/4)+1;
        }
        else
        {
            offset = (offset/4);
        }
        auto itMem = memory->begin();
        while(itMem!=memory->end())
        {
            if(itMem->first.second == "sp")
            {
                counter++;
            }
            if(counter == offset)
            {
                break;
            }
            else
            {
                itMem++;
            }
        }
        auto itReg = reg->begin();
        string wordSize;
        while(itReg != reg->end())
        {
            if(itReg->first.first == dest || itReg->first.second == dest)
            {
               
                cout<<itMem->second<<endl;
                try{
                val = stoi(itMem->second);
                }
                catch (std::out_of_range const &e) 
                {
                cout << "Integer overflow: std::out_of_range thrown" <<endl;
                }
                int num = binaryToDecimal(val);
                wordSize = decimalToBinary(num);
                itReg->second = stoi(wordSize);
            }
            itReg++;
        }
    }
}

void lh(string line, map<pair<string,string>,int> *reg, map<pair<int,string>, string> *memory)
{
    int val;
    int counter = 0;
    char space = ' ';
    string source;
    string dest;
    int offset;
    vector<string> words;
    string temp;
    for(int i=0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove 'sw' word
    if(words.size() == 3)
    {
        int counter = 0;
        offset = stoi(words[0]);
        source = words[1];
        dest = words[2];
        if(offset == 0)
        {
            offset = (offset/4)+1;
        }
        else
        {
            offset = (offset/4);
        }
        auto itMem = memory->begin();
        while(itMem!=memory->end())
        {
            if(itMem->first.second == "sp")
            {
                counter++;
            }
            if(counter == offset)
            {
                break;
            }
            else
            {
                itMem++;
            }
        }
        auto itReg = reg->begin();
        string wordSize;
        while(itReg != reg->end())
        {
            if(itReg->first.first == dest || itReg->first.second == dest)
            {
                val = binaryToDecimalUsingString(itMem->second);
                int num = binaryToDecimal(val);
                wordSize = decimalToBinary16Bits(num);
                itReg->second = stoi(wordSize);
            }
            itReg++;
        }
    }
}

void lb(string line, map<pair<string,string>,int> *reg, map<pair<int,string>, string> *memory)
{
    int val;
    int counter = 0;
    char space = ' ';
    string source;
    string dest;
    int offset;
    vector<string> words;
    string temp;
    for(int i=0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove 'sw' word
    if(words.size() == 3)
    {
        int counter = 0;
        offset = stoi(words[0]);
        source = words[1];
        dest = words[2];
        if(offset == 0)
        {
            offset = (offset/4)+1;
        }
        else
        {
            offset = (offset/4);
        }
        auto itMem = memory->begin();
        while(itMem!=memory->end())
        {
            if(itMem->first.second == "sp")
            {
                counter++;
            }
            if(counter == offset)
            {
                break;
            }
            else
            {
                itMem++;
            }
        }
        auto itReg = reg->begin();
        string wordSize;
        while(itReg != reg->end())
        {
            if(itReg->first.first == dest || itReg->first.second == dest)
            {
                val = binaryToDecimalUsingString(itMem->second);
                int num = binaryToDecimal(val);
                wordSize = decimalToBinary8Bits(num);
                itReg->second = stoi(wordSize);
            }
            itReg++;
        }
    }
}

void lui(string line, map<pair<string,string>,int> *reg)
{
    int imm;
    string rd;
    string src1_str;
    int src1_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin());
    rd = words[0];
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    src1_int = values[0];
    imm = stoi(words[1]);
    imm = imm<<12;
    src1_str = decimalToBinary20Bits(imm) + "000000000000";
    src1_int = binaryToDecimalUsingString(src1_str);
    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
}
 void auipc(string line, map<pair<string,string>,int> *reg, int pc)
 {
    int imm;
    string rd;
    string src1_str;
    int src1_int;
    vector<int> values;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin());
    rd = words[0];
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == words[0] || it->first.second == words[0])
        {
            values.push_back(it->second);
        }
            it++;
    }
    src1_int = values[0];
    imm = stoi(words[1]);
    imm = imm<<12;
    src1_str = decimalToBinary20Bits(imm) + "000000000000";
    src1_int = binaryToDecimalUsingString(src1_str);
    src1_int = src1_int + pc;
    it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = src1_int;
        }
        it++;
    } 
 }


bool jal(string line,  map<pair<string,string>,int> *reg, string *label, int pc)
{
    string ra;
    vector<string> words;
    string temp;
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove jal 
    ra = words[0];
    *label = words[1];
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == ra || it->first.second == ra)
        {
            it->second = pc;
        }
        it++;
    }
    return true;
}

void jalr(string line, map<pair<string,string>,int> *reg, int pc)
{
    int addressRa;
    char space = ' ';
    string  rd;
    string ra;
    int offset;
    vector<string> words;
    string temp;
    for(int i = 0; i<line.size(); i++)
    {
        if(line[i] == '(' || line[i] == ')')
        {
            line[i] = space;
        }
    }
    istringstream splitter(line); //split by space
    while(splitter>>temp) //read word by word
    {
        words.push_back(temp);
    }
    words.erase(words.begin()); //remove jalr
    rd = words[0];
    offset = stoi(words[1]);
    offset = offset/4;
    offset = offset+pc;
    ra = words[2];
    if(rd == "x0")
    {
        exit(0);
    }
    auto it = reg->begin();
    while(it!=reg->end())
    {
        if(it->first.first == rd || it->first.second == rd)
        {
            it->second = offset;
        }
        it++;
    }
}

#endif