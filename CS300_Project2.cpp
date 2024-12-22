//============================================================================
// Name        : Project2.cpp
// Author      : Andrew J Cummins
// Version     : 1.0
// Description : Project 1 ABCU Algorithm and Data Structures
//============================================================================

#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// define a structure to hold course information
struct Course {
    string courseId;
    string name;
    string prerequisite;

    // Constructor to initialize the Course object
    Course(string id, string name, string prereq)
        : courseId(id), name(name), prerequisite(prereq) {}
};

class HashTable {
private:
    struct Node {
        Course course;
        Node* next;
        unsigned int key;

        // default constructor
        Node() : key(UINT_MAX), next(nullptr) {}

        // initialize with a course similar to aBid
        Node(Course aCourse) : course(aCourse), key(UINT_MAX), next(nullptr) {}

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : course(aCourse), key(aKey), next(nullptr) {}
    };

    vector<Node*> table; // Hash table of course nodes
    unsigned int tableSize;

    unsigned int hash(const string& key);

public:
    HashTable(unsigned int size = DEFAULT_SIZE);
    ~HashTable();
    void Insert(const Course& course);
    void PrintAll() const;
    Course* Search(const string& courseId);
    size_t Size() const;
};

// Constructor to initialize the hash table
HashTable::HashTable(unsigned int size) : tableSize(size) {
    table.resize(tableSize, nullptr);  // Initialize the table with nullptrs
}

// Destructor to clean up memory used by the nodes
HashTable::~HashTable() {
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;  // Delete the node to free memory
        }
    }
}

// Hash function to convert a string into a hash index
unsigned int HashTable::hash(const string& key) {
    unsigned int hashValue = 0;
    for (char ch : key) {
        hashValue = hashValue * 31 + ch; // Simple hash function
    }
    return hashValue % tableSize;
}

// Insert a course into the hash table
void HashTable::Insert(const Course& course) {
    unsigned int index = hash(course.courseId);  // Compute the hash index
    Node* newNode = new Node(course);  // Create a new node with the course

    if (table[index] == nullptr) {
        // No collision detected, insert course directly at the index
        table[index] = newNode;
    }
    else {
        // Collision occurred, use chaining
        Node* temp = table[index];
        while (temp->next) {
            temp = temp->next;  // Traverse to the end of the chain
        }
        temp->next = newNode;  // Append the new node to the chain
    }
}

// Search for a course by courseId
Course* HashTable::Search(const string& courseId) {
    unsigned int index = hash(courseId);  // Compute the hash index
    Node* current = table[index];

    // Traverse the chain to find the course
    while (current) {
        if (current->course.courseId == courseId) {
            return &current->course;  // Return pointer to the found course
        }
        current = current->next;  // Move to the next node in the chain
    }
    return nullptr;  // Return nullptr if course not found
}

// Print all courses in the hash table
void HashTable::PrintAll() const {
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current) {
            cout << "Course ID: " << current->course.courseId << ", "
                << "Course Name: " << current->course.name << ", "
                << "Prerequisite: " << current->course.prerequisite << endl;
            current = current->next;  // Move to the next node in the chain
        }
    }
}

size_t HashTable::Size() const {
    size_t size = 0;
    for (unsigned int i = 0; i < tableSize; ++i) {
        Node* current = table[i];
        while (current) {
            ++size;
            current = current->next;
        }
    }
    return size;
}

// Function to load course data from file into hash table
void loadDataStructure(const string& fileName, HashTable& table) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening: " << fileName << endl;
        return;
    }

    string courseId, name, prerequisite;
    while (getline(file, courseId, ',') &&
        getline(file, name, ',') &&
        getline(file, prerequisite)) {
        Course course(courseId, name, prerequisite);
        table.Insert(course);
    }
}

// Main driver function
int main() {
    HashTable courseTable; // creates an instance of the hash table
    string fileName = "CS 300 ABCU_Advising_Program_Input"; // file name to load data

    // Load data into hash table
    loadDataStructure(fileName, courseTable);

    // Menu loop
    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner." << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Load data
            cout << "Courses loaded successfully." << endl;
            break;
        case 2:
            // Output full course list
            courseTable.PrintAll();
            break;
        case 3: {
            // Output specific course
            string courseId;
            cout << "What course do you want to know about? ";
            cin >> courseId;
            Course* course = courseTable.Search(courseId);
            if (course) {
                cout << "Course ID: " << course->courseId << ", "
                    << "Course Name: " << course->name << ", "
                    << "Prerequisite: " << course->prerequisite << endl;
            }
            else {
                cout << "Course ID " << courseId << " not found." << endl;
            }
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
