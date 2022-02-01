#pragma once

namespace CarsActivityUI_RequestDialogueHook { // im aware that this is very overcomplicated lol, i will rewrite eventually

    char* AllocatedStringTable;
    char* AllocatedCode;

    void GenerateStringTableAndHook(std::string path) { // request_dialogue_patch.bin
        std::ifstream file(path, std::ios::in);
        std::string line;
        std::vector<std::string> lines;
        std::vector<int> table_offsets;
        int size = 0;
        while (std::getline(file, line)) {
            table_offsets.push_back(size); // is really the offset in the allocated string table
            lines.push_back(line);
            size += line.size() + 1;
        }
        file.close();
        AllocatedStringTable = new char[size]; // if this doesnt work then use VirtualAlloc
        int code_pos = 0;
        AllocatedCode = AllocateCode(12 * lines.size());
        for (int x = 0; x < lines.size(); x++) {
            memcpy(AllocatedStringTable + table_offsets[x], lines[x].c_str(), lines[x].size());

            DWORD relativeAddress = (DWORD)(AllocatedStringTable + table_offsets[x] - (DWORD)AllocatedCode + code_pos) - 5;

            *(AllocatedCode + code_pos) = 0x68;
            *(DWORD*)(AllocatedCode + code_pos + 1) = relativeAddress;
            // push OFFSET string
            *(WORD*)(AllocatedCode + code_pos + 5) = 0xF189;
            // mov ecx, esi
            WriteCALL(AllocatedCode + code_pos + 7, CarsActivity_AddNameToDialogueList);
            // call CarsActivity_AddNameToDialogueList
            code_pos += 12;
        }
    }

    void install() {
        GenerateStringTableAndHook("request_dialogue_patch.bin");
        InlineHook32(CarsActivityUI_RequestDialogue + 0x105, 5, AllocatedCode);
    }

    void uninstall() {
        RemoveInlineHook32(CarsActivityUI_RequestDialogue + 0x105, 5, AllocatedCode);
        FreeCode(AllocatedCode);
        delete[] AllocatedStringTable;
    }

};