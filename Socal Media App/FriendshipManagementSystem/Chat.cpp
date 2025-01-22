// Chat.cpp
#include "Chat.h"

Chat::Chat(const string& senderId, const string& receiverId)
    : senderId(senderId), receiverId(receiverId) {}

string Chat::getChatFileName() const {
    return senderId + "_" + receiverId + "_chats.txt";
}

void Chat::sendMessage(const string& message) {
    ofstream chatFile(getChatFileName(), ios::app);
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now);
    chatFile << senderId << " (" << 1900 + ltm.tm_year << "-"
        << 1 + ltm.tm_mon << "-" << ltm.tm_mday << "): " << message << endl;
    chatFile.close();
}
