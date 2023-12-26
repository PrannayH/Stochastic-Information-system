#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Student       //parent class 1
{
protected:
    string name;    //string
    int SRN;

public:
    Student(string name, int SRN) : name(name), SRN(SRN) {}    //parameterized constructor using initialization list

    int getSRN() 
    {
        return SRN;
    }

    virtual void displayDetails() = 0;   //pure virtual function

    friend class MarkSheet;    //friend class 
};

class MarkSheet    //base class 2
{
protected:
    int percentage;
    int numSubjects;
    int *marks;
    string *subs;

public:
    MarkSheet(int no_of_subjects)
    {
        numSubjects = no_of_subjects;
        marks = new int[numSubjects];
        subs = new string[numSubjects];
    }

    void getSubjects(Student &obj)
    {
        cout << "Enter subjects for student " << obj.SRN << " (" << obj.name << ") :" << endl;
        for (int i = 0; i < numSubjects; i++)
        {
            cout << "Enter subject " << i + 1 << ": ";
            cin >> subs[i];
        }
    }

    void setMarks(Student &obj)
    {
        int sum = 0;
        cout << "Enter marks for student " << obj.SRN << " (" << obj.name << "):" << endl;
        for (int i = 0; i < numSubjects; i++)
        {
            cout << "Enter marks for  " << subs[i] << ": ";
            cin >> marks[i];
            sum += marks[i];
        }

        percentage = sum / numSubjects;
    }

    virtual void displayMarks() = 0;    //pure virtual function

    ~MarkSheet()               //destructor
    { 
        delete[] marks;
        delete[] subs;           //syntax to delete free memory for an array
    }
};

class Grade : public Student, public MarkSheet     //dervied class using multiple inheritance
{
private:
    char grade;

public:
    Grade(string name, int SRN, int subjects) : Student(name, SRN), MarkSheet(subjects)
    {
        getSubjects(*this); 
        setMarks(*this);
        calculateGrade();
    }

    void calculateGrade()
    {
        if (percentage >= 90)
            grade = 'A';
        else if (percentage >= 80)
            grade = 'B';
        else if (percentage >= 70)
            grade = 'C';
        else if (percentage >= 60)
            grade = 'D';
        else
            grade = 'F';
    }

    void displayDetails()     // overriding the pure virtual function
    {
        cout << "Name: " << name << endl;
        cout << "SRN: " << SRN << endl;
        cout << "Grade: " << grade << endl;
    }

    void displayMarks()       //overriding the pure virtual function
    {
        for (int i = 0; i < numSubjects; i++)
        {
            cout << "Marks for " << subs[i] << ": " << marks[i] << endl;
        }
        cout << "Percentage: " << percentage << endl;
    }

};

class Database
{
private:
    vector<Grade *> students;    //vector - template function for array of variable size

public:
    void addStudent(Grade *student)
    {
        students.push_back(student); //adding to end of vector
    }

    void displayAllStudents()
    {
        if (students.empty())     //checks if vector is empty
        {
            cout << "No students in the database." << endl;
            return;
        }

        cout << "Student Details:" << endl;
        for (Grade *student : students)
        {
            student->displayDetails();
            student->displayMarks();
            cout << endl;
        }
    }

    Grade* searchStudent(int SRN)   // returns pointer to grade object
    {
        for (Grade *student : students)
        {
            if (student->getSRN() == SRN)
                return student;
        }
        return nullptr;
    }

    void deleteStudent(int SRN)
    {
        for (auto ptr = students.begin(); ptr != students.end(); ++ptr) // ptr- bidirectional iterator returned by <vector>.begin()
                                                                     //  bidirectional iterator- iterator that can be used to access a 
                                                                     //sequence of elementsin a range in both direction
        {
            if ((*ptr)->getSRN() == SRN)
            {
                delete *ptr;
                students.erase(ptr);
                cout << "Student with SRN " << SRN << " deleted." << endl;
                return;
            }
        }
        cout << "Student with SRN " << SRN << " not found." << endl;
    }
};

int main()
{
    Database database;
    int choice, SRN, numSubs;
    string name;

    Grade *foundStudent = nullptr; 

    do
    {   cout<<"\n***********************************\n";
        cout << "1. Add Student" << endl;
        cout << "2. Display All Students" << endl;
        cout << "3. Search Student" << endl;
        cout << "4. Delete Student" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        
        cin >> choice;
        cout<<"\n";

        switch (choice)
        {
        case 1:
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter SRN: ";
            cin >> SRN;
            cout << "Enter number of subjects: ";
            cin >> numSubs;
            database.addStudent(new Grade(name, SRN, numSubs));
            break;
        case 2:
            database.displayAllStudents();
            break;
        case 3:
            cout << "Enter SRN to search: ";
            cin >> SRN;
            cout << "\n";
            foundStudent = database.searchStudent(SRN);
            if (foundStudent != nullptr)
            {
                foundStudent->displayDetails();
                foundStudent->displayMarks();
            }
            else
            {
                cout << "Student with SRN " << SRN << " not found." << endl;
            }
            break;
        case 4:
            cout << "Enter SRN to delete: ";
            cin >> SRN;
            cout<<"\n";
            database.deleteStudent(SRN);
            break;
        case 5:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;

    } while (choice != 5);

    return 0;
}