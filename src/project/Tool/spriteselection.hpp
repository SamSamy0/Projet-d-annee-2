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


  std::vector<uint64_t> selected_;
  SelectionState state_ = NONE;
  sf::Vector2i startSelectionPos_;
  bool isSelected(uint64_t id)const;
public:
  SpriteSelection(std::shared_ptr<Map> map);
  std::vector<uint64_t>& getSelection();
  const SelectionState getState() const;
  void deleteSelectedSprite();
  void clearSelection();
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos)override{}
  void onRelease() override{}
};
