// Group.cpp
#include "Group.h"

Group::Group(const string& creatorUserId, const string& groupName, const string& location, const string& interest)
    : creatorUserId(creatorUserId), groupName(groupName), location(location), interest(interest), memberCount(0) {
    groupId = generateGroupId();
    memberIds[memberCount++] = creatorUserId;
}

string Group::generateGroupId() {
    return "group" + to_string(time(nullptr));
}

string Group::getGroupId() const {
    return groupId;
}

void Group::addMember(const string& memberId) {
    if (memberCount < 100) {
        memberIds[memberCount++] = memberId;
    }
    else {
        cout << "Group member limit reached." << endl;
    }
}

void Group::saveGroupToFile() {
    ofstream file(groupId + "_group.txt");
    file << groupId << "," << groupName << "," << creatorUserId << "," << location << "," << interest << endl;
    for (int i = 0; i < memberCount; i++) {
        file << memberIds[i] << endl;
    }
    file.close();
}

void Group::postMessage(const string& userId, const string& gId,const string& message) {
    //cout << "Entering group ID: " << gId << endl;
    ofstream discussionFile(gId + "_discussions.txt", ios::app);
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);

    discussionFile << userId << " [" << 1900 + ltm.tm_year << "-"
        << 1 + ltm.tm_mon << "-" << ltm.tm_mday << "]: " << message << endl;
    discussionFile.close();
}


void Group::viewMessages(const string& gId) const {
    ifstream discussionFile(gId + "_discussions.txt");
    string line;
    cout << "Group " << groupId << " Discussion:" << endl;
    while (getline(discussionFile, line)) {
        cout << line << endl;
    }
    discussionFile.close();
}
