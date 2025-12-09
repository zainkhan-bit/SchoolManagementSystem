#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Student {
    int id;
    string name;
    string className;
    float grade;
    Student* next;
};

struct UndoNode {
    string actionType;
    Student backupData;
    UndoNode* next;
};

struct StudentTreeNode {
    int id;
    string name;
    Student* studentPtr;
    StudentTreeNode* left;
    StudentTreeNode* right;
};

class StudentModule {
private:
    Student* head;
    UndoNode* top;
    StudentTreeNode* root;

    StudentTreeNode* insertBST(StudentTreeNode* node, int id, string name, Student* studentPtr) {
        if (node == NULL) {
            StudentTreeNode* newNode = new StudentTreeNode();
            newNode->id = id;
            newNode->name = name;
            newNode->studentPtr = studentPtr;
            newNode->left = NULL;
            newNode->right = NULL;
            return newNode;
        }
        if (id < node->id) {
            node->left = insertBST(node->left, id, name, studentPtr);
        } else {
            node->right = insertBST(node->right, id, name, studentPtr);
        }
        return node;
    }

    StudentTreeNode* searchBSTById(StudentTreeNode* node, int id) {
        if (node == NULL || node->id == id) {
            return node;
        }
        if (id < node->id) {
            return searchBSTById(node->left, id);
        }
        return searchBSTById(node->right, id);
    }
    
    void searchBSTByName(StudentTreeNode* node, string name, bool &found) {
        if (node == NULL) return;
        searchBSTByName(node->left, name, found);
        
        if (node->name == name) {
            cout << "Found: ID: " << node->id << ", Name: " << node->name << ", Class: " << node->studentPtr->className << ", Grade: " << node->studentPtr->grade << endl;
            found = true;
        }
        
        searchBSTByName(node->right, name, found);
    }

    void clearBST(StudentTreeNode* node) {
        if (node == NULL) return;
        clearBST(node->left);
        clearBST(node->right);
        delete node;
    }

    void rebuildBST() {
        clearBST(root);
        root = NULL;
        Student* temp = head;
        while (temp != NULL) {
            root = insertBST(root, temp->id, temp->name, temp);
            temp = temp->next;
        }
    }

    void saveToFile() {
        ofstream outFile("students.txt");
        if (!outFile) {
            cout << "Error opening file for writing.\n";
            return;
        }
        
        Student* temp = head;
        while (temp != NULL) {
            outFile << temp->id << "|" << temp->name << "|" << temp->className << "|" << temp->grade << endl;
            temp = temp->next;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile("students.txt");
        if (!inFile) {
            return;
        }
        
        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string segment;
            int id;
            string name, className;
            float grade;

            if (!getline(ss, segment, '|')) continue;
            id = stoi(segment);

            if (!getline(ss, segment, '|')) continue;
            name = segment;

            if (!getline(ss, segment, '|')) continue;
            className = segment;

            if (!getline(ss, segment, '|')) continue;
            grade = stof(segment);

            addStudentInternal(id, name, className, grade);
        }
        inFile.close();
    }

    void addStudentInternal(int id, string name, string className, float grade) {
        Student* newStudent = new Student();
        newStudent->id = id;
        newStudent->name = name;
        newStudent->className = className;
        newStudent->grade = grade;
        newStudent->next = NULL;

        if (head == NULL) {
            head = newStudent;
        } else {
            Student* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newStudent;
        }
        root = insertBST(root, id, name, newStudent);
    }

public:
    StudentModule() {
        head = NULL;
        top = NULL;
        root = NULL;
        loadFromFile();
    }

    void addStudent(int id, string name, string className, float grade) {
        if (searchBSTById(root, id) != NULL) {
            cout << "Error: Student with Roll Number " << id << " already exists.\n";
            return;
        }

        addStudentInternal(id, name, className, grade);
        saveToFile();
        cout << "Student Added Successfully.\n";
    }

    void displayStudents() {
        if (head == NULL) {
            cout << "No students found.\n";
            return;
        }
        
        Student* temp = head;
        cout << "\n--- Student List ---\n";
        while (temp != NULL) {
            cout << "ID: " << temp->id << " | Name: " << temp->name << " | Class: " << temp->className << " | Grade: " << temp->grade << endl;
            temp = temp->next;
        }
        cout << "--------------------\n";
    }

    void pushUndo(string action, Student data) {
        UndoNode* newNode = new UndoNode();
        newNode->actionType = action;
        newNode->backupData = data;
        newNode->next = top;
        top = newNode;
    }

    void undoLastAction() {
        if (top == NULL) {
            cout << "Nothing to undo.\n";
            return;
        }

        UndoNode* undo = top;
        top = top->next;

        if (undo->actionType == "gradeUpdate") {
            StudentTreeNode* node = searchBSTById(root, undo->backupData.id);
            if (node && node->studentPtr) {
                node->studentPtr->grade = undo->backupData.grade;
                cout << "Undo Successful: Grade reverted for ID " << undo->backupData.id << endl;
            } else {
                cout << "Error: Student not found for undo.\n";
            }
        } else if (undo->actionType == "delete") {
            addStudentInternal(undo->backupData.id, undo->backupData.name, undo->backupData.className, undo->backupData.grade);
            cout << "Undo Successful: Student restored.\n";
        }

        saveToFile();
        delete undo;
    }

    void updateStudentGrade(int id, float newGrade) {
        StudentTreeNode* node = searchBSTById(root, id);
        if (node == NULL) {
            cout << "Student not found.\n";
            return;
        }

        pushUndo("gradeUpdate", *(node->studentPtr));

        node->studentPtr->grade = newGrade;
        saveToFile();
        cout << "Grade updated successfully.\n";
    }

    void deleteStudent(int id) {
        if (head == NULL) {
            cout << "List is empty.\n";
            return;
        }

        Student* temp = head;
        Student* prev = NULL;

        while (temp != NULL && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL) {
            cout << "Student not found.\n";
            return;
        }

        pushUndo("delete", *temp);

        if (prev == NULL) {
            head = temp->next;
        } else {
            prev->next = temp->next;
        }

        delete temp;
        rebuildBST();

        saveToFile();
        cout << "Student deleted successfully.\n";
    }

    void searchStudentById(int id) {
        StudentTreeNode* node = searchBSTById(root, id);
        if (node != NULL) {
            cout << "Found: ID: " << node->id << ", Name: " << node->name << ", Class: " << node->studentPtr->className << ", Grade: " << node->studentPtr->grade << endl;
        } else {
            cout << "Student not found.\n";
        }
    }

    void searchStudentByName(string name) {
        bool found = false;
        searchBSTByName(root, name, found);
        if (!found) {
            cout << "Student with name '" << name << "' not found.\n";
        }
    }
    
    void addAttendance(int id) {
         StudentTreeNode* node = searchBSTById(root, id);
         if(node != NULL) {
             cout << "Attendance marked for " << node->name << " (ID: " << id << ").\n";
         } else {
             cout << "Student not found.\n";
         }
    }
};

#endif
