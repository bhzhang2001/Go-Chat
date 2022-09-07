//group_chat_message.cpp
#include "group_chat_message.h"

//constructor
GroupChatMessage::GroupChatMessage(const ChatMessage& chatmsg, const UserInfo& userinfo) :
chatmsg(0, 0, "") {
    this->chatmsg = chatmsg;
    this->userinfo = userinfo;
}

//getter
ChatMessage& GroupChatMessage::getChatMessage() {
    return chatmsg;
}

UserInfo& GroupChatMessage::getUserInfo() {
    return userinfo;
}
