/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 15.01.2024 > */


#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;
void measureAndOutputTime(clock_t start, clock_t end, const string& operationType) {
    double elapsed_time = static_cast<double>(end - start) * 1000 / CLOCKS_PER_SEC;
    ofstream timeFile("time_log.txt", ios_base::app);
    timeFile << operationType << ":" << elapsed_time << " ms\n";
    timeFile.close();
}
 
class Employee {
private:
    int id;
    int salary;
    int department;

public:
    Employee(int id, int salary, int department) : id(id), salary(salary), department(department) {}

    // getters
    int get_id() const {
        return id;
    }

    int get_salary() const {
        return salary;
    }

    int get_department() const {
        return department;
    }
    void setSalary(int newSalary){
        this->salary = newSalary;
    }
     void setDepartment(int newDepartment){
        this->department = newDepartment;
    }
};

void deleteEmployee(map<int, Employee*>& employee_map, int& employee_count, int employeeID) {
    auto it = employee_map.find(employeeID);
    if (it != employee_map.end()) {
        delete it->second;
        employee_map.erase(it);
        employee_count--;
    } else {
        cout << "ERROR: An invalid ID to delete" << endl;
    }
}

void addEmployee(map<int, Employee*>& employee_map, int& employee_count, int& lastEmployeeCreated, int salary, int department) {
    Employee* newEmployee = new Employee(++lastEmployeeCreated, salary, department);
    employee_map[newEmployee->get_id()] = newEmployee;
    employee_count++;
}

void updateEmployee(std::map<int, Employee*>& employee_map, int employeeID, int newSalary, int newDepartment) {
    auto it = employee_map.find(employeeID);
    if (it != employee_map.end()) {
        it->second->setSalary(newSalary);
        it->second->setDepartment(newDepartment);
    } else {
         cout << "ERROR: An invalid ID to update" << endl;
    }
}
int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <employee_file> <operations_file>" << endl;
        return 1;
    }

    int employee_count = 0;
    int lastEmployeeCreated = 0;
    map<int, Employee*> employee_map;

    ifstream employeeFile(argv[1]);
    if (!employeeFile.is_open()) {
        cout << "Error opening employee file" << endl;
        return 1;
    }

    // Read employee information
    string line;
    getline(employeeFile, line);
    while (getline(employeeFile, line)) {
        istringstream iss(line);
        string employeeIDStr, salaryStr, departmentStr;
        if (getline(iss, employeeIDStr, ';') && getline(iss, salaryStr, ';') && getline(iss, departmentStr)) {
            int employeeID, salary, department;
            istringstream(employeeIDStr) >> employeeID;
            istringstream(salaryStr) >> salary;
            istringstream(departmentStr) >> department;

            Employee* newEmployee = new Employee(employeeID, salary, department);
            employee_map[employeeID] = newEmployee;
            employee_count++;
            lastEmployeeCreated = max(lastEmployeeCreated, employeeID);
        }
    }

    ifstream operationsFile(argv[2]);
    if (!operationsFile.is_open()) {
        cout << "Error opening operations file" << endl;
        return 1;
    }
   
    // Process operations
    //clock_t start3 = clock();
    if (operationsFile.is_open()) {
        while (getline(operationsFile, line)) {
            istringstream iss(line);

            string command, employeeID, newSalary, newDepartment;
            getline(iss, command, ';');

            if (command == "ADD") {
        //   clock_t add1 = clock();  
        clock_t start = clock();
                int opSalary, opDepartment;
                if (getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                    istringstream(newSalary) >> opSalary;
                    istringstream(newDepartment) >> opDepartment;
              
                    addEmployee(employee_map, employee_count, lastEmployeeCreated, opSalary, opDepartment);
           
      //                          clock_t add2 = clock();
   /// double add = static_cast<double>(add2 - add1) * 1000 / CLOCKS_PER_SEC;

   // cout << "Execution Time for Add: " << add <<" milliseconds"<<endl;
                
                

    clock_t end = clock();  
	measureAndOutputTime(start, end, "ADD");
                }
            } else if (command == "DELETE") {
    //            clock_t delete1 = clock(); 
     clock_t start = clock();

                if (getline(iss, employeeID, ';')) {

                    int opID;
                    istringstream(employeeID) >> opID;
                      deleteEmployee(employee_map, employee_count, opID);

  //                         clock_t delete2 = clock();
//                     double deletion = static_cast<double>(delete2 - delete1) * 1000 / CLOCKS_PER_SEC;

                  //      cout << "Execution Time for Delete: " << deletion <<" milliseconds"<<endl;
               
    clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
               
                }
            } else if (command == "UPDATE") {
//clock_t update1= clock(); 
 clock_t start = clock();

                int opID, opSalary, opDepartment;
                if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                    istringstream(employeeID) >> opID;
                    istringstream(newSalary) >> opSalary;
                    istringstream(newDepartment) >> opDepartment;
                updateEmployee(employee_map, opID, opSalary, opDepartment);
          //      clock_t update2 = clock();
//                     double update = static_cast<double>(update2 - update1) * 1000 / CLOCKS_PER_SEC;

         //                 cout << "Execution Time for Update: " << update <<" milliseconds"<<endl;
         
    clock_t end = clock();  
	measureAndOutputTime(start, end, "UPDATE");
                }

    }
        }
    }

    // Write the final employee data to CSV
    ofstream csvFile("output.csv");
    if (csvFile.is_open()) {
        csvFile << "Employee_ID;Salary;Department" << endl;
        for (const auto& entry : employee_map) {
            csvFile << entry.second->get_id() << ";"
                    << entry.second->get_salary() << ";"
                    << entry.second->get_department() << "\n";
        }
        csvFile.close();
    }

    // Cleanup
    for (const auto& entry : employee_map) {
        delete entry.second;
    }

    return 0;
}