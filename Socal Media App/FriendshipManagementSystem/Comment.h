// Comment.h
#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

class Comment {
private:
    string postId;
    string commenterName;
    string content;
    string dateCommented;

    string getCurrentDate() const;

public:
    Comment(const string& postId, const string& commenterName, const string& content);
    void saveToFile() const;
};

#endif // COMMENT_H
