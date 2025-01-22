// Page.cpp
#include "Page.h"

Page::Page(const string& creatorUserId, const string& pageName, const string& description)
    : creatorUserId(creatorUserId), pageName(pageName), description(description), memberCount(0) {
    pageId = generatePageId();
    memberIds[memberCount++] = creatorUserId;
}

string Page::generatePageId() {
    return "page" + to_string(time(nullptr));
}

void Page::savePageToFile() {
    ofstream file(pageId + "_page.txt");
    file << pageId << "," << pageName << "," << creatorUserId << "," << description << endl;
    for (int i = 0; i < memberCount; i++) {
        file << memberIds[i] << endl;
    }
    file.close();
}

string Page::getPageId() const {
    return pageId;
}
