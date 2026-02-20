#include "displayWindow.hpp"
#include "../project/project.hpp"
#include "../project/tool.hpp"
#include "../server/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <iostream>

void Window::signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  // NOTE: I have to ask how the usernames are stored in the server
  // -> Asked Tomas
  // NOTE: I have to put the condition username 3-15 car

  // Sending Identifiers to server
  // NOTE: Need to forbid empty usrnames
  if (usrname->getText() != "") {
    manager.askRegister(static_cast<std::string>(usrname->getText()),
                        static_cast<std::string>(pswd->getText()));
    std::cout << usrname->getText() << "   " << pswd->getText() << std::endl;

    if (!isLoggedIn) {
      // Ask another Usrname
      usrname->setText("");
      usrname->setDefaultText("Username already taken or invalid ");
      pswd->setText("");
    } else {
      // Change from Login menu -> Game menu
      // state = projectState::MENU;
    }
    // initMenuWidget();
    setState(projectState::MENU);
  }
}

void Window::login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  manager.login(static_cast<std::string>(usrname->getText()),
                static_cast<std::string>(pswd->getText()));
  // If the handler changed isLoggedIn to true, access granted
  if (isLoggedIn) {
    setState(projectState::MENU);
  }
  // The identifiers were wrong
  else {
    usrname->setDefaultText("Wrong Identifier(s)");
    pswd->setText("");
  }
}
void Window::loginWidget() {
  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"37.5%", "25%"});
  editBoxUsername->setSize({"25%", "6.25%"});
  editBoxUsername->setDefaultText("Username...");
  gui.add(editBoxUsername, "Username");

  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxPassword->setPosition({"37.5%", "43.75%"});
  editBoxPassword->setDefaultText("Password...");
  editBoxPassword->setPasswordCharacter('*');
  gui.add(editBoxPassword, "Password");

  auto loginButton = tgui::Button::create("Login");
  loginButton->setPosition({"25%", "53.125%"});
  loginButton->setSize({"12.5%", "3.125%"});
  gui.add(loginButton);
  loginButton->onPress(&Window::login, this, editBoxUsername, editBoxPassword);

  auto signInButton = tgui::Button::create("Sign in");
  signInButton->setPosition({"62.5%", "53.125%"});
  signInButton->setSize({"12.5%", "3.125%"});
  gui.add(signInButton);
  signInButton->onPress(&Window::signIn, this, editBoxUsername,
                        editBoxPassword);
}

void Window::refreshLayerList() {
  layersList_->removeAllWidgets();
  const vector<shared_ptr<Layer>>& layers = project->getMap()->getLayers();
    for (size_t i = 0; i<layers.size(); i++) {
      auto layerButton = tgui::Button::create("Couche " + std::to_string(i+1));
      layerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
      layerButton->getRenderer()->setTextColor(tgui::Color::White);
      layerButton->getRenderer()->setBorders({2});
      layerButton->getRenderer()->setBorderColor(tgui::Color(36, 40, 47));
      layerButton->setSize(240, 40);
      layerButton->setPosition(10, i * 50);
      layerButton->onClick([this, i]() { project->getMap()->setLayerSelected(static_cast<unsigned int>(i)); });
      layersList_->add(layerButton);
    }
    
  layersList_->setContentSize(sf::Vector2f(300, layers.size() * 50));
}

void Window::initWidget() {
  gui.removeAllWidgets();
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  if (this->state == projectState::LOGIN) {
    Window::loginWidget();
  } else if (this->state == projectState::MENU) {
    Window::initMenuWidget();
  } else if (this->state == projectState::GAME) {
    // Ajout du bouton de Home (retour en arrière)
    auto homeButton = tgui::Button::create();
    homeButton->setSize(30, 30);
    homeButton->setPosition(15, 5);
    homeButton->getRenderer()->setTexture("../res/images/accueil.png");
    homeButton->getRenderer()->setBorders({0});
    homeButton->onPress(&Window::setState, this, projectState::MENU);
    gui.add(homeButton);

    // Ajout du bouton de zoom in
    auto zoomInButton = tgui::Button::create();
    zoomInButton->setSize(30, 30);
    zoomInButton->setPosition(80, 5);
    zoomInButton->getRenderer()->setTexture("../res/images/zoom-avant.png");
    zoomInButton->getRenderer()->setBorders({0});
    zoomInButton->onPress([this]() {
        project->getMap()->getZoom().zoomIn();
    });
    gui.add(zoomInButton);

    // Ajout du bouton de zoom out
    auto zoomOutButton = tgui::Button::create();
    zoomOutButton->setSize(30, 30);
    zoomOutButton->setPosition(125, 5);
    zoomOutButton->getRenderer()->setTexture("../res/images/zoom-arriere.png");
    zoomOutButton->getRenderer()->setBorders({0});
    zoomOutButton->onPress([this]() {
        project->getMap()->getZoom().zoomOut();
    });
    gui.add(zoomOutButton);

    // Ajout boutton crayon
    auto penButton = tgui::Button::create();
    penButton->setSize(25, 25);
    penButton->setPosition(350, 9);
    penButton->getRenderer()->setTexture("../res/images/crayon.png");
    penButton->getRenderer()->setBorders({0});
    penButton->onPress([this]() {
        project->getToolBar().selectTool(PIXELBRUSH);
        project->getToolBar().getSelectedTool()->setErraser(false);
    });
    gui.add(penButton);

    // Ajout boutton gomme
    auto brushButton = tgui::Button::create();
    brushButton->setSize(30, 30);
    brushButton->setPosition(420, 13);
    brushButton->getRenderer()->setTexture("../res/images/la-gomme.png");
    brushButton->getRenderer()->setBorders({0});
    brushButton->onPress([this]() {
        project->getToolBar().selectTool(PIXELBRUSH);
        project->getToolBar().getSelectedTool()->setErraser(true);
    });
    gui.add(brushButton);

    // Ajout boutton deplacement couche
    auto shiftButton = tgui::Button::create();
    shiftButton->setSize(30, 30);
    shiftButton->setPosition(490, 13);
    shiftButton->getRenderer()->setTexture("../res/images/couche.png");
    shiftButton->getRenderer()->setBorders({0});
    shiftButton->onPress([this]() {
        project->getToolBar().selectTool(PIXELSHIFT);
    });
    gui.add(shiftButton);

    // Ajout de la liste des couches
    layersList_ = tgui::ScrollablePanel::create();
    layersList_->setSize(mainWindow.getSize().x * 0.18, mainWindow.getSize().y * 0.28);
    layersList_->setPosition(mainWindow.getSize().x * 0.02, mainWindow.getSize().y * 0.14);
    layersList_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    layersList_->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);

    vector<shared_ptr<Layer>>& layers = project->getMap()->getLayers();
    for (size_t i = 0; i<layers.size(); i++) {
      auto layerButton = tgui::Button::create("Couche " + std::to_string(i+1));
      layerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
      layerButton->getRenderer()->setTextColor(tgui::Color::White);
      layerButton->getRenderer()->setBorders({2});
      layerButton->getRenderer()->setBorderColor(tgui::Color(36, 40, 47));
      layerButton->setSize(240, 40);
      layerButton->setPosition(10, i * 50);
      layerButton->onClick([this, i]() { project->getMap()->setLayerSelected(static_cast<unsigned int>(i)); });
      layersList_->add(layerButton);
    }
    
    layersList_->setContentSize(sf::Vector2f(300, layers.size() * 50));
    gui.add(layersList_);

    // Ajout du bouton Créer une couche
    auto addLayerButton = tgui::Button::create("Ajouter couche");
    addLayerButton->setSize({mainWindow.getSize().x * 0.165, 40});
    addLayerButton->setPosition(mainWindow.getSize().x * 0.03, mainWindow.getSize().y * 0.50);
    addLayerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    addLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(65, 72, 84));
    addLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    addLayerButton->getRenderer()->setBorders({1});
    addLayerButton->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    addLayerButton->onClick([this]() {
      project->getMap()->createPixelLayer();
      refreshLayerList();
    });
    gui.add(addLayerButton);
    addLayerButton->setTextSize(19);

    // Ajout du bouton supprimer une couche
    auto removeLayerButton = tgui::Button::create("Supprimer couche");
    removeLayerButton->setSize({mainWindow.getSize().x * 0.165, 40});
    removeLayerButton->setPosition(mainWindow.getSize().x * 0.03, mainWindow.getSize().y * 0.55);
    removeLayerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    removeLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(65, 72, 84));
    removeLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    removeLayerButton->getRenderer()->setBorders({1});
    removeLayerButton->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    removeLayerButton->onClick([this]() {
      vector<shared_ptr<Layer>>& layers = project->getMap()->getLayers();
      if (layers.size() <= 1) return; 
      layers.erase(layers.begin() + project->getMap()->getLayerSelected());
      unsigned int newLayerSelected = project->getMap()->getLayerSelected() - 1;
      project->getMap()->setLayerSelected(newLayerSelected);
      refreshLayerList();
      
    });
    gui.add(removeLayerButton);
    removeLayerButton->setTextSize(19);
  }
}

void Window::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}
void Window::processEvents() {
  while (const std::optional event = mainWindow.pollEvent()) {
    gui.handleEvent(*event);
    // Close the Window
    if (event->is<sf::Event::Closed>())
      mainWindow.close();

    // Echap to escape
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
        mainWindow.close();
    }

    //Dans un projet
    if(state == projectState::GAME && project){

      project->getMap()->detectMovement();

      
      
      

      //TOOLS
      if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
        sf::Vector2i mousePos = mousePressed->position; // mouse position
        if (!gui.getWidgetBelowMouseCursor(mousePos, true)){ 
          sf::Vector2f pos = mainWindow.mapPixelToCoords(mousePos, project->getView());
          sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
          project->getToolBar().getSelectedTool()->onPress(mapPos);}

      }

      if (auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()){
        if (mouseEvent->button == sf::Mouse::Button::Left){
        if (!gui.getWidgetBelowMouseCursor(mouseEvent->position, true)){ 
        project->getToolBar().getSelectedTool()->onRelease();
        }}}

      if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = mouseMoved->position; // mouse position
        if (!gui.getWidgetBelowMouseCursor(mousePos, true)){ 
        sf::Vector2f pos = mainWindow.mapPixelToCoords(mousePos, project->getView());
        sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
        project->getToolBar().getSelectedTool()->onDrag(mapPos);
      }}



      //SET SIZE
      if (const auto* wheelEvent = event->getIf<sf::Event::MouseWheelScrolled>()){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem )|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
          // toolType tool = project->getToolBar().getSelected();
          // if(tool == PIXELBRUSH || tool == SPRITEBRUSH){
          //   sf::Vector2u size = project->getToolBar().getSelectedTool()->getSize();
          //   float sizex = size.x;
          //   float sizey = size.y;
          //   sf::Vector2u mapSize = project->getMap()->getSize();
          //   float step = std::min(mapSize.x,mapSize.y)/100.0f;
          //
          //   if (wheelEvent->delta > 0){sizex += 1; sizey += 1;}
          //   else if(wheelEvent->delta <0){sizex -= 1 ; sizey -= 1;}
          //
          //
          //   if (sizex<1){sizex = 1;}
          //   else if(sizex > mapSize.x ){sizex = mapSize.x;}
          //   if (sizey<1){sizey = 1;}
          //   else if(sizey > mapSize.y ){sizey = mapSize.y;}
          //
          //   size =  sf::Vector2u(static_cast<unsigned int>(std::round(sizex)), static_cast<unsigned int>(std::round(sizey)));
          //   project->getToolBar().getSelectedTool()->setSize(size.x,size.y);
          // }
        }
        else {
          if (!gui.getWidgetBelowMouseCursor(wheelEvent->position, true))
            project->getMap()->zooming(wheelEvent); //ZOOM
        }
      }
      }

  }
}


void Window::initMenuWidget() {
  gui.removeAllWidgets();
  auto title = tgui::Label::create("Liste de projets");
  title->setPosition("2%", "5%");
  title->setTextSize(40);

  title->getRenderer()->setTextStyle(tgui::TextStyle::Underlined);
  title->getRenderer()->setTextColor(sf::Color::Black);
  gui.add(title);

  auto createProjB = tgui::Button::create("Créer Projet");
  createProjB->setPosition({"62.5%", "31.25%"});
  createProjB->setSize({"20%", "4%"});
  gui.add(createProjB);
  createProjB->onPress(&Window::createProj, this);

  //  signInButton->onPress(&AuthWindow::signIn, this, editBoxUsername,
  // editBoxPassword);

  // loginButton->onPress(&Window::login, this, editBoxUsername,
  // editBoxPassword);

  auto joinProjB = tgui::Button::create("Rejoindre Projet");
  joinProjB->setPosition({"62.5%", "18.75%"});
  joinProjB->setSize({"20%", "4%"});
  gui.add(joinProjB);
}
void Window::createProj() {
  // Notify the server that a Proj is being created by using
  // ClientNetworkManager
  // manager.createProject("Owner", 100.0, 1);
  // Vector2u = map size
  // Project newProj(1, sf::Vector2u(500, 500), mainWindow);
  gui.removeAllWidgets();
  this->project = std::make_unique<Project>(5, sf::Vector2u(500, 500), "project", 1, mainWindow, gui);
  setState(projectState::GAME);
}

void Window::setState(projectState newState) {
  this->state = newState;
  this->initWidget();
}

Window::Window(ClientNetworkManager &manager)
    : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
                 sf::State::Fullscreen),
      gui{mainWindow}, manager{manager}, project{nullptr} {
  initWidget();
}
// Window::Window(ClientNetworkManager &manager)
//     : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
//                  sf::State::Fullscreen),
//       gui{mainWindow}, manager{manager} {
//   initWidget();
// }
void Window::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color::White);

    if (Window::state == projectState::LOGIN ||
      Window::state == projectState::MENU) {
    }
    else if (Window::state == projectState::GAME && project) {
      // Display Game
      project->display();
    }
    gui.draw();
    mainWindow.display();
  }
}
