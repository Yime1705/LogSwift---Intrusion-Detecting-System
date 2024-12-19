#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // For std::transform
using namespace std;

struct Log {
    string timestamp;
    string IP;
    string activity;
    int severity;
};


struct QueueNode {
    Log log;
    QueueNode* next;

    QueueNode(const Log& log) : log(log), next(NULL) {}
};

class LogQueue {
public:
    QueueNode* front;
    QueueNode* rear;

    LogQueue() : front(NULL), rear(NULL) {}

    bool isEmpty() {
        return front == NULL;
    }

    void enqueue(const Log& log) {
        QueueNode* newNode = new QueueNode(log);
        if (rear != NULL) { 
            rear->next = newNode;
        } else {  //if queue is empty
            front = newNode;
        }
        rear = newNode;
    }

    Log dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty.");
        }
        QueueNode* temp = front;
        Log log = temp->log;
        front = front->next;
        if (!front) {
            rear = NULL;
        }
        delete temp;
        return log;
    }

    ~LogQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

struct TreeNode {  //for binary tree
    Log log;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const Log& log) : log(log), left(NULL), right(NULL) {}
};

class BST {
public:
    TreeNode* root;

    BST() : root(NULL) {}

    TreeNode* insert(TreeNode* node, const Log& log) {
        if (node == NULL) return new TreeNode(log);

        if (log.timestamp < node->log.timestamp) {
            node->left = insert(node->left, log);
        } else {
            node->right = insert(node->right, log);
        }
        return node;
    }

    void inOrder(TreeNode* node, Log* logs, int& index) {
        if (node == NULL) return;
        inOrder(node->left, logs, index);
        logs[index++] = node->log;
        inOrder(node->right, logs, index);
    }

    TreeNode* search(TreeNode* node, const string& timestamp) {
        if (node == NULL || node->log.timestamp == timestamp) return node;
        if (timestamp < node->log.timestamp) return search(node->left, timestamp);
        return search(node->right, timestamp);
    }

    void destroy(TreeNode* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    void insert(const Log& log) {
        root = insert(root, log);
    }

    int getCount(TreeNode* node) {
        if (!node) return 0;
        return 1 + getCount(node->left) + getCount(node->right);
    }

    int getCount() {
        return getCount(root);
    }

    void sort(Log* logs, int count) {
        int index = 0;
        inOrder(root, logs, index);
    }

    Log* search(const string& timestamp) {
        TreeNode* result = search(root, timestamp);
        return result ? &result->log : NULL;
    }

    ~BST() {
        destroy(root);
    }
};

void merge(Log* logs, int left, int mid, int right) {
   int n1 = mid - left + 1;
   int n2 = right - mid;

   Log* L = new Log[n1];
   Log* R = new Log[n2];

   for (int i = 0; i < n1; ++i) L[i] = logs[left + i];
   for (int i = 0; i < n2; ++i) R[i] = logs[mid + 1 + i];

   int i = 0, j = 0, k = left;

   while (i < n1 && j < n2) {
       if (L[i].severity >= R[j].severity) {
           logs[k++] = L[i++];
       } else {
           logs[k++] = R[j++];
       }
   }

   while (i < n1) logs[k++] = L[i++];
   while (j < n2) logs[k++] = R[j++];

   delete[] L;
   delete[] R;
}

void mergeSort(Log* logs, int left, int right) {
   if (left >= right) return;

   int mid = left + (right - left) / 2;

   mergeSort(logs, left, mid);
   mergeSort(logs, mid + 1, right);
   merge(logs, left, mid, right);
}

void display(Log* logs, int count) {
   cout << "\nCategorized Logs:" << endl;

   for (int i = 0; i < count; ++i) {
       cout << "Timestamp: " << logs[i].timestamp << endl;
       cout << " | IP: " << logs[i].IP << endl;
       cout << " | Activity: " << logs[i].activity << endl;
       cout << " | Severity: " << logs[i].severity << endl;
       cout << "----------------------------------------" << endl;
   }
}

int main() {
   ifstream file("logs.csv");
   if (!file.is_open()) {
       cerr << "Error: Unable to open file." << endl;
       return 1;
   }

   LogQueue logQueue;                             
   string line;

   while (getline(file, line)) {
       stringstream ss(line);
       string timestamp, IP, activity, severityStr;

       getline(ss, timestamp, ',');                     //get timestamp from file
       getline(ss, IP, ',');
       getline(ss, activity, ',');
       getline(ss, severityStr);


       try {
           Log log;
           log.timestamp = timestamp; 
           log.IP = IP;               
           log.activity = activity;   
           log.severity = stoi(severityStr);         // Convert severity to integer

           logQueue.enqueue(log);                    // Enqueue the valid log
       } catch (const invalid_argument&) { 
           cerr << "Invalid severity value: " << severityStr << ". Skipping this entry." << endl; 
       } catch (const out_of_range&) { 
           cerr << "Severity value out of range: " << severityStr << ". Skipping this entry." << endl; 
       }
   }
   file.close();

   BST logBST;                                  
   while (!logQueue.isEmpty()) 
   {                    
       logBST.insert(logQueue.dequeue());
   }

   cout << "Choose an option:\n1. Search logs by timestamp\n2. Display logs sorted by severity\nEnter choice: ";  //prompt user
   
   int choice;
   cin >> choice;

   cin.ignore(); 

   if (choice == 1) 
   {                                                 // Search by timestamp
       cout << "Enter timestamp to search: ";
       string searchTimestamp; 
       getline(cin, searchTimestamp); 


       Log* result = logBST.search(searchTimestamp);
       if (result) 
       {                                            // If found
           cout << "\nLog Details:" << endl;
           cout << "Timestamp: " << result->timestamp << endl;
           cout << "IP: " << result->IP << endl;
           cout << "Activity: " << result->activity << endl;
           cout << "Severity: " << result->severity << endl;
       } 
       else 
       {                                             // If not found
           cout << "No log found with the given timestamp." << endl;
       }
   } 
   else if (choice == 2)                          // Display sorted logs
   { 
       int logCount = logBST.getCount();
       Log* logs = new Log[logCount];
       logBST.sort(logs, logCount);             // Sort by time stamps and enter logs in array

       mergeSort(logs, 0, logCount - 1);        // Sort by severity using merge sort
       display(logs, logCount);                 // Display sorted logs

       delete[] logs;                          
   } 
   else 
   { 
       cout << "Invalid choice." << endl;
   }

 return 0;
}