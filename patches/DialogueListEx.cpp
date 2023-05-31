
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../FileSystem.h"
#include "../framework.hpp"
#include "../Logging.h"

#include "DialogueListEx.h"

struct CarsActivity;
struct CarsActivityUI;

DeclareFunction(void, __thiscall, CarsActivity_AddNameToDialogueList, 0x004B68B0, CarsActivity*, const char *);

std::vector<std::string> dialogue_list_vec = {
    "mcq",  "mat",  "hud", "ram",  "flo",  "sar",  "fil",  "lui", "sven",
    "otto", "hiro", "gio", "emma", "mcqm", "matm", "sulm", "mike"};


DefineReplacementHook(CarsActivityUIRequestDialogue) {
    static void __fastcall callback(CarsActivityUI* _this) {
        for (size_t x = 0; x < dialogue_list_vec.size(); x++) {
            CarsActivity_AddNameToDialogueList(reinterpret_cast<CarsActivity*>(_this), dialogue_list_vec[x].c_str());
            logging::log("[DialogueListEx::CarsActivityUI::RequestDialogue] Added {} to the Dialogue List!", dialogue_list_vec[x]);
        }
    }
};

void dialogue_list::install() {
    CarsActivityUIRequestDialogue::install_at_ptr(0x004BD360);

  std::ifstream file(fs::resolve_path("c\\global\\chars\\dialoguelist.ini"), std::ios::in);
  if (file) {
      std::string line;
      while (std::getline(file, line)) {
          if (std::find(dialogue_list_vec.begin(), dialogue_list_vec.end(), line) ==
              dialogue_list_vec.end())
              dialogue_list_vec.push_back(line);
      }
      file.close();
  }

  logging::log("[DialogueListEx::Install] Successfully installed patch!");
};
