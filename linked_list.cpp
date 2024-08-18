/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 10.11.2023 > */
/*Execution Time for ADD: 0.002 milliseconds
Execution Time for DELETE: 0.855 milliseconds
Execution Time for ADD: 0 milliseconds
Execution Time for DELETE: 0.406 milliseconds
Execution Time for ADD: 0.001 milliseconds
Execution Time for UPDATE: 0.54 milliseconds
Execution Time for ADD: 0 milliseconds
Execution Time for UPDATE: 1.877 milliseconds
Execution Time for DELETE: 1.031 milliseconds*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
    Employee* next;

    Employee(int id, int salary, int department) {
        this->id = id;
        this->salary = salary;
        this->department = department;
        this->next = nullptr;
    }
};

class LinkedList {
private:
    Employee* head;
    Employee* tail;
public:
    LinkedList() {
        head = nullptr;
        tail = nullptr;
    }
    void readEmployee(int Id, int Salary, int Department) {
     Employee* newEmployee = new Employee(Id, Salary, Department);
       if (!head) {
            head = newEmployee;
            tail = newEmployee;  // First employee becomes both head and tail
        } else {
            tail->next = newEmployee;
            tail = newEmployee;  // Update the tail to the new employee
        }
    }


    void addEmployee(int lastEmployeeCreated, int salary, int department) {
     //  clock_t start1 = clock();  
        Employee* newEmployee = new Employee(lastEmployeeCreated, salary, department);
        tail->next = newEmployee;
            tail = newEmployee;
           
 //clock_t end1 = clock();
   // double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
   //cout << "Execution Time for ADD: " << elapsed_time1 <<" milliseconds"<<"\n";
    }

    bool updateEmployee(int id, int salary, int department) {
     //   clock_t start1 = clock();
        bool FoundId=false;
        Employee* current = head;
        while (current) {
            if (current->id == id) {
                current->salary = salary;
                current->department = department;
                FoundId=true;
                
       //     clock_t end1 = clock();
         //    double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
           //  cout << "Execution Time for UPDATE: " << elapsed_time1 <<" milliseconds"<<"\n";
                return true;

            }
      
            current = current->next;
        }
         if(!FoundId){cout << "ERROR: An invalid ID to update"<<"/n";}
        return false;
    }

    bool deleteEmployee(int id) {
        //clock_t start1 = clock();
        bool FoundId=false;
        if (!head) {
            return false;
        }
        if (head->id == id) {
            Employee* temp = head;
            head = head->next;
            FoundId=true;
            delete temp;
          //  clock_t end1 = clock();
            //  double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
              //cout << "Execution Time for DELETE: " << elapsed_time1 <<" milliseconds"<<"\n";
            return true;
        }
      
        Employee* current = head;
        while (current->next) {
            if (current->next->id == id) {
                Employee* temp = current->next;
                current->next = temp->next;
                FoundId=1;
                delete temp;
                //  clock_t end1 = clock();
                // double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
             //cout << "Execution Time for DELETE: " << elapsed_time1 <<" milliseconds"<<"\n";
                return true;
            }
            current = current->next;
        }
          
         if(!FoundId){cout << "ERROR: An invalid ID to delete"<<"/n";}
    
        return false;
    }

     void saveToCSV() {
    ofstream csvFile("linked_list_solution.csv");
        if (csvFile.is_open()) {
            Employee* current = head;
            csvFile << "Employee_ID;Salary;Department" << "\n";

            while (current) {
                csvFile << current->id << ";" << current->salary << ";" << current->department << "\n";
                current = current->next;
            }
            csvFile.close();
        } else {
           cout << "CSV file is closed" << std::endl;
        }
     }

    void clear() {
        while (head) {
            Employee* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main(int argc, char** argv) {
   
    if (argc != 3) {
      
        return 0;
    }
   

    LinkedList employees;


   ifstream EmployeeFile;
    EmployeeFile.open(argv[1]);
    if (!EmployeeFile.is_open()){ 
        cout << "couldnt open the csv file"<<endl;
        return 0;
    }
         ifstream OperationsFile;
    OperationsFile.open(argv[2]);
    if (!OperationsFile.is_open()){ 
        cout << "couldnt open the operation file"<<endl;
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
           employees.readEmployee(Id, Salary, Department);
           
             
               }
         } 
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
          employees.updateEmployee(opEmployee,opSalary,opDepartment); 
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
            employees.addEmployee(lastEmployeeCreated, opSalary, opDepartment);
              
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
          employees.deleteEmployee(opEmployee);
        }
        }
        else{cout << "ERROR: There is no Employee"<<"/n";}
        clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
      }
      
    
     } 


    employees.saveToCSV();
  
    employees.clear();

    return 0;

}
