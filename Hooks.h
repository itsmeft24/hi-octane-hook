#pragma once
#include <unordered_map>
#include <Windows.h>
#include <winternl.h>

namespace CarsActivityUI_RequestDialogueHook
{

    DECL_FUNCTION(void, __thiscall, AddNameToDialogueList, CarsActivity_AddNameToDialogueList, void*, char*);

    std::vector<std::string> dialogue_list_append;

    void DECL_CARSHOOK HookFunction() {

        GetInlineContext();

        __asm
        {
            push ebp
            mov ebp, esp
        }

        {
            for (const auto& codename : dialogue_list_append) {
                AddNameToDialogueList((void*)ctx.ECX, (char*)codename.c_str());
                Logging::Log("[Patches::CarsActivityUI::RequestDialogue] Added %s to the Dialogue List!\n", codename.c_str());
            }
        } // Required to be able to use C++ objects. (lots of classes raise exceptions which arent supported in naked functions)

        __asm
        {
            mov esp, ebp
            pop ebp
            ret
        }

    }

    bool CollectCharactersToPatch() {
        std::ifstream file(CURRENT_DIRECTORY + "\\DataPC\\C\\Global\\Chars\\DialogueList.ini", std::ios::in);
        if (!file)
            return false;
        std::string line;
        while (std::getline(file, line)) {
            dialogue_list_append.push_back(line);
        }
        file.close();
        return true;
    }

    void install() {
        if (CollectCharactersToPatch())
            InlineHook32(CarsActivityUI_RequestDialogue + 0xFE, 5, &HookFunction);
        else
            Logging::Log("[Patches::CarsActivityUI::RequestDialogue] Failed to install hook.\n");
    }

    void uninstall() {
        dialogue_list_append.clear();
        RemoveInlineHook32(CarsActivityUI_RequestDialogue + 0xFE, 5, &HookFunction);
    }

};

namespace DataAccessLogging {

    // Will do more interesting things in the future right now this is all it does

    void DECL_CARSHOOK DataAccess_FOpenHook() {

        GetInlineContext();

        __asm
        {
            push ebp
            mov ebp, esp
        }

        char* FileName;
        FileName = (char*)ctx.EBP;

        Logging::Log("[Logging::DataAccess::FOpen] Opened file: %s\n", FileName);

        __asm
        {
            mov esp, ebp
            pop ebp
        }

        RETURN;

    }

    void install() {
        InlineHook32(DataAccess_FOPEN + 0xB, 8, &DataAccess_FOpenHook);
    }

    void uninstall() {
        RemoveInlineHook32(DataAccess_FOPEN + 0xB, 8, &DataAccess_FOpenHook);
    }
};
