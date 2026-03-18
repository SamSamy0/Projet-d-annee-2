#include "Application.hpp"
#include "LoginView.hpp"
#include "MenuView.hpp"
#include "projectWidgets/GameView.hpp"

Application::Application(ClientNetworkManager &manager)
    : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
                 sf::State::Fullscreen),
      gui{mainWindow}, manager{manager} {
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });
  changeView(std::make_unique<LoginView>(*this));
}

void Application::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}

void Application::changeView(std::unique_ptr<View> newView) {
  gui.removeAllWidgets();
  currentView = std::move(newView);
  currentView->init();
}

void Application::processEvents() {
  while (const std::optional event = mainWindow.pollEvent()) {
    gui.handleEvent(*event);

    if (event->is<sf::Event::Closed>())
      mainWindow.close();

    // Echap to escape
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
        mainWindow.close();
    }

    currentView->handleEvents(*event);
  }
}

void Application::run() {
  processEvents();

  mainWindow.clear(sf::Color(35, 35, 40));
  if (project)
    project->display();
  gui.draw();
  mainWindow.display();
}

void Application::setState(projectState newState) {
  this->state = newState;
  currentView->init();
}
bool Application::isOpen() const { return mainWindow.isOpen(); }
tgui::Gui &Application::getGui() { return gui; }
sf::RenderWindow &Application::getWindow() { return mainWindow; }

void Application::addProjectList(ProjectData project) {
  auto menuView = dynamic_cast<MenuView *>(currentView.get());
  menuView->addProjectList(project);
}
void Application::updateProjectNameInList(long long id,
                                          const std::string &newName) {
  auto menuView = dynamic_cast<MenuView *>(currentView.get());
  menuView->updateProjectNameInList(id, newName);
};
void Application::updateCreatedProjectId(uint32_t projId) {
  if (auto menuView = dynamic_cast<MenuView *>(currentView.get())) {
    menuView->updateCreatedProjectId(projId);
  };
};

ClientNetworkManager &Application::getNetwork() { return manager; }

User &Application::getUser() { return currentUser_; }

std::unique_ptr<View> &Application::getCurrentView() { return currentView; }
std::unique_ptr<Project> &Application::getProject() { return project; }


void Application::loadProjectData(unsigned int scale, sf::Vector2u size, std::string name, uint id){
  project = std::make_unique<Project>(scale,size,name,id,mainWindow,gui,manager);
  changeView(std::make_unique<GameView>(*this));
};