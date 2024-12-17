#ifndef SETTINGS_VIEW_HPP
#define SETTINGS_VIEW_HPP

#include "View.hpp"

class SettingsView : public View {
public:
    SettingsView();
    void onActivate() override;
private:
    void createUI();
};

#endif
