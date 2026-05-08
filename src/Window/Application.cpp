#include "Application.hpp"
#include "../project/Layer/pixellayer.hpp"
#include "../project/Layer/spritelayer.hpp"
#include "IpView.hpp"
#include "LoginView.hpp"
#include "MenuView.hpp"
#include "projectWidgets/GameView.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Application::Application(ClientNetworkManager &manager)
    : mainWindow(
          sf::VideoMode({static_cast<unsigned int>(
                             sf::VideoMode::getDesktopMode().size.x * 0.90),
                         static_cast<unsigned int>(
                             sf::VideoMode::getDesktopMode().size.y * 0.90)}),
          "OpenRPG", sf::Style::Default),
      gui{mainWindow}, manager{manager} {
  // Centering the Window
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  unsigned int windowWidth = desktop.size.x * 0.90;
  unsigned int windowHeight = desktop.size.y * 0.90;

  mainWindow.setPosition(sf::Vector2i((desktop.size.x - windowWidth) / 2,
                                      (desktop.size.y - windowHeight) / 2));
  mainWindow.setMinimumSize(sf::Vector2u(1000, 600));
  // ------------------------------

  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });
  changeView(std::make_unique<IpView>(*this));
}

void Application::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}

void Application::showLoginError(const std::string &message) {
  if (auto loginView = dynamic_cast<LoginView *>(currentView.get())) {
    loginView->showError(message);
  }
}

void Application::clearProjList() {
  if (auto menuView = dynamic_cast<MenuView *>(currentView.get())) {
    menuView->clearProjList();
  }
}

void Application::refreshImportPanel() {
  auto gameView = dynamic_cast<GameView *>(currentView.get());
  gameView->refreshImportPanel();
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
      if (keyPressed->code == sf::Keyboard::Key::C && keyPressed->control) {
        mainWindow.close();
      }
    }

    currentView->handleEvents(*event);
  }
}

void Application::run() {
  processEvents();

  mainWindow.clear(sf::Color(35, 35, 40));
  bool inGame = dynamic_cast<GameView *>(currentView.get()) != nullptr;
  if (project && inGame)
    project->display();
  currentView->render();
  gui.draw();
  if (project && inGame)
    project->displayScale();
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
  project->setId(projId);

  if (auto menuView = dynamic_cast<MenuView *>(currentView.get())) {
    // If we still are on menu
    menuView->updateCreatedProjectId(projId);
  };
};

void Application::updateShareToken(std::string newTok) {
  std::cout << "token in app" << newTok << std::endl;
  auto menuView = dynamic_cast<MenuView *>(currentView.get());
  menuView->setShareToken(newTok);
}

ClientNetworkManager &Application::getNetwork() { return manager; }

User &Application::getUser() { return currentUser_; }

std::unique_ptr<View> &Application::getCurrentView() { return currentView; }
std::unique_ptr<Project> &Application::getProject() { return project; }

void Application::loadProjectData(unsigned int scale, sf::Vector2u size,
                                  std::string name, uint id) {
  project = std::make_unique<Project>(scale, size, name, id, mainWindow, gui,
                                      manager, currentProjRole);
  if (isExporting_) {
    bool res = project->exportToPng(exportFormat_);

  } else {
    std::cout << "opening with authorisation " << currentProjRole << std::endl;
    changeView(std::make_unique<GameView>(*this));
  }
};

void Application::loadProjectData(
    unsigned int scale, sf::Vector2u size, std::string name, uint id,
    uint nextLayerId, const std::vector<LayerLoadData> &layers, Chat chat,
    const std::map<uint, sf::Texture> &textureMap) {
  // Constructeur avec vecteur vide → aucun layer par défaut créé
  project = std::make_unique<Project>(
      scale, size, name, id, mainWindow, gui, manager,
      std::vector<std::shared_ptr<Layer>>{}, currentProjRole);
  auto map = project->getMap();
  project->setChat(chat);
  for (const auto &[id, texture] : textureMap) {
    map->getAssetManager().addAsset(id, texture);
  }

  for (const auto &ld : layers) {
    ;
    if (ld.type == 0) {
      // --- PixelLayer : données = PNG brut ---
      auto layer = std::make_shared<PixelLayer>(ld.id, ld.name, size);
      sf::Texture texture;
      if (texture.loadFromMemory(ld.data.constData(), ld.data.size())) {
        sf::Sprite sprite(texture);
        layer->draw(sprite);
        layer->display();
      }
      if (ld.x != 0 || ld.y != 0)
        layer->shift(sf::Vector2i(ld.x, ld.y));
      map->getLayers().push_back(layer);

    } else {
      // --- SpriteLayer : données = JSON compressé ---
      auto layer = std::make_shared<SpriteLayer>(ld.id, ld.name, size);
      QByteArray jsonRaw = qUncompress(ld.data);
      if (!jsonRaw.isEmpty()) {
        QJsonObject spriteJson = QJsonDocument::fromJson(jsonRaw).object();

        layer->setNextId(static_cast<uint>(spriteJson["nextId"].toInt()));

        QJsonArray sprites = spriteJson["sprites"].toArray();
        for (const auto &sv : sprites) {
          QJsonObject s = sv.toObject();
          std::string nameId = s["nameId"].toString().toStdString();
          int sx = s["x"].toInt();
          int sy = s["y"].toInt();
          float spriteSize = static_cast<float>(s["size"].toDouble());
          float angle = static_cast<float>(s["angle"].toDouble());

          Asset *asset = map->getAssetManager().getAsset(nameId);
          if (asset && asset->texture) {
            sf::Sprite sprite(*(asset->texture));
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(
                sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));
            float spriteScale = spriteSize *
                                static_cast<float>(map->getScale()) /
                                bounds.size.x;
            sprite.setScale(sf::Vector2f(spriteScale, spriteScale));
            sprite.setPosition(
                sf::Vector2f(static_cast<float>(sx), static_cast<float>(sy)));
            layer->draw(sprite, nameId);
          }
        }
      }
      if (ld.x != 0 || ld.y != 0)
        layer->shift(sf::Vector2i(ld.x, ld.y));
      map->getLayers().push_back(layer);
    }
  }

  map->setNextLayerId(nextLayerId);
  // Exporting to png
  if (isExporting_) {
    isExporting_ = false;
    bool res = project->exportToPng(exportFormat_);
    res ? currentView->popupWarning("exportPngOK")
        : currentView->popupWarning("exportPngKO");
  } else {
    changeView(std::make_unique<GameView>(*this));
  }
};
