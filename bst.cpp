/* @Author
StudentName :< Ahmet Enes Yılmaz > 
StudentID :< 820220319 >
Date :< 10.11.2023 > */

//Ordered 100k dataset takes 80seconds
//Shuffled takes a few miliseconds
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stack>
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

    EmployeeClass(int Employee, int Salary, int Department) : Employee(Employee), Salary(Salary), Department(Department) {}

    int getEmployee() const {
        return Employee;
    }

    int getDepartment() const {
        return Department;
    }

    int getSalary() const {
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

class Node {
    friend class BinaryTree;

private:
    EmployeeClass *employee;
    Node *left;
    Node *right;
    Node *parent;

public:
    Node(EmployeeClass *employee) : employee(employee), left(nullptr), right(nullptr), parent(nullptr) {}

    EmployeeClass *getEmployee() const {
        return employee;
    }

    Node *getLeft() const {
        return left;
    }

    Node *getRight() const {
        return right;
    }

    Node *getParent() const {
        return parent;
    }

    void setLeft(Node *left) {
        this->left = left;
        if (left != nullptr) {
            left->parent = this;
        }
    }

    void setRight(Node *right) {
        this->right = right;
        if (right != nullptr) {
            right->parent = this;
        }
    }
};

class BinaryTree {
private:
    Node *root;

  void insertIterative(EmployeeClass *employee ) {
    Node* newNode = new Node(employee);

    if (root == nullptr) {
        root = newNode;
     
        return;
    }

    Node* current = root;
    int id = employee->getEmployee();

    while (true) {
        if (id > current->getEmployee()->getEmployee()) {
            if (current->getRight() == nullptr) {
                current->setRight(newNode);
                return;
            }
            current = current->getRight();
        } else if (id < current->getEmployee()->getEmployee()) {
            if (current->getLeft() == nullptr) {
                current->setLeft(newNode);
                return;
            }
            current = current->getLeft();
        } else {
            // If the new employee ID is equal to the current node's ID, do nothing (duplicate ID)
            delete newNode;
            return;
        }
    }
}



    Node *searchIterative(int id) {
        Node *current = root;
        while (current != nullptr && current->getEmployee()->getEmployee() != id) {
            if (id < current->getEmployee()->getEmployee()) {
                current = current->getLeft();
            } else {
                current = current->getRight();
            }
        }
        return current;
    }

   Node* removeIterative(int id) {
        if (root == nullptr) {
            cout << "ERROR: An invalid ID to delete" << "\n";
            return root;
        }

        Node *parent = nullptr;
        Node *current = root;
        bool hasLeftChild = false;

        // Find the node to be deleted and its parent
        while (current != nullptr && current->getEmployee()->getEmployee() != id) {
            parent = current;
            if (id < current->getEmployee()->getEmployee()) {
                hasLeftChild = true;
                current = current->getLeft();
            } else {
                hasLeftChild = false;
                current = current->getRight();
            }
        }

        if (current == nullptr) {
            cout << "ERROR: An invalid ID to delete" << "\n";
            return root;
        }

        // Node with only one child or no child
        if (current->getLeft() == nullptr || current->getRight() == nullptr) {
            Node *newNode;

            // Node with no child
            if (current->getLeft() == nullptr && current->getRight() == nullptr) {
                newNode = nullptr;
            } else {
                // Node with one child
                newNode = (current->getLeft() != nullptr) ? current->getLeft() : current->getRight();
            }

            // If the node to be deleted is the root node
            if (parent == nullptr) {
                delete current;
                return newNode;
            }

            // If the node to be deleted is not the root node
            if (hasLeftChild) {
                parent->setLeft(newNode);
            } else {
                parent->setRight(newNode);
            }

            delete current;
        } else {
            // Node with two children
            Node *parentOfSuccessor = current;
            Node *successor = current->getRight();

            while (successor->getLeft() != nullptr) {
                parentOfSuccessor = successor;
                successor = successor->getLeft();
            }

            current->getEmployee()->setEmployee(successor->getEmployee()->getEmployee());
            current->getEmployee()->setSalary(successor->getEmployee()->getSalary());
            current->getEmployee()->setDepartment(successor->getEmployee()->getDepartment());

            if (parentOfSuccessor != current) {
                parentOfSuccessor->setLeft(successor->getRight());
            } else {
                parentOfSuccessor->setRight(successor->getRight());
            }

            delete successor;
        }

        return root;
    }


    void transplant(Node *parent, Node *oldNode, Node *newNode) {
        if (parent == nullptr) {
            root = newNode;
        } else if (oldNode == parent->getLeft()) {
            parent->setLeft(newNode);
        } else {
            parent->setRight(newNode);
        }
        if (newNode != nullptr) {
            newNode->parent = parent;
        }
    }

    Node *findMin(Node *node) {
        while (node->getLeft() != nullptr) {
            node = node->getLeft();
        }
        return node;
    }

    int getHeightIterative() {
        if (root == nullptr) {
            return 0;
        }

        int height = 0;
        Node *current = root;

        while (current != nullptr) {
            if (current->getLeft() == nullptr) {
                height++;
                current = current->getRight();
            } else {
                Node *predecessor = findMax(current->getLeft());
                if (predecessor->getRight() == nullptr) {
                    predecessor->setRight(current);
                    current = current->getLeft();
                } else {
                    predecessor->setRight(nullptr);
                    height++;
                    current = current->getRight();
                }
            }
        }

        return height;
    }

    Node *findMax(Node *node) {
       // clock_t start1, end1; 
      //  start1 = clock();
        while (node->getRight() != nullptr) {
            node = node->getRight();
        }
      //  end1 = clock();

 //   double duration = double(end1 - start1) / CLOCKS_PER_SEC; // Calculate the duration in seconds

 //   cout << "Time taken by findMax: " << duration << " seconds" << "\n";
        return node;
    }


void printInOrderIterative(ofstream &output_file) {
    Node *current = root;
    stack<Node*> s;

    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->getLeft();
        }

        current = s.top();
        s.pop();

        output_file << current->getEmployee()->getEmployee() << ";" << current->getEmployee()->getSalary() << ";"
                    << current->getEmployee()->getDepartment() << "\n";

        current = current->getRight();
    }
}


    void printPreOrderIterative() {
        Node *current = root;

        while (current != nullptr) {
            if (current->getLeft() == nullptr) {
                cout << current->getEmployee()->getEmployee() << ";" << current->getEmployee()->getSalary() << ";"
                     << current->getEmployee()->getDepartment() << "\n";
                current = current->getRight();
            } else {
                Node *predecessor = findMax(current->getLeft());
                if (predecessor->getRight() == nullptr) {
                    predecessor->setRight(current);
                    cout << current->getEmployee()->getEmployee() << ";" << current->getEmployee()->getSalary() << ";"
                         << current->getEmployee()->getDepartment() << "\n";
                    current = current->getLeft();
                } else {
                    predecessor->setRight(nullptr);
                    current = current->getRight();
                }
            }
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    void insert(EmployeeClass *employee) {
        insertIterative(employee);
    }

    void remove(int id) {
        removeIterative(id);
    }

    EmployeeClass *search(int id) {
        Node *result = searchIterative(id);
        return result != nullptr ? result->getEmployee() : nullptr;
    }

    int getHeight() {
        return getHeightIterative();
    }

    void printToFile(ofstream &output_file) {
        printInOrderIterative(output_file);
    }

    void printToConsole() {
        printPreOrderIterative();
    }
};

int main(int argc, char **argv) {
    if (argc != 3) {
        return 0;
    }
 //clock_t start, end;
  //start = clock();
    ifstream OperationsFile;
    OperationsFile.open(argv[2]);
    if (!OperationsFile.is_open()) {
        return 0;
    }

    ifstream EmployeeFile;
    EmployeeFile.open(argv[1]);
    if (!EmployeeFile.is_open()) {
        return 0;
    }

    BinaryTree employeeTree;
  
    string line;
    int lastEmployeeCreated = 1;
    getline(EmployeeFile, line);
    while (getline(EmployeeFile, line)) {
        istringstream iss(line);
        string EmployeeStr, SalaryStr, DepartmentStr;

        if (getline(iss, EmployeeStr, ';') && getline(iss, SalaryStr, ';') && getline(iss, DepartmentStr)) {
            int Employee, Salary, Department;
            istringstream(EmployeeStr) >> Employee;
            istringstream(SalaryStr) >> Salary;
            istringstream(DepartmentStr) >> Department;
            lastEmployeeCreated++;
            EmployeeClass *newEmployee = new EmployeeClass(Employee, Salary, Department);
         
            employeeTree.insert(newEmployee);
      
          
        }
    }

    string operation;
    while (getline(OperationsFile, operation)) {
        istringstream iss(operation);

        string command, employeeID, newSalary, newDepartment;
        getline(iss, command, ';');

        if (command == "UPDATE") {
                            clock_t start= clock();

            int opEmployee, opSalary, opDepartment;
            if (getline(iss, employeeID, ';') && getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                istringstream(employeeID) >> opEmployee;
                istringstream(newSalary) >> opSalary;
                istringstream(newDepartment) >> opDepartment;

                EmployeeClass *existingEmployee = employeeTree.search(opEmployee);
                if (existingEmployee != nullptr) {
                    existingEmployee->setSalary(opSalary);
                    existingEmployee->setDepartment(opDepartment);
                } else {
                   cerr << "ERROR: An invalid ID to update" << "\n";
                }
              clock_t end = clock();  
	measureAndOutputTime(start, end, "UPDATE");            
            }
        } else if (command == "ADD") {
                       clock_t start = clock();

                                           

            int opSalary, opDepartment;
            if (getline(iss, newSalary, ';') && getline(iss, newDepartment)) {
                istringstream(newSalary) >> opSalary;
                istringstream(newDepartment) >> opDepartment;

                EmployeeClass *newEmployee = new EmployeeClass(lastEmployeeCreated, opSalary, opDepartment);
                employeeTree.insert(newEmployee);
                lastEmployeeCreated++;

            }
            clock_t end = clock();  
	measureAndOutputTime(start, end, "ADD");
                    
        } else if (command == "DELETE") {
             clock_t start = clock();

            int opEmployee;
            if (getline(iss, employeeID, ';')) {
                istringstream(employeeID) >> opEmployee;
                employeeTree.remove(opEmployee);
            }
                                             
        clock_t end = clock();  
	measureAndOutputTime(start, end, "DELETE");
        }

    }

    ofstream csvFile("output.csv");
    if (csvFile.is_open()) {
        csvFile << "Employee_ID;Salary;Department" << "\n";
        employeeTree.printToFile(csvFile);
        csvFile.close();
    }
//end = clock();

//double duration = double(end - start) / CLOCKS_PER_SEC; // Calculate the duration in seconds
//cout << "Time taken by function: " << duration << " seconds" << "\n";

    return 0;
}