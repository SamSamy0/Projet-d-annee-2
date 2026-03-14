#include "receiverwindow.hpp"
#include <iostream>


ReceiverInWindow::ReceiverInWindow(Window& w): window_(&w){ 
}


void ReceiverInWindow::switchConnectState(uint8_t connect){
    if (connect == 1){
        window_->setLogIn();
    }
}

void ReceiverInWindow::addProjectToList(ProjectData projet){
    window_->addProjectList(projet);
}

void ReceiverInWindow::updateProjectNameInList(long long id, const std::string& newName){
    window_->updateProjectNameInList(id, newName);
    
}
