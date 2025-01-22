// Page.h
#ifndef PAGE_H
#define PAGE_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Page {
private:
    string pageId;
    string creatorUserId;
    string pageName;
    string description;
    string memberIds[100];
    int memberCount;

    string generatePageId();

public:
    Page(const string& creatorUserId, const string& pageName, const string& description);
    void savePageToFile();
    string getPageId() const;
};

#endif // PAGE_H
