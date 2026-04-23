#include "chat.hpp"
#include "userMessage.hpp"
#include "systemNotification.hpp"
#include <QJsonObject>

using namespace std;

MessageChat::MessageChat(const User& author, const Date& date, MessageType type) : author_(author), date_(date), type_(type) {}

User MessageChat::getAuthor() const { return author_; }
Date MessageChat::getDate() const { return date_; }
MessageType MessageChat::getType() const { return type_; }

Date MessageChat::currentDate() {
  time_t t = time(nullptr);
  tm *now = localtime(&t);
  return Date(now->tm_min, now->tm_hour, now->tm_mday, now->tm_mon + 1, now->tm_year - 100);
}

void Chat::addMessage(shared_ptr<MessageChat> message) {
    messages_.push_back(message);
}

vector<shared_ptr<MessageChat>> Chat::getMessages() const {
    return messages_;
}

Chat::Chat(const QJsonArray& jsonArray) {
    for (const auto& item : jsonArray) {
        QJsonObject obj = item.toObject();

        string pseudo = obj["author"].toString().toStdString();
        uint id = static_cast<uint>(obj["id"].toInt());
        User author(pseudo, id);

        int min = obj["min"].toInt();
        int hour = obj["hour"].toInt();
        int day = obj["day"].toInt();
        int month = obj["month"].toInt();
        int year = obj["year"].toInt();
        Date date(min, hour, day, month, year);

        MessageType type = obj["type"].toString() == "USER" ? MessageType::USER : MessageType::SYSTEM;
        string texte = obj["texte"].toString().toStdString();

        if (type == MessageType::USER) {
            addMessage(make_shared<UserMessage>(author, date, texte));
        } else {
            addMessage(make_shared<SystemNotification>(author, date, texte));
        }
    }
}

QJsonArray Chat::toJson() const {
    QJsonArray jsonArray;
    for (const auto& message : messages_) {
        QJsonObject obj;
        obj["author"] = QString::fromStdString(message->getAuthor().getUser());
        obj["id"] = static_cast<int>(message->getAuthor().getId());

        obj["min"] = message->getDate().min_;
        obj["hour"] = message->getDate().hour_;
        obj["day"] = message->getDate().day_;
        obj["month"] = message->getDate().month_;
        obj["year"] = message->getDate().year_;

        obj["type"] = message->getType() == MessageType::USER ? "USER" : "SYSTEM";
        obj["texte"] = QString::fromStdString(message->getJsonTexte());
        jsonArray.append(obj);
    }
    return jsonArray;
}
