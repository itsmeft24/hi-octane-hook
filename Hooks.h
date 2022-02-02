#pragma once

namespace CarsActivityUI_RequestDialogue_Hook
{

    std::vector<std::string> dialogue_list_append;
    
    void DECL_CARSHOOK HookFunction() {
        
        GetInlineContext();
        
        __asm
        {
            push ebp
            mov ebp, esp
        }
        
        {
            for (int x = 0; x < dialogue_list_append.size()) {
                CarsActivity_AddNameToDialogueList(ctx.ECX, dialogue_list_append[x].c_str());
            }
        }
        
        __asm
        {
            mov esp, ebp
            pop ebp
            ret
        }
        
    }
    
    void CollectCharactersToPatch() {
        std::ifstream file("DataPC\\C\\Global\\Chars\\DialogueList.ini", std::ios::in);
        std::string line;
        int size = 0;
        while (std::getline(file, line)) {
            dialogue_list_append.push_back(line);
        }
        file.close();
    }
    
    void install() {
        CollectCharactersToPatch();
        InlineHook32(CarsActivityUI_RequestDialogue + 0x105, 5, &HookFunction);
    }
    
    void uninstall() {
        RemoveInlineHook32(CarsActivityUI_RequestDialogue + 0x105, 5, &HookFunction);
    }
    
};
