#pragma once
#include "../client/clientnetwork.hpp"
#include "../project/Tool/tool.hpp"
#include "../project/project.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Layout.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <iostream>
#include <memory>
#include <vector>
enum class projectState { LOGIN, MENU, GAME };
enum class focusPopup { CREATE, RENAME, DUPLICATE, TOKEN };
class Application;
class View {
protected:
  Application &app_;

public:
  View(Application &app) : app_{app} {};
  virtual ~View() = default;
  virtual void init() = 0;
  virtual void handleEvents(const sf::Event &event) = 0;
};
