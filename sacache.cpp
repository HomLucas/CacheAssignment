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

class Set
{
public:
  unsigned int arr[5];
  Set();
  int LRU();
  int LRUmax();
  Lines lines[5];
 
};
int Set::LRUmax()
{
  unsigned int high = arr[0];
  int index = 0;
  for(int i = 0; i<5; i++)
     if(arr[i] > high)
     {
       high = arr[i];
       index = i;
     }    

  return index;
}
int Set::LRU()
{
  unsigned int low = arr[0];
  int index=0;
  for(int i = 0; i < 5; i++)
    if(arr[i] < low)
    {
      low = arr[i];
      index = i;
    }
  return index; 
}

Set::Set()
{
  for(int i = 0; i<5; i++)
     arr[i] =0;
}
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
Set cache[8];


void writeline(int inLine, int set, int data, int offset)
{
   
    cache[set].lines[inLine].data[offset] = data;
    cache[set].lines[inLine].dirty = true;
    cache[set].arr[inLine]= cache[set].arr[cache[set].LRUmax()]+1;
    
}

void readline(int line, unsigned int set, int hit, unsigned int offset, ofstream &outFile){
    cache[set].arr[line] = cache[set].arr[cache[set].LRUmax()] + 1;
    outFile << hex << setw(2) << setfill('0') << uppercase << cache[set].lines[line].data[offset];
    outFile << ' ' << hit << ' ' << cache[set].lines[line].dirty << endl;
}

int main(int argc, char *argv[])
{
    
    ifstream inFile;
    ofstream outFile;
    
    inFile.open(argv[1]);
    outFile.open("sa-out.txt");
    
    unsigned int add, address, set, offset, tag, operation, data;
    int isHit=0, i=0, s;
    int check=0, inLine = 0;
    while(inFile >>hex >> add >> operation >> data)
    {
        address = add;
        
        add>>=5;
        tag = add;
        
        add = address;
        add<<=30;
        add>>=30;
        offset = add;
        
        add = address;
        add<<=27;
        add>>=29;
        set = add;
      
        for(s=0; s < 5; s++)
       	{
          if(cache[set].lines[s].tag == tag)
          {
             inLine = s;
             isHit  = 1;
             break;
          }//we got a hit
          else{
            isHit = 0;
          }//miss
        }
        //if tag not in cache
        check++;
        if(isHit == 0)
	{ 
          if(cache[set].lines[cache[set].LRU()].dirty == true)
          {
               for(i = 0; i < 4; i++)
                 memory[cache[set].lines[cache[set].LRU()].address + i] = cache[set].lines[cache[set].LRU()].data[i];
          }
          for(i = 0; i < 4; i++)
              cache[set].lines[cache[set].LRU()].data[i] = memory[(address-offset)+i]; 
          inLine = cache[set].LRU();
          cache[set].lines[cache[set].LRU()].tag = tag; 
	  cache[set].lines[cache[set].LRU()].address = (address - offset);               
          cache[set].lines[cache[set].LRU()].dirty = false; 
          cache[set].arr[cache[set].LRU()] = cache[set].arr[cache[set].LRUmax()] +1; 
        }//if miss
        //if write to byte
        for(s=0; s <5; s++)
          if(cache[set].lines[s].tag == tag)
          { inLine = s;
   
    
          }
        if(operation == 255)    
          writeline(inLine, set, data, offset);
   
        //else read from cache
        else 
           readline(inLine, set, isHit, offset, outFile);
        
    }
    inFile.close();
    outFile.close();
    return 0;
    
}



