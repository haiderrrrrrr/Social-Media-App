// Post.cpp
#include "Post.h"

Post::Post(const string& postId, const string& content, const string& userId)
    : postId(postId), content(content), userId(userId) {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now); // Use localtime_s
    stringstream ss;
    ss << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday;
    datePosted = ss.str();
}

string Post::getPostId() const {
    return postId;
}

void Post::displayPostWithDetails() const {
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

string Post::getPostInfo() const {
    return datePosted + ": " + content;
}

string Post::getUserId() const {
    return userId;
}
