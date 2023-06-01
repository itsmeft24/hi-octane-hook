#include <fstream>
#include <iostream>
#include <string>
#include <rapidjson/document.h>

#include "../ConfigManager.h"
#include "../FileSystem.h"
#include "../framework.hpp"
#include "../Logging.h"

#include "GameTextJSON.h"
#include "WidescreenPatch.h"

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

DefineReplacementHook(GameTextCreate) {
    static GameText* __fastcall callback(GameText* this_ptr, void* in_EDX, char* name) {

        logging::log("[GameText::CreateFromJSON] Reading {} strings from JSON...",
            name);

        strcpy(this_ptr->name, name);

        std::ifstream jsonFile(
            fs::resolve_path("c\\loc\\" + std::string(name) + ".json"),
            std::ios::in | std::ios::binary);
        jsonFile.seekg(0, std::ios::end);
        unsigned int jsonSize = jsonFile.tellg();

        char* jsonBuffer = new char[jsonSize];
        jsonFile.seekg(0, std::ios::beg);
        jsonFile.read(jsonBuffer, jsonSize);
        jsonFile.close();

        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseStopWhenDoneFlag>(jsonBuffer);

        if (stricmp(name, "commonui") == 0) {
            std::string formattedStr = "Hi-Octane Version: " + std::string(VERSION);
            rapidjson::Value v(rapidjson::kObjectType);
            v.AddMember("TextID", "STR_HI_OCTANE_VER", doc.GetAllocator());
            v.AddMember("Value", rapidjson::Value(formattedStr.c_str(), doc.GetAllocator()), doc.GetAllocator());
            v.AddMember("MaxSize", rapidjson::Value(formattedStr.size() * 2), doc.GetAllocator());
            v.AddMember("IsDynamic", rapidjson::Value(0), doc.GetAllocator());
            doc.GetArray().PushBack(v, doc.GetAllocator());
            jsonSize += formattedStr.size() + 18;
        }
        if (config::g_WidescreenEnabled && stricmp(name, "pcfrontendui") == 0) {
            for (int x = 0; x < (int)widescreen::SDResolution::Max; x++) {
                const auto& sd = widescreen::resolve_sd((widescreen::SDResolution)x);
                const auto& hd = widescreen::resolve_hd((widescreen::SDResolution)x);
                const auto textId = "STR_" + std::to_string(sd.first) + "X" + std::to_string(sd.second);
                const auto value = std::to_string(hd.first) + "X" + std::to_string(hd.second);
                const auto& array = doc.GetArray();
                std::size_t foundIndex = 0;
                for (std::size_t index = 0; index < array.Size(); index++) {
                    if (textId == array[index].GetObject()["TextID"].GetString()) {
                        foundIndex = index;
                        break;
                    }
                }
                jsonSize = jsonSize - doc.GetArray()[foundIndex].GetObject()["Value"].GetStringLength() + value.size() + 1;
                doc.GetArray()[foundIndex].GetObject()["Value"].SetString(value.c_str(), value.size(), doc.GetAllocator());
                doc.GetArray()[foundIndex].GetObject()["MaxSize"].SetInt(value.size() * 2);
            }
        }

        delete[] jsonBuffer;

        size_t textIdBufWritten = 0;
        size_t valueBufWritten = 0;

        this_ptr->textIDBuffer = (char*)calloc(
            1, jsonSize); // technically this is larger than we need but its fine
        this_ptr->valueBuffer = (char*)calloc(
            1, jsonSize); // technically this is larger than we need but its fine

        this_ptr->numberOfStrings = doc.GetArray().Size();
        this_ptr->textIdPointers = new char* [this_ptr->numberOfStrings];
        this_ptr->stringInfos = new StringInfo[this_ptr->numberOfStrings];

        this_ptr->CHTMap = malloc(0x18);
        memset(this_ptr->CHTMap, 0, 0x18);

        ContainerHashTable__charptrToint__CHTCreateFull(
            this_ptr->CHTMap, this_ptr->numberOfStrings, 100, StringHashValueFunction,
            StringHashCompareFunction, nullptr, 0);

        for (int x = 0; x < this_ptr->numberOfStrings; x++) {

            auto& currentInfo = this_ptr->stringInfos[x];

            const auto& elem = doc.GetArray()[x];

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
                const auto& appends = elem["Appends"].GetArray();
                const auto& positions = elem["AtPositions"].GetArray();

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
                const auto& appendedTo = elem["IsAppendedTo"].GetArray();

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
        return this_ptr;
    }
};

DefineReplacementHook(GameTextLoadGameText) {
    static void __fastcall callback(GameText* this_ptr, void* in_EDX, int param_1, int param_2) {
        return;
    }
};

DefineReplacementHook(GameTextDtor) {
    static void __fastcall callback(GameText* this_ptr, void* in_EDX) {
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
};

#pragma optimize("", on)

void game_text_json::install() {
    GameTextCreate::install_at_ptr(0x006074E0);
    // Since we end up allocating memory ourselves, we need to hook the destructor
    // so the memory is freed properly.
    GameTextDtor::install_at_ptr(0x00607c40);
    // All class initialization now happens in GameText::Create, so LoadGameText
    // can just be stubbed out.
    GameTextLoadGameText::install_at_ptr(0x00607820);

    logging::log("[game_text_json::install] Successfully installed patch!");
}