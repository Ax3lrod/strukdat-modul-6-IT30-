#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

const string FILENAME = "todolist.txt";

string getCurrentDateTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char dateTime[20];
    sprintf(dateTime, "%04d-%02d-%02d %02d:%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min);
    return string(dateTime);
}

int dateDifference(const string& dateTime1, const string& dateTime2) {
    struct tm tm1 = {0}, tm2 = {0};
    istringstream ss1(dateTime1), ss2(dateTime2);
    ss1 >> get_time(&tm1, "%Y-%m-%d %H:%M");
    ss2 >> get_time(&tm2, "%Y-%m-%d %H:%M");
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);
    return difftime(time2, time1) / (60 * 60 * 24);
}

bool isSameDate(const string& dateTime1, const string& dateTime2) {
    return dateTime1.substr(0, 10) == dateTime2.substr(0, 10);
}

class Task {
protected:
    string title;
    string description;
    string createdDateTime;
    string deadline;
public:
    Task(string t, string d, string dl, string cdt = getCurrentDateTime()) : title(t), description(d), deadline(dl), createdDateTime(cdt) {}
    virtual void display() const = 0;
    void setTitle(string t) { title = t; }
    void setDescription(string d) { description = d; }
    void setDeadline(string dl) { deadline = dl; }
    string getTitle() const { return title; }
    string getDescription() const { return description; }
    string getCreatedDateTime() const { return createdDateTime; }
    string getDeadline() const { return deadline; }

    virtual string toString() const {
        return title + "|" + description + "|" + createdDateTime + "|" + deadline;
    }
};

class ToDoTask : public Task {
private:
    bool completed;
    int urgency;
public:
    ToDoTask(string t, string d, string dl, int u, bool c = false, string cdt = getCurrentDateTime()) : Task(t, d, dl, cdt), completed(c), urgency(u) {}

    void display() const override {
        cout << setw(6) << left << urgency
             << setw(40) << left << title
             << setw(70) << left << description
             << setw(20) << left << createdDateTime
             << setw(20) << left << deadline
             << setw(15) << left << (completed ? "Completed" : "Not Completed") << endl;
    }

    void markComplete() {
        completed = true;
    }

    void markIncomplete() {
        completed = false;
    }

    bool isCompleted() const {
        return completed;
    }

    void setUrgency(int u) {
        urgency = u;
    }

    int getUrgency() const {
        return urgency;
    }

    string toString() const override {
        return Task::toString() + "|" + to_string(urgency) + "|" + (completed ? "1" : "0");
    }
};

class ToDoList {
private:
    vector<ToDoTask> tasks;
public:
    ToDoList() {
        loadFromFile();
    }

    ~ToDoList() {
        saveToFile();
    }

    void createTask(string title, string description, string deadline, int urgency) {
        ToDoTask newTask(title, description, deadline, urgency);
        tasks.push_back(newTask);
    }

    void readTasks() const {
        if (tasks.empty()) {
            cout << "No tasks available." << endl;
            return;
        }
        displayHeader();
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << setw(6) << left << (i + 1);
            tasks[i].display();
        }
    }

    void readCompletedTasks() const {
        bool found = false;
        displayHeader();
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (tasks[i].isCompleted()) {
                cout << setw(6) << left << (i + 1);
                tasks[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No completed tasks available." << endl;
        }
    }

    void readIncompleteTasks() const {
        bool found = false;
        displayHeader();
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (!tasks[i].isCompleted()) {
                cout << setw(6) << left << (i + 1);
                tasks[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No incomplete tasks available." << endl;
        }
    }

    void readTasksByDeadline(int days) const {
        bool found = false;
        string currentDate = getCurrentDateTime();
        displayHeader();
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (dateDifference(currentDate, tasks[i].getDeadline()) <= days) {
                cout << setw(6) << left << (i + 1);
                tasks[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No tasks with deadlines in the specified range available." << endl;
        }
    }

    void readTasksBySpecificDate(const string& date) const {
        bool found = false;
        displayHeader();
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (isSameDate(tasks[i].getDeadline(), date)) {
                cout << setw(6) << left << (i + 1);
                tasks[i].display();
                found = true;
            }
        }
        if (!found) {
            cout << "No tasks with deadlines on the specified date available." << endl;
        }
    }

    void updateTask(size_t index, string newTitle, string newDescription, string newDeadline, int newUrgency) {
        if (index < tasks.size()) {
            tasks[index].setTitle(newTitle);
            tasks[index].setDescription(newDescription);
            tasks[index].setDeadline(newDeadline);
            tasks[index].setUrgency(newUrgency);
            cout << "Task updated successfully." << endl;
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void deleteTask(size_t index) {
        if (index < tasks.size()) {
            tasks.erase(tasks.begin() + index);
            cout << "Task deleted successfully." << endl;
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void markTaskComplete(size_t index) {
        if (index < tasks.size()) {
            tasks[index].markComplete();
            cout << "Task marked as complete." << endl;
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void markTaskIncomplete(size_t index) {
        if (index < tasks.size()) {
            tasks[index].markIncomplete();
            cout << "Task marked as incomplete." << endl;
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void sortByUrgency() {
        sort(tasks.begin(), tasks.end(), [](const ToDoTask& a, const ToDoTask& b) {
            return a.getUrgency() < b.getUrgency();
        });
    }

    void saveToFile() const {
        ofstream file(FILENAME);
        if (file.is_open()) {
            for (const auto& task : tasks) {
                file << task.toString() << endl;
            }
            file.close();
        } else {
            cout << "Unable to open file for saving." << endl;
        }
    }

    void loadFromFile() {
        ifstream file(FILENAME);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream ss(line);
                string title, description, createdDateTime, deadline, urgencyStr, completedStr;
                getline(ss, title, '|');
                getline(ss, description, '|');
                getline(ss, createdDateTime, '|');
                getline(ss, deadline, '|');
                getline(ss, urgencyStr, '|');
                getline(ss, completedStr);

                int urgency = stoi(urgencyStr);
                bool completed = completedStr == "1";
                ToDoTask task(title, description, deadline, urgency, completed, createdDateTime);
                tasks.push_back(task);
            }
            file.close();
        } else {
            cout << "No existing task file found. A new file will be created upon exiting." << endl;
        }
    }

    void displayHeader() const {
        cout << setw(6) << left << "Index"
             << setw(6) << left << "Urg."
             << setw(40) << left << "Title"
             << setw(70) << left << "Description"
             << setw(20) << left << "Date Created"
             << setw(20) << left << "Deadline"
             << setw(15) << left << "Status" << endl;
        cout << string(180, '=') << endl;
    }
};

bool isNumber(const std::string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int main() {
    ToDoList myToDoList;
    string choice;
    string title, description, deadline, specificDate;
    int urgency;
    size_t index;

    while (true) {
        cout << "\nTo-Do List Management System\n";
        cout << "0. Exit\n";
        cout << "1. Create Task\n";
        cout << "2. Read All Tasks\n";
        cout << "3. Read Completed Tasks\n";
        cout << "4. Read Incomplete Tasks\n";
        cout << "5. Read Tasks with Deadline Today\n";
        cout << "6. Read Tasks with Deadline in a Week\n";
        cout << "7. Read Tasks with Deadline in a Month\n";
        cout << "8. Read Tasks with Specific Deadline\n";
        cout << "9. Update Task\n";
        cout << "10. Delete Task\n";
        cout << "11. Mark Task Complete\n";
        cout << "12. Mark Task Incomplete\n";
        cout << "13. Sort Tasks by Urgency\n";
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");
        cout << endl;
        if(!isNumber(choice)) {
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }
        int numchoice = stoi(choice);
        switch (numchoice) {
            case 1:
                cout << "Enter title: ";
                cin.ignore();
                getline(cin, title);
                while(title.size() > 20) {
                    cout << "Title too long. Please enter a title with less than 20 characters." << endl;
                    cout << "Enter title: ";
                    getline(cin, title);
                }
                cout << "Enter description: ";
                getline(cin, description);
                while(description.size() > 70) {
                    cout << "Description too long. Please enter a description with less than 70 characters." << endl;
                    cout << "Enter description: ";
                    getline(cin, description);
                }
                cout << "Enter deadline (YYYY-MM-DD HH:MM): ";
                getline(cin, deadline);
                while(dateDifference(getCurrentDateTime(), deadline) < 0 || deadline.size() != 16){
                    cout << "Invalid deadline. Please enter a future date." << endl;
                    cout << "Enter deadline (YYYY-MM-DD HH:MM): ";
                    getline(cin, deadline);
                }
                cout << "Enter urgency (1-10): ";
                cin >> urgency;
                while(urgency < 1 || urgency > 10) {
                    cout << "Invalid urgency. Please enter a value between 1 and 10." << endl;
                    cout << "Enter urgency (1-10): ";
                    cin >> urgency;
                }
                myToDoList.createTask(title, description, deadline, urgency);
                break;
            case 2:
                myToDoList.readTasks();
                break;
            case 3:
                myToDoList.readCompletedTasks();
                break;
            case 4:
                myToDoList.readIncompleteTasks();
                break;
            case 5:
                myToDoList.readTasksByDeadline(0);
                break;
            case 6:
                myToDoList.readTasksByDeadline(7);
                break;
            case 7:
                myToDoList.readTasksByDeadline(30);
                break;
            case 8:
                cout << "Enter the specific date (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, specificDate);
                if(specificDate.size() != 10) {
                    cout << "Invalid date format. Please enter in the format YYYY-MM-DD." << endl;
                    break;
                }
                myToDoList.readTasksBySpecificDate(specificDate);
                break;
            case 9:
                cout << "Enter task index to update: ";
                cin >> index;
                cout << "Enter new title: ";
                cin.ignore();
                getline(cin, title);
                while(title.size() > 20) {
                    cout << "Title too long. Please enter a title with less than 20 characters." << endl;
                    cout << "Enter new title: ";
                    getline(cin, title);
                }
                cout << "Enter new description: ";
                getline(cin, description);
                while(description.size() > 70) {
                    cout << "Description too long. Please enter a description with less than 70 characters." << endl;
                    cout << "Enter new description: ";
                    getline(cin, description);
                }
                cout << "Enter new deadline (YYYY-MM-DD HH:MM): ";
                getline(cin, deadline);
                while(dateDifference(getCurrentDateTime(), deadline) < 0 || deadline.size() != 16){
                    cout << "Invalid deadline. Please enter a future date." << endl;
                    cout << "Enter new deadline (YYYY-MM-DD HH:MM): ";
                    getline(cin, deadline);
                }
                cout << "Enter new urgency (1-10): ";
                cin >> urgency;
                while(urgency < 1 || urgency > 10) {
                    cout << "Invalid urgency. Please enter a value between 1 and 10." << endl;
                    cout << "Enter new urgency (1-10): ";
                    cin >> urgency;
                }
                myToDoList.updateTask(index - 1, title, description, deadline, urgency);
                break;
            case 10:
                cout << "Enter task index to delete: ";
                cin >> index;
                myToDoList.deleteTask(index - 1);
                break;
            case 11:
                cout << "Enter task index to mark complete: ";
                cin >> index;
                myToDoList.markTaskComplete(index - 1);
                break;
            case 12:
                cout << "Enter task index to mark incomplete: ";
                cin >> index;
                myToDoList.markTaskIncomplete(index - 1);
                break;
            case 13:
                myToDoList.sortByUrgency();
                cout << "Tasks sorted by urgency." << endl;
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
}
