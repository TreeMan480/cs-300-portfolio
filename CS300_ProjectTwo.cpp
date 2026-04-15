#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() = default;

    Course(const string& number, const string& title)
        : courseNumber(number), courseTitle(title) {
    }
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    explicit Node(const Course& aCourse)
        : course(aCourse), left(nullptr), right(nullptr) {
    }
};

class BinarySearchTree {
private:
    Node* root;

    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void addNode(Node* node, const Course& course) {
        if (course.courseNumber < node->course.courseNumber) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) const {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    Course* search(Node* node, const string& courseNumber) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->course.courseNumber == courseNumber) {
            return &(node->course);
        }

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }

        return search(node->right, courseNumber);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        Clear();
    }

    void Insert(const Course& course) {
        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    void PrintCourseList() const {
        inOrder(root);
    }

    Course* Search(const string& courseNumber) const {
        return search(root, courseNumber);
    }

    bool IsEmpty() const {
        return root == nullptr;
    }

    void Clear() {
        destroyTree(root);
        root = nullptr;
    }
};

string trim(const string& text) {
    size_t start = 0;
    while (start < text.length() && isspace(static_cast<unsigned char>(text[start]))) {
        ++start;
    }

    size_t end = text.length();
    while (end > start && isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }

    return text.substr(start, end - start);
}

string toUpperCase(string text) {
    for (char& ch : text) {
        ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    }
    return text;
}

vector<string> splitLine(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

bool loadCourses(const string& fileName, BinarySearchTree& courseTree) {
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    courseTree.Clear();

    string line;
    while (getline(inputFile, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> tokens = splitLine(line);
        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            continue;
        }

        Course course(tokens[0], tokens[1]);
        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        courseTree.Insert(course);
    }

    inputFile.close();
    return true;
}

void printCourseInformation(const Course* course) {
    if (course == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course->courseNumber << ", " << course->courseTitle << endl;
    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            cout << course->prerequisites[i];
            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

int getMenuChoice() {
    string input;
    getline(cin, input);
    input = trim(input);

    if (input.empty()) {
        return -1;
    }

    for (char ch : input) {
        if (!isdigit(static_cast<unsigned char>(ch))) {
            return -1;
        }
    }

    return stoi(input);
}

int main() {
    BinarySearchTree courseTree;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl << endl;

    while (choice != 9) {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";

        choice = getMenuChoice();

        switch (choice) {
        case 1: {
            string fileName;
            cout << "Enter file name: ";
            getline(cin, fileName);
            fileName = trim(fileName);

            if (loadCourses(fileName, courseTree)) {
                dataLoaded = true;
            }
            cout << endl;
            break;
        }
        case 2:
            if (!dataLoaded || courseTree.IsEmpty()) {
                cout << "Please load the data file first." << endl << endl;
            }
            else {
                cout << "Here is a sample schedule:" << endl << endl;
                courseTree.PrintCourseList();
                cout << endl;
            }
            break;
        case 3:
            if (!dataLoaded || courseTree.IsEmpty()) {
                cout << "Please load the data file first." << endl << endl;
            }
            else {
                string courseNumber;
                cout << "What course do you want to know about? ";
                getline(cin, courseNumber);
                courseNumber = toUpperCase(trim(courseNumber));
                printCourseInformation(courseTree.Search(courseNumber));
                cout << endl;
            }
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            if (choice == -1) {
                cout << "That is not a valid option." << endl << endl;
            }
            else {
                cout << choice << " is not a valid option." << endl << endl;
            }
            break;
        }
    }

    return 0;
}
