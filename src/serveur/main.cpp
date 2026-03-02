#include "servernetwork.hpp"

int main() {
    MutexQueue<IMessage> mesQ;
    MutexQueue<Reponse> repQ;
    ServerNetworkManager servNewt(mesQ, repQ);
    servNewt.run();

}