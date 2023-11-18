#include "executor.h"
#include <string>
#include <fstream>
//#include "fileBrowser.h"
#include "imgui.h"

struct tabData
{
    bool hasCustomName = false;

    std::string name;
    std::string path;
};

struct Funcs
{
    static int MyResizeCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            ImVector<char>* my_str = (ImVector<char>*)data->UserData;
            IM_ASSERT(my_str->begin() == data->Buf);
            my_str->resize(data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
            data->Buf = my_str->begin();
        }
        return 0;
    }

    // Note: Because ImGui:: is a namespace you would typically add your own function into the namespace.
    // For example, you code may declare a function 'ImGui::InputText(const char* label, MyString* my_str)'
    static bool MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, Funcs::MyResizeCallback, (void*)my_str);
    }
};

void ExecutorUi::render()
{
    /*
    * An ui made of tabs who show a text input thing,
    * a button "open" and "save",
    * a button "execute"
    */

    ImGui::Begin("Text editor");

    static ImVector<int> active_tabs;
    static tabData tabs[100];
    static ImVector<char> text[100];

    static int next_tab_id = 0;
    if (next_tab_id == 0) // Initialize with some default tabs
        active_tabs.push_back(next_tab_id++);

    // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown;
  
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        // Demo a Leading TabItemButton(): click the "?" button to open a menu
            if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
                ImGui::OpenPopup("MyHelpMenu");
        if (ImGui::BeginPopup("MyHelpMenu"))
        {
            // help menu code
            ImGui::Selectable("Be carefull, there's a limit of 100 tabs\n");
            ImGui::EndPopup();
        }

        // Demo Trailing Tabs: click the "+" button to add a new tab (in your app you may want to use a font icon instead of the "+")
        // Note that we submit it before the regular tabs, but because of the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
            if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                active_tabs.push_back(next_tab_id++); // Add new tab

        // Submit our regular tabs
        for (int n = 0; n < active_tabs.Size; )
        {
            bool open = true;
            std::string name;
            tabData current_tab = tabs[active_tabs[n]];

            if (current_tab.hasCustomName)
            {
                name = current_tab.name;
            }
            else
            {
                name = "Untitled " + std::to_string(active_tabs[n]);
            }

            if (ImGui::BeginTabItem(name.c_str(), &open, ImGuiTabItemFlags_None))
            {

                // For this demo we are using ImVector as a string container.
                // Note that because we need to store a terminating zero character, our size/capacity are 1 more
                // than usually reported by a typical string class.

                ImVector<char> current_text = text[active_tabs[n]];
                if (current_text.empty())
                    current_text.push_back(0);
                Funcs::MyInputTextMultiline("##MyStr", &current_text, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
                //ImGui::Text("Data: %p\nSize: %d\nCapacity: %d", (void*)current_text.begin(), current_text.size(), current_text.capacity()); //that's some usefull info
                text[active_tabs[n]] = current_text;

                if (ImGui::Button("Open file"))
                {
                    //tabs[active_tabs[n]].path = openFile();
                }

                ImGui::EndTabItem();
            }

            if (!open)
                active_tabs.erase(active_tabs.Data + n);
            else
                n++;
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
