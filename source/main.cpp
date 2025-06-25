#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main(){
    ifstream InputFile("input.txt");
    ofstream OutputFile("output.txt");
    vector <vector<string>> Timetable;
    

    string line;

    if(!InputFile && !OutputFile){
        cout << "file not found!" << endl;

    
    } else {
        while (getline(InputFile,line)){
           if (line.find("MOV")!= string::npos){
               istringstream iss(line);
               int NumberofAddress;
               string position;
               string commands;
               string comma;
               
               
          
               iss >> commands >> NumberofAddress >> position;
               cout << "MOV 5 R1" << endl;
               OutputFile << "MOV 5 R1" << endl;

               cout << "This is " << commands << endl;
               OutputFile <<  "This is " << commands << endl;

               cout <<"This is " << NumberofAddress << endl;
               OutputFile <<"This is " << NumberofAddress << endl;

               cout <<"This is " << position << endl;
               OutputFile <<"This is " << position << endl;
           }
        }  
    }




    
} 
