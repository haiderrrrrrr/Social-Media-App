// Comment.cpp
#include "Comment.h"

Comment::Comment(const string& postId, const string& commenterName, const string& content)
    : postId(postId), commenterName(commenterName), content(content) {
    dateCommented = getCurrentDate();
}

string Comment::getCurrentDate() const {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now); // Use localtime_s for safety
    stringstream ss;
    ss << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday;
    return ss.str();
}

void Comment::saveToFile() const {
    ofstream file(postId + "_comments.txt", ios::app);
    file << commenterName << " (" << dateCommented << "): " << content << endl;
    file.close();
}
