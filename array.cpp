/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 10.11.2023 > */

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


class EmployeeClass {
private:
    int Employee;
    int Salary;
    int Department;

public:
    EmployeeClass() : Employee(0), Salary(0), Department(0) {} // initializing default value

    EmployeeClass(int Employee, int Salary, int Department) : Employee(Employee), Salary(Salary), Department(Department) {}

    // getters and setters
    int getEmployee() {
        return Employee;
    }

    int getDepartment() {
        return Department;
    }

    int getSalary() {
        return Salary;
    }

    void setEmployee(int Employee) {
        this->Employee = Employee;
    }

    void setSalary(int Salary) {
        this->Salary = Salary;
    }

    void setDepartment(int Department) {
        this->Department = Department;
    }
};



void deleteEmployee(EmployeeClass*& Employees, int& Employeecount, int employeeID) {
    int foundIndex = -1;
   
    for (int i = 0; i < Employeecount; i++) {
        if (Employees[i].getEmployee() == employeeID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        for (int i = foundIndex; i < Employeecount - 1; i++) {
            Employees[i] = Employees[i + 1];
               
 
        }
      Employeecount--;
                
    }else{
        cout << "ERROR: An invalid ID to delete";
    }
   
}



void addEmployee(EmployeeClass*& Employees, int& Employeecount, int lastEmployeeCreated, int Salary, int Department) {
   EmployeeClass* temp = new EmployeeClass[Employeecount + 1];
 
    for (int i = 0; i < Employeecount; i++) {
        temp[i] = Employees[i];
    }
    
    temp[Employeecount] = EmployeeClass(lastEmployeeCreated, Salary, Department);
    
    delete[] Employees;
   
    Employees = temp;
       
       Employeecount++; 
    
  
}


void updateEmployee(EmployeeClass* Employees, int employeeID, int newSalary, int newDepartment, int opEmployeeCount) {
    int idCheck=0;
    for (int i = 0; i < opEmployeeCount; i++) {
        if (Employees[i].getEmployee() == employeeID) {
            Employees[i].setSalary(newSalary);
            Employees[i].setDepartment(newDepartment);
            idCheck = 1;
            break;
        }
    }if(!idCheck){
      cout << "ERROR: An invalid ID to update";
    }
}






int main(int argc, char** argv) {
   
    if (argc != 3) {
      
        return 0;
    }
   


    int Employeecount = 0;
    EmployeeClass* Employees = nullptr;  
   
  
    ifstream OperationsFile;
    OperationsFile.open(argv[2]);
    if (!OperationsFile.is_open()) {
      
        if (Employees) {
            delete[] Employees;
        }
        return 0;
    }
   
    ifstream EmployeeFile;
    EmployeeFile.open(argv[1]);
    if (!EmployeeFile.is_open()){ 
  
        if (Employees) {
            delete[] Employees;  
        }
        return 0;
    }
	
	
	
	
      int maxEmployees = 500001; 
    Employees = new EmployeeClass[maxEmployees];

    string line;
    int lastEmployeeCreated = 0;
    while (getline(EmployeeFile, line)) {
        istringstream iss(line);
        string EmployeeStr, SalaryStr, DepartmentStr;
        
        if (getline(iss, EmployeeStr, ';') && getline(iss, SalaryStr, ';') && getline(iss, DepartmentStr)) {
            int Employee, Salary, Department;
            // an alternative for stoi
            istringstream(EmployeeStr) >> Employee;
            istringstream(SalaryStr) >> Salary;
            istringstream(DepartmentStr) >> Department;
           lastEmployeeCreated++;
          if (Employeecount < maxEmployees) {
                Employees[Employeecount] = EmployeeClass(Employee, Salary, Department);
                Employeecount++;
              } else {
                break;  
               }
         } 
    }
   
   
   
   
    string operation; 
     while (getline(OperationsFile, operation)) {
    
        istringstream iss(operation);    
        
        string command, employeeID, newSalary, newDepartment;
        getline(iss, command, ';');
      
       if(command == "UPDATE"){
         clock_t start = clock();
        int opEmployee, opSalary, opDepartment;
        
        if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') &&  getline(iss, newDepartment)) {         
            istringstream(employeeID) >> opEmployee;
            istringstream(newSalary) >> opSalary;
            istringstream(newDepartment) >> opDepartment;
          updateEmployee(Employees,opEmployee,opSalary,opDepartment,Employeecount); 
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
             addEmployee(Employees, Employeecount, lastEmployeeCreated, opSalary, opDepartment);
              lastEmployeeCreated++;
        }
           clock_t end = clock();
             measureAndOutputTime(start, end, "ADD");
       }
        
		else if(command == "DELETE"){
         int opEmployee;
                     clock_t start = clock();

         if(Employeecount>1){
         if (getline(iss, employeeID, ';')) {
             istringstream(employeeID) >> opEmployee;
             deleteEmployee(Employees, Employeecount, opEmployee);
             
        }
         clock_t end = clock();
             measureAndOutputTime(start, end, "DELETE");
        }
        else{cout << "ERROR: There is no Employee";}
      }
          
       }
        
    ofstream csvFile("array_solution.csv");

    if (csvFile.is_open()) {
     
        csvFile << "Employee_ID;Salary;Department" << "\n";

        for (int i = 1; i < Employeecount; i++) {
            csvFile << Employees[i].getEmployee() << ";"
                    << Employees[i].getSalary() << ";"
                    << Employees[i].getDepartment() << "\n";
        }

        csvFile.close();

    } 
   delete[] Employees;


   return 0;
    
}