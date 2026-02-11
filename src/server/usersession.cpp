#include "usersession.h"

UserSession::UserSession(QTcpSocket* socket)
    : m_socket(socket), m_authenticated(false) {}

void UserSession::setPseudo(const QString& pseudo) {
    m_pseudo = pseudo;
    m_authenticated = true;
}

void UserSession::sendMessage(const QString& message) {
    if (m_socket && m_socket->isOpen()) {
        m_socket->write(message.toUtf8());
    }
}
