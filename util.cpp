#include "util.h"
#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

namespace exploringBB {

int write(string path, string filename, string value){
  ofstream fs;
  fs.open((path + filename).c_str());
  if (!fs.is_open()) {
    cout << "GPIO: write failed to open file " << endl;
    return -1;
  }
  fs << value;
  fs.close();
  return 0;
}

string read(string path, string filename){
   ifstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
    cout << "GPIO: read failed to open file " << endl;
   }
   string input;
   getline(fs,input);
   fs.close();
   return input;
}

int write(string path, string filename, int value){
   stringstream s;
   s << value;
   return write(path,filename,s.str());
}

}
