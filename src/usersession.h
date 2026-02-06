#ifndef USERSESSION_H
#define USERSESSION_H

#include <QString>
#include <QTcpSocket>

class UserSession {
public:
    explicit UserSession(QTcpSocket* socket);

    void setPseudo(const QString& pseudo);

    QString getPseudo() const { return m_pseudo; }
    QTcpSocket* socket() const { return m_socket; }
    bool isAuthenticated() const { return m_authenticated; }

    void sendMessage(const QString& message);

private:
    QTcpSocket* m_socket;
    QString m_pseudo;
    bool m_authenticated;
};

#endif
