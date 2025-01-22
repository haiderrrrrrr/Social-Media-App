// Post.h
#ifndef POST_H
#define POST_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iterator>
#include <algorithm>

using namespace std;

class Post {
private:
    string postId;
    string content;
    string datePosted;
    string userId;

public:
    Post(const string& postId, const string& content, const string& userId);
    string getPostId() const;
    void displayPostWithDetails() const;
    string getPostInfo() const;
    string getUserId() const;
};

#endif // POST_H
