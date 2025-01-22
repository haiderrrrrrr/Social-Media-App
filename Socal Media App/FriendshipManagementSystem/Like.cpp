// Like.cpp
#include "Like.h"

Like::Like(const string& postId) : postId(postId) {}

void Like::saveToFile() const {
    ofstream file(postId + "_likes.txt", ios::app);
    file << "1 Like" << endl;
    file.close();
}
