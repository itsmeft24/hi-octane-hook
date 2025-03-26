#include "core/globals.hpp"
#include "cars2_event_database.hpp"

DeclareFunction(Cars2EventInfo*, __thiscall, _Cars2EventDatabase_GetEventInfo_Impl, 0x004294a0, Cars2EventDatabase*, Cars2ActivityInfo*);

Cars2EventInfo* Cars2EventDatabase::GetEventInfo(Cars2ActivityInfo* activity)
{
    return _Cars2EventDatabase_GetEventInfo_Impl(this, activity);
}
