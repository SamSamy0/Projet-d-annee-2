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

void ReceiverInWindow::updateProjectNameInList(uint64_t id, const std::string& newName){
    window_->updateProjectNameInList(id, newName);
    
}

void ReceiverInWindow::updateCreatedProjectId(uint32_t projId) {
    window_->updateCreatedProjectId(projId);
}

void ReceiverInWindow::setState(){
    window_->setState(projectState::GAME);
}
