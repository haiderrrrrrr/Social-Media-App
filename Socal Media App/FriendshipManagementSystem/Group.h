// Group.h
#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Group {
private:
    string groupId;
    string groupName;
    string creatorUserId;
    string location;
    string interest;
    string memberIds[100];
    int memberCount;

    string generateGroupId();

public:
    Group(const string& creatorUserId, const string& groupName, const string& location, const string& interest);
    string getGroupId() const;
    void addMember(const string& memberId);
    void saveGroupToFile();
    void postMessage(const string& userId, const string& gId, const string& message);
    void viewMessages(const string& gId) const;
};

#endif // GROUP_H
