#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/autofill.hpp"

void GameView::initAutoFillOptions() {
    auto& gui = app_.getGui();

    autoFillOptionsPanel_ = tgui::Panel::create();
    autoFillOptionsPanel_->setSize("15%", "35%");
    autoFillOptionsPanel_->setPosition("73%", "5%");
    autoFillOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    autoFillOptionsPanel_->getRenderer()->setBorders({1});
    autoFillOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    autoFillOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
    autoFillOptionsPanel_->setVisible(false);
    gui.add(autoFillOptionsPanel_);

    // Title
    auto title = tgui::Label::create("AutoFill Options");
    title->getRenderer()->setTextColor(tgui::Color::White);
    title->setTextSize(11);
    title->setPosition("5%", "5%");
    autoFillOptionsPanel_->add(title);

    // Size
    auto sizeEdit = tgui::EditBox::create();
    sizeEdit->setSize("90%", "10%");
    sizeEdit->setDefaultText("Size");
    sizeEdit->setInputValidator("[0-9]+\\.?[0-9]*");
    sizeEdit->setPosition("5%", "15%");
    sizeEdit->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    sizeEdit->getRenderer()->setTextColor(tgui::Color::White);
    sizeEdit->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
    sizeEdit->getRenderer()->setBorders({1});
    sizeEdit->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    sizeEdit->onTextChange([this](const tgui::String& text) {
        if (text.length() > 0) {
            try {
                float value = std::stof(text.toStdString());
                auto tool = project->getToolBar().getSelectedTool();
                auto autoFill = std::dynamic_pointer_cast<AutoFill>(tool);
                if (autoFill) autoFill->setSize(value);
            } catch (...) {}
        }
    });
    autoFillOptionsPanel_->add(sizeEdit);

    // Spacing
    auto spacingEdit = tgui::EditBox::create();
    spacingEdit->setSize("90%", "10%");
    spacingEdit->setPosition("5%", "30%");
    spacingEdit->setInputValidator("[0-9]+\\.?[0-9]*");
    spacingEdit->setDefaultText("Spacing");
    spacingEdit->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    spacingEdit->getRenderer()->setTextColor(tgui::Color::White);
    spacingEdit->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
    spacingEdit->getRenderer()->setBorders({1});
    spacingEdit->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    spacingEdit->onTextChange([this](const tgui::String& text) {
        if (text.length() > 0) {
            try {
                float value = std::stof(text.toStdString());
                auto tool = project->getToolBar().getSelectedTool();
                auto autoFill = std::dynamic_pointer_cast<AutoFill>(tool);
                if (autoFill) autoFill->setSpacing(value);
            } catch (...) {}
        }
    });
    autoFillOptionsPanel_->add(spacingEdit);

    // Rotation
    auto rotationEdit = tgui::EditBox::create();
    rotationEdit->setSize("90%", "10%");
    rotationEdit->setPosition("5%", "45%");
    rotationEdit->setInputValidator("[0-9]+\\.?[0-9]*");
    rotationEdit->setDefaultText("Rotation");
    rotationEdit->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    rotationEdit->getRenderer()->setTextColor(tgui::Color::White);
    rotationEdit->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
    rotationEdit->getRenderer()->setBorders({1});
    rotationEdit->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    rotationEdit->onTextChange([this](const tgui::String& text) {
        if (text.length() > 0) {
            try {
                float value = std::stof(text.toStdString());
                auto tool = project->getToolBar().getSelectedTool();
                auto autoFill = std::dynamic_pointer_cast<AutoFill>(tool);
                if (autoFill) autoFill->setRotation(value);
            } catch (...) {}
        }
    });
    autoFillOptionsPanel_->add(rotationEdit);

    // Seed
    auto seedEdit = tgui::EditBox::create();
    seedEdit->setSize("90%", "10%");
    seedEdit->setPosition("5%", "60%");
    seedEdit->setInputValidator("[0-9]*");
    seedEdit->setDefaultText("Seed");
    seedEdit->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    seedEdit->getRenderer()->setTextColor(tgui::Color::White);
    seedEdit->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
    seedEdit->getRenderer()->setBorders({1});
    seedEdit->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    seedEdit->onTextChange([this](const tgui::String& text) {
        if (text.length() > 0) {
            try {
                uint seed = std::stoul(text.toStdString());
                auto tool = project->getToolBar().getSelectedTool();
                auto autoFill = std::dynamic_pointer_cast<AutoFill>(tool);
                if (autoFill) autoFill->setSeed(seed);
            } catch (...) {}
        }
    });
    autoFillOptionsPanel_->add(seedEdit);


    // Validate / Cancel
    auto validateBtn = tgui::Button::create("Apply");
    validateBtn->setSize("42.5%", "12%");
    validateBtn->setPosition("5%", "80%");
    validateBtn->getRenderer()->setBackgroundColor(tgui::Color(60, 160, 60));
    validateBtn->getRenderer()->setTextColor(tgui::Color::White);
    validateBtn->getRenderer()->setBorders({1});
    validateBtn->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    validateBtn->onPress([this]() {
        auto tool = project->getToolBar().getSelectedTool();
        auto autoFill = std::dynamic_pointer_cast<AutoFill>(tool);
        if (autoFill) autoFill->apply();
    });
    autoFillOptionsPanel_->add(validateBtn);

    auto cancelBtn = tgui::Button::create("Cancel");
    cancelBtn->setSize("42.5%", "12%");
    cancelBtn->setPosition("52.5%", "80%");
    cancelBtn->getRenderer()->setBackgroundColor(tgui::Color(160, 60, 60));
    cancelBtn->getRenderer()->setTextColor(tgui::Color::White);
    cancelBtn->getRenderer()->setBorders({1});
    cancelBtn->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    cancelBtn->onPress([this]() {
        auto tool = project->getToolBar().getSelectedTool();
        auto autoFill = std::dynamic_pointer_cast<AutoFill>(tool);
        if (autoFill) autoFill->clear();
    });
    autoFillOptionsPanel_->add(cancelBtn);
}
