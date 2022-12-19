#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <regex>
#include <chrono>
using namespace std::chrono;

std::string fixCapitalization(std::string _input)
{
    for (int i = 0; i < _input.size(); i++)
    {
        _input[i] = tolower(_input[i]);
    }

    _input[0] = toupper(_input[0]);

    for (int i = 1; i < _input.size(); i++)
    {
        if (_input[i - 1] == ' ')
        {
            _input[i] = toupper(_input[i]);
        }
    }
    return _input;
}


class Job {
private:
    std::string job;
    std::string location;
    std::string salary;
    std::string education;
    std::string experience;
    int educationValue = 0;

    void setEducationValue(std::string temp);

public:
    Job();
    Job(std::string typeOfJob, std::string jobLocation, std::string jobSalary, std::string typeOfEducation, std::string numExperience);
    std::string GetJob();
    std::string GetLocation();
    int GetSalary();
    std::string GetEducation();
    int getEducationValue();
    int GetExperience();

};

void Job::setEducationValue(std::string temp) {
    if (temp == "") {
        educationValue = 0;
    }
    else if (temp == "high school diploma") {
        educationValue = 1;
    }
    else if (temp == "college associate") {
        educationValue = 2;
    }
    else if (temp == "college  bachelor's") {
        educationValue = 3;
    }
    else if (temp == "college master's") {
        educationValue = 4;
    }
    else if (temp == "doctoral") {
        educationValue = 5;
    }
}

Job::Job() {
    job = "";
    location = "";
    salary = "";
    education = "";
    experience = "";
}

Job::Job(std::string typeOfJob, std::string jobLocation, std::string jobSalary, std::string typeOfEducation, std::string numExperience) {
    job = typeOfJob;
    location = jobLocation;
    salary = jobSalary;
    education = typeOfEducation;
    experience = numExperience;

}

std::string Job::GetJob() {
    return job;
}

std::string Job::GetLocation() {
    return location;
}

int Job::GetSalary() {
    return stoi(salary);
}

std::string Job::GetEducation() {
    return education;
}

int Job::getEducationValue() {
    return educationValue;
}

int Job::GetExperience() {
    return stoi(experience);
}

// location nodes
struct LocationTreeNode {
    std::string loc;
    std::vector<Job> j;
    LocationTreeNode* parent;
    LocationTreeNode* left;
    LocationTreeNode* right;
    int color;
};

// red black tree sorted by location
struct LocationTree {
    LocationTreeNode* root;
    LocationTreeNode* nil;

    // rb tree constructor
    LocationTree()
    {
        nil = new LocationTreeNode;
        nil->color = 0;
        nil->left = nullptr;
        nil->right = nullptr;
        root = nil;
    }

    // searches tree by location
    std::vector<Job> searchTreeLocation(LocationTreeNode* node, std::string _location)
    {
        if (node == nil || _location == node->loc) {
            std::vector<Job> r;

            if (node == nil)
            {
                return r;
            }

            else
            {
                return node->j;
            }
        }

        if (_location < node->loc)
        {
            return searchTreeLocation(node->left, _location);
        }
        return searchTreeLocation(node->right, _location);
    }

    // leftRotate tree
    void leftRotate(LocationTreeNode* x)
    {
        LocationTreeNode* y = x->right;
        x->right = y->left;

        if (y->left != nil)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            this->root = y;
        }

        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }

        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    // rightRotate tree
    void rightRotate(LocationTreeNode* x)
    {
        LocationTreeNode* y = x->left;
        x->left = y->right;

        if (y->right != nil)
        {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            this->root = y;
        }

        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }

        else
        {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    // balances tree after insertion
    void insertFix(LocationTreeNode* n)
    {
        LocationTreeNode* u;
        while (n->parent->color == 1)
        {
            if (n->parent == n->parent->parent->right)
            {
                u = n->parent->parent->left;
                if (u->color == 1)
                {
                    u->color = 0;
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    n = n->parent->parent;
                }
                else
                {
                    if (n == n->parent->left)
                    {
                        n = n->parent;
                        rightRotate(n);
                    }
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    leftRotate(n->parent->parent);
                }
            }
            else
            {
                u = n->parent->parent->right;

                if (u->color == 1)
                {
                    u->color = 0;
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    n = n->parent->parent;
                }
                else {
                    if (n == n->parent->right)
                    {
                        n = n->parent;
                        leftRotate(n);
                    }
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    rightRotate(n->parent->parent);
                }
            }
            if (n == root)
            {
                break;
            }
        }
        root->color = 0;
    }

    // inserts node into tree sorted by location
    void insert(Job _job)
    {
        LocationTreeNode* node = new LocationTreeNode;
        node->parent = nullptr;
        node->loc = _job.GetLocation();
        node->left = nil;
        node->j.push_back(_job);
        node->right = nil;
        node->color = 1;

        LocationTreeNode* y = nullptr;
        LocationTreeNode* find = this->root;

        while (find != nil)
        {
            y = find;
            if (node->loc == find->loc)
            {
                break;
            }
            if (node->loc < find->loc)
            {
                find = find->left;
            }
            else
            {
                find = find->right;
            }
        }

        if (node->loc == find->loc)
        {
            find->j.push_back(_job);
            delete node;
        }
        else {
            node->parent = y;
            if (y == nullptr)
            {
                root = node;
            }
            else if (node->loc < y->loc)
            {
                y->left = node;
            }
            else
            {
                y->right = node;
            }

            if (node->parent == nullptr)
            {
                node->color = 0;
                return;
            }

            if (node->parent->parent == nullptr)
            {
                return;
            }
            insertFix(node);
        }
    }
};

// job tree node
struct JobTreeNode {
    std::string title;
    LocationTree rbt;
    JobTreeNode* parent;
    JobTreeNode* left;
    JobTreeNode* right;
    int color;
};

// red black tree sorted by job title
// nodes contain trees of job locations
// map<jobs, map<locations, jobInfo>>
struct JobTree {
    JobTreeNode* root;
    JobTreeNode* nil;

    // rb tree constructor
    JobTree()
    {
        nil = new JobTreeNode;
        nil->color = 0;
        nil->left = nullptr;
        nil->right = nullptr;
        root = nil;
    }

    // searches tree by title
    JobTreeNode* searchTree(JobTreeNode* node, std::string _title)
    {
        if (node == nil || _title == node->title) {
            return node;
        }

        if (_title < node->title)
        {
            return searchTree(node->left, _title);
        }
        return searchTree(node->right, _title);
    }

    // leftRotate tree
    void leftRotate(JobTreeNode* x)
    {
        JobTreeNode* y = x->right;
        x->right = y->left;

        if (y->left != nil)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            this->root = y;
        }

        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }

        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    // rightRotate tree
    void rightRotate(JobTreeNode* x)
    {
        JobTreeNode* y = x->left;
        x->left = y->right;

        if (y->right != nil)
        {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == nullptr)
        {
            this->root = y;
        }

        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }

        else
        {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    // balances tree after insertion
    void insertRBTFix(JobTreeNode* n)
    {
        JobTreeNode* u;
        while (n->parent->color == 1)
        {
            if (n->parent == n->parent->parent->right)
            {
                u = n->parent->parent->left;
                if (u->color == 1)
                {
                    u->color = 0;
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    n = n->parent->parent;
                }
                else
                {
                    if (n == n->parent->left)
                    {
                        n = n->parent;
                        rightRotate(n);
                    }
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    leftRotate(n->parent->parent);
                }
            }
            else
            {
                u = n->parent->parent->right;

                if (u->color == 1)
                {
                    u->color = 0;
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    n = n->parent->parent;
                }
                else {
                    if (n == n->parent->right)
                    {
                        n = n->parent;
                        leftRotate(n);
                    }
                    n->parent->color = 0;
                    n->parent->parent->color = 1;
                    rightRotate(n->parent->parent);
                }
            }
            if (n == root)
            {
                break;
            }
        }
        root->color = 0;
    }

    // inserts node into tree sorted by title
    void insertRBT(std::string _title, Job _job)
    {
        JobTreeNode* node = new JobTreeNode;
        node->parent = nullptr;
        node->title = _title;
        node->left = nil;
        node->right = nil;
        node->color = 1;

        JobTreeNode* y = nullptr;
        JobTreeNode* find = this->root;

        while (find != nil)
        {
            y = find;
            if (node->title == find->title)
            {
                break;
            }
            if (node->title < find->title)
            {
                find = find->left;
            }
            else
            {
                find = find->right;
            }
        }

        if (node->title == find->title)
        {
            find->rbt.insert(_job);
            delete node;
        }
        else
        {
            node->parent = y;
            if (y == nullptr)
            {
                root = node;
                node->rbt = LocationTree();
            }
            else if (node->title < y->title)
            {
                y->left = node;
            }
            else
            {
                y->right = node;
            }

            if (node->parent == nullptr)
            {
                node->color = 0;
                return;
            }

            if (node->parent->parent == nullptr)
            {
                return;
            }
            insertRBTFix(node);
        }
    }
};

class HashTable {
private:
    int hashSize;
    std::vector<Job>* array;
    int capacity;
    float currentLoadFactor;
    float maxLoadFactor = 0.75f;

    long Hash(std::string temp);
    bool has(std::string temp);
    void helpSearch(std::string location, std::string salary, std::string education, std::string experience, int index);
    void sortVector(std::vector<std::pair<std::string, Job>>& temp);
    int educationValue(std::string temp);
    std::string lowerCase(std::string temp);
    void capitalizeFirstLetter(std::string& temp);

public:
    HashTable();
    void insert(Job temp);
    void search(std::string job, std::string location, std::string salary, std::string education, std::string experience);
};

HashTable::HashTable() {
    hashSize = 0;
    capacity = 100;
    array = new std::vector<Job>[capacity];
    currentLoadFactor = 0.0f;
}

long HashTable::Hash(std::string temp) {
    long x = 31;
    long hash = 0;
    for (long i = 0; i < temp.length(); i++) {
        hash = x * hash + temp[i];
    }
    return hash;
}

void HashTable::insert(Job temp) {
    long valueIndex = (Hash(temp.GetJob()) & 0x7fffffff) % capacity;
    int maxSearch = valueIndex - 1;
    while (valueIndex != maxSearch) {
        if (array[valueIndex].empty()) {
            array[valueIndex].push_back(temp);
            hashSize++;
            break;
        }
        else if (array[valueIndex][0].GetJob() == temp.GetJob()) {
            array[valueIndex].push_back(temp);
            hashSize++;
            break;
        }
        valueIndex++;
        if (valueIndex == capacity) {
            valueIndex = 0;
        }
    }


    currentLoadFactor = hashSize / 1.0f;
    currentLoadFactor = currentLoadFactor / capacity;
    if (currentLoadFactor >= maxLoadFactor) {
        hashSize = 0;
        std::vector <Job>* originalArray = array;
        capacity = capacity * 2;
        array = new std::vector<Job>[capacity * 2];
        for (int i = 0; i < capacity / 2; i++) {
            if (!originalArray[i].empty()) {
                for (int j = 0; j < originalArray[i].size(); j++) {
                    insert(originalArray[i][j]);
                }
            }
        }
        delete[] originalArray;
    }
}


void HashTable::search(std::string job, std::string location, std::string salary, std::string education, std::string experience) {
    capitalizeFirstLetter(job);
    if (!has(job)) {
        std::cout << "Job searched is not in the database" << std::endl;
    }
    else {
        long valueIndex = (Hash(job) & 0x7fffffff) % capacity;
        int maxSearch = valueIndex - 1;
        while (valueIndex != maxSearch) {
            if (array[valueIndex][0].GetJob() == job) {
                helpSearch(location, salary, education, experience, valueIndex);
                break;
            }
            valueIndex++;
            if (valueIndex == capacity) {
                valueIndex = 0;
            }
        }
    }
}

bool HashTable::has(std::string temp) {
    long valueIndex = (Hash(temp) & 0x7fffffff) % capacity;
    int maxSearch = valueIndex - 1;
    while (valueIndex != maxSearch) {
        if (array[valueIndex].empty()) {
            return false;
        }
        else if (array[valueIndex][0].GetJob() == temp) {
            return true;
        }
        valueIndex++;
        if (valueIndex == capacity) {
            valueIndex = 0;
        }
    }
}

void HashTable::helpSearch(std::string location, std::string salary, std::string education, std::string experience, int index) {
    int totalJobs = 0;
    std::vector <std::pair<std::string, Job>> searchedJobs;
    for (int i = 0; i < array[index].size(); i++) {
        int count = 0;
        bool state = false;
        bool userSalary = false;
        bool userEducation = false;
        bool userExperience = false;
        std::string tempLocation = lowerCase(array[index][i].GetLocation());
        std::string tempEducation = lowerCase(array[index][i].GetEducation());;
        if (location == "" || lowerCase(location) == tempLocation) {
            count++;
            state = true;
        }
        if (salary == "" || stoi(salary) <= array[index][i].GetSalary()) {
            count++;
            userSalary = true;
        }
        if (education == "" || educationValue(lowerCase(education)) >= educationValue(tempEducation)) {
            count++;
            userEducation = true;
        }
        if (experience == "" || stoi(experience) >= array[index][i].GetExperience()) {
            userExperience = true;
            count++;
        }
        if (count > 0 && state == true && userSalary == true && userEducation == true && userExperience == true) {
            searchedJobs.push_back(std::make_pair(array[index][i].GetLocation(), array[index][i]));
            totalJobs++;
        }
    }
    sortVector(searchedJobs);
    if (totalJobs > 0) {
        std::cout << "These listing match closest to your search: " << std::endl;
        for (int i = 0; i < searchedJobs.size(); i++) {
            if (searchedJobs[i].second.GetExperience() == 0) {
                std::cout << searchedJobs[i].second.GetJob() << " " << searchedJobs[i].second.GetLocation() << " " << searchedJobs[i].second.GetSalary() << " " << searchedJobs[i].second.GetEducation() << " " << "No prior experience required" << std::endl;
            }
            else {
                std::cout << searchedJobs[i].second.GetJob() << " " << searchedJobs[i].second.GetLocation() << " " << searchedJobs[i].second.GetSalary() << " " << searchedJobs[i].second.GetEducation() << " " << searchedJobs[i].second.GetExperience() << " year(s) of experience required" << std::endl;
            }
        }
    }
    else {
        std::cout << "There are no listings that match any of your filters" << std::endl;
    }

}

void HashTable::sortVector(std::vector<std::pair<std::string, Job>>& temp) {
    std::sort(temp.begin(), temp.end(), [](auto& left, auto& right) {
        return left.first < right.first;
        });
}

int HashTable::educationValue(std::string temp) {
    if (temp == "") {
        return 0;
    }
    else if (temp == "high school diploma") {
        return 1;
    }
    else if (temp == "college associate") {
        return 2;
    }
    else if (temp == "college bachelor's") {
        return 3;
    }
    else if (temp == "college master's") {
        return 4;
    }
    else if (temp == "doctoral") {
        return 5;
    }
    else {
        return -1;
    }
}

std::string HashTable::lowerCase(std::string temp) {
    std::transform(temp.begin(), temp.end(), temp.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return temp;
}

void HashTable::capitalizeFirstLetter(std::string& temp) {
    for (int x = 0; x < temp.length(); x++)
    {
        if (x == 0)
        {
            temp[x] = toupper(temp[x]);
        }
        else if (temp[x - 1] == ' ')
        {
            temp[x] = toupper(temp[x]);
        }
    }
}

bool checkJobName(std::regex word, std::string command);
bool checkNumber(std::string);
void lowerCase(std::string& temp);
bool checkWord(std::regex word, std::string command);


int main() {

    std::string userJob;
    std::string userLocation;
    std::string userSalary;
    std::string userEducation;
    std::string userExperience;
    std::string userInput;
    bool dataStructure = false;
    bool userContinue = true;
    bool hash = false;
    bool map = false;
    bool isNumber = false;
    bool isNumExperience = false;
    bool isJob = false;
    bool isState = false;
    bool isDegree = false;
    bool response1 = false;
    bool response2 = false;
    std::string x;
    int numFilters = 0;
    std::regex jobName = std::regex("[A-Za-z[:space:]]+");
    std::regex word = std::regex("[A-Za-z\"[:space:]]+");
    std::regex num = std::regex("[0-9]+");

    std::string location[50] = { "Alabama", "Alaska", "Arizona", "Arkansas", "California", "Colorado", "Connecticut", "Delaware", "Florida", "Georgia",
                                 "Hawaii", "Idaho", "Illinois", "Indiana", "Iowa", "Kansas", "Kentucky", "Louisiana", "Maine", "Maryland",
                                 "Massachusetts", "Michigan", "Minnesota", "Mississippi", "Missouri", "Montana", "Nebraska", "Nevada", "New Hampshire", "New Jersey",
                                 "New Mexico", "New York", "North Carolina", "North Dakota", "Ohio", "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina",
                                 "South Dakota", "Tennessee", "Texas", "Utah", "Vermont", "Virginia", "Washington", "West Virginia", "Wisconsin", "Wyoming" };

    std::string job[25] = { "Computer Engineer", "Software Developer", "Electrical Engineer", "Civil Engineer", "Aerospace Engineer",
                            "Agricultural Engineer", "Automotive Engineer", "Biomedical Engineer", "Chemical Engineer", "Civil Engineer",
                            "Data Analyst", "Mechanical Engineer", "Petroleum Engineer", "Product Design Manager", "Custodian",
                            "Architect", "Teacher", "Accountant", "Auditor", "Lawyer",
                            "Sales Manager", "App Developer", "Dentist", "University Professor", "Real Estate Agent" };

    std::string salary[25] = { "35000", "40000", "45000", "50000", "55000",
                               "60000", "65000", "70000", "75000", "80000",
                               "85000", "90000", "95000", "100000", "105000",
                               "110000", "115000", "120000", "125000", "130000",
                               "135000", "140000", "145000", "150000", "155000" };

    std::string education[5] = { "High School Diploma", "College Associate", "College Bachelor's", "College Master's", "Doctoral" };

    std::string experience[11] = { "0", "1", "2", "3", "4", "5",
                                   "6", "7", "8", "9", "10" };

    HashTable table;
    JobTree tree;
    for (int i = 0; i < 100000; i++) {
        int v1 = rand() % 25;
        int v2 = rand() % 50;
        int v3 = rand() % 25;
        int v4 = rand() % 5;
        int v5 = rand() % 11;

        std::string typeOfJob = job[v1];
        std::string jobLocation = location[v2];
        std::string jobSalary = salary[v3];
        std::string jobEducation = education[v4];
        std::string jobExperience = experience[v5];
        Job newJob(typeOfJob, jobLocation, jobSalary, jobEducation, jobExperience);
        table.insert(newJob);
        tree.insertRBT(job[v1], newJob);
        // Miguel insert each node into your Red-Black tree in this for loop
    }

    std::cout << "Welcome to the job search engine. With this engine we give users the possibility to search for jobs" << std::endl;
    std::cout << "using two different data structures." << std::endl;
    std::cout << std::endl;
    std::cout << "Before you can begin here is some background information to help use the engine" << std::endl;
    std::cout << "1. While it does not matter which letters are capitalized, spelling and spacing can affect searching" << std::endl;
    std::cout << "2. Not every possible job you can think of is currently in our database. Therefore, we will list all the jobs currently available" << std::endl;
    std::cout << "3. You will be asked at the start which data structure you would like to use: hash table or map" << std::endl;
    std::cout << "4. When searching you will be asked to provide input for each filter. If you don't care about that specific filter click enter without typing anything (except for job name)" << std::endl;
    std::cout << "5. After each search you will be asked if you would like to search again and if you want to change the current data structure being used" << std::endl;
    std::cout << "Here's the list of jobs: " << std::endl;
    int count = 0;
    for (int i = 0; i < sizeof(job) / sizeof(job[0]); i++) {
        if (i < (sizeof(job) / sizeof(job[0])) - 1) {
            std::cout << job[i] << ", ";
        }
        else {
            std::cout << job[i];
        }
        if (count == 4) {
            std::cout << std::endl;
            count = 0;
            continue;
        }
        count++;

    }
    std::cout << std::endl;
    std::cout << "That's all you need to know. Lets begin" << std::endl;

    while (dataStructure == false) {
        std::cout << "Which data structure would you like to use: hash table (type 1) or map (type 2): ";
        getline(std::cin, x);
        if (x == "1" || x == "2") {
            dataStructure = true;
        }
        else {
            std::cout << "Please type 1 or 2 to select data structure" << std::endl;
        }
    }


    while (userContinue) {
        if (x == "1") {
            isJob = false;
            isState = false;
            isNumber = false;
            isNumExperience = false;
            isDegree = false;
            response1 = false;
            response2 = false;

            while (isJob == false) {
                std::cout << "What job are you searching for: ";
                getline(std::cin, userJob);
                if (checkJobName(jobName, userJob)) {
                    isJob = true;
                    lowerCase(userJob);
                }
                else {
                    std::cout << "Error: please enter a job name" << std::endl;
                }
            }

            while (isState == false) {
                std::cout << "Where you would like to work (input state in the United States): ";
                getline(std::cin, userLocation);
                if (checkWord(word, userLocation)) {
                    isState = true;
                }
                else if (userLocation == "") {
                    isState = true;
                }
                else {
                    std::cout << "Error: please enter a state or leave blank" << std::endl;
                }
            }

            while (isNumber == false) {
                std::cout << "What salary do you want: ";
                getline(std::cin, userSalary);
                if (checkNumber(userSalary)) {
                    isNumber = true;
                }
                else if (userSalary == "") {
                    isNumber = true;
                }
                else {
                    std::cout << "Error: please enter a number as your salary (no commas or symbols) or leave blank" << std::endl;
                }
            }

            while (isDegree == false) {
                std::cout << "What form of education do you have (High School Diploma, College Associate, College Bachelor's, College Master's, Doctoral): ";
                getline(std::cin, userEducation);
                if (checkWord(word, userEducation)) {
                    isDegree = true;
                }
                else if (userEducation == "") {
                    isDegree = true;
                }
                else {
                    std::cout << "Error: please enter a form of education or leave blank" << std::endl;
                }
            }

            while (isNumExperience == false) {
                std::cout << "How many years of experience do you have: ";
                getline(std::cin, userExperience);
                if (checkNumber(userExperience)) {
                    isNumExperience = true;
                }
                else if (userExperience == "") {
                    isNumExperience = true;
                }
                else {
                    std::cout << "Error: please enter a number as your experience (no commas or symbols) or leave blank" << std::endl;
                }
            }

            auto start = high_resolution_clock::now();
            table.search(userJob, userLocation, userSalary, userEducation, userExperience);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

            while (response1 == false) {
                std::cout << "Would you like to continue searching: ";
                getline(std::cin, userInput);
                lowerCase(userInput);
                if (userInput == "no") {
                    response1 = true;
                    userContinue = false;
                }
                else if (userInput == "yes") {
                    response1 = true;
                }
                else {
                    std::cout << "Please type yes or no" << std::endl;
                }
            }
            while (response2 == false && userContinue == true) {
                std::cout << "Would you like to switch data structures: ";
                getline(std::cin, userInput);
                lowerCase(userInput);
                if (userInput == "no") {
                    response2 = true;
                }
                else if (userInput == "yes") {
                    x = "2";
                    response2 = true;
                }
                else {
                    std::cout << "Please type yes or no" << std::endl;
                }
            }
        }
        else if (x == "2") {
            isJob = false;
            isState = false;
            isNumber = false;
            isNumExperience = false;
            isDegree = false;
            response1 = false;
            response2 = false;

            while (isJob == false) {
                std::cout << "What job are you searching for: ";
                getline(std::cin, userJob);
                if (checkJobName(word, userJob)) {
                    isJob = true;
                }
                else {
                    std::cout << "Error: please enter a job name" << std::endl;
                }
            }
            userJob = fixCapitalization(userJob);
            while (isState == false) {
                std::cout << "Where you would like to work (input state in the United States): ";
                getline(std::cin, userLocation);
                if (checkWord(word, userLocation)) {
                    isState = true;
                }
                else if (userLocation == "") {
                    isState = true;
                }
                else {
                    std::cout << "Error: please enter a state or leave blank" << std::endl;
                }
            }
            userLocation = fixCapitalization(userLocation);

            while (isNumber == false) {
                std::cout << "What salary do you want: ";
                getline(std::cin, userSalary);
                if (checkNumber(userSalary)) {
                    isNumber = true;
                }
                else if (userSalary == "") {
                    isNumber = true;
                }
                else {
                    std::cout << "Error: please enter a number as your salary (no commas or symbols) or leave blank" << std::endl;
                }
            }

            while (isDegree == false) {
                std::cout << "What form of education do you have (High School Diploma, College Associate, College Bachelor's, College Master's, Doctoral): ";
                getline(std::cin, userEducation);
                if (checkWord(word, userEducation)) {
                    isDegree = true;
                }
                else if (userEducation == "") {
                    isDegree = true;
                }
                else {
                    std::cout << "Error: please enter a form of education or leave blank" << std::endl;
                }
            }
            userEducation = fixCapitalization(userEducation);

            while (isNumExperience == false) {
                std::cout << "How many years of experience do you have: ";
                getline(std::cin, userExperience);
                if (checkNumber(userExperience)) {
                    isNumExperience = true;
                }
                else if (userExperience == "") {
                    isNumExperience = true;
                }
                else {
                    std::cout << "Error: please enter a number as your experience (no commas or symbols) or leave blank" << std::endl;
                }
            }
            auto start = high_resolution_clock::now();
            JobTreeNode* j = tree.searchTree(tree.root, userJob);

            if (tree.searchTree(tree.root, userJob) == tree.nil)
            {
                std::cout << "Job searched not in database." << std::endl;
            }

            // creates vector of jobs
            else
            {
                std::vector<Job> v1;
                if (!userLocation.empty())
                {
                    v1 = j->rbt.searchTreeLocation(j->rbt.root, userLocation);
                }

                else
                {
                    for (int i = 0; i < 50; i++)
                    {
                        std::vector<Job> v2 = j->rbt.searchTreeLocation(j->rbt.root, location[i]);
                        v1.insert(v1.end(), v2.begin(), v2.end());
                    }
                }

                // filters by salary
                if (!userSalary.empty())
                {
                    for (int i = 0; i < v1.size(); i++)
                    {
                        if (v1[i].GetSalary() < stoi(userSalary))
                        {
                            v1.erase(v1.begin() + i);
                            i--;
                        }
                    }
                }

                // filters by education
                if (!userEducation.empty())
                {
                    int i;
                    bool found = false;
                    for (i = 0; i < education->size(); i++)
                    {
                        if (education[i] == userEducation)
                        {
                            found = true;
                            break;
                        }
                    }

                    if (found)
                    {
                        for (int k = 0; k < v1.size(); k++)
                        {
                            bool match = false;
                            int l = 0;
                            while (l <= i)
                            {
                                if (v1[k].GetEducation() == education[l])
                                {
                                    match = true;
                                }
                                l++;
                            }
                            if (!match)
                            {
                                v1.erase(v1.begin() + k);
                                k--;
                            }
                        }
                    }
                    else
                    {
                        while (v1.size() > 0)
                        {
                            v1.erase(v1.begin());
                        }
                    }
                }

                // filters by experience
                if (!userExperience.empty())
                {
                    int i = 0;
                    while (experience[i] != userExperience)
                    {
                        i++;
                    }
                    for (int k = 0; k < v1.size(); k++)
                    {
                        bool match = false;
                        int l = 0;
                        while (l <= i)
                        {
                            if (v1[k].GetExperience() == stoi(experience[l]))
                            {
                                match = true;
                            }
                            l++;
                        }
                        if (!match)
                        {
                            v1.erase(v1.begin() + k);
                            k--;
                        }
                    }
                }

                // prints resultant jobs
                if (v1.size() == 0)
                {
                    std::cout << "There are no listings that match any of your filters." << std::endl;
                }
                else
                {
                    std::cout << "These listings match closest to your search:" << std::endl;
                    for (int j = 0; j < v1.size(); j++)
                    {
                        std::cout << v1[j].GetJob() << " " << v1[j].GetLocation() << " " << v1[j].GetSalary()
                            << " " << v1[j].GetEducation() << " ";
                        if (v1[j].GetExperience() == 0)
                        {
                            std::cout << "No prior experience required" << std::endl;
                        }
                        else
                        {
                            std::cout << v1[j].GetExperience() << " year(s) of experience required" << std::endl;
                        }
                    }
                }
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
            //Do your form of searching using variables above
            //Remember that some of the variables are empty std::strings as well aka users searchers don't care about those filters

            while (response1 == false) {
                std::cout << "Would you like to continue searching: ";
                getline(std::cin, userInput);
                lowerCase(userInput);
                if (userInput == "no") {
                    userContinue = false;
                    break;
                }
                else if (userInput == "yes") {
                    response1 = true;
                }
                else {
                    std::cout << "Please type yes or no" << std::endl;
                }
            }
            while (response2 == false && userContinue == true) {
                std::cout << "Would you like to switch data structures: ";
                getline(std::cin, userInput);
                lowerCase(userInput);
                if (userInput == "no") {
                    response2 = true;
                }
                else if (userInput == "yes") {
                    x = "1";
                    response2 = true;
                }
                else {
                    std::cout << "Please type yes or no" << std::endl;
                }
            }
        }
    }
    return 0;
}

bool checkJobName(std::regex word, std::string command) {
    // Makes sure input for name job name is only letters and spaces
    if (std::regex_match(command, word) == true) {
        return true;
    }
    return false;
}

bool checkNumber(std::string search) {
    // Makes sure string provided for salary is a number
    for (int i = 0; i < search.length(); i++) {
        if (isdigit(search[i]) == false) {
            return false;
        }
    }
    return true;
}

void lowerCase(std::string& temp) {
    std::transform(temp.begin(), temp.end(), temp.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

bool checkWord(std::regex word, std::string command) {
    // Makes sure input for name job name is only letters and spaces
    if (std::regex_match(command, word) == true) {
        return true;
    }
    return false;
}