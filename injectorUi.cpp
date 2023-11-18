#include "injectorUi.h"
#include "imgui.h"
#include "llai.h"
#include "fileBrowser.h"

std::string filePath = "NULL_PATH";
std::string appToInject = "NULL_APP";

using namespace std;

std::string runningApp[100] = { "dummy ", "test", "new test" }; // contain all of the apps

//  shoutout for xCENTx on stackoverflow

namespace InjectorUi
{

    void render()
    {

        ImGui::Begin("Injector");
        static int item_current = 0;


        if (ImGui::Button("Inject"))
        {
            if (filePath != "NULL_PATH" && appToInject != "NULL_APP")
            {
                llai::loadDll(runningApp[item_current].c_str(), (char*)filePath.c_str());
            }
            else
            {
                MessageBox(0, "File isn't probably defined", "Can't inject", 0);
            }
        }
        if (ImGui::Button("Chose Dll"))
        {
            filePath = openFile();
        }
        ImGui::SameLine();
        ImGui::Text("file chosen: %s", filePath.c_str());

        ImGui::Combo("App to inject dll", &item_current, runningApp->c_str(), IM_ARRAYSIZE(runningApp));
        ImGui::SameLine();

        appToInject = runningApp[item_current];


        ImGui::End();
    }

}
