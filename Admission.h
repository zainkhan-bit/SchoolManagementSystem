#ifndef ADMISSION_H
#define ADMISSION_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct AdmissionNode {
    int id;
    string name;
    string className;
    float grade;
    AdmissionNode* next;
};

class AdmissionModule {
private:
    AdmissionNode* front;
    AdmissionNode* rear;

    void saveToFile() {
        ofstream outFile("admissions.txt");
        if (!outFile) {
            cout << "Error opening file for writing.\n";
            return;
        }
        
        AdmissionNode* temp = front;
        while (temp != NULL) {
            outFile << temp->id << "|" << temp->name << "|" << temp->className << "|" << temp->grade << endl;
            temp = temp->next;
        }
        outFile.close();
    }

    void loadFromFile() {
        ifstream inFile("admissions.txt");
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

            enqueueInternal(id, name, className, grade);
        }
        inFile.close();
    }

    void enqueueInternal(int id, string name, string className, float grade) {
        AdmissionNode* newNode = new AdmissionNode();
        newNode->id = id;
        newNode->name = name;
        newNode->className = className;
        newNode->grade = grade;
        newNode->next = NULL;

        if (rear == NULL) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

public:
    AdmissionModule() {
        front = NULL;
        rear = NULL;
        loadFromFile();
    }

    void enqueue(int id, string name, string className, float grade) {
        enqueueInternal(id, name, className, grade);
        saveToFile();
        cout << "Admission Request Added to Queue.\n";
    }

    bool dequeue(int &id, string &name, string &className, float &grade) {
        if (front == NULL) {
            cout << "Admission Queue is empty.\n";
            return false;
        }

        AdmissionNode* temp = front;
        id = temp->id;
        name = temp->name;
        className = temp->className;
        grade = temp->grade;

        front = front->next;
        if (front == NULL) {
            rear = NULL;
        }

        delete temp;
        saveToFile();
        return true;
    }

    void displayQueue() {
        if (front == NULL) {
            cout << "No pending admission requests.\n";
            return;
        }
        
        AdmissionNode* temp = front;
        cout << "\n--- Admission Waiting List ---\n";
        while (temp != NULL) {
            cout << "ID: " << temp->id << " | Name: " << temp->name << " | Class: " << temp->className << endl;
            temp = temp->next;
        }
        cout << "------------------------------\n";
    }
    
    bool isEmpty() {
        return front == NULL;
    }
};

#endif
