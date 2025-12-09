#ifndef TEACHER_H
#define TEACHER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Teacher {
    int id;
    string name;
    string subject;
    Teacher* next;
};

struct TeacherTreeNode {
    int id;
    string name;
    Teacher* teacherPtr;
    TeacherTreeNode* left;
    TeacherTreeNode* right;
};

class TeacherModule {
private:
    Teacher* head;
    TeacherTreeNode* root;

    TeacherTreeNode* insertBST(TeacherTreeNode* node, int id, string name, Teacher* teacherPtr) {
        if (node == NULL) {
            TeacherTreeNode* newNode = new TeacherTreeNode();
            newNode->id = id;
            newNode->name = name;
            newNode->teacherPtr = teacherPtr;
            newNode->left = NULL;
            newNode->right = NULL;
            return newNode;
        }
        if (id < node->id) {
            node->left = insertBST(node->left, id, name, teacherPtr);
        } else {
            node->right = insertBST(node->right, id, name, teacherPtr);
        }
        return node;
    }

    TeacherTreeNode* searchBSTById(TeacherTreeNode* node, int id) {
        if (node == NULL || node->id == id) {
            return node;
        }
        if (id < node->id) {
            return searchBSTById(node->left, id);
        }
        return searchBSTById(node->right, id);
    }

    void saveToFile() {
        ofstream outFile("teachers.txt");
        if (!outFile) {
            cout << "Error opening file for writing.\n";
            return;
        }
        
        Teacher* temp = head;
        while (temp != NULL) {
            outFile << temp->id << "|" << temp->name << "|" << temp->subject << endl;
            temp = temp->next;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile("teachers.txt");
        if (!inFile) {
            return;
        }

        string line;
        while (getline(inFile, line)) {
            stringstream ss(line);
            string segment;
            int id;
            string name, subject;

            if (!getline(ss, segment, '|')) continue;
            id = stoi(segment);

            if (!getline(ss, segment, '|')) continue;
            name = segment;

            if (!getline(ss, segment, '|')) continue;
            subject = segment;

            addTeacherInternal(id, name, subject);
        }
        inFile.close();
    }

    void addTeacherInternal(int id, string name, string subject) {
        Teacher* newTeacher = new Teacher();
        newTeacher->id = id;
        newTeacher->name = name;
        newTeacher->subject = subject;
        newTeacher->next = NULL;

        if (head == NULL) {
            head = newTeacher;
        } else {
            Teacher* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newTeacher;
        }
        root = insertBST(root, id, name, newTeacher);
    }

public:
    TeacherModule() {
        head = NULL;
        root = NULL;
        loadFromFile();
    }

    void addTeacher(int id, string name, string subject) {
        if (searchBSTById(root, id) != NULL) {
            cout << "Error: Teacher with ID " << id << " already exists.\n";
            return;
        }

        addTeacherInternal(id, name, subject);
        saveToFile();
        cout << "Teacher Added Successfully.\n";
    }

    void displayTeachers() {
        if (head == NULL) {
            cout << "No teachers found.\n";
            return;
        }

        Teacher* temp = head;
        cout << "\n--- Teacher List ---\n";
        while (temp != NULL) {
            cout << "ID: " << temp->id << " | Name: " << temp->name << " | Subject: " << temp->subject << endl;
            temp = temp->next;
        }
        cout << "--------------------\n";
    }

    void searchTeacherById(int id) {
        TeacherTreeNode* node = searchBSTById(root, id);
        if (node != NULL) {
            cout << "Found: ID: " << node->id << ", Name: " << node->name << ", Subject: " << node->teacherPtr->subject << endl;
        } else {
            cout << "Teacher not found.\n";
        }
    }

    void assignSubject(int id, string newSubject) {
        TeacherTreeNode* node = searchBSTById(root, id);
        if (node != NULL) {
            node->teacherPtr->subject = newSubject;
            saveToFile();
            cout << "Subject updated successfully for " << node->name << ".\n";
        } else {
            cout << "Teacher not found.\n";
        }
    }
};

#endif
