#include "receiverwindow.hpp"


ReceiverInWindow::ReceiverInWindow(Window& w): window_(&w){ 
}


void ReceiverInWindow::switchConnectState(uint8_t connect){
    if (connect == 1){
        window_->setLogIn();
    }
}
