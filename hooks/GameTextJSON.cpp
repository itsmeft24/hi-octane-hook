
#include <iostream>
#include <fstream>
#include <rapidjson/document.h>

#include "../Globals.h"
#include "../HookFunction.h"
#include "../FileSystem.h"
#include "../Logging.h"

#include "GameTextJSON.h"

DeclareFunction(void*, __thiscall, GameText_Create, 0x006074E0, void*, char*);
DeclareFunction(void, __thiscall, GameText_LoadGameText, 0x006074E0, void*, int);

struct StringInfo {
	unsigned short MaxSize;
	char unk[2];
	char* ValuePTR;
	int IsDynamic;
	bool IsDynamic_;
	char has_unk[3]; // might actually be padding
	int** unk_arr;
	int has_unk2;
	unsigned char** unk_arr2;
	//char reserved[0x1C];

}; // sizeof == 0x1C

struct GameText {
	char Name[16];
	int NumberOfStrings;
	char** TextIDPointers;
	void* CHTMap; // ContainerHashTable<char*, int>*
	StringInfo* StringInfos;
	int unused;
	char* TextIDBuffer;
	char* ValueBuffer;
};

// Prevent compiler from removing the in_EDX arguments.
#pragma optimize( "", off )
GameText* __fastcall GameText_Create_Hook(GameText* this_ptr, void* in_EDX, char* name) {

	Logging::Log("[GameText::CreateFromJSON] Reading %s strings from JSON...\n", name);

	strcpy(this_ptr->Name, name);

	this_ptr->unused = 0;

	std::ifstream json_file(FileSystem::GetPathForFile("c\\loc\\" + std::string(name) + ".json"), std::ios::in | std::ios::binary);
	json_file.seekg(0, std::ios::end);
	unsigned int json_size = json_file.tellg();
	char* json_buffer = new char[json_size];
	json_file.seekg(0, std::ios::beg);
	json_file.read(json_buffer, json_size);
	json_file.close();
	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseStopWhenDoneFlag>(json_buffer);
	delete[] json_buffer;

	this_ptr->TextIDBuffer = (char*)calloc(1, json_size); // technically this is larger than we need but its fine
	size_t textid_buf_written = 0;
	this_ptr->ValueBuffer = (char*)calloc(1, json_size); // technically this is larger than we need but its fine
	size_t value_buf_written = 0;

	this_ptr->NumberOfStrings = doc.GetArray().Size();

	this_ptr->TextIDPointers = new char* [this_ptr->NumberOfStrings];

	this_ptr->StringInfos = new StringInfo[this_ptr->NumberOfStrings];

	for (int x = 0; x < this_ptr->NumberOfStrings; x++) {

		const auto& elem = doc.GetArray()[x];

		auto textid = elem.GetObject()["TextID"].GetString();                
		strcpy(this_ptr->TextIDBuffer + textid_buf_written, textid);
		this_ptr->TextIDPointers[x] = this_ptr->TextIDBuffer + textid_buf_written;
		textid_buf_written += strlen(textid) + 1;

		

		auto value = elem.GetObject()["Value"].GetString();
		strcpy(this_ptr->ValueBuffer + value_buf_written, value);

		this_ptr->StringInfos[x] = {
			(unsigned short)strlen(textid),
			{0, 0},
			this_ptr->ValueBuffer + value_buf_written,
			1,
			1,
			{0, 0, 0},
			nullptr,
			false,
			nullptr
		};

		value_buf_written += strlen(textid) + 1;
	}
	Logging::Log("[GameText::CreateFromJSON] Returning...\n", name);
	return this_ptr;

}

void __fastcall GameText_LoadGameText_Hook(GameText* this_ptr, void* in_EDX, int param_1) {
	return;
}
#pragma optimize( "", on )

void GameTextJSON::Install() {
	HookFunction((void*&)GameText_Create, &GameText_Create_Hook, 7, FunctionHookType::EntireReplacement);
	HookFunction((void*&)GameText_LoadGameText, &GameText_LoadGameText_Hook, 6, FunctionHookType::EntireReplacement);
}