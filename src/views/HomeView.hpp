#ifndef HOME_VIEW_HPP
#define HOME_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <set>
#include "View.hpp"
#include "../core/ui/Component.hpp"
#include "../core/ui/Button.hpp"
#include "../core/ui/Text.hpp"

class HomeView : public View {
public:
    HomeView(); // Constructor
    void onActivate() override;
private:
    void createUI(); // Function to create UI components
};

#endif
