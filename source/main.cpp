#define TESLA_INIT_IMPL // If you have more than one file using the tesla header, only define this in the main one
#include <tesla.hpp>    // The Tesla Header
#include <vector>

std::vector<std::string> speed_options = {
    "Default",
    "0.5",
    "1.0",
    "1.5",
    "2.0",
    "2.5",
    "3.0"
};

std::vector<std::string> time_of_day_options = {
    "Default",
    "Daytime",
    "Evening",
    "Nighttime"
};



class GuiSecondary : public tsl::Gui {
public:
    std::string title;
    std::vector<std::string> options;
    std::string data;

    GuiSecondary(std::string title, std::vector<std::string> options, std::string data) {
        this->title = title;
        this->options = options;
        this->data = data;
    }

    virtual tsl::elm::Element* createUI() override {
        auto *rootFrame = new tsl::elm::OverlayFrame(title, "");

        // A list that can contain sub elements and handles scrolling
        auto list = new tsl::elm::List();

        for(std::string option : options){
            auto *clickableListItem = new tsl::elm::ListItem(option);

            clickableListItem->setClickListener([](u64 keys) { 
                if (keys & KEY_A) {
                    tsl::goBack();
                    return true;
                }
            });

            list->addItem(clickableListItem);
        }
        
        rootFrame->setContent(list);

        return rootFrame;
    }
};

class GuiTest : public tsl::Gui {
public:
    GuiTest(u8 arg1, u8 arg2, bool arg3) { }

    // Called when this Gui gets loaded to create the UI
    // Allocate all elements on the heap. libtesla will make sure to clean them up when not needed anymore
    virtual tsl::elm::Element* createUI() override {
        // A OverlayFrame is the base element every overlay consists of. This will draw the default Title and Subtitle.
        // If you need more information in the header or want to change it's look, use a HeaderOverlayFrame.
        auto frame = new tsl::elm::OverlayFrame("Ultimate Customizer (P.O.C.)", "v1.0.0");

        // A list that can contain sub elements and handles scrolling
        auto list = new tsl::elm::List();

        // List Items
        list->addItem(new tsl::elm::CategoryHeader("Town & City Options"));

        auto *clickableListItem = new tsl::elm::ListItem("Transition Speed");
        clickableListItem->setClickListener([](u64 keys) { 
            if (keys & KEY_A) {
                tsl::changeTo<GuiSecondary>("Transition Speed", speed_options, "tncSpeed");
                return true;
            }

            return false;
        });

        list->addItem(clickableListItem);
        
        // Add the list to the frame for it to be drawn
        frame->setContent(list);
        
        // Return the frame to have it become the top level element of this Gui
        return frame;
    }

    // Called once every frame to update values
    virtual void update() override {

    }

    // Called once every frame to handle inputs not handled by other UI elements
    virtual bool handleInput(u64 keysDown, u64 keysHeld, touchPosition touchInput, JoystickPosition leftJoyStick, JoystickPosition rightJoyStick) override {
        return false;   // Return true here to singal the inputs have been consumed
    }
};

class OverlayTest : public tsl::Overlay {
public:
                                             // libtesla already initialized fs, hid, pl, pmdmnt, hid:sys and set:sys
    virtual void initServices() override {}  // Called at the start to initialize all services necessary for this Overlay
    virtual void exitServices() override {}  // Callet at the end to clean up all services previously initialized

    virtual void onShow() override {}    // Called before overlay wants to change from invisible to visible state
    virtual void onHide() override {}    // Called before overlay wants to change from visible to invisible state

    virtual std::unique_ptr<tsl::Gui> loadInitialGui() override {
        return initially<GuiTest>(1, 2, true);  // Initial Gui to load. It's possible to pass arguments to it's constructor like this
    }
};

int main(int argc, char **argv) {
    return tsl::loop<OverlayTest>(argc, argv);
}
