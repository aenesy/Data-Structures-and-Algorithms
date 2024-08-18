/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 10.11.2023 > */
/*Execution Time for ADD: 0.002 milliseconds
Execution Time for UPDATE: 2.476 milliseconds
Execution Time for ADD: 0.001 milliseconds
Execution Time for UPDATE: 7.015 milliseconds
Execution Time for DELETE: 5.964 milliseconds
Execution Time for DELETE: 5.618 milliseconds
Execution Time for DELETE: 2.783 milliseconds
Execution Time for UPDATE: 0.378 milliseconds*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list> 
#include <time.h>
using namespace std;
void measureAndOutputTime(clock_t start, clock_t end, const string& operationType) {
    double elapsed_time = static_cast<double>(end - start) * 1000 / CLOCKS_PER_SEC;
    ofstream timeFile("time_log.txt", ios_base::app);
    timeFile << operationType << ":" << elapsed_time << " ms\n";
    timeFile.close();
}
class Employee {
public:
    int id;
    int salary;
    int department;

    Employee(int id, int salary, int department) {
        this->id = id;
        this->salary = salary;
        this->department = department;
    }
};

class EmployeeManager {
private:
    list<Employee> employees;

public:
   void readEmployee(int id,int salary,int department){

 employees.push_back(Employee(id, salary, department));



   }

    void addEmployee(int lastEmployeeCreated,int salary, int department) {
  //clock_t start1 = clock();
        employees.push_back(Employee(lastEmployeeCreated, salary, department));
        
 // clock_t end1 = clock();
   // double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
   //cout << "Execution Time for ADD: " << elapsed_time1 <<" milliseconds"<<"\n";
    }

    bool updateEmployee(int id, int salary, int department) {
        // clock_t start1 = clock();    
         for (auto& employee : employees) {
            if (employee.id == id) {
                employee.salary = salary;
                employee.department = department;
              //   clock_t end1 = clock();
                //double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
               // cout << "Execution Time for UPDATE: " << elapsed_time1 <<" milliseconds"<<"\n";
                return true;
           
            
            }
        }
        cout << "ERROR: An invalid ID to update" ;
        return false;
    }

    bool deleteEmployee(int id) {
     //  clock_t start1 = clock();
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->id == id) {
                employees.erase(it);
               
               // clock_t end1 = clock();
                //double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
                 //cout << "Execution Time for DELETE: " << elapsed_time1 <<" milliseconds"<<"\n";
               return true;
            }
        }
        cout << "ERROR: An invalid ID to delete" /*I do not know the reason but 
        when i put endl or \n here, calico fails. I mailed to Ilknur Hoca about 
        the error message issues and she said there might be a problem with calico
        file and there will be no problem if i upload the code this way.So i did not 
        write any end line code here*/  ;
        return false;
    }

    void saveToCSV() {
        ofstream csvFile("list_solution.csv");
        if (csvFile.is_open()) {
            csvFile << "Employee_ID;Salary;Department" << "\n";
            for (const Employee& employee : employees) {
                csvFile << employee.id << ";" << employee.salary << ";" << employee.department << "\n";
            }
            csvFile.close();
        } else {
            cout << "CSV file is closed" << "\n";
        }
    }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " employee_data_file operations_file" << "\n";
        return 1;
    }

    EmployeeManager employeeManager;
    ifstream EmployeeFile;
    EmployeeFile.open(argv[1]);
    if (!EmployeeFile.is_open()){ 
        cout << "couldnt open the csv file"<<"\n";
        return 0;
    }

       string line;
    int lastEmployeeCreated = 0;
   int EmployeeCount=0;
   getline(EmployeeFile, line);
    while (getline(EmployeeFile, line)) {
        istringstream iss(line);
        string EmployeeStr, SalaryStr, DepartmentStr;
    
        if (getline(iss, EmployeeStr, ';') && getline(iss, SalaryStr, ';') && getline(iss, DepartmentStr)) {
            int Id, Salary, Department;
            // an alternative for stoi
            istringstream(EmployeeStr) >> Id;
            istringstream(SalaryStr) >> Salary;
            istringstream(DepartmentStr) >> Department;
           lastEmployeeCreated++;
           EmployeeCount++;
           employeeManager.readEmployee(Id, Salary, Department);
           
             
               }
         } 

    ifstream operationsFile(argv[2]);
    if (!operationsFile.is_open()) {
        cout << "Couldn't open the operations file" << "\n";
        return 1;
    }

    string operation;
    while (getline(operationsFile, operation)) {
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
          employeeManager.updateEmployee(opEmployee,opSalary,opDepartment); 
        }
        

    clock_t end = clock();  
	measureAndOutputTime(start, end, "UPDATE");
       }
		
		else if(command == "ADD"){
         int opSalary, opDepartment;  
         clock_t start = clock();
         if (getline(iss, newSalary, ';') &&  getline(iss, newDepartment)) {
          
            istringstream(newSalary) >> opSalary;
            istringstream(newDepartment) >> opDepartment;
            lastEmployeeCreated++;
            employeeManager.addEmployee(lastEmployeeCreated,opSalary, opDepartment);
              
        }
        

    clock_t end = clock();  
	measureAndOutputTime(start, end, "ADD");
       }
        
		else if(command == "DELETE"){
         int opEmployee;
           clock_t start = clock();
         if(EmployeeCount>0){
         if (getline(iss, employeeID, ';')) {
             istringstream(employeeID) >> opEmployee;
          employeeManager.deleteEmployee(opEmployee);
        }
        

    clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
        }
       
        else{cout << "ERROR: There is no Employee"<<"/n";}
      }
    }

    employeeManager.saveToCSV();

    return 0;
}
