#pragma once
#include "tool.hpp"
#include "../Layer/spritelayer.hpp"
#include <memory>


enum SelectionState{
  NONE,
  SELECTION,
  DRAGING,
  RESIZING,
  ROTATING,
};
class SpriteSelection : public Tool {


  std::vector<uint> selected_;
  SelectionState state_ = NONE;
  sf::Vector2i startSelectionPos_;
  bool isSelected(uint)const;
public:
  SpriteSelection(std::shared_ptr<Map> map, ClientNetworkManager& manager);
  std::vector<uint>& getSelection();
  const SelectionState getState() const;
  void deleteSelectedSprite();
  void clearSelection();
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos)override{}
  void onRelease() override{}
};
