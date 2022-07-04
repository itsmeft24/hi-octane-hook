
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../Globals.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../FileSystem.h"

#include "DialogueListEx.h"

DeclareFunction(void, __thiscall, CarsActivity_AddNameToDialogueList, 0x004B68B0, void*, const char*);
DeclareFunction(void, __thiscall, CarsActivityUI_RequestDialogue, 0x004BD360, void*);

HookedFunctionInfo request_dialogue_finfo;

std::vector<std::string> dialogue_list = {
"mcq",
"mat",
"hud",
"ram",
"flo",
"sar",
"fil",
"lui",
"sven",
"otto",
"hiro",
"gio",
"emma",
"mcqm",
"matm",
"sulm",
"mike"
};

void __fastcall RequestDialogue(void* this_ptr) {
	for (size_t x = 0; x < dialogue_list.size(); x++) {
		CarsActivity_AddNameToDialogueList(this_ptr, dialogue_list[x].c_str());
		Logging::Log("[DialogueListEx::CarsActivityUI::RequestDialogue] Added %s to the Dialogue List!\n", dialogue_list[x].c_str());
	}
}

bool DL_CollectCharactersToPatch() {
	std::ifstream file(FileSystem::GetPathForFile("c\\global\\chars\\dialoguelist.ini"), std::ios::in);
	if (!file)
		return false;

	std::string line;
	while (std::getline(file, line)) {
		if (std::find(dialogue_list.begin(), dialogue_list.end(), line) == dialogue_list.end())
			dialogue_list.push_back(line);
	}
	file.close();
	return true;
}

HIOCTANE_API bool RegisterDialogueListAddition(const char* character) {
	if (std::find(dialogue_list.begin(), dialogue_list.end(), character) == dialogue_list.end()) {
		dialogue_list.push_back(character);
		return true;
	}
	return false;
}

void DialogueListEx::Install() {
	request_dialogue_finfo = HookFunction((void*&)CarsActivityUI_RequestDialogue, &RequestDialogue, 0x110, FunctionHookType::EntireReplacement);
	if (request_dialogue_finfo.type != FunctionHookType::Invalid)
		Logging::Log("[DialogueListEx::Install] Successfully installed patch!\n");
	DL_CollectCharactersToPatch();
};

void DialogueListEx::Uninstall() {
	UninstallFunctionHook(request_dialogue_finfo);
};
