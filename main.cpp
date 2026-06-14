#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int MAX_TASKS = 100;

struct Task {
    int id;
    string title;
    string description;
};

Task tasks[MAX_TASKS];
int taskCount = 0;

struct Node {
    Task task;
    Node* next;
};
Node* todayHead = nullptr;

enum ActionType { ADD, DELETE, EDIT };

struct Action {
    ActionType type;
    Task task;
    int index;
};

Action undoStack[MAX_TASKS];
int top = -1;

void push(Action action) {
    if(top < MAX_TASKS - 1) {
        undoStack[++top] = action;
    }
}

Action pop() {
    if(top >= 0) {
        return undoStack[top--];
    }
    return {};
}

bool isEmpty() {
    return top == -1;
}

void addTask() {
    if(taskCount >= MAX_TASKS) {
        cout << "Task storage full!\n";
        return;
    }
    Task t;
    t.id = (taskCount == 0) ? 1 : tasks[taskCount-1].id + 1;

    cout << "Enter task title: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, t.title);

    cout << "Enter task description: ";
    getline(cin, t.description);

    tasks[taskCount] = t;
    taskCount++;

    Action act = {ADD, t, taskCount - 1};
    push(act);

    cout << "Task added successfully.\n";
}

void deleteTask() {
    if(taskCount == 0) {
        cout << "No tasks to delete!\n";
        return;
    }
    int id;
    cout << "Enter Task ID to delete: ";
    cin >> id;

    int index = -1;
    for(int i = 0; i < taskCount; i++) {
        if(tasks[i].id == id) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        cout << "Task not found!\n";
        return;
    }

    Action act = {DELETE, tasks[index], index};
    push(act);

    for(int i = index; i < taskCount - 1; i++) {
        tasks[i] = tasks[i+1];
    }
    taskCount--;

    cout << "Task deleted successfully.\n";
}

void editTask() {
    if(taskCount == 0) {
        cout << "No tasks to edit!\n";
        return;
    }
    int id;
    cout << "Enter Task ID to edit: ";
    cin >> id;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int index = -1;
    for(int i = 0; i < taskCount; i++) {
        if(tasks[i].id == id) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        cout << "Task not found!\n";
        return;
    }

    Action act = {EDIT, tasks[index], index};
    push(act);

    cout << "Enter new title: ";
    getline(cin, tasks[index].title);

    cout << "Enter new description: ";
    getline(cin, tasks[index].description);

    cout << "Task updated successfully.\n";
}

void searchTask() {
    if(taskCount == 0) {
        cout << "No tasks available!\n";
        return;
    }
    string title;
    cout << "Enter task title to search: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, title);

    bool found = false;
    for(int i = 0; i < taskCount; i++) {
        if(tasks[i].title.find(title) != string::npos) {
            cout << "ID: " << tasks[i].id 
                 << ", Title: " << tasks[i].title
                 << ", Description: " << tasks[i].description << "\n";
            found = true;
        }
    }
    if(!found) cout << "No matching tasks found.\n";
}

void showAllTasks() {
    if(taskCount == 0) {
        cout << "No tasks available!\n";
        return;
    }
    cout << "\n--- All Tasks ---\n";
    for(int i = 0; i < taskCount; i++) {
        cout << "ID: " << tasks[i].id 
             << ", Title: " << tasks[i].title
             << ", Description: " << tasks[i].description << "\n";
    }
}

void addTodayTask() {
    if(taskCount == 0) {
        cout << "No tasks available to add!\n";
        return;
    }
    int id;
    cout << "Enter Task ID to add to today's list: ";
    cin >> id;

    int index = -1;
    for(int i = 0; i < taskCount; i++) {
        if(tasks[i].id == id) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        cout << "Task not found!\n";
        return;
    }

    Node* newNode = new Node;
    newNode->task = tasks[index];
    newNode->next = todayHead;
    todayHead = newNode;

    cout << "Task added to today's list.\n";
}

void removeTodayTask() {
    if(!todayHead) {
        cout << "Today's task list is empty!\n";
        return;
    }
    int id;
    cout << "Enter Task ID to remove from today's list: ";
    cin >> id;

    Node *curr = todayHead, *prev = nullptr;
    while(curr) {
        if(curr->task.id == id) {
            if(prev) prev->next = curr->next;
            else todayHead = curr->next;
            delete curr;
            cout << "Task removed from today's list.\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    cout << "Task not found in today's list.\n";
}

void showTodayTasks() {
    if(!todayHead) {
        cout << "Today's task list is empty!\n";
        return;
    }
    cout << "\n--- Today's Tasks ---\n";
    Node* curr = todayHead;
    while(curr) {
        cout << "ID: " << curr->task.id 
             << ", Title: " << curr->task.title
             << ", Description: " << curr->task.description << "\n";
        curr = curr->next;
    }
}

void undoLastOperation() {
    if(isEmpty()) {
        cout << "No operations to undo!\n";
        return;
    }

    Action act = pop();

    switch(act.type) {
        case ADD:
            for(int i = act.index; i < taskCount - 1; i++)
                tasks[i] = tasks[i+1];
            taskCount--;
            cout << "Undo ADD: Task removed.\n";
            break;

        case DELETE:
            for(int i = taskCount; i > act.index; i--)
                tasks[i] = tasks[i-1];
            tasks[act.index] = act.task;
            taskCount++;
            cout << "Undo DELETE: Task restored.\n";
            break;

        case EDIT:
            tasks[act.index] = act.task;
            cout << "Undo EDIT: Task reverted.\n";
            break;
    }
}

void showMenu() {
    cout << "\n--- Task Organizer ---\n";
    cout << "1. Add New Task\n";
    cout << "2. Delete Task\n";
    cout << "3. Edit Task\n";
    cout << "4. Search for a Task\n";
    cout << "5. Show All Tasks\n";
    cout << "6. Add Task to Today's List\n";
    cout << "7. Remove Task from Today's List\n";
    cout << "8. Show Today's Tasks\n";
    cout << "9. Undo Last Operation\n";
    cout << "10. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    int choice;
    do {
        showMenu();
        cin >> choice;

        switch(choice) {
            case 1: addTask(); break;
            case 2: deleteTask(); break;
            case 3: editTask(); break;
            case 4: searchTask(); break;
            case 5: showAllTasks(); break;
            case 6: addTodayTask(); break;
            case 7: removeTodayTask(); break;
            case 8: showTodayTasks(); break;
            case 9: undoLastOperation(); break;
            case 10: cout << "Exiting...\n"; break;
            default: cout << "Invalid option!\n"; break;
        }
    } while(choice != 10);

    return 0;
}