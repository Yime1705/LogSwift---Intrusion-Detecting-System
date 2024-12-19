# LogSwift - Intrusion Detecting System  

LogSwift is a C++-based Intrusion Detecting System designed to process, analyze, and categorize logs from a CSV file. It utilizes data structures like queues and binary search trees (BST), combined with merge sort, to enable efficient log management and intrusion detection.  

---

## Features  

- **Log Queueing**: Processes log entries using a dynamic queue.  
- **Binary Search Tree (BST)**: Stores and organizes logs by timestamp for quick search and retrieval.  
- **Timestamp Search**: Allows users to search logs by their timestamps.  
- **Severity Sorting**: Sorts logs by severity in descending order using merge sort.  
- **CSV File Support**: Reads logs directly from a file named `logs.csv`.  
- **Robust Error Handling**: Skips invalid or improperly formatted entries gracefully.  

---

## Installation  

### 1. Clone the Repository  

```bash  
git clone https://github.com/Yime1705/ids.git  
cd ids 
