#include "imgui_custom.h"
#include "application.h"
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#include "platform/platform.h"

#if defined(_MSC_VER) && !defined(snprintf)
#define snprintf    _snprintf
#endif
#if defined(_MSC_VER) && !defined(vsnprintf)
#define vsnprintf   _vsnprintf
#endif

namespace ImGui {
void CustomSetup()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
}

struct AppConsole {
    char input_text[256];
    std::vector<std::string> items;
    
    AppConsole()
    {
        items.emplace_back("iocod console\n");
    }

    ~AppConsole()
    {

    }

    void clear_log()
    {
        items.clear();
    }

    void add_log(const std::string &message)
    {
        items.emplace_back(message);
    }

    // command completion
    int command_completion_callback(ImGuiInputTextCallbackData *data)
    {
        return 0;
    }

    void print_os_version()
    {
        add_log(platform->get_os_version_string() + "\n");
    }

    void exec(const std::string &command)
    {
        add_log("> " + command + "\n");

        if (command == "clear")
            clear_log();
        else if (command == "quit" || command == "exit")
            app->request_exit();
        else if (command == "osversion")
            print_os_version();
        else
            add_log("Unknown command '" + command + "'\n");
    }

    void draw()
    {
        static bool copy_to_clipboard = false;

        bool active = true;
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;

        ImGui::SetNextWindowSize(ImVec2(800, 600));
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        if (!ImGui::Begin("console", &active, window_flags)) {
            ImGui::End();
            return;
        }

        //
        // console
        //
        const float footer_height_to_reserve = 
            ImGui::GetStyle().ItemSpacing.y + 
            ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion",
                          ImVec2(0, -footer_height_to_reserve),
                          false,
                          ImGuiWindowFlags_None);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

        if (copy_to_clipboard)
            ImGui::LogToClipboard();

        for (const auto &str : items) {
            ImVec4 color;
            bool colored = false;

            // commands entered
            if (str.substr(0, 2) == "> ") {
                color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                colored = true;
            }

            if (colored)
                ImGui::PushStyleColor(ImGuiCol_Text, color);

            ImGui::TextWrapped(str.c_str());

            if (colored)
                ImGui::PopStyleColor();
        }

        if (copy_to_clipboard)
            ImGui::LogFinish();

        // always scroll to the bottom
        ImGui::SetScrollHereY(1.0f);

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        //
        // buttons
        //
        bool reclaim_focus = false;
        if (ImGui::Button("clear")) {
            clear_log();
            reclaim_focus = true;
        }

        ImVec2 button_rect = ImGui::GetItemRectSize();

        ImGui::SameLine();

        copy_to_clipboard = ImGui::Button("copy");
        if (copy_to_clipboard)
            reclaim_focus = true;

        ImGui::SameLine();

        //
        // text input
        //
        ImGuiInputTextFlags text_flags =
            ImGuiInputTextFlags_EnterReturnsTrue |
            ImGuiInputTextFlags_CallbackCompletion |
            ImGuiInputTextFlags_CallbackHistory;

        auto completion_callback = [](ImGuiInputTextCallbackData *data) {
            AppConsole *console = (AppConsole *) data->UserData;
            return console->command_completion_callback(data);
        };

        ImGui::PushItemWidth(590.0f);
        if (ImGui::InputTextWithHint("console input", "type a command here",
                                     input_text, IM_ARRAYSIZE(input_text), 
                                     text_flags, completion_callback, 
                                     (void *) this)) {
            std::string str(input_text);

            // trim beginning
            str.erase(str.begin(),
                      std::find_if(str.begin(), str.end(),
                                   [](unsigned char ch) {
                                       return !std::isspace(ch);
                                   }));

            // trim end
            str.erase(std::find_if(str.rbegin(), str.rend(),
                                   [](unsigned char ch) {
                                       return !std::isspace(ch);
                                   }).base(), str.end());

            if (!str.empty())
                exec(str);

            input_text[0] = 0;
            reclaim_focus = true;
        }

        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1);

        ImGui::End();
    }
};

void CustomFrame()
{
    static AppConsole console;
    console.draw();
}
};
