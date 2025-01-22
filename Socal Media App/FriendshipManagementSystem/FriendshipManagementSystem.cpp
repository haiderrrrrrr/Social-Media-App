/*  Haider Ali                       22i-2655    */
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <regex>
#include "Date.h"
#include "User.h"
#include "Post.h"
#include "Comment.h"
#include "Like.h"
#include "Page.h"
#include "Group.h"
#include "Chat.h"

using namespace std;

// Class to manage user operations
class UserManager {
private:
    User* users;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        User* temp = new User[capacity];
        for (int i = 0; i < size; i++) {
            temp[i] = users[i];
        }
        delete[] users;
        users = temp;
    }

    string generateUserId() {
        stringstream ss;
        ss << "user" << time(nullptr) << (size + 1); // Simple unique ID generation
        return ss.str();
    }

    User* getUserByEmail(const string& email) {
        for (int i = 0; i < size; i++) {
            if (users[i].email == email) {
                return &users[i];
            }
        }
        return nullptr;
    }

    string getLoggedInUserName(const string& email) {
        User* user = getUserByEmail(email);
        if (user) {
            return user->name;
        }
        return "";
    }

    User* getUserById(const string& userid) {
        for (int i = 0; i < size; i++) {
            if (users[i].userid == userid) {
                return &users[i];
            }
        }
        return nullptr;
    }

    void loadFromFile() {
        ifstream file("users.txt");
        string line;
        User tempUser;

        while (getline(file, line)) {
            stringstream ss(line);
            string item;

            getline(ss, tempUser.userid, ',');
            getline(ss, tempUser.name, ',');
            getline(ss, tempUser.email, ',');
            getline(ss, tempUser.password, ',');
            getline(ss, tempUser.birthdate);

            if (size == capacity) {
                resize();
            }
            users[size++] = tempUser;
        }
        file.close();
    }

    void loadUserPosts(User& user) {
        ifstream file(user.userid + "_posts.txt");
        string line;
        while (getline(file, line)) {
            cout << line << endl; // Display each post
        }
        file.close();
    }

    string generatePostId() {
        stringstream ss;
        ss << "post" << time(nullptr);
        return ss.str();
    }

    void loadAndDisplayUserPosts(const string& userId) {
        ifstream file(userId + "_posts.txt");
        if (!file.is_open() || file.peek() == ifstream::traits_type::eof()) {
            cout << "No posts from this user yet." << endl;
            return;
        }

        string line;
        int postNumber = 1;
        cout << "Posts from User ID " << userId << ":" << endl;
        while (getline(file, line)) {
            stringstream ss(line);
            string postId, content;
            getline(ss, postId, ',');
            getline(ss, content);

            cout << "Post number: " << postNumber++ << endl;
            cout << "Content: " << content << endl;
            displayLikesAndComments(postId);

            cout << "--------------------------------------" << endl;
        }
    }

    void displayLikesAndComments(const string& postId) {
        ifstream likeFile(postId + "_likes.txt");
        int likeCount = count(istreambuf_iterator<char>(likeFile), istreambuf_iterator<char>(), '\n');
        cout << "Likes: " << likeCount << endl;

        ifstream commentFile(postId + "_comments.txt");
        cout << "Comments:" << endl;
        string comment;
        while (getline(commentFile, comment)) {
            cout << " - " << comment << endl;
        }
    }

    string userNumberMap[100];

    string groupNumberMap[100];

    string pageIdMap[100];

public:
    UserManager() : size(0), capacity(10) {
        users = new User[capacity];
        loadFromFile(); // Load users from file
    }

    ~UserManager() {
        delete[] users;
    }

    string getUserName(const string& userId) {
        User* user = getUserById(userId);
        if (user) {
            return user->name;
        }
        return "";
    }

    void likePost(const string& postId) {
        ofstream file(postId + "_likes.txt", ios::app);
        file << "1 Like" << endl;
        file.close();
    }

    void commentOnPost(const string& postId, const string& commenterName, const string& commentContent) {
        Comment comment(postId, commenterName, commentContent);
        comment.saveToFile();
    }

    bool isPasswordValid(const string& password) {
        if (password.length() < 8) return false;

        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            else if (islower(c)) hasLower = true;
            else if (isdigit(c)) hasDigit = true;
        }
        return hasUpper && hasLower && hasDigit;
    }

    bool isBirthdateValid(const string& birthdate) {
        regex datePattern(R"(^\d{4}-\d{2}-\d{2}$)"); // Simple YYYY-MM-DD format check
        return regex_match(birthdate, datePattern);
    }

    void registerUser(User& newUser) {
        if (size == capacity) {
            resize();
        }
        newUser.userid = generateUserId(); // Generate a unique ID for the new user
        users[size++] = newUser;
        saveToFile();
    }

    bool authenticateUser(const string& email, const string& password) {
        //cout << "Size: " << size << endl;
        for (int i = 0; i < size; i++) {
            //cout << "Found Email: " << users[i].email << endl;
            //cout << "Found Password: " << users[i].password << endl;
            if (users[i].email == email && users[i].password == password) {
                return true;
            }
        }
        cout << "----------------------------------------------------------" << endl;
        return false;
    }

    void saveToFile() {
        ofstream file("users.txt");
        for (int i = 0; i < size; i++) {
            file << users[i].userid << "," << users[i].name << "," << users[i].email << ","
                << users[i].password << "," << users[i].birthdate << endl;
        }
        file.close();
    }

    void updateUser(const string& userid) {
        User* user = getUserById(userid);
        if (user) {
            cout << "Updating user information. Leave blank to keep current value." << endl;
            string input;

            cout << "Name (" << user->name << "): ";
            getline(cin.ignore(), input); // Ignore leftover newline
            if (!input.empty()) user->name = input;

            cout << "New Password: ";
            getline(cin, input);
            if (!input.empty() && isPasswordValid(input)) user->password = input;

            cout << "Birthdate (" << user->birthdate << " YYYY-MM-DD): ";
            getline(cin, input);
            if (!input.empty() && isBirthdateValid(input)) user->birthdate = input;

            saveToFile();
            cout << "User information updated for UserID: " << userid << endl;
        }
        else {
            cout << "User not found." << endl;
        }
    }

    string getLoggedInUserId(const string& email) {
        for (int i = 0; i < size; i++) {
            if (users[i].email == email) {
                return users[i].userid;
            }
        }
        return "";
    }

    void addPost(const string& userId, const string& content) {
        string postId = generatePostId();
        Post newPost(postId, content, userId);
        ofstream file(userId + "_posts.txt", ios::app); // Append mode
        file << newPost.getPostId() << "," << newPost.getPostInfo() << endl;
        file.close();
        cout << "Post added successfully." << endl;
    }

    void viewUserTimeline(const string& userId) {
        User* user = getUserById(userId);
        if (user) {
            cout << "Timeline for " << user->name << ":" << endl;
            loadUserPosts(*user);
        }
        else {
            cout << "User not found." << endl;
        }
    }

    void searchUserByName(const string& name) {
        bool found = false;
        for (int i = 0; i < size; i++) {
            if (users[i].name.find(name) != string::npos) {
                cout << "UserID: " << users[i].userid << ", Name: " << users[i].name << ", Email: " << users[i].email << endl;
                loadAndDisplayUserPosts(users[i].userid);
                found = true;
            }
        }

        if (!found) {
            cout << "No users found with the name '" << name << "'." << endl;
        }
    }

    void searchUserByName(const string& name, string postIds[], int& numPosts) {
        bool found = false;
        numPosts = 0;

        for (int i = 0; i < size; i++) {
            if (users[i].name.find(name) != string::npos) {
                cout << "UserID: " << users[i].userid << ", Name: " << users[i].name << ", Email: " << users[i].email << endl;

                ifstream file(users[i].userid + "_posts.txt");
                string line;
                int postNumber = 0;
                while (getline(file, line) && postNumber < 100) {
                    stringstream ss(line);
                    string postId, content;
                    getline(ss, postId, ',');
                    getline(ss, content);

                    postIds[postNumber] = postId; // Store the post ID
                    cout << "Post number: " << postNumber + 1 << endl;
                    cout << "Content: " << content << endl;

                    // Display number of likes
                    ifstream likeFile(postId + "_likes.txt");
                    int likeCount = count(istreambuf_iterator<char>(likeFile), istreambuf_iterator<char>(), '\n');
                    cout << "Likes: " << likeCount << endl;

                    // Display comments
                    ifstream commentFile(postId + "_comments.txt");
                    cout << "Comments:" << endl;
                    string comment;
                    while (getline(commentFile, comment)) {
                        cout << " - " << comment << endl;
                    }

                    cout << "--------------------------------------" << endl;
                    postNumber++;
                }
                file.close();
                numPosts = postNumber;

                found = true;
                break; // Break after finding the first matching user
            }
        }

        if (!found) {
            cout << "No users found with the name '" << name << "'." << endl;
        }
    }

    void sendFriendRequest(const string& fromUserId, const string& toUserId) {
        ofstream file(toUserId + "_friendsrequests.txt", ios::app);
        file << fromUserId << endl;
        file.close();
    }

    void viewFriendRequests(const string& userId) {
        ifstream file(userId + "_friendsrequests.txt");
        string requesterId;
        int requestNumber = 1;
        while (getline(file, requesterId)) {
            User* requester = getUserById(requesterId);
            cout << "Friend Request Number: " << requestNumber++ << endl;
            cout << requester->name << " sent a friend request" << endl;
        }
        file.close();
    }

    bool areFriends(const string& userId1, const string& userId2) {
        ifstream file1(userId1 + "_friends.txt");
        string line;
        while (getline(file1, line)) {
            if (line == userId2) {
                return true;
            }
        }

        ifstream file2(userId2 + "_friends.txt");
        while (getline(file2, line)) {
            if (line == userId1) {
                return true;
            }
        }

        return false;
    }

    void manageFriendRequest(const string& userId, int requestNumber, const string& action) {
        string requests[100]; // Fixed-size array for simplicity
        int numRequests = 0;
        string line;

        // Read the current friend requests into the array
        ifstream file(userId + "_friendsrequests.txt");
        while (getline(file, line) && numRequests < 100) {
            requests[numRequests++] = line;
        }
        file.close();

        if (requestNumber <= 0 || requestNumber > numRequests) {
            cout << "Invalid request number." << endl;
            return;
        }

        string targetUserId = requests[requestNumber - 1];

        if (action == "accept") {
            // Add to each other's friends list
            ofstream friendsFile(userId + "_friends.txt", ios::app);
            friendsFile << targetUserId << endl;
            friendsFile.close();

            ofstream targetFriendsFile(targetUserId + "_friends.txt", ios::app);
            targetFriendsFile << userId << endl;
            targetFriendsFile.close();
        }

        // Rewrite the requests file without the handled request
        ofstream tempFile("temp.txt");
        for (int i = 0; i < numRequests; i++) {
            if (i != requestNumber - 1) {
                tempFile << requests[i] << endl;
            }
        }
        tempFile.close();

        remove((userId + "_friendsrequests.txt").c_str());
        rename("temp.txt", (userId + "_friendsrequests.txt").c_str());
    }

    void populateUserNumberMap(const string& searchName) {
        int num = 0;
        for (int i = 0; i < size && num < 100; ++i) {
            if (users[i].name.find(searchName) != string::npos) {
                userNumberMap[num++] = users[i].userid;
            }
        }
        // Fill remaining slots with empty strings
        for (int i = num; i < 100; ++i) {
            userNumberMap[i].clear();
        }
    }

    string getUserIdByNumber(int number) {
        if (number > 0 && number <= 100 && !userNumberMap[number - 1].empty()) {
            return userNumberMap[number - 1];
        }
        return "";
    }

    void viewFriends(const string& userId) {
        ifstream friendFile(userId + "_friends.txt");
        string friendId;
        int friendNumber = 1;
        cout << "Friends of " << userId << ":" << endl;
        cout << "-------------------------------------------------------------" << endl;

        while (getline(friendFile, friendId)) {
            User* friendUser = getUserById(friendId);
            if (friendUser) {
                cout << "Friend Number " << friendNumber++ << ":" << endl;
                cout << "UserID: " << friendUser->userid << ", Name: " << friendUser->name
                    << ", Email: " << friendUser->email << endl;
                cout << "-------------------------------------------------------------" << endl;
            }
        }

        friendFile.close();
    }

    string getFriendUserId(const string& userId, int friendNumber) {
        ifstream friendFile(userId + "_friends.txt");
        string friendId;
        int currentNumber = 1;
        while (getline(friendFile, friendId)) {
            if (currentNumber == friendNumber) {
                return friendId;
            }
            currentNumber++;
        }
        return ""; // Return empty string if not found
    }

    void viewMessages(const string& userId, const string& friendUserId) {
        string chatFileName = userId < friendUserId ?
            userId + "_" + friendUserId + "_chats.txt" :
            friendUserId + "_" + userId + "_chats.txt";

        ifstream chatFile(chatFileName);
        if (!chatFile.is_open()) {
            cout << "No messages found." << endl;
        }
        else {
            string line;
            while (getline(chatFile, line)) {
                cout << line << endl;
            }
            chatFile.close();
        }

        // Prompt for sending a new message
        cout << "Do you want to send a message? (yes/no): ";
        string sendMessageChoice;
        cin >> sendMessageChoice;

        if (sendMessageChoice == "yes") {
            cout << "Enter Message: ";
            cin.ignore(); // Clear the newline left in the input buffer
            string message;
            getline(cin, message);

            Chat chat(userId, friendUserId);
            chat.sendMessage(message);
            cout << "Message sent successfully." << endl;
        }
    }

    void createGroup(const string& userId) {
        cout << "Enter group name: ";
        string groupName;
        cin.ignore(); // Clear the newline left in the input buffer
        getline(cin, groupName);

        cout << "Enter group location: ";
        string location;
        getline(cin, location);

        cout << "Enter group interest: ";
        string interest;
        getline(cin, interest);

        Group newGroup(userId, groupName, location, interest);
        newGroup.saveGroupToFile();

        // Save the new group's ID to groups.txt
        ofstream groupsFile("groups.txt", ios::app);
        groupsFile << newGroup.getGroupId() << endl;
        groupsFile.close();

        cout << "Group created successfully. Group ID: " << newGroup.getGroupId() << endl;
    }

    void joinGroup(const string& userId, const string& groupId) {
        // Add user to the group's member list
        ofstream groupFile(groupId + "_group.txt", ios::app);
        groupFile << userId << endl;
        groupFile.close();
        cout << "You have joined the group." << endl;
    }

    void leaveGroup(const string& userId, const string& groupId) {
        string tempFileName = "temp_" + groupId + "_group.txt";
        ifstream groupFile(groupId + "_group.txt");
        ofstream tempFile(tempFileName);

        string line;
        while (getline(groupFile, line)) {
            if (line != userId) {
                tempFile << line << endl;
            }
        }

        groupFile.close();
        tempFile.close();
        remove((groupId + "_group.txt").c_str());
        rename(tempFileName.c_str(), (groupId + "_group.txt").c_str());
        cout << "You have left the group." << endl;
    }

    void displayGroupMembers(const string& groupId) {
        ifstream groupFile(groupId + "_group.txt");
        string userId;
        getline(groupFile, userId); // Skip the first line containing group details

        while (getline(groupFile, userId)) {
            User* user = getUserById(userId);
            if (user) {
                cout << "UserID: " << user->userid << ", Name: " << user->name
                    << ", Email: " << user->email << endl;
            }
        }
        groupFile.close();
    }

    bool isUserInGroup(const string& userId, const string& groupId) {
        ifstream groupFile(groupId + "_group.txt");
        string line;
        while (getline(groupFile, line)) {
            if (line == userId) {
                groupFile.close();
                return true;
            }
        }
        groupFile.close();
        return false;
    }

    void populateGroupNumberMap(const string& searchTerm) {
        ifstream groupsFile("groups.txt");
        string groupId;
        int groupNumber = 0;
        while (getline(groupsFile, groupId) && groupNumber < 100) {
            string groupFileName = groupId + "_group.txt";
            ifstream groupFile(groupFileName);
            if (groupFile.is_open()) {
                string groupDetails;
                getline(groupFile, groupDetails);
                if (groupDetails.find(searchTerm) != string::npos) {
                    groupNumberMap[groupNumber++] = groupId;
                }
                groupFile.close();
            }
        }
        // Fill the remaining slots with empty strings
        for (int i = groupNumber; i < 100; ++i) {
            groupNumberMap[i].clear();
        }
        groupsFile.close();
    }

    string getGroupIdByNumber(int number) {
        if (number > 0 && number <= 100 && !groupNumberMap[number - 1].empty()) {
            return groupNumberMap[number - 1];
        }
        return "";
    }

    void searchGroups(const string& searchTerm) {
        ifstream groupsFile("groups.txt");
        string groupId;
        bool foundGroup = false;
        int groupNumber = 1;

        while (getline(groupsFile, groupId)) {
            string groupFileName = groupId + "_group.txt";
            ifstream groupFile(groupFileName);
            if (!groupFile.is_open()) {
                continue; 
            }

            string groupDetails;
            getline(groupFile, groupDetails); 
            groupFile.close();

            if (groupDetails.find(searchTerm) != string::npos) {
                foundGroup = true;
                cout << "----------------------------------------------------------" << endl;
                cout << "Group Number " << groupNumber++ << ":" << endl;
                displayFormattedGroupDetails(groupId, groupDetails);
                Group selectedGroup = getGroupById(groupId);
                selectedGroup.viewMessages(groupId);
            }
        }

        if (!foundGroup) {
            cout << "No groups found with the term: " << searchTerm << endl;
        }

        groupsFile.close();
    }

    void displayFormattedGroupDetails(const string& groupId, const string& groupDetails) {
        stringstream ss(groupDetails);
        string detail;
        getline(ss, detail, ','); // Group ID
        cout << "Group ID: " << detail << endl;
        getline(ss, detail, ','); // Group Name
        cout << "Group Name: " << detail << endl;
        getline(ss, detail, ','); // Creator User ID
        cout << "Creator User ID: " << detail << endl;
        getline(ss, detail, ','); // Location
        cout << "Location: " << detail << endl;
        getline(ss, detail); // Interest
        cout << "Interest: " << detail << endl;

        cout << "Group Members:" << endl;
        displayGroupMembers(groupId);
    }

    void createPage(const string& userId) {
        cout << "Enter page name: ";
        string pageName;
        cin.ignore();
        getline(cin, pageName);

        cout << "Enter page description: ";
        string description;
        getline(cin, description);

        Page newPage(userId, pageName, description);
        newPage.savePageToFile();

        // Append the new page's ID to pages.txt
        ofstream pagesFile("pages.txt", ios::app);
        pagesFile << newPage.getPageId() << endl;
        pagesFile.close();

        cout << "Page created successfully. Page ID: " << newPage.getPageId() << endl;
    }

    void joinPage(const string& userId, const string& pageId) {
        ofstream pageFile(pageId + "_page.txt", ios::app);
        pageFile << userId << endl;
        pageFile.close();
        cout << "You have followed the page." << endl;
    }

    void leavePage(const string& userId, const string& pageId) {
        string tempFileName = "temp_" + pageId + "_page.txt";
        ifstream pageFile(pageId + "_page.txt");
        ofstream tempFile(tempFileName);

        string line;
        getline(pageFile, line); // Skip the first line containing page details
        tempFile << line << endl; // Write page details to temp file

        while (getline(pageFile, line)) {
            if (line != userId) {
                tempFile << line << endl;
            }
        }

        pageFile.close();
        tempFile.close();
        remove((pageId + "_page.txt").c_str());
        rename(tempFileName.c_str(), (pageId + "_page.txt").c_str());
        cout << "You have unfollowed the page." << endl;
    }

    void populatePageIdMap() {
        ifstream pagesFile("pages.txt");
        string pageId;
        int pageNum = 0;
        while (getline(pagesFile, pageId) && pageNum < 100) {
            pageIdMap[pageNum++] = pageId;
        }
        
        for (int i = pageNum; i < 100; ++i) {
            pageIdMap[i].clear();
        }
        pagesFile.close();
    }

    string getPageIdByNumber(int number) {
        if (number > 0 && number <= 100 && !pageIdMap[number - 1].empty()) {
            return pageIdMap[number - 1];
        }
        return "";
    }
    
    bool isUserInPage(const string& userId, const string& pageId) {
        ifstream pageFile(pageId + "_page.txt");
        if (!pageFile.is_open()) {
            return false; // Page file does not exist
        }

        string line;
        while (getline(pageFile, line)) {
            if (line == userId) {
                pageFile.close();
                return true; // User is a member of the page
            }
        }
        pageFile.close();
        return false; // User is not found in the page
    }

    void searchPages(const string& searchTerm) {
        ifstream pagesFile("pages.txt");
        string pageId;
        int pageNumber = 1;
        bool foundPage = false;

        while (getline(pagesFile, pageId)) {
            ifstream pageFile(pageId + "_page.txt");
            if (!pageFile.is_open()) {
                continue; // Skip if the page file doesn't exist
            }

            string pageDetails;
            getline(pageFile, pageDetails); // First line contains page details
            pageFile.close();

            if (pageDetails.find(searchTerm) != string::npos) {
                foundPage = true;
                cout << "----------------------------------------------------------" << endl;
                cout << "Page Number " << pageNumber++ << ":" << endl;
                displayFormattedPageDetails(pageDetails);
                cout << "Followers of the Page:" << endl;
                displayPageFollowers(pageId);
            }
        }

        if (!foundPage) {
            cout << "No pages found with the term: " << searchTerm << endl;
        }

        pagesFile.close();
    }

    void displayFormattedPageDetails(const string& pageDetails) {
        stringstream ss(pageDetails);
        string detail;

        getline(ss, detail, ','); // Page ID
        cout << "Page ID: " << detail << endl;

        getline(ss, detail, ','); // Page Name
        cout << "Page Name: " << detail << endl;

        getline(ss, detail, ','); // Creator User ID
        cout << "Creator User ID: " << detail << endl;

        getline(ss, detail); // Description
        cout << "Description: " << detail << endl;
    }

    void displayPageFollowers(const string& pageId) {
        ifstream pageFile(pageId + "_page.txt");
        string userId;
        getline(pageFile, userId); // Skip the first line containing page details

        while (getline(pageFile, userId)) {
            User* user = getUserById(userId);
            if (user) {
                cout << "UserID: " << user->userid << ", Name: " << user->name
                    << ", Email: " << user->email << endl;
            }
        }
        pageFile.close();
    }

    void handleSignup() {
        User newUser;
        string password, birthdate;

        cout << "Register new user:" << endl;
        cout << "Name: ";
        cin >> newUser.name;
        cout << "Email: ";
        cin.ignore();
        cin >> newUser.email;

        do {
            cout << "Password (min 8 characters, include uppercase, lowercase, and digit): ";
            cin >> password;
        } while (!isPasswordValid(password));
        newUser.password = password;

        do {
            cout << "Birthdate (YYYY-MM-DD): ";
            cin >> birthdate;
        } while (!isBirthdateValid(birthdate));
        newUser.birthdate = birthdate;

        registerUser(newUser);
        cout << "Registration successful. Your UserID is: " << newUser.userid << endl;
    }

    bool handleLogin(string& email, string& password) {
        cout << "Login:" << endl;
        cout << "Email: "; cin >> email;
        cout << "Password: "; cin >> password;

        return authenticateUser(email, password);
    }

    void handlePost(const string& loggedInUserId) {
        string postContent;
        cout << "Enter your post content: ";
        cin.ignore(); // Ignore leftover newline
        getline(cin, postContent);
        addPost(loggedInUserId, postContent);
    }

    void handleUpdate(const string& loggedInUserId) {
        updateUser(loggedInUserId);
    }

    void handleSearch(const string& loggedInUserId) {
        string name;
        cout << "Enter name to search: ";
        cin.ignore(); // Ignore leftover newline
        getline(cin, name);

        populateUserNumberMap(name);

        string postIds[100]; // Assuming a max of 100 posts for simplicity
        int numPosts = 0;
        searchUserByName(name, postIds, numPosts);

        if (numPosts > 0) {
            cout << "Enter user number to go to profile: ";
            int userNumber;
            cin >> userNumber;

            string searchedUserId = getUserIdByNumber(userNumber);

            if (!areFriends(loggedInUserId, searchedUserId)) {
                handleFriendRequest(loggedInUserId, searchedUserId);
            }

            handleLikesAndComments(loggedInUserId, postIds, numPosts);
        }
        else {
            cout << "No users found or no posts available for the searched user." << endl;
        }
    }

    void handleFriendRequest(const string& loggedInUserId, const string& searchedUserId) {
        cout << "Do you want to send a friend request? (yes/no): ";
        string sendRequest;
        cin >> sendRequest;
        if (sendRequest == "yes") {
            sendFriendRequest(loggedInUserId, searchedUserId);
        }
    }

    void handleLikesAndComments(const string& loggedInUserId, const string postIds[], int numPosts) {
        cout << "Do you want to add likes and comments? (yes/no): ";
        string likeCommentChoice;
        cin >> likeCommentChoice;

        if (likeCommentChoice == "yes") {
            cout << "Select post number: ";
            int postNumber;
            cin >> postNumber;

            if (postNumber >= 1 && postNumber <= numPosts) {
                string postId = postIds[postNumber - 1];
                likeAndCommentOnPost(loggedInUserId, postId);
            }
            else {
                cout << "Invalid post number." << endl;
            }
        }
    }

    void likeAndCommentOnPost(const string& loggedInUserId, const string& postId) {
        cout << "Like (yes/no): ";
        string likeChoice;
        cin >> likeChoice;
        if (likeChoice == "yes") {
            likePost(postId);
        }

        cout << "Comment: ";
        string comment;
        cin.ignore(); // Clear the newline left in the input buffer
        getline(cin, comment);
        commentOnPost(postId, getUserName(loggedInUserId), comment);
    }

    void handleFriendRequest(const string& loggedInUserId) {
        viewFriendRequests(loggedInUserId);

        cout << "Enter Friend Request Number to accept or delete (0 to abort): ";
        int requestNumber;
        cin >> requestNumber;

        if (requestNumber > 0) {
            cout << "Want to accept or delete (accept/delete): ";
            string decision;
            cin >> decision;

            if (decision == "accept" || decision == "delete") {
                manageFriendRequest(loggedInUserId, requestNumber, decision);
            }
            else {
                cout << "Invalid choice. Please enter 'accept' or 'delete'." << endl;
            }
        }
    }

    void handleFriends(const string& loggedInUserId) {
        viewFriends(loggedInUserId);

        cout << "Enter Friend Number: ";
        int friendNumber;
        cin >> friendNumber;

        string friendUserId = getFriendUserId(loggedInUserId, friendNumber);
        if (!friendUserId.empty()) {
            cout << "Do you want to send a message? (yes/no): ";
            string sendMessageChoice;
            cin >> sendMessageChoice;

            if (sendMessageChoice == "yes") {
                cout << "Enter Message: ";
                string message;
                cin.ignore(); // Clear the newline left in the input buffer
                getline(cin, message);

                Chat chat(loggedInUserId, friendUserId);
                chat.sendMessage(message);
            }
        }
        else {
            cout << "Invalid friend number." << endl;
        }
    }

    void handleCheckMessages(const string& loggedInUserId) {
        viewFriends(loggedInUserId);

        cout << "Enter Friend Number to view messages: ";
        int friendNumber;
        cin >> friendNumber;

        string friendUserId = getFriendUserId(loggedInUserId, friendNumber);
        if (!friendUserId.empty()) {
            viewMessages(loggedInUserId, friendUserId);
        }
        else {
            cout << "Invalid friend number." << endl;
        }
    }

    void handleGroups(const string& loggedInUserId) {
        cout << "Select option: [create_group], [search_group]: ";
        string groupChoice;
        cin >> groupChoice;

        if (groupChoice == "create_group") {
            createGroup(loggedInUserId);
        }
        else if (groupChoice == "search_group") {
            handleGroupSearch(loggedInUserId);
        }
    }

    void handleGroupMembership(const string& loggedInUserId, const string& groupId) {
        if (!groupId.empty()) {
            bool isMember = isUserInGroup(loggedInUserId, groupId);
            if (!isMember) {
                cout << "Do you want to join group? (yes/no): ";
                string joinChoice;
                cin >> joinChoice;
                if (joinChoice == "yes") {
                    joinGroup(loggedInUserId, groupId);
                }
            }
            else {
                cout << "Do you want to leave group? (yes/no): ";
                string leaveChoice;
                cin >> leaveChoice;
                if (leaveChoice == "yes") {
                    leaveGroup(loggedInUserId, groupId);
                }
            }
        }
    }

    void handlePages(const string& loggedInUserId) {
        cout << "Select option: [create_page], [search_page]: ";
        string pageChoice;
        cin >> pageChoice;

        if (pageChoice == "create_page") {
            createPage(loggedInUserId);
        }
        else if (pageChoice == "search_page") {
            handlePageSearch(loggedInUserId);
        }
    }

    void handlePageSearch(const string& loggedInUserId) {
        cout << "Enter search term (name or description): ";
        string searchTerm;
        cin.ignore(); // Clear the newline left in the input buffer
        getline(cin, searchTerm);

        populatePageIdMap();
        searchPages(searchTerm);

        cout << "Enter Page Number: ";
        int pageNumber;
        cin >> pageNumber;

        string pageId = getPageIdByNumber(pageNumber);
        handlePageMembership(loggedInUserId, pageId);
    }

    void handlePageMembership(const string& loggedInUserId, const string& pageId) {
        if (!pageId.empty()) {
            bool isMember = isUserInPage(loggedInUserId, pageId);
            if (!isMember) {
                cout << "Do you want to join page? (yes/no): ";
                string joinChoice;
                cin >> joinChoice;
                if (joinChoice == "yes") {
                    joinPage(loggedInUserId, pageId);
                }
            }
            else {
                cout << "Do you want to leave page? (yes/no): ";
                string leaveChoice;
                cin >> leaveChoice;
                if (leaveChoice == "yes") {
                    leavePage(loggedInUserId, pageId);
                }
            }
        }
    }

    Group getGroupById(const string& groupId) {
        ifstream file(groupId + "_group.txt");
        if (!file.is_open()) {
            cout << "Group not found." << endl;
            return Group("", "", "", ""); // Return an empty group in case of failure
        }

        string groupName, creatorUserId, location, interest;
        getline(file, groupName, ',');
        getline(file, creatorUserId, ',');
        getline(file, location, ',');
        getline(file, interest);

        Group group(creatorUserId, groupName, location, interest);
        string memberId;
        while (getline(file, memberId)) {
            if (!memberId.empty()) {
                group.addMember(memberId);
            }
        }

        file.close();
        return group;
    }

    void handleGroupSearch(const string& loggedInUserId) {
        cout << "Enter search term (name, location, or interest): ";
        string searchTerm;
        cin.ignore(); // Clear the newline left in the input buffer
        getline(cin, searchTerm);

        populateGroupNumberMap(searchTerm);
        searchGroups(searchTerm);

        cout << "----------------------------------------------------------" << endl;
        cout << "Enter Group Number: ";
        int groupNumber;
        cin >> groupNumber;

        string groupId = getGroupIdByNumber(groupNumber);
        if (!groupId.empty()) {
            handleGroupDiscussion(loggedInUserId, groupId);
            handleGroupMembership(loggedInUserId, groupId);
        }
        else {
            cout << "Invalid group number." << endl;
        }
    }

    void handleGroupDiscussion(const string& loggedInUserId, const string& groupId) {
        Group selectedGroup = getGroupById(groupId);
        //cout << "Here---- Group ID: " << groupId << endl;
        if (selectedGroup.getGroupId().empty()) {
            cout << "Group not found or error occurred." << endl;
            return;
        }
        bool choice = true;
     
        while (choice) {
            cout << "Do you want to send a message in group?(yes/no): ";
            string sendMessageChoice;
            cin >> sendMessageChoice;

            if (sendMessageChoice == "yes") {
                cout << "Enter your message: ";
                string message;
                cin.ignore(); // Clear the newline left in the input buffer
                getline(cin, message);
                selectedGroup.postMessage(loggedInUserId, groupId, message);

                cout << "Group discussion:" << endl;
                selectedGroup.viewMessages(groupId);
            }
            else {
                choice = false;
            }
        }
        cout << "----------------------------------------------------------" << endl;
    }
};

int main() {
    UserManager userManager;
    string choice;

    while (true) {
        cout << "Do you want to [login] or [signup]? ";
        cin >> choice;

        if (choice == "signup") {
            User newUser;
            string password, birthdate;

            cout << "Register new user:" << endl;
            cout << "Name: ";
            cin >> newUser.name;
            cout << "Email: ";
            cin.ignore();
            cin >> newUser.email;

            do {
                cout << "Password (min 8 characters, include uppercase, lowercase, and digit): ";
                cin >> password;
            } while (!userManager.isPasswordValid(password));
            newUser.password = password;

            do {
                cout << "Birthdate (YYYY-MM-DD): ";
                cin >> birthdate;
            } while (!userManager.isBirthdateValid(birthdate));
            newUser.birthdate = birthdate;

            userManager.registerUser(newUser);
            cout << "Registration successful. Your UserID is: " << newUser.userid << endl;
        }
        else if (choice == "login") {
            string email, password;
            cout << "Login:" << endl;
            cout << "Email: "; cin >> email;
            cout << "Password: "; cin >> password;

            if (userManager.authenticateUser(email, password)) {
                cout << "Login successful!" << endl;
                string loggedInUserId = userManager.getLoggedInUserId(email);
                string loggedInUserName = userManager.getUserName(loggedInUserId);

                cout << "Choose action: [post], [update], [search], [friends_request], [friends], [check_messages], [groups], [pages]: ";
                cin >> choice;

                if (choice == "post") {
                    string postContent;
                    cout << "Enter your post content: ";
                    cin.ignore(); // Ignore leftover newline
                    getline(cin, postContent);
                    userManager.addPost(loggedInUserId, postContent);
                }
                else if (choice == "update") {
                    userManager.updateUser(loggedInUserId);
                }
                else if (choice == "search") {
                    string name;
                    cout << "Enter name to search: ";
                    cin.ignore(); // Ignore leftover newline
                    getline(cin, name);

                    userManager.populateUserNumberMap(name);

                    string postIds[100]; // Assuming a max of 100 posts for simplicity
                    int numPosts = 0;
                    userManager.searchUserByName(name, postIds, numPosts);

                    cout << "Enter user number to go to profile: ";
                    int userNumber;
                    cin >> userNumber;

                    // Assuming userNumber is valid and corresponds to a userId
                    string searchedUserId = userManager.getUserIdByNumber(userNumber);

                    if (!userManager.areFriends(loggedInUserId, searchedUserId)) {
                        cout << "Do you want to send a friend request? (yes/no): ";
                        string sendRequest;
                        cin >> sendRequest;
                        if (sendRequest == "yes") {
                            userManager.sendFriendRequest(loggedInUserId, searchedUserId);
                        }
                    }

                    cout << "Do you want to add likes and comments? (yes/no): ";
                    string likeCommentChoice;
                    cin >> likeCommentChoice;

                    if (likeCommentChoice == "yes") {
                        cout << "Select post number: ";
                        int postNumber;
                        cin >> postNumber;

                        if (postNumber >= 1 && postNumber <= numPosts) {
                            string postId = postIds[postNumber - 1];

                            cout << "Like (yes/no): ";
                            string likeChoice;
                            cin >> likeChoice;
                            if (likeChoice == "yes") {
                                userManager.likePost(postId);
                            }

                            cout << "Comment: ";
                            string comment;
                            cin.ignore();
                            getline(cin, comment);
                            userManager.commentOnPost(postId, loggedInUserName, comment);
                        }
                        else {
                            cout << "Invalid post number." << endl;
                        }
                    }
                    else {
                        cout << "Invalid choice." << endl;
                    }
                }
                else if (choice == "friends_request") {
                    cout << "Viewing friend requests for " << loggedInUserId << endl;
                    userManager.viewFriendRequests(loggedInUserId);

                    cout << "Enter Friend Request Number to accept or delete (0 to abort): ";
                    int requestNumber;
                    cin >> requestNumber;

                    if (requestNumber > 0) {
                        cout << "Want to accept or delete (accept/delete): ";
                        string decision;
                        cin >> decision;

                        if (decision == "accept" || decision == "delete") {
                            userManager.manageFriendRequest(loggedInUserId, requestNumber, decision);
                        }
                        else {
                            cout << "Invalid choice. Please enter 'accept' or 'delete'." << endl;
                        }
                    }
                }
                else if (choice == "friends") {
                    userManager.viewFriends(loggedInUserId);

                    cout << "Enter Friend Number: ";
                    int friendNumber;
                    cin >> friendNumber;

                    // Assuming friendNumber is valid and corresponds to a friend's userId
                    string friendUserId = userManager.getFriendUserId(loggedInUserId, friendNumber); // Implement this method in UserManager

                    cout << "Do you want to send a message? (yes/no): ";
                    string sendMessageChoice;
                    cin >> sendMessageChoice;

                    if (sendMessageChoice == "yes") {
                        cout << "Enter Message: ";
                        string message;
                        cin.ignore();
                        getline(cin, message);

                        Chat chat(loggedInUserId, friendUserId);
                        chat.sendMessage(message);
                    }
                }
                else if (choice == "check_messages") {
                    userManager.viewFriends(loggedInUserId);

                    cout << "Enter Friend Number to view messages: ";
                    int friendNumber;
                    cin >> friendNumber;

                    string friendUserId = userManager.getFriendUserId(loggedInUserId, friendNumber);
                    if (!friendUserId.empty()) {
                        userManager.viewMessages(loggedInUserId, friendUserId);
                    }
                    else {
                        cout << "Invalid friend number." << endl;
                    }
                }
                else if (choice == "groups") {
                    cout << "Select option: [create_group], [search_group]: ";
                    string groupChoice;
                    cin >> groupChoice;

                    if (groupChoice == "create_group") {
                        userManager.createGroup(loggedInUserId);
                    }
                    else if (groupChoice == "search_group") {
                        cout << "Enter search term (name, location, or interest): ";
                        string searchTerm;
                        cin.ignore(); // Clear the newline left in the input buffer
                        getline(cin, searchTerm);

                        userManager.populateGroupNumberMap(searchTerm);
                        userManager.searchGroups(searchTerm);

                        cout << "----------------------------------------------------------" << endl;
                        cout << "Enter Group Number: ";
                        int groupNumber;
                        cin >> groupNumber;

                        string groupId = userManager.getGroupIdByNumber(groupNumber); // Implement this method in UserManager
                        userManager.handleGroupDiscussion(loggedInUserId, groupId);
                        if (!groupId.empty()) {
                            bool isMember = userManager.isUserInGroup(loggedInUserId, groupId);
                            if (!isMember) {
                                cout << "Do you want to join group? (yes/no): ";
                                string joinChoice;
                                cin >> joinChoice;
                                if (joinChoice == "yes") {
                                    userManager.joinGroup(loggedInUserId, groupId);
                                }
                            }
                            else {
                                cout << "Do you want to leave group? (yes/no): ";
                                string leaveChoice;
                                cin >> leaveChoice;
                                if (leaveChoice == "yes") {
                                    userManager.leaveGroup(loggedInUserId, groupId);
                                }
                            }
                        }
                    }
                }
                else if (choice == "pages") {
                    cout << "Select option: [create_page], [search_page]: ";
                    string pageChoice;
                    cin >> pageChoice;

                    if (pageChoice == "create_page") {
                        userManager.createPage(loggedInUserId);
                    }
                    else if (pageChoice == "search_page") {
                        cout << "Enter search term (name or description): ";
                        string searchTerm;
                        cin.ignore(); // Clear the newline left in the input buffer
                        getline(cin, searchTerm);

                        userManager.populatePageIdMap();
                        userManager.searchPages(searchTerm);

                        cout << "----------------------------------------------------------" << endl;
                        cout << "Enter Page Number: ";
                        int pageNumber;
                        cin >> pageNumber;

                        string pageId = userManager.getPageIdByNumber(pageNumber); // Implement this method in UserManager
                        if (!pageId.empty()) {
                            bool isMember = userManager.isUserInPage(loggedInUserId, pageId); // Implement this method in UserManager
                            if (!isMember) {
                                cout << "Do you want to join page? (yes/no): ";
                                string joinChoice;
                                cin >> joinChoice;
                                if (joinChoice == "yes") {
                                    userManager.joinPage(loggedInUserId, pageId);
                                }
                            }
                            else {
                                cout << "Do you want to leave page? (yes/no): ";
                                string leaveChoice;
                                cin >> leaveChoice;
                                if (leaveChoice == "yes") {
                                    userManager.leavePage(loggedInUserId, pageId);
                                }
                            }
                        }
                    }
                    else {
                        cout << "Invalid email or password!" << endl;
                    }
                }
            }
        }
        else {
            cout << "Invalid choice. Please enter either 'login' or 'signup'." << endl;
        }
    }


    return 0;
}
