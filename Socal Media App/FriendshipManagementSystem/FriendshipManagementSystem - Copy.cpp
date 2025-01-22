#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <regex>

using namespace std;

// Date class for handling date operations
class Date {
private:
    int year, month, day;

public:
    Date() : year(0), month(0), day(0) {}

    void setDate(int y, int m, int d) {
        year = y;
        month = m;
        day = d;
    }

    string getDate() const {
        stringstream ss;
        ss << year << "-" << (month < 10 ? "0" : "") << month << "-" << (day < 10 ? "0" : "") << day;
        return ss.str();
    }
};

// Structure to hold user data
struct User {
    string userid;
    string name;
    string email;
    string password;
    string birthdate;
};

class Post {
private:
    string postId;
    string content;
    string datePosted;
    string userId;

public:
    Post(const string& postId, const string& content, const string& userId)
        : postId(postId), content(content), userId(userId) {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now); // Use localtime_s
        stringstream ss;
        ss << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday;
        datePosted = ss.str();
    }

    string getPostId() const {
        return postId;
    }

    void displayPostWithDetails() const {
        cout << "Post ID: " << postId << endl;
        cout << "Content: " << content << endl;
        cout << "Date Posted: " << datePosted << endl;

        ifstream likeFile(postId + "_likes.txt");
        int likeCount = count(istreambuf_iterator<char>(likeFile), istreambuf_iterator<char>(), '\n');
        cout << "Likes: " << likeCount << endl;

        ifstream commentFile(postId + "_comments.txt");
        cout << "Comments:" << endl;
        string comment;
        while (getline(commentFile, comment)) {
            cout << comment << endl;
        }

        cout << "--------------------------------------" << endl;
    }

    string getPostInfo() const {
        return datePosted + ": " + content;
    }

    string getUserId() const {
        return userId;
    }
};

class Comment {
private:
    string postId;
    string commenterName;
    string content;
    string dateCommented;

    string getCurrentDate() const {
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now); // Use localtime_s for safety
        stringstream ss;
        ss << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday;
        return ss.str();
    }

public:
    Comment(const string& postId, const string& commenterName, const string& content)
        : postId(postId), commenterName(commenterName), content(content) {
        dateCommented = getCurrentDate();
    }

    void saveToFile() const {
        ofstream file(postId + "_comments.txt", ios::app);
        file << commenterName << " (" << dateCommented << "): " << content << endl;
        file.close();
    }
};

class Like {
private:
    string postId;

public:
    Like(const string& postId) : postId(postId) {}

    void saveToFile() const {
        ofstream file(postId + "_likes.txt", ios::app);
        file << "1 Like" << endl;
        file.close();
    }
};

class Chat {
private:
    string senderId;
    string receiverId;

    string getChatFileName() const {
        return senderId + "_" + receiverId + "_chats.txt";
    }

public:
    Chat(const string& senderId, const string& receiverId) : senderId(senderId), receiverId(receiverId) {}

    void sendMessage(const string& message) {
        ofstream chatFile(getChatFileName(), ios::app);
        time_t now = time(0);
        tm ltm;
        localtime_s(&ltm, &now);
        chatFile << senderId << " (" << 1900 + ltm.tm_year << "-"
            << 1 + ltm.tm_mon << "-" << ltm.tm_mday << "): " << message << endl;
        chatFile.close();
    }
};

class Group {
private:
    string groupId;
    string groupName;
    string creatorUserId;
    string location;
    string interest;
    string memberIds[100]; // Fixed-size array
    int memberCount;

    string generateGroupId() {
        return "group" + to_string(time(nullptr));
    }

public:
    Group(const string& creatorUserId, const string& groupName, const string& location, const string& interest)
        : creatorUserId(creatorUserId), groupName(groupName), location(location), interest(interest), memberCount(0) {
        groupId = generateGroupId();
        memberIds[memberCount++] = creatorUserId;
    }

    string getGroupId() const {
        return groupId;
    }

    void addMember(const string& memberId) {
        if (memberCount < 100) {
            memberIds[memberCount++] = memberId;
        }
        else {
            cout << "Group member limit reached." << endl;
        }
    }

    void saveGroupToFile() {
        ofstream file(groupId + "_group.txt");
        file << groupId << "," << groupName << "," << creatorUserId << "," << location << "," << interest << endl;
        for (int i = 0; i < memberCount; i++) {
            file << memberIds[i] << endl;
        }
        file.close();
    }
};

class Page {
private:
    string pageId;
    string creatorUserId;
    string pageName;
    string description;
    string memberIds[100]; // Fixed-size array for simplicity
    int memberCount;

    string generatePageId() {
        return "page" + to_string(time(nullptr));
    }

public:
    Page(const string& creatorUserId, const string& pageName, const string& description)
        : creatorUserId(creatorUserId), pageName(pageName), description(description), memberCount(0) {
        pageId = generatePageId();
        memberIds[memberCount++] = creatorUserId; // Creator is the first member
    }

    void savePageToFile() {
        ofstream file(pageId + "_page.txt");
        file << pageId << "," << pageName << "," << creatorUserId << "," << description << endl;
        for (int i = 0; i < memberCount; i++) {
            file << memberIds[i] << endl;
        }
        file.close();
    }

    string getPageId() const {
        return pageId;
    }
};

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
                continue; // Skip if the group file doesn't exist
            }

            string groupDetails;
            getline(groupFile, groupDetails); // First line contains group details
            groupFile.close();

            if (groupDetails.find(searchTerm) != string::npos) {
                foundGroup = true;
                cout << "----------------------------------------------------------" << endl;
                cout << "Group Number " << groupNumber++ << ":" << endl;
                displayFormattedGroupDetails(groupId, groupDetails);
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

    void handleGroupSearch(const string& loggedInUserId) {
        cout << "Enter search term (name, location, or interest): ";
        string searchTerm;
        cin.ignore(); // Clear the newline left in the input buffer
        getline(cin, searchTerm);

        populateGroupNumberMap(searchTerm);
        searchGroups(searchTerm);

        cout << "Enter Group Number: ";
        int groupNumber;
        cin >> groupNumber;

        string groupId = getGroupIdByNumber(groupNumber);
        handleGroupMembership(loggedInUserId, groupId);
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
};

int main() {
    UserManager userManager;
    string choice, email, password;

    while (true) {
        cout << "Do you want to [login], [signup] or [exit]? ";
        cin >> choice;

        if (choice == "signup") {
            // Registration logic
            userManager.handleSignup();
        }
        else if (choice == "login") {
            if (userManager.handleLogin(email, password)) {
                bool loggedIn = true;
                while (loggedIn) {
                    cout << "Choose action: [post], [update], [search], [friends_request], [friends], [check_messages], [groups], [pages], [logout]: ";
                    cin >> choice;

                    if (choice == "post") {
                        userManager.handlePost(email);
                    }
                    else if (choice == "update") {
                        userManager.handleUpdate(email);
                    }
                    else if (choice == "search") {
                        userManager.handleSearch(email);
                    }
                    else if (choice == "friends_request") {
                        userManager.handleFriendRequest(email);
                    }
                    else if (choice == "friends") {
                        userManager.handleFriends(email);
                    }
                    else if (choice == "check_messages") {
                        userManager.handleCheckMessages(email);
                    }
                    else if (choice == "groups") {
                        userManager.handleGroups(email);
                    }
                    else if (choice == "pages") {
                        userManager.handlePages(email);
                    }
                    else if (choice == "logout") {
                        loggedIn = false;
                        cout << "Logged out successfully." << endl;
                    }
                    else {
                        cout << "Invalid choice, please try again." << endl;
                    }
                }
            }
            else {
                cout << "Login failed, please try again." << endl;
            }
        }
        else if (choice == "exit") {
            break;
        }
        else {
            cout << "Invalid choice, please try again." << endl;
        }
    }

    return 0;
}
