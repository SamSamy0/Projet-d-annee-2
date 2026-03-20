#pragma once

#include "Application.hpp"
#include "View.hpp"

class MenuView : public View {
  std::vector<ProjectData> projectList = {};
  tgui::Button::Ptr activeMoreButton = nullptr;
  std::string shareToken = "FFFFF";

  void shareProj();
  void joinProj(tgui::Panel::Ptr panel);
  void displayProjList(tgui::Panel::Ptr parent);
  void showProjectMenu(ProjectData, tgui::Button::Ptr toHover);
  void initInputWidget(focusPopup focus, ProjectData project = ProjectData{});
  void popupRename(tgui::Panel::Ptr back, ProjectData project, focusPopup view);
  void popupCreate(tgui::Panel::Ptr background);
  void popupDuplicate(tgui::Panel::Ptr background, ProjectData project,
                      focusPopup view);
  void popupCreateToken(tgui::Panel::Ptr background, ProjectData project);
  tgui::Panel::Ptr displayToken();

  void exitAction(tgui::Panel::Ptr background);
  void closePopup();
  void createPopup();
  bool checkInput(tgui::EditBox::Ptr scale, tgui::EditBox::Ptr sizeX,
                  tgui::EditBox::Ptr sizeY);
  bool boxError(tgui::EditBox::Ptr box);
  void createProj(tgui::String scale, tgui::String sizeX, tgui::String sizeY,

                  tgui::String name, unsigned int id, sf::RenderWindow &window,
                  tgui::Gui &gui);
  ProjectData getProjectData(std::unique_ptr<Project> &proj);
  ProjectData askProjectData();
  void generateToken(uint8_t role, uint id );

public:
  MenuView(Application &app);
  ~MenuView() override = default;
  void init() override;
  void handleEvents(const sf::Event &event) override;

  void addProjectList(ProjectData projet);
  void updateProjectNameInList(long long id, const std::string &name);
  void updateList();
  void updateCreatedProjectId(uint32_t projId);
  void setShareToken(std::string token);
  void resetShareToken();
};
