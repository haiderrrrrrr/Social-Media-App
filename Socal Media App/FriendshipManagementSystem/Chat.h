// Chat.h
#ifndef CHAT_H
#define CHAT_H

#include <string>
#include <fstream>
#include <ctime>

using namespace std;

class Chat {
private:
    string senderId;
    string receiverId;
    string getChatFileName() const;

public:
    Chat(const string& senderId, const string& receiverId);
    void sendMessage(const string& message);
};

#endif // CHAT_H
