//Jagdeep Singh and Lucas Hom
#include<iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include<bitset>
#include<iomanip>
using namespace std;


class Lines
{
public:
    Lines();
    unsigned int address;
    unsigned int tag;
    unsigned int data[4];
    bool dirty;
};

Lines::Lines()
{
    address = 0;
    tag = -1;
    for(int i = 0; i < 4; i++)
        data[i] = 0;
    dirty = false;
}
//mem and cache declared them global so i dont need to pass htem in each time lazy
int memory[65536];
Lines cache[64];


void writeline(int line, int data, int offset)
{
   
    cache[line].data[offset] = data;
    cache[line].dirty = true;
    
}

void readline(int line, int hit, int offset, ofstream &outFile){
    outFile << hex << setw(2) << setfill('0') << uppercase << cache[line].data[offset];
    outFile << ' ' << hit << ' ' << cache[line].dirty << endl;
}

int main(int argc, char *argv[])
{
    
    ifstream inFile;
    ofstream outFile;
    
    inFile.open(argv[1]);
    outFile.open("dm-out.txt");
    
    unsigned int add, address, line, offset, tag, operation, data;
    int isHit, i;
    int check=0;
    while(inFile >>hex >> add >> operation >> data)
    {
        address = add;
        
        add>>=8;
        tag = add;
        
        add = address;
        add<<=30;
        add>>=30;
        offset = add;
        
        add = address;
        add<<=24;
        add>>=26;
        line = add;
      
	if(cache[line].tag == tag)
        {
            isHit = 1;
        }//we got a hit
        else{
            isHit = 0;
        }//miss
        //if tag not in cache
        check++;
        if(isHit == 0)
	{ 
          if(cache[line].dirty == true)
          {
               for(i = 0; i < 4; i++)
                 memory[cache[line].address + i] = cache[line].data[i];
          }
          for(i = 0; i < 4; i++)
              cache[line].data[i] = memory[(address-offset)+i]; 
          cache[line].tag = tag; 
	  cache[line].address = (address - offset);               
          cache[line].dirty = false; 
            
        }//if miss
        //if write to byte
        if(operation == 255)    
            writeline(line, data, offset);
   
        //else read from cache
        else 
           readline(line, isHit, offset, outFile);
        
    }
    inFile.close();
    outFile.close();
    return 0;
    
}



