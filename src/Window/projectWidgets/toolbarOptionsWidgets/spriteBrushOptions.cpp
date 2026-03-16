#include "../../Window.hpp"
#include <unordered_map>

void Window::initSpriteBrushOptions() {
    float width  = mainWindow.getSize().x;
    float height = mainWindow.getSize().y;

    spriteBrushOptionsPanel_ = tgui::Panel::create();
    spriteBrushOptionsPanel_->setSize(width * 0.50, height * 0.4);
    spriteBrushOptionsPanel_->setPosition(width * 0.34, height * 0.05);
    spriteBrushOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    spriteBrushOptionsPanel_->getRenderer()->setBorders({1});
    spriteBrushOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    spriteBrushOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
    spriteBrushOptionsPanel_->setVisible(false);
    gui.add(spriteBrushOptionsPanel_);

    auto picture = tgui::Picture::create(tgui::Texture("../res/sprites/tree_1.png"));
    picture->setSize(128, 128);
    picture->setPosition(0, 0);
    spriteBrushOptionsPanel_->add(picture);
}
