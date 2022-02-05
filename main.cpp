#include <iostream>
#include<string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <chrono>


using namespace std;

class TreeNode {
public:
    long data;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(long x) : data(x), left(nullptr), right(nullptr) {}
};


// ================= Helper function =================== //
int height(TreeNode *root) {
    if (root == nullptr)
        return 0;
    return 1 + std::max(height(root->left), height(root->right));
}

TreeNode *findMin(TreeNode *root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}

int findBalanceFactor(TreeNode *root) {
    if (root == nullptr)
        return 0;
    else
        return height(root->left) - height(root->right);
}

TreeNode *rotateRight(TreeNode *root) {
    TreeNode *child = root->left;
    TreeNode *grandChild = child->right;
    child->right = root;
    root->left = grandChild;
    return child;
}

TreeNode *rotateLeft(TreeNode *root) {
    TreeNode *child = root->right;
    TreeNode *grandChild = child->left;
    child->left = root;
    root->right = grandChild;
    return child;
}

// ===================== BST Implementation ========================//

TreeNode *insertBST(TreeNode *root, long key) {
    if (root == nullptr)
        return new TreeNode(key);

    if (key < root->data)
        root->left = insertBST(root->left, key);
    else
        root->right = insertBST(root->right, key);

    return root;
}

TreeNode *removeBST(TreeNode *root, long target) {
    if (root == nullptr) {
        return nullptr;
    } else if (target < root->data)
        root->left = removeBST(root->left, target);
    else if (target > root->data)
        root->right = removeBST(root->right, target);

        // case 1: no child
    else if (root->left == nullptr && root->right == nullptr) {
        delete root;
        root = nullptr;
        return root;
    }

        // case 2: one child
    else if (root->left == nullptr) {
        TreeNode *temp = root;
        root = root->right;
        delete temp;
        return root;
    } else if (root->right == nullptr) {
        TreeNode *temp = root;
        root = root->left;
        delete temp;
        return root;
    }
        // case 3: two children
    else {
        TreeNode *temp = findMin(root->right);
        root->data = temp->data;
        root->right = temp->right;
        delete temp;
        return root;
    }
    return root;
}

void preorderBST(TreeNode *head) {
    if (head == nullptr)
        cout << "";
    else {
        cout << head->data << endl;
        preorderBST(head->left);
        preorderBST(head->right);
    }
}


// ===================== AVL Implementation ========================//

TreeNode *insertAVL(TreeNode *root, long key) {
    if (root == nullptr)
        return new TreeNode(key);

    if (key < root->data)
        root->left = insertBST(root->left, key);
    else
        root->right = insertBST(root->right, key);

    int factor = findBalanceFactor(root);
    // Left, Left Rotation
    if (factor > 1 && key < root->left->data)
        return rotateRight(root);
    // Right, Right Rotation
    if (factor < -1 && key > root->right->data)
        return rotateLeft(root);
    // Left, Right Rotation
    if (factor > 1 && key > root->left->data) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    // Right, Left Rotation
    if (factor < -1 && key < root->right->data) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}


TreeNode *removeAVL(TreeNode *root, long target) {
    if (root == nullptr) {
        return nullptr;
    } else if (target < root->data)
        root->left = removeAVL(root->left, target);
    else if (target > root->data)
        root->right = removeAVL(root->right, target);

        // case 1: no child
    else if (root->left == nullptr && root->right == nullptr) {
        delete root;
        root = nullptr;
        return root;
    }
        // case 2: one child
    else if (root->left == nullptr) {
        TreeNode *temp = root;
        root = root->right;
        delete temp;
        return root;
    } else if (root->right == nullptr) {
        TreeNode *temp = root;
        root = root->left;
        delete temp;
        return root;
    }
        // case 3: two children
    else {
        TreeNode *temp = findMin(root->right);
        root->data = temp->data;
        root->right = temp->right;
        delete temp;
        return root;
    }

    int factor = findBalanceFactor(root);
    // Right rotation parent == 2 and child == 1
    if (factor == 2 && findBalanceFactor(root->left) == 1)
        return rotateRight(root);
        // Left rotation  parent == -2 and child == -1
    else if (factor == -2 && findBalanceFactor(root->right) == -1)
        return rotateLeft(root);
        // Left, Right rotation parent == 2 and child == -1
    else if (factor == 2 && findBalanceFactor(root->left) == -1) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
        // Right, Left rotation parent == 2 and child == 1
    else if (factor == -2 && findBalanceFactor(root->right) == 1) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    return root;
}

void preorderAVL(TreeNode *head) {
    if (head == nullptr)
        cout << "";
    else {
        cout << head->data << endl;
        preorderBST(head->left);
        preorderBST(head->right);
    }
}


// ====================== Input Data file generator ======================== //

void InputDataGenerator(vector<long> vec, const string &sizeOfData) {
    ofstream myfile;
    // Random data generation
    myfile.open(sizeOfData + " Random.txt");
    myfile << sizeOfData << " " << " Random" << endl;
    for (long i: vec) {
        myfile << i << endl;
    }
    myfile.close();

    // Ascending data generation
    myfile.open(sizeOfData + " Ascending.txt");
    myfile << sizeOfData << " Ascending" << endl;
    sort(vec.begin(), vec.end());
    for (long i: vec) {
        myfile << i << endl;
    }
    myfile.close();

    // Descending data generation
    sort(vec.begin(), vec.end(), greater<long>());
    myfile.open(sizeOfData + " Descending.txt");
    myfile << sizeOfData << " Descending" << endl;
    for (long i: vec) {
        myfile << i << endl;
    }
    myfile.close();
}

// ===================== Printing out time ======================== //
void printTime(const string &str, clock_t start, clock_t end) {

    double timeTaken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by " << str << " is : " << fixed
         << timeTaken << setprecision(10) << " sec " << endl;
}


int main() {
    // ================== creating input files by changing size ============== //

  /*  I have commented this portion to avoid creating files at every run
     to check the implemented file creator functionality please uncomment this portion
     at the below */

    // change the number of input such 100, 1000, and 10000
    /* int size = 10000;
     // Change size of data in Small, Medium, Large
     string sizeOfData = "Large";

     vector<long> vec;
    // Random number generator
     srand(time(0));
     vec.reserve(size);
     for (int i = 0; i < size; i++) {
         vec.push_back(rand());
     }
     // calling input file creator function
     InputDataGenerator(vec, sizeOfData);*/

    TreeNode *root = nullptr;
    string str;
    ifstream infile;
    vector<long> vec1;
    clock_t start, end;
    int counter = 0;

    // Ascending, Descending, Random
    // change the name of file to see its execution time
    infile.open("Large Ascending.txt");
    while (!infile.eof())
    {
        getline(infile, str);
        if (!str.empty() && str.find_first_not_of("0123456789") == string::npos) {
            vec1.push_back(stol(str));
        }
    }
    infile.close();

    // ============== BST insertion ===================//
    start = clock();
    for (auto &i: vec1) {
        root = insertBST(root, i);
        //counter++;
    }
    //cout << "BST Insertion count: " << counter << endl;
    end = clock();
    printTime("BST Insertion", start, end);
    //counter = 0;

    // =============== BST pre order traversal ================//
    start = clock();
    preorderBST(root);
    end = clock();
    printTime("BST Traversal", start, end);

    //================== BST deletion ======================//
    start = clock();
    for (auto &i: vec1) {
        root = removeBST(root, i);
        //counter++;
    }
    //cout << "BST Deletion count: " << counter << endl;
    //counter = 0;
    end = clock();
    printTime("BST Deletion", start, end);

    //=========================== AVL insertion ====================//
    start = clock();
    for (auto &i: vec1) {
        root = insertAVL(root, i);
        //counter++;
    }
    //cout << "AVL Insertion count: " << counter << endl;
    //counter = 0;
    end = clock();
    printTime("AVL Insertion", start, end);

    //=========================== AVL pre order print ====================//
    start = clock();
    preorderAVL(root);
    end = clock();
    printTime("AVL Traversal", start, end);

    //========================== AVL deletion ===========================//
    start = clock();
    for (auto &i: vec1) {
        root = removeAVL(root, i);
        //counter++;
    }
    //cout << "AVL Deletion count: " << counter << endl;
    //counter = 0;
    end = clock();
    printTime("AVL Deletion", start, end);

    return 0;
}
