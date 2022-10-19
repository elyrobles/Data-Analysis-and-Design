/*
* 
* 
* PROJECT 2 SUBMISSION BY ELIZABETH ROBLES
* 
* CS-300 DSA: ANALYSIS AND DESIGN
* 
* SOUTHERN NEW HAMPSHIRE UNIVERSITY
* 
* DATE: OCTOBER 16TH, 2022
* 
* 
*/

#include <iostream>
#include <fstream> // fstream library for both reading and writing from a file
#include "CSVparser.hpp"

using namespace std;


/*
* Global definitions visible to all methods and classes
*/



/*defined a structure to hold course information; this info includes the courses id number,
  its name, and the prerequisites are placed inside a vector so that way it can hold as many 
  prerequisites as needed per course 
*/
struct Course {
	string courseNumber;
	string courseName;
	vector<string>prerequisite;
	Course() {}
};

// internal structure for tree node
struct Node {
	Course course;
	Node* left;     // The binary tree contains both left and right children, this will hold our course's information 
	Node* right;

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}
	//initialize with a course
	Node(Course aCourse) :
		Node() {
		this->course = aCourse;
	}
};

/*
* Binary Search Tree class definition 
* 
* I defined a class containing data members and methods to
* implement a binary search tree
* 
*/

class BinarySearchTree {

private:
	Node* root;
	void addNode(Node* node, Course course);
	void printSampleSchedule(Node* node);
	void printCourseInformation(Node* node, string courseNumber);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void Insert(Course course);
	void PrintSampleSchedule();
	void PrintCourseInformation(string courseNumber);
	int NumberPrerequisites(Course course);
};

/*
* Created a default constructor
*/

BinarySearchTree::BinarySearchTree() {
	//set root equal to nullptr
	root = nullptr;
}

/* 
*  Created a Destructor
*/

BinarySearchTree::~BinarySearchTree() {
}


// Now we will traverse the tree in order
void BinarySearchTree::PrintSampleSchedule() {
	// call the in order function (PrintSampleSchedule) and pass the root
	this->printSampleSchedule(root);
}
// This allows for the BST root and courseNumber to be searched for and displayed to the screen if it is found
void BinarySearchTree::PrintCourseInformation(string courseNumber) {
	// call the PrintCourseInformation function and pass the root and course number
	this->printCourseInformation(root, courseNumber);
}

// Created an insert function to insert a node(course) into the proper location in the Binary Seach Tree
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		//root is equal to the new node course
		root = new Node(course);
	}
	else {
		// else we will add the root and course
		this->addNode(root, course);
	}
}

// Created a function to return the number of prerequsites the courses have
int BinarySearchTree::NumberPrerequisites(Course course) {
	int number = 0;
	for (int i = 0; i < course.prerequisite.size(); i++) {
		if (course.prerequisite.at(i).length() > 0) {
			number++;
		}
	}
	return number;
}

/* This will display a searched courses information along with its prerequisites
*  by searching the tree for a matching course
*/
void BinarySearchTree::printCourseInformation(Node* current, string courseNumber) {
	//keep looping downwards until the bottom is reached or until a matching course is found
	while (current != nullptr) {
		//if a match is found then return the current course
		if (current->course.courseNumber == courseNumber) {
			// display the course
			cout << current->course.courseNumber << ", " << current->course.courseName << endl;
			// next find the number of prerequisites the searched course will have 
			unsigned int size = NumberPrerequisites(current->course);
			cout << "Prerequisites: ";
			// if there are prerequisites then we will display them to the screen
			int i = 0;
			for (i = 0; i < size; i++) {
				cout << current->course.prerequisite.at(i);
				if (i != size - 1) {
					cout << ", ";
				}
			}
			if (i == 0) {
				cout << "No prerequisites for this course." << endl;
			}
			cout << endl;
			return;
		}
		// If course is smaller than the current node then traverse left
		else if (courseNumber < current->course.courseNumber) {
			current = current->left;
		}
		// else larger so traverse right
		else {
			current = current->right;
		}
		
	}
		// inform the user that the bottom of the file has been reached and no match found
		cout << courseNumber << " was not found in the system" << endl;
}


// Here we will insert a course into the tree
void BinarySearchTree::addNode(Node* node, Course course) {
	//if node is larger then add to the elft
	if (node->course.courseNumber > course.courseNumber) {
		// if no left node
		if (node->left == nullptr) {
			//this node becomes the left node
			node->left = new Node(course);
		}
		// else we will recurse do the left node
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		//if no right node
		if (node->right == nullptr) {
			// this node becomes the right node
			node->right = new Node(course);
		}
		else {
			//recurse down the right node
			this->addNode(node->right, course);
		}
	}
}

// function to list all courses alphanumerically
void BinarySearchTree::printSampleSchedule(Node* node) {
	// if node is equal to nullptr then return
	if (node == nullptr) {
		return;
	}
	// recursion on left child
	printSampleSchedule(node->left);
	//print node data
	cout << node->course.courseNumber << ", " << node->course.courseName << endl;
	// recursion on right child 
	printSampleSchedule(node->right);
}

/*
* 
* Static methods used for testing
* 
* 
*/



/*
*  Load a csv file containing courses into a container
*/

bool loadCourses(string csvPath, BinarySearchTree* bst) { //bool instead of void to check if the load was successful or not
	try {
		// open the file 
		ifstream inputFile(csvPath);
		// if no errors then proceed 
		if (inputFile.good()) {
			while (!inputFile.eof()) { //as long as end of file is not reached then proceed 
				// Here a vector will hold the file info
				vector<string>file;
				string courseData;
				getline(inputFile, courseData); //read lines from file
				while (courseData.length() > 0) {
					unsigned int comma = courseData.find(',');
					if (comma < 50) { //file is only allowed less than 50 commas
						file.emplace_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}
					else {
						file.emplace_back(courseData.substr(0, courseData.length()));
						courseData = "";

					}
				}
				//Separated values will now be placed into the BST
				Course course;
				course.courseNumber = file[0];
				course.courseName = file[1];
				for (unsigned int i = 2; i < file.size(); i++) {
					course.prerequisite.emplace_back(file[i]);
				}
				// push this course to the end 
				bst->Insert(course);
			}
			// close the file
			inputFile.close();
			return true; // method completed successfully, in main() let user know that the file load was successful
		}
	}
	catch (csv::Error &e) { //catch any erros
		std::cerr << e.what() << std::endl;
	}
	return false; // if file was not loaded successfuly then fileExists will be false and will display error message in main()
}


/*
*  main() method
*/

int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, courseKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseKey = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}

	// define a binary search tree to hold all courses
	BinarySearchTree* bst = nullptr;

	cout << "Welcome to the course planner." << endl; //displays title once 

	int choice = 0; // sets choice to 0
	while (choice != 4) { // while loop to display menu to user until user decides to exit the program
		cout << endl;
		cout << "1. Load Data Structure." << endl;
		cout << "2: Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "4. Exit." << endl;
		cout << endl;
		cout << "What would you like to do? ";

		cin >> choice;

		bool fileExists = 1; //will cehck to see fi the file does exsit

		if (choice > 3 && choice > 4) { // if loop for if the user chooses a number that is greater than 3 and greater than 4
			cout << choice << " is not a valid option" << endl; // message will display to let user know their choice is not a valid input
		}
		

		switch (choice) {

		// if user enters option 1 then they will be prompted to enter the file name containing the course information
		case 1:
			//here we will instaniate a BST
			if (bst == nullptr) {
				bst = new BinarySearchTree();;
			}

			if (csvPath.length() == 0) { // if no file path then ask user to enter the file info
				cout << "Please enter the file name that contains the course information: ";
				cin >> csvPath; //get the file name from user
				cout << endl; 
				fileExists = loadCourses(csvPath, bst); //find out if the load was successful or not
			}

			// If the file exists then a message will be displayed to the screen informing the user that the load has been successfull 
			if (fileExists) {
				cout << "Loading CSV file " << csvPath << endl;
				cout << "The courses from the file have now been loaded" << endl;
			}
			else {
			// Else a messatge will display to the user letting them know a file was not found 
				cout << "Sorry, file was not found!" << endl;
			}
			csvPath = "";

			break;

		// If user enters option 2 then we will call upon the PrintSampleSchedule function  to display a list of courses to the screen alphanumerically
		case 2:
			// check to make sure that the BST does exist
			if (bst != nullptr) {
				if (fileExists) {
					cout << "Here is a sample scheudle: " << endl;
					cout << endl;
					bst->PrintSampleSchedule();
					cout << endl;
				}
				
			}
			else { // if the user did not load the file firs then they will be prompted to 
				cout << "Please load the file with data first. " << endl;
			}
			
			break;

		// IF user enters option 3 they will be promted to enter a course number they wish to search, then that course and its info will be displayed to them 
		case 3:
			// check to make sure that the BST does exist
			if (bst != nullptr) {
				if (fileExists) {
					//prompt user to enter the course they which to search
					if (courseKey.length() == 0) {
						cout << "What course do you want to know about? ";
						cin >> courseKey;
						for (auto& c : courseKey) c = toupper(c); //changes courseKey to uppsercase so it can be searched for 
						cout << endl;
					}
				}
				// call the PrintCourseInformation function to search for the courseKey
				bst->PrintCourseInformation(courseKey);
				cout << endl;
				
			}
			// prompt user to load the courses if they haven't done so
			else {
				cout << "Please load the courses first." << endl;
			}
			courseKey = "";
			break;


		}
		
			
	}
	cout << "Thank you for using the course planner!" << endl; //message to user when they choose to leave program

	return 0;
	}