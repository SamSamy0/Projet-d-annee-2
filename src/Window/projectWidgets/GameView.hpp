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
  bool Transferring = false;
  tgui::Button::Ptr activeMoreButton = nullptr;

  tgui::Panel::Ptr chatPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr chatMessages_ = nullptr;
  tgui::EditBox::Ptr chatInput_ = nullptr;
  tgui::Button::Ptr chatSendButton_ = nullptr;
  tgui::Panel::Ptr penOptionsPanel_ = nullptr;
  tgui::Panel::Ptr penSpriteOptionsPanel_ = nullptr;
  tgui::Panel::Ptr eraserOptionsPanel_ = nullptr;
  tgui::Panel::Ptr eraserSpriteOptionsPanel_ = nullptr;
  tgui::Panel::Ptr spriteBrushOptionsPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr importPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr natureSpritePanel_ = nullptr;
  tgui::ScrollablePanel::Ptr constructSpritePanel_ = nullptr;
  tgui::ScrollablePanel::Ptr objectsSpritePanel_ = nullptr;
  
  std::shared_ptr<std::vector<std::string>> spritesSelected_;

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
  void refreshLayerList();

  void displayMemberList();
  void getAllUsers();
  void showUserManagment(tgui::Button::Ptr toHover, int place);
  void exportToPng();

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

  void createLayer(LayerType);
  void deleteLayer(uint layer_id);
  void layerUp(uint layer_id);
  void layerDown(uint layer_id);
  void renameLayer(uint layer_id, std::string name);
  void setAllUsers(std::vector<MemberEntry> users);
  void updateMemberRole(uint targetId, int8_t role);
  void clearMemberList();
  void popupWarning(std::string motif);
  void refreshChat();
  void refreshImportPanel();
  void refreshNatureSpritePanel();
  void refreshConstructSpritePanel();
  void refreshObjectsSpritePanel();
  void refreshSpriteBrushOptions();
};
