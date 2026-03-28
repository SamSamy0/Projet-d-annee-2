#pragma once
#include "../../server/datamanager/memberentry.hpp"
#include "../Application.hpp"
#include "../View.hpp"

class GameView : public View {
  bool isWatingRep = false;
  bool isToolSelected_ = false;
  tgui::Panel::Ptr layerPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr layersList_ = nullptr;
  Project *project;
  User currentUser;
  std::vector<MemberEntry> allUsers_;

  // Chat widget
  tgui::Panel::Ptr chatPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr chatMessages_ = nullptr;
  tgui::EditBox::Ptr chatInput_ = nullptr;
  tgui::Button::Ptr chatSendButton_ = nullptr;

  // Pen options panel
  tgui::Panel::Ptr penOptionsPanel_ = nullptr;
  tgui::Panel::Ptr penSpriteOptionsPanel_ = nullptr;
  // Eraser options panel
  tgui::Panel::Ptr eraserOptionsPanel_ = nullptr;
  tgui::Panel::Ptr eraserSpriteOptionsPanel_ = nullptr;
  tgui::Panel::Ptr spriteBrushOptionsPanel_ = nullptr;

  void initToolbar();
  void initLayerPanel();
  void initChatWidget();
  void initPenOptions();
  void initPenSpriteOptions();
  void initEraserOptions();
  void initEraserSpriteOptions();
  void initSpriteBrushOptions();
  void initMinimap();
  void drawMinimap();
  void refreshChat();
  void refreshLayerList();

  void displayMemberList();
  void getAllUsers();

  // Detection in map
  void toolOnClick();
  void toolOnRelease();
  void checkTypeTool(LayerType previous_type);

public:
  GameView(Application &app);
  ~GameView() override = default;
  void init() override;
  void render();
  void handleEvents(const sf::Event &events) override;
  void setAllUsers(std::vector<MemberEntry> users);
  void clearMemberList();
};
