#include <fstream>
#include <iostream>
#include <rapidjson/document.h>

#include "../FileSystem.h"
#include "../Globals.h"
#include "../HookFunction.h"
#include "../Logging.h"

#include "GameTextJSON.h"

DeclareFunction(void *, __thiscall, GameText_Create, 0x006074E0, void *,
                char *);
DeclareFunction(void, __thiscall, GameText_Destructor, 0x00607c40, void *);
DeclareFunction(void, __thiscall, GameText_LoadGameText, 0x00607820, void *,
                int, int);

DeclareFunction(int, __cdecl, StringHashValueFunction, 0x00547f90, char *);
DeclareFunction(bool, __cdecl, StringHashCompareFunction, 0x00547fb0, char *,
                char *);

DeclareFunction(void, __thiscall,
                ContainerHashTable__charptrToint__CHTCreateFull, 0x0056da70,
                void *, int, int, void *, void *, void *, int);
DeclareFunction(void, __thiscall, ContainerHashTable__charptrToint__Destructor,
                0x0056d910, void *);
DeclareFunction(void, __thiscall, ContainerHashTable__charptrToint__CHTAdd,
                0x005bb280, void *, char *, int, void *, int);

HookedFunctionInfo gametext_create_finfo;
HookedFunctionInfo gametext_dtor_finfo;
HookedFunctionInfo gametext_loadgametext_finfo;

struct AppensionInfo {
  unsigned char appensionCount;
  char pad[3];
  int *appends; // Array of string indexes to add.
  short *
      atPositions; // Array of positions, for where each string should be added.
  char *copiedToEnd; // Copied at the end of the string.
};

static_assert(sizeof(AppensionInfo) == 0x10);

class StringInfo {
public:
  unsigned short maxSize;
  char padding[2];
  char *value;
  int flags;
  bool isDynamic;
  char padding2[3];
  AppensionInfo *appensionInfo;
  unsigned char isAppendedToCount; // Number of strings this string is added to.
  char padding3[3];
  int *isAppendedTo; // Array of string indices this string is added to.

  StringInfo() {
    this->maxSize = 0;
    this->value = nullptr;
    this->flags = 0;
    this->isDynamic = false;
    this->appensionInfo = nullptr;
    this->isAppendedToCount = 0;
    this->isAppendedTo = 0;
  }

  ~StringInfo() {
    if (this->appensionInfo) {
      if (this->appensionInfo->appends != nullptr) {
        delete[] this->appensionInfo->appends;
      }
      if (this->appensionInfo->atPositions != (short *)0x0) {
        delete[] this->appensionInfo->atPositions;
      }
      delete this->appensionInfo;
    }
    if (this->isAppendedTo != nullptr) {
      delete[] this->isAppendedTo;
    }
    return;
  }
};

static_assert(sizeof(StringInfo) == 0x1C);

struct GameText {
  char name[16];
  int numberOfStrings;
  char **textIdPointers;
  void *CHTMap; // ContainerHashTable<char*, int>*
  StringInfo *stringInfos;
  int unused;
  char *textIDBuffer;
  char *valueBuffer;
};

static_assert(sizeof(GameText) == 0x2C);

// Prevent compiler from removing the in_EDX arguments.
#pragma optimize("", off)
GameText *__fastcall GameText_Create_Hook(GameText *this_ptr, void *in_EDX,
                                          char *name) {

  Logging::Log("[GameText::CreateFromJSON] Reading %s strings from JSON...\n",
               name);

  strcpy(this_ptr->name, name);

  std::ifstream jsonFile(
      FileSystem::GetPathForFile("c\\loc\\" + std::string(name) + ".json"),
      std::ios::in | std::ios::binary);
  jsonFile.seekg(0, std::ios::end);
  unsigned int jsonSize = jsonFile.tellg();

  char *json_buffer = new char[jsonSize];
  jsonFile.seekg(0, std::ios::beg);
  jsonFile.read(json_buffer, jsonSize);
  jsonFile.close();

  rapidjson::Document doc;
  doc.Parse<rapidjson::kParseStopWhenDoneFlag>(json_buffer);

  delete[] json_buffer;

  size_t textIdBufWritten = 0;
  size_t valueBufWritten = 0;

  this_ptr->textIDBuffer = (char *)calloc(
      1, jsonSize); // technically this is larger than we need but its fine
  this_ptr->valueBuffer = (char *)calloc(
      1, jsonSize); // technically this is larger than we need but its fine

  this_ptr->numberOfStrings = doc.GetArray().Size();
  this_ptr->textIdPointers = new char *[this_ptr->numberOfStrings];
  this_ptr->stringInfos = new StringInfo[this_ptr->numberOfStrings];

  this_ptr->CHTMap = malloc(0x18);
  memset(this_ptr->CHTMap, 0, 0x18);

  ContainerHashTable__charptrToint__CHTCreateFull(
      this_ptr->CHTMap, this_ptr->numberOfStrings, 100, StringHashValueFunction,
      StringHashCompareFunction, nullptr, 0);

  for (int x = 0; x < this_ptr->numberOfStrings; x++) {

    auto &currentInfo = this_ptr->stringInfos[x];

    const auto &elem = doc.GetArray()[x];

    auto textid = elem.GetObject()["TextID"].GetString();
    strcpy(this_ptr->textIDBuffer + textIdBufWritten, textid);
    this_ptr->textIdPointers[x] = this_ptr->textIDBuffer + textIdBufWritten;
    textIdBufWritten += strlen(textid) + 1;

    currentInfo.maxSize = elem.GetObject()["MaxSize"].GetInt();
    currentInfo.isDynamic = elem.GetObject()["IsDynamic"].GetInt();
    currentInfo.flags = !currentInfo.isDynamic;

    auto value = elem.GetObject()["Value"].GetString();
    strcpy(this_ptr->valueBuffer + valueBufWritten, value);
    currentInfo.value = this_ptr->valueBuffer + valueBufWritten;
    valueBufWritten += currentInfo.maxSize + 1;

    currentInfo.appensionInfo = nullptr;
    currentInfo.isAppendedToCount = 0;
    currentInfo.isAppendedTo = nullptr;

    if (elem.HasMember("Appends") && elem.HasMember("AtPositions") &&
        elem.HasMember("CopiedToEnd")) {
      const auto &appends = elem["Appends"].GetArray();
      const auto &positions = elem["AtPositions"].GetArray();

      currentInfo.appensionInfo = new AppensionInfo();
      currentInfo.appensionInfo->appensionCount = appends.Size();
      currentInfo.appensionInfo->appends = new int[appends.Size()]();
      currentInfo.appensionInfo->atPositions = new short[appends.Size()]();

      auto copiedToEnd = elem.GetObject()["CopiedToEnd"].GetString();
      strcpy(this_ptr->valueBuffer + valueBufWritten, copiedToEnd);
      currentInfo.appensionInfo->copiedToEnd =
          this_ptr->valueBuffer + valueBufWritten;
      valueBufWritten += strlen(copiedToEnd) + 1;

      for (int appensionIndex = 0;
           appensionIndex < currentInfo.appensionInfo->appensionCount;
           appensionIndex++) {
        currentInfo.appensionInfo->appends[appensionIndex] =
            appends[appensionIndex].GetInt();
        currentInfo.appensionInfo->atPositions[appensionIndex] =
            positions[appensionIndex].GetInt();
      }
    }

    if (elem.HasMember("IsAppendedTo")) {
      const auto &appendedTo = elem["IsAppendedTo"].GetArray();

      currentInfo.isAppendedToCount = appendedTo.Size();
      currentInfo.isAppendedTo = new int[currentInfo.isAppendedToCount]();

      for (int appendedToIndex = 0;
           appendedToIndex < currentInfo.isAppendedToCount; appendedToIndex++) {
        currentInfo.isAppendedTo[appendedToIndex] =
            appendedTo[appendedToIndex].GetInt();
      }
    }

    ContainerHashTable__charptrToint__CHTAdd(
        this_ptr->CHTMap, this_ptr->textIdPointers[x], x, nullptr, 0);
  }

  Logging::Log("[GameText::CreateFromJSON] Returning...\n", name);
  return this_ptr;
}

void __fastcall GameText_LoadGameText_Hook(GameText *this_ptr, void *in_EDX,
                                           int param_1, int param_2) {
  return;
}

void __fastcall GameText_Destructor_Hook(GameText *this_ptr, void *in_EDX) {
  if (this_ptr->textIdPointers != nullptr) {
    delete[] this_ptr->textIdPointers;
  }
  if (this_ptr->CHTMap != nullptr) {
    ContainerHashTable__charptrToint__Destructor(this_ptr->CHTMap);
    free(this_ptr->CHTMap);
  }
  if (this_ptr->stringInfos != nullptr) {
    delete[] this_ptr->stringInfos;
  }
  if (this_ptr->textIDBuffer != nullptr) {
    free(this_ptr->textIDBuffer);
  }
  if (this_ptr->valueBuffer != nullptr) {
    free(this_ptr->valueBuffer);
  }
}

#pragma optimize("", on)

void GameTextJSON::Install() {
  gametext_create_finfo =
      HookFunction((void *&)GameText_Create, &GameText_Create_Hook, 7,
                   FunctionHookType::EntireReplacement);
  // Since we end up allocating memory ourselves, we need to hook the destructor
  // so the memory is freed properly.
  gametext_dtor_finfo =
      HookFunction((void *&)GameText_Destructor, &GameText_Destructor_Hook, 6,
                   FunctionHookType::EntireReplacement);
  // All class initialization now happens in GameText::Create, so LoadGameText
  // can just be stubbed out.
  gametext_loadgametext_finfo =
      HookFunction((void *&)GameText_LoadGameText, &GameText_LoadGameText_Hook,
                   6, FunctionHookType::EntireReplacement);

  if (gametext_create_finfo.type != FunctionHookType::Invalid &&
      gametext_dtor_finfo.type != FunctionHookType::Invalid &&
      gametext_loadgametext_finfo.type != FunctionHookType::Invalid)
    Logging::Log("[GameTextJSON::Install] Successfully installed patch!\n");
}