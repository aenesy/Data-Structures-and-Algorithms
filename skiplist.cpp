/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 08.01.2024 > */

/*Tested for edge cases...

  TIME MEASUREMENTS (500K_employees.csv , operations_for_500k_2.csv):

    Optimal height can be calculated by logarithm.

                HEIGHT= 19 AND MOD = 5:
                                        Execution Time for Delete: 0.008 milliseconds
                                        Execution Time for Add: 0.003 milliseconds
                                        Execution Time for Update: 0.008 milliseconds

                                        Execution Time for Insertion: 2.20635 seconds
                                        Execution Time for Operations: 2.09588 seconds
                                                            Total Time: 4.8185 seconds
                                                             

                HEIGHT= 9 AND MOD = 2:

                                        Execution Time for Add: 0.108 milliseconds
                                        Execution Time for Update: 0.048 milliseconds
                                        Execution Time for Delete: 0.011 milliseconds  

                                        Execution Time for Insertion: 6.3098 seconds
                                        Execution Time for Operations: 22.1031 seconds
                                                             Total Time: 28.9945 seconds


    VALGRIND MEMLEAK TEST RESULTS    (500K_employees.csv , operations_for_500k_2.csv):
            ==7298== HEAP SUMMARY:
            ==7298==     in use at exit: 0 bytes in 0 blocks
            ==7298==   total heap usage: 2,994,432 allocs, 2,994,432 frees, 49,779,655 bytes allocated
            ==7298== 
            ==7298== All heap blocks were freed -- no leaks are possible
            ==7298== 
            ==7298== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
            ==7298== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)                                      
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h> //time measuring
using namespace std;
void measureAndOutputTime(clock_t start, clock_t end, const string& operationType) {
    double elapsed_time = static_cast<double>(end - start) * 1000 / CLOCKS_PER_SEC;
    ofstream timeFile("time_log.txt", ios_base::app);
    timeFile << operationType << ":" << elapsed_time << " ms\n";
    timeFile.close();
}
 
    int totalEmployee = 0;
class EmployeeClass {
private:
    int Id;
    int Salary;
    int Department;

public:
    EmployeeClass() : Id(0), Salary(0), Department(0) {}
    EmployeeClass(int Id, int Salary, int Department) : Id(Id), Salary(Salary), Department(Department) {}

    int getId() { return Id; }
    int getSalary() { return Salary; }
    int getDepartment() { return Department; }
    void setId(int Id) { this->Id = Id; }
    void setSalary(int Salary) { this->Salary = Salary; }
    void setDepartment(int Department) { this->Department = Department; }
     ~EmployeeClass() {
        // No dynamic memory to deallocate
    }
};

class DoublySkipList_Node {
private:
    EmployeeClass* data;
    DoublySkipList_Node* next;
    DoublySkipList_Node* below;

public:
    DoublySkipList_Node(EmployeeClass* data) : next(nullptr), below(nullptr) {
        if (data != nullptr) {
            this->data = new EmployeeClass(*data);
        } else {
            this->data = nullptr;
        }
    }

    ~DoublySkipList_Node() {
        delete data;
        
    }

    EmployeeClass* getData() const { return data; }
    DoublySkipList_Node* getNext() const { return next; }
    DoublySkipList_Node* getBelow() const { return below; }

    void setNext(DoublySkipList_Node* n) { next = n; }
    void setBelow(DoublySkipList_Node* b) { below = b; }
};

class DoublySkipList {
private:
    DoublySkipList_Node* head;

    int height;

public:
    DoublySkipList(int height_in) : height(height_in) {
        head = new DoublySkipList_Node(nullptr);
        DoublySkipList_Node* current = head;
        for (int i = 1; i < height_in; ++i) {
            DoublySkipList_Node* newLevelHead = new DoublySkipList_Node(nullptr);
            current->setBelow(newLevelHead);
            current = newLevelHead;
        }
    }

    ~DoublySkipList() {
                 DoublySkipList_Node* currentFloor = head;
    while (currentFloor != nullptr) {
                DoublySkipList_Node* current;
                DoublySkipList_Node* nextFloor;
        nextFloor = currentFloor->getBelow();
        current = currentFloor;
        while (current != nullptr) {
                DoublySkipList_Node* nextNode;
            nextNode = current->getNext();
            delete current; 
            current = nextNode;
        }
             currentFloor = nextFloor;
    }
    }

    void insert(EmployeeClass* data) {
        int newLevel = 1;

   
        while (rand() % 5 == 0) {
            newLevel++;
            if (newLevel == height) {
                break;
            }
        }

        //cout << "Inserting employee with ID: " << data->getId() << " at level: " << newLevel << endl;

        DoublySkipList_Node* current = head;
        DoublySkipList_Node* lastInserted = nullptr;

        for (int i = height; i >= 1; --i) {
            while (current->getNext() != nullptr && current->getNext()->getData()->getId() < data->getId()) {
                current = current->getNext();
            }

            if (i <= newLevel) {
                DoublySkipList_Node* newHorizontalNode = new DoublySkipList_Node(data);
                if (lastInserted != nullptr) {
                    lastInserted->setBelow(newHorizontalNode);
                }

                if (current->getNext() != nullptr) {
                    newHorizontalNode->setNext(current->getNext());
                }

                current->setNext(newHorizontalNode);
                lastInserted = newHorizontalNode;
            }
    
            if (current->getBelow() != nullptr) {
                current = current->getBelow();
            } else {
                break;
            }
        }

    
    }

    bool remove(int remove_id) {
        DoublySkipList_Node* current = head;
        bool found = false;
        while (current != nullptr) {
            while (current->getNext() != nullptr && current->getNext()->getData()->getId() < remove_id) {
                current = current->getNext();
            }
            if (current->getNext() != nullptr && current->getNext()->getData()->getId() == remove_id) {
                found = true;
                DoublySkipList_Node* toDelete = current->getNext();
                current->setNext(toDelete->getNext());
                delete toDelete; 
            }
            current = current->getBelow();
        }
        if (!found) {
              std::cout << "ERROR: An invalid ID to delete" << "\n" ;
        }
        return found;
    }

    EmployeeClass* search(int search_id) {
        DoublySkipList_Node* current = head;
        while (current != nullptr) {
            while (current->getNext() != nullptr && current->getNext()->getData()->getId() < search_id) {
                current = current->getNext();
            }
            if (current->getBelow() != nullptr) {
                current = current->getBelow();
            } else {
                break;
            }
        }
        if (current->getNext() != nullptr && current->getNext()->getData()->getId() == search_id) {
            return current->getNext()->getData();
        }
        return nullptr;
    }

    void dumpToFile(std::ofstream& out_file) {
        out_file << "Employee_ID;Salary;Department" << "\n";
        DoublySkipList_Node* currentLevel = head;
        while (currentLevel->getBelow() != nullptr) { // Go to the bottom level
            currentLevel = currentLevel->getBelow();
        }

        DoublySkipList_Node* currentNode = currentLevel->getNext();
        while (currentNode != nullptr) {
            out_file << currentNode->getData()->getId() << ";"
                     << currentNode->getData()->getSalary() << ";"
                     << currentNode->getData()->getDepartment() << "\n";
            currentNode = currentNode->getNext();
        }
    }
};

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <employee_file> <operations_file> " << std::endl;
        return 1;
    }
    //clock_t start1 = clock();

    int employeeCount = 0;
 
    DoublySkipList skipList(19); 

    // Reading Employee Data
    ifstream employeeFile;
   // clock_t start2 = clock();
    employeeFile.open(argv[1]);
    string line;
    if (employeeFile.is_open()) {
        while (getline(employeeFile, line)) {
            istringstream iss(line);
            string IdStr, SalaryStr, DepartmentStr;
            int Id, Salary, Department;
            if (getline(iss, IdStr, ';') && getline(iss, SalaryStr, ';') && getline(iss, DepartmentStr)) { // error
                // an alternative for stoi
                istringstream(IdStr) >> Id;
                istringstream(SalaryStr) >> Salary;
                istringstream(DepartmentStr) >> Department;
                if (Id != 0) {
                    EmployeeClass* emp = new EmployeeClass(Id, Salary, Department);
                    skipList.insert(emp);
                    totalEmployee++;
                    delete emp;
                    employeeCount = max(employeeCount, Id);
                
                }
            }
        }
        employeeFile.close();
      //  clock_t end2 = clock();
        //double elapsed_time2 = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC;

        //cout << "Execution Time for Insertion: " << elapsed_time2 << " seconds" << endl;
    } else {
        std::cerr << "Failed to open employee file." << std::endl;
        return 1;
    }

    ifstream operationsFile;
    operationsFile.open(argv[2]);
    //clock_t start3 = clock();
    if (operationsFile.is_open()) {
        while (getline(operationsFile, line)) {
            istringstream iss(line);

            string command, employeeID, newSalary, newDepartment;
            getline(iss, command, ';');

            if (command == "ADD") {
        //        clock_t add1 = clock();                    
            clock_t start = clock();
                int opSalary, opDepartment;
                if (getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                    istringstream(newSalary) >> opSalary;
                    istringstream(newDepartment) >> opDepartment;
                    EmployeeClass* emp = new EmployeeClass(employeeCount + 1, opSalary, opDepartment);
                    skipList.insert(emp);
                    totalEmployee++;
                    delete emp;
                    employeeCount++;

    clock_t end = clock();  
	measureAndOutputTime(start, end, "ADD");
      //                          clock_t add2 = clock();
    //double add = static_cast<double>(add2 - add1) * 1000 / CLOCKS_PER_SEC;

    //cout << "Execution Time for Add: " << add <<" milliseconds"<<endl;
                }
            } else if (command == "DELETE") {
               // clock_t delete1 = clock(); 
                clock_t start = clock();
                if (getline(iss, employeeID, ';')) {

                    int opId;
                    istringstream(employeeID) >> opId;
                    if (totalEmployee > 0) {
                        if(skipList.remove(opId)){totalEmployee--;}
                    

    clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
                    } else {
                        cout << "ERROR: There is no Employee"<<"\n";
                    }
                //           clock_t delete2 = clock();
              //       double deletion = static_cast<double>(delete2 - delete1) * 1000 / CLOCKS_PER_SEC;

                     //   cout << "Execution Time for Delete: " << deletion <<" milliseconds"<<endl;
                }
            } else if (command == "UPDATE") {
              //  clock_t update1= clock();
                clock_t start = clock();
                int opId, opSalary, opDepartment;
                if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                    istringstream(employeeID) >> opId;
                    istringstream(newSalary) >> opSalary;
                    istringstream(newDepartment) >> opDepartment;

                    if (skipList.remove(opId)) {
                        EmployeeClass* emp = new EmployeeClass(opId, opSalary, opDepartment);
                        skipList.insert(emp);
                        delete emp;
                    }
                     else{cout << "ERROR: An invalid ID to update"<< "\n";}
    clock_t end = clock();  
	measureAndOutputTime(start, end, "UPDATE");
                }
                //       clock_t update2 = clock();
              //        double update = static_cast<double>(update2 - update1) * 1000 / CLOCKS_PER_SEC;

            //              cout << "Execution Time for Update: " << update <<" milliseconds"<<endl;
            }
        }
        operationsFile.close();
        //clock_t end3 = clock();
        //double elapsed_time3 = static_cast<double>(end3 - start3) / CLOCKS_PER_SEC;

      //  cout << "Execution Time for Operations: " << elapsed_time3 << " seconds" << endl;
    } else {
        std::cerr << "Failed to open operations file." << std::endl;
        return 1;
    }

    // Writing to Output File
    std::ofstream outFile("doubly_output.csv");
    if (outFile.is_open()) {
        //clock_t start4 = clock();
        skipList.dumpToFile(outFile);
        outFile.close();
        //clock_t end4 = clock();
       // double elapsed_time4 = static_cast<double>(end4 - start4) / CLOCKS_PER_SEC;

       // cout << "Execution Time for Dump: " << elapsed_time4 << " seconds" << endl;
    } else {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }
    //clock_t end1 = clock();
 //   double elapsed_time1 = static_cast<double>(end1 - start1) / CLOCKS_PER_SEC;

   // cout << "Total Time: " << elapsed_time1 << " seconds" << endl;
    return 0;
}