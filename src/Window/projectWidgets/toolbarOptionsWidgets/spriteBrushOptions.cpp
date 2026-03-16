#include "../../Window.hpp"
#include "../../../project/Tool/spritebrush.hpp"
#include <iostream>

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

    auto scrollPanel = tgui::ScrollablePanel::create();
    scrollPanel->setSize("100%", "100%");
    scrollPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    scrollPanel->getRenderer()->setBorders({0});
    scrollPanel->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    scrollPanel->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    spriteBrushOptionsPanel_->add(scrollPanel);

    const auto& allSprites = project->getMap()->getAssetManager().getAllAssets();

    const float cols = 5.f;
    const float padding = 10.f;
    float panelWidth = spriteBrushOptionsPanel_->getSize().x;
    float cellSize = (panelWidth - (cols + 1.f) * padding) / cols;
    int col = 0;
    int row = 0;

    for (auto& [id, asset] : allSprites) {
        float x = padding + col * (cellSize + padding);
        float y = padding + row * (cellSize + padding);

        tgui::Texture tex("../res/sprites/" + asset.filename);

        auto btn = tgui::Button::create();
        btn->setSize(cellSize, cellSize);
        btn->setPosition(x, y);
        btn->setText("");
        btn->getRenderer()->setTexture(tex);
        btn->getRenderer()->setTextureHover(tex);
        btn->getRenderer()->setTextureDown(tex);
        btn->getRenderer()->setBorders({3});
        btn->getRenderer()->setBorderColor(tgui::Color::Transparent);
        btn->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
        btn->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
        btn->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
        btn->getRenderer()->setBackgroundColorHover(tgui::Color::Transparent);
        btn->getRenderer()->setBackgroundColorDown(tgui::Color::Transparent);

        std::string assetId = id;
        auto selected = std::make_shared<bool>(false);
        btn->onClick([this, btn, assetId, selected]() {
            cout << "appuyé" <<endl;
            auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
            cout << "avant check" <<endl;
            if (!tool) return;
            cout << "apres check" <<endl;
            *selected = !*selected;
            if (*selected) {
                btn->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
                tool->addAsset(assetId);
                cout << "bingo" <<endl;
            } else {
                btn->getRenderer()->setBorderColor(tgui::Color::Transparent);
                tool->removeAsset(assetId);
            }
        });

        scrollPanel->add(btn);

        col++;
        if (col >= static_cast<int>(cols)) { col = 0; row++; }
    }
}
