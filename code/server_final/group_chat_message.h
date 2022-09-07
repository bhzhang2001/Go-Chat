//group_chat_message.h
#ifndef GROUP_CHAT_MESSAGE_H
#define GROUP_CHAT_MESSAGE_H

#include "chatmessage.h"
#include "userinfo.h"

class GroupChatMessage {
public:
    explicit GroupChatMessage(const ChatMessage& chatmsg, const UserInfo& userinfo);
    ChatMessage& getChatMessage();
    UserInfo& getUserInfo();
private:
    ChatMessage chatmsg;
    UserInfo userinfo;
};

#endif // GROUP_CHAT_MESSAGE_H


