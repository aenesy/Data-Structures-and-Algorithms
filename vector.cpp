/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 10.11.2023 > */
/* 
Execution Time for UPDATE: 4.618 milliseconds
Execution Time for DELETE: 4.653 milliseconds
Execution Time for ADD: 0.001 milliseconds
Execution Time for ADD: 0 milliseconds
Execution Time for UPDATE: 2.101 milliseconds
Execution Time for DELETE: 5.845 milliseconds
Execution Time for DELETE: 3.476 milliseconds
Execution Time for UPDATE: 2.471 milliseconds
Execution Time for ADD: 0 milliseconds
Execution Time for UPDATE: 0.481 milliseconds
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
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
    EmployeeClass() : Employee(0), Salary(0), Department(0) {}

    EmployeeClass(int Employee, int Salary, int Department)
        : Employee(Employee), Salary(Salary), Department(Department) {}

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


void addEmployee(vector<EmployeeClass>& Employees, int lastEmployeeCreated, int Salary, int Department) {
   // clock_t start1 = clock();
    Employees.push_back(EmployeeClass(lastEmployeeCreated, Salary, Department));
  // clock_t end1 = clock();
    //  double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
  //    cout << "Execution Time for ADD: " << elapsed_time1 <<" milliseconds"<<"\n";
  
}

void updateEmployee(vector<EmployeeClass>& Employees, int employeeID, int newSalary, int newDepartment) {
   //  clock_t start1 = clock();
    bool found = false;
    for (EmployeeClass& emp : Employees) {
        if (emp.getEmployee() == employeeID) {
            emp.setSalary(newSalary);
            emp.setDepartment(newDepartment);
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "ERROR: An invalid ID to update" ;
    }
  //  clock_t end1 = clock();
     // double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
    //  cout << "Execution Time for UPDATE: " << elapsed_time1 <<" milliseconds"<<"\n";
}

void deleteEmployee(vector<EmployeeClass>& Employees, int employeeID) {
  //   clock_t start1 = clock();
    if (Employees.empty()) {
        cout << "ERROR: There is no Employee";
        return;
    }

    auto it = Employees.begin();
    while (it != Employees.end()) {
        if (it->getEmployee() == employeeID) {
            it = Employees.erase(it);
      //      clock_t end1 = clock();
      //double elapsed_time1 = static_cast<double>(end1 - start1) * 1000 / CLOCKS_PER_SEC;
      //cout << "Execution Time for DELETE: " << elapsed_time1 <<" milliseconds"<<"\n";
            return;
        } else {
            ++it;
        }
    }

    cout << "ERROR: An invalid ID to delete";
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << "employee_data_file operations_file" << endl;
        return 1;
    }

    int lastEmployeeCreated = 0;
    vector<EmployeeClass> Employees;

    ifstream EmployeeFile(argv[1]);
    if (!EmployeeFile.is_open()) {
        cout << "ERROR: couldnt open the CSV file.";
        return 1;
    }

    string line;
    getline(EmployeeFile, line);
    while (getline(EmployeeFile, line)) {
        istringstream iss(line);
        string EmployeeStr, SalaryStr, DepartmentStr;

        if (getline(iss, EmployeeStr, ';') && getline(iss, SalaryStr, ';') && getline(iss, DepartmentStr, ';')) {
            int Employee, Salary, Department;
            istringstream(EmployeeStr) >> Employee;
            istringstream(SalaryStr) >> Salary;
            istringstream(DepartmentStr) >> Department;
            lastEmployeeCreated++;
            Employees.push_back(EmployeeClass(Employee, Salary, Department));
        }
    }

    EmployeeFile.close();  

    ifstream OperationsFile(argv[2]);
    if (!OperationsFile.is_open()) {
        cout << "ERROR: couldnt open Operations file.";
        return 1;
    }


    string operation;
    while (getline(OperationsFile, operation)) {
        istringstream iss(operation);

        string command, employeeID, newSalary, newDepartment;
        getline(iss, command, ';');

        if (command == "UPDATE") {
            int opEmployee, opSalary, opDepartment;
  clock_t start = clock();

            if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                istringstream(employeeID) >> opEmployee;
                istringstream(newSalary) >> opSalary;
                istringstream(newDepartment) >> opDepartment;
                updateEmployee(Employees, opEmployee, opSalary, opDepartment);
                
    clock_t end = clock();  
	measureAndOutputTime(start, end, "UPDATE");
            }
        } else if (command == "ADD") {
            int opSalary, opDepartment;                
  clock_t start = clock();
            if (getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                istringstream(newSalary) >> opSalary;
                istringstream(newDepartment) >> opDepartment;
                lastEmployeeCreated++;
                addEmployee(Employees, lastEmployeeCreated, opSalary, opDepartment);

    clock_t end = clock();  
	measureAndOutputTime(start, end, "ADD");
            }
        } else if (command == "DELETE") {
            int opEmployee;            
  clock_t start = clock();
            if (getline(iss, employeeID, ';')) {
                istringstream(employeeID) >> opEmployee;
                deleteEmployee(Employees, opEmployee);
            }

    clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
        }

    }

    OperationsFile.close();  

    ofstream csvFile("vector_solution.csv");
    if (csvFile.is_open()) {
        csvFile << "Employee_ID;Salary;Department" << "\n";
        for (EmployeeClass& emp : Employees) {
            csvFile << emp.getEmployee() << ";"
                    << emp.getSalary() << ";"
                    << emp.getDepartment() << "\n";
        }
        csvFile.close();
    }
    // we do not need to deallocate the memory since we use vector
    return 0;
}