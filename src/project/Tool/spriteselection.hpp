#pragma once
#include "tool.hpp"
#include <memory>

enum class SelectionState {
  NONE,
  SELECTION,
  DRAGING,
  RESIZING,
  ROTATING,
};
class SpriteSelection : public Tool {

  std::vector<uint> selected_;
  SelectionState state_ = SelectionState::NONE;
  sf::Vector2i startSelectionPos_;

  bool isSelected(uint) const;
  sf::Vector2f findCenter();

public:
  SpriteSelection(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  std::vector<uint> &getSelection();
  const SelectionState getState() const;
  void deleteSelectedSprite();
  void clearSelection();
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos) override;
  void onRelease() override;
  void rotateSprite(uint spriteId, float angle, sf::Vector2f pivot);
  void resizeSprite(uint spriteId, float delta, sf::Vector2f pivot);
};
