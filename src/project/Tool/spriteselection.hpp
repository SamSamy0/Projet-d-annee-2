#pragma once
#include "tool.hpp"
#include "../Layer/spritelayer.hpp"
#include <memory>


class SpriteSelection : public Tool {

  enum SelectionState{
    NONE,
    SELECTION,
    DRAGING
  };

  std::vector<uint64_t> selected_;
  SelectionState statuts_ = NONE;
  sf::Vector2i startSelectionPos_;
  bool isSelected(uint64_t id)const;
public:
  SpriteSelection(std::shared_ptr<Map> map);
  void deleteSelectedSprite();
  void clearSelection();
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos)override;
  void onRelease() override;
};
