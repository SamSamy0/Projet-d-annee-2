#include "receiverwindow.hpp"
#include <iostream>
#include "../Window/MenuView.hpp"
#include "../Window/projectWidgets/GameView.hpp"


ReceiverInWindow::ReceiverInWindow(Application& app): app_(&app){ 
}


void ReceiverInWindow::switchConnectState(uint8_t connect){
    if (connect == 1){
        app_->changeView(std::make_unique<MenuView>(*app_));
        app_->getNetwork().getProjectList();
    }
}

void ReceiverInWindow::addProjectToList(ProjectData projet){
    app_->addProjectList(projet);
}

void ReceiverInWindow::updateProjectNameInList(uint64_t id, const std::string& newName){
    app_->updateProjectNameInList(id, newName);
    
}

void ReceiverInWindow::updateCreatedProjectId(uint32_t projId) {
    app_->updateCreatedProjectId(projId);
}

void ReceiverInWindow::setState(){
    app_->changeView(std::make_unique<GameView>(*app_));
}
