// Like.h
#ifndef LIKE_H
#define LIKE_H

#include <string>
#include <fstream>

using namespace std;

class Like {
private:
    string postId;

public:
    Like(const string& postId);
    void saveToFile() const;
};

#endif // LIKE_H
