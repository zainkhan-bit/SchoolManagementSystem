#include <iostream>
#include <limits> // For numeric_limits
#include "Student.h"
#include "Teacher.h"
#include "Admission.h"

using namespace std;

// --- Global Instances ---
StudentModule studentModule;
TeacherModule teacherModule;
AdmissionModule admissionModule;

// --- Helper for Input Validation ---
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// --- Menus ---

void studentMenu() {
    int choice;
    do {
        cout << "\n=== Student Module ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display Students\n";
        cout << "3. Update Student Grade\n";
        cout << "4. Delete Student\n";
        cout << "5. Add Attendance\n";
        cout << "6. Undo Last Change\n";
        cout << "7. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                string name, className;
                float grade;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Class: "; getline(cin, className);
                cout << "Enter Grade: "; cin >> grade;
                studentModule.addStudent(id, name, className, grade);
                break;
            }
            case 2:
                studentModule.displayStudents();
                break;
            case 3: {
                int id;
                float grade;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter New Grade: "; cin >> grade;
                studentModule.updateStudentGrade(id, grade);
                break;
            }
            case 4: {
                int id;
                cout << "Enter ID to delete: "; cin >> id;
                studentModule.deleteStudent(id);
                break;
            }
            case 5: {
                int id;
                cout << "Enter ID for attendance: "; cin >> id;
                studentModule.addAttendance(id);
                break;
            }
            case 6:
                studentModule.undoLastAction();
                break;
            case 7:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

void teacherMenu() {
    int choice;
    do {
        cout << "\n=== Teacher Module ===\n";
        cout << "1. Add Teacher\n";
        cout << "2. Display Teachers\n";
        cout << "3. Search Teacher by ID\n";
        cout << "4. Assign Subject\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                string name, subject;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Subject: "; getline(cin, subject);
                teacherModule.addTeacher(id, name, subject);
                break;
            }
            case 2:
                teacherModule.displayTeachers();
                break;
            case 3: {
                int id;
                cout << "Enter ID: "; cin >> id;
                teacherModule.searchTeacherById(id);
                break;
            }
            case 4: {
                int id;
                string subject;
                cout << "Enter ID: "; cin >> id;
                cout << "Enter New Subject: "; cin.ignore(); getline(cin, subject);
                teacherModule.assignSubject(id, subject);
                break;
            }
            case 5:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void admissionMenu() {
    int choice;
    do {
        cout << "\n=== Admission Module ===\n";
        cout << "1. Add Admission Request\n";
        cout << "2. Approve Admission (Move to Student List)\n";
        cout << "3. Display Waiting List\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                string name, className;
                float grade;
                cout << "Enter ID (Roll No): "; cin >> id;
                cout << "Enter Name: "; cin.ignore(); getline(cin, name);
                cout << "Enter Class: "; getline(cin, className);
                cout << "Enter Initial Grade: "; cin >> grade;
                admissionModule.enqueue(id, name, className, grade);
                break;
            }
            case 2: {
                int id;
                string name, className;
                float grade;
                if (admissionModule.dequeue(id, name, className, grade)) {
                    studentModule.addStudent(id, name, className, grade);
                    cout << "Admission Approved. Student moved to main list.\n";
                }
                break;
            }
            case 3:
                admissionModule.displayQueue();
                break;
            case 4:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

void searchMenu() {
    int choice;
    do {
        cout << "\n=== Search Module ===\n";
        cout << "1. Search Student by ID\n";
        cout << "2. Search Student by Name\n";
        cout << "3. Search Teacher by ID\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                cout << "Enter Student ID: "; cin >> id;
                studentModule.searchStudentById(id);
                break;
            }
            case 2: {
                string name;
                cout << "Enter Student Name: "; cin.ignore(); getline(cin, name);
                studentModule.searchStudentByName(name);
                break;
            }
            case 3: {
                int id;
                cout << "Enter Teacher ID: "; cin >> id;
                teacherModule.searchTeacherById(id);
                break;
            }
            case 4:
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

// --- Main Function ---

int main() {
    int choice;
    do {
        cout << "\n========================================\n";
        cout << "   SCHOOL MANAGEMENT SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Student Module\n";
        cout << "2. Teacher Module\n";
        cout << "3. Admission Module\n";
        cout << "4. Search Module\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: studentMenu(); break;
            case 2: teacherMenu(); break;
            case 3: admissionMenu(); break;
            case 4: searchMenu(); break;
            case 5: cout << "Exiting System. Goodbye!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}