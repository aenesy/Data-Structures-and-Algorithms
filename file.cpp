/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 15.01.2023 > */

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

void measureAndOutputTime(clock_t start, clock_t end, const string& operationType) {
    double elapsed_time = static_cast<double>(end - start) * 1000 / CLOCKS_PER_SEC;
    ofstream timeFile("time_log.txt", ios_base::app);
    timeFile << operationType << ":" << elapsed_time << " ms\n";
    timeFile.close();
}
 
void updateEmployee(fstream& employeeFile,int opEmployee,int opSalary,int opDepartment){
string line,employeeID,Salary,Department;        
 int currentPosition = 0;
 employeeFile.seekp(currentPosition, std::ios::beg);
 int intEmployee,intSalary,intDepartment;
    while (getline(employeeFile,line)) {
        istringstream iss(line);
       if (getline(iss, employeeID, ';') && getline(iss, Salary, ';') &&  getline(iss, Department)) {         
            istringstream(employeeID) >> intEmployee;
            istringstream(Salary) >> intSalary;
            istringstream(Department) >> intDepartment;
            
           
        int currentId;
        
        currentId = intEmployee;
        // add oluo currentPosition = employeeFile.tellg();
        if (currentId == opEmployee) {
          
            employeeFile.seekp(currentPosition, std::ios::beg);
            employeeFile << opEmployee << ';' << opSalary << ';' << opDepartment;
            employeeFile.flush();
            
            break;
        }


        currentPosition = employeeFile.tellg();
        
        if (employeeFile.eof()){
            cerr<< "eof"<<endl;
            break;
    }
      
    }
    }
};
void addEmployee(fstream& employeeFile,int lastEmployeeCreated, int opSalary,int opDepartment){
employeeFile.seekp(0,ios::end);
employeeFile<<lastEmployeeCreated<<";"<<opSalary<<";"<<opDepartment<<endl;


};
void deleteEmployee(fstream& employeeFile,int opEmployee,string fileName){
  ofstream tempFile("temporary.csv");
   
string line,employeeID,Salary,Department;        
 int currentPosition = 0;
 employeeFile.seekp(currentPosition, std::ios::beg);
 int intEmployee;
    while (getline(employeeFile,line)) {
        istringstream iss(line);
       if (getline(iss, employeeID, ';') && getline(iss, Salary, ';') &&  getline(iss, Department)) {         
            istringstream(employeeID) >> intEmployee ;        
        int currentId;      
        currentId = intEmployee;
        if (currentId != opEmployee) {
    
            
            tempFile << line<<"\n";
          
         
         
        }    
    }
    }
    employeeFile.close();
    tempFile.close();
    remove(fileName.c_str());
    rename("temporary.csv", fileName.c_str());
    employeeFile.open(fileName,ios::in | ios::out); 

  
};

int main(int argc, char** argv) {

    if (argc != 3) {
      
        return 0;
    }

    ifstream OperationsFile;
    OperationsFile.open(argv[2]);
    if (!OperationsFile.is_open()) {
      cerr<<"op file error";
        return 0;
    }


     fstream EmployeeFile;
    EmployeeFile.open(argv[1]);
    string fileName = argv[1];
   
    if (!EmployeeFile.is_open()){ 
       cerr<<"csv file error";
          return 0;
        }       
  int lastEmployeeCreated;
    string line; 
     while (getline(EmployeeFile, line)) {
    
        istringstream iss(line);    
        string employeeID, newSalary, newDepartment;
        int csvEmployee, csvSalary, csvDepartment;
        if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') &&  getline(iss, newDepartment)) {         
            istringstream(employeeID) >> csvEmployee;
            istringstream(newSalary) >> csvSalary;
            istringstream(newDepartment) >> csvDepartment;
        
           lastEmployeeCreated = csvEmployee;
    
        }
       }
		    EmployeeFile.close();
        EmployeeFile.open(argv[1]);

    





    string operation; 
     while (getline(OperationsFile, operation)) {
    
        istringstream iss(operation);    
        
        string command, employeeID, newSalary, newDepartment;
        getline(iss, command, ';');

       if(command == "UPDATE"){
        int opEmployee, opSalary, opDepartment;
                  clock_t start = clock();

        if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') &&  getline(iss, newDepartment)) {         
            istringstream(employeeID) >> opEmployee;
            istringstream(newSalary) >> opSalary;
            istringstream(newDepartment) >> opDepartment;
          updateEmployee(EmployeeFile,opEmployee,opSalary,opDepartment); 
        }
    clock_t end = clock();  
	measureAndOutputTime(start, end, "UPDATE");
       }
		
		else if(command == "ADD"){
            clock_t start = clock();

         int opSalary, opDepartment;
         if (getline(iss, newSalary, ';') &&  getline(iss, newDepartment)) {
          
            istringstream(newSalary) >> opSalary;
            istringstream(newDepartment) >> opDepartment;
             lastEmployeeCreated++;
            addEmployee(EmployeeFile,lastEmployeeCreated,opSalary,opDepartment);
             
        }
    clock_t end = clock();  
	measureAndOutputTime(start, end, "ADD");
       }
        
		else if(command == "DELETE"){
         int opEmployee;
         
          clock_t start = clock();
         if (getline(iss, employeeID, ';')) {
             istringstream(employeeID) >> opEmployee;
             deleteEmployee(EmployeeFile,opEmployee,fileName);
        }
    clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
      }
        
       }
      



}          