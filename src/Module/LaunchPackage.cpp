/**
 *  LaunchPackage.cpp
 *
 *  This file is part of the MRH project.
 *  See the AUTHORS file for Copyright information.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// C / C++
#include <cstring>

// External

// Project
#include "./LaunchPackage.h"

// Pre-defined
#ifndef MODULE_LAUNCH_PACKAGE_TIMEOUT_MS
    #define MODULE_LAUNCH_PACKAGE_TIMEOUT_MS 10000
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

LaunchPackage::LaunchPackage(std::string const& s_PackagePath,
                             std::string const& s_LaunchInput,
                             MRH_Sint32 s32_LaunchCommandID,
                             bool& b_LaunchSet) : MRH_Module("LaunchPackage"),
                                                  c_Timer(MODULE_LAUNCH_PACKAGE_TIMEOUT_MS),
                                                  s_PackagePath(s_PackagePath),
                                                  s_LaunchInput(s_LaunchInput),
                                                  s32_LaunchCommandID(s32_LaunchCommandID),
                                                  b_LaunchSet(b_LaunchSet),
                                                  b_AnswerRecieved(false)
{
    MRH_ModuleLogger::Singleton().Log("LaunchPackage", "Sending launch request: [ " +
                                                       s_PackagePath +
                                                       " | " +
                                                       (s_LaunchInput.size() > 0 ? s_LaunchInput : " No Input ") +
                                                       " | " +
                                                       std::to_string(s32_LaunchCommandID) +
                                                       " ]",
                                      "LaunchPackage.cpp", __LINE__);
    
    b_LaunchSet = false;
    MRH_EvD_A_LaunchSOA_U c_Launch;
    
    memset(c_Launch.p_PackagePath, '\0', MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
    memset(c_Launch.p_LaunchInput, '\0', MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
    
    std::strncpy(c_Launch.p_PackagePath,
                 s_PackagePath.c_str(),
                 MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
    c_Launch.s32_LaunchCommandID = s32_LaunchCommandID;
    
    if (s_LaunchInput.size() > 0)
    {
        std::strncpy(c_Launch.p_LaunchInput,
                     s_LaunchInput.c_str(),
                     MRH_EVD_A_STRING_LAUNCH_BUFFER_MAX_TERMINATED);
    }
    
    MRH_Event* p_Event = MRH_EVD_CreateSetEvent(MRH_EVENT_APP_LAUNCH_SOA_U, &c_Launch);
    
    if (p_Event == NULL)
    {
        throw MRH_ModuleException("LaunchPackage", "Failed to create launch event!");
    }
    
    try
    {
        MRH_EventStorage::Singleton().Add(p_Event);
    }
    catch (MRH_ABException& e)
    {
        MRH_EVD_DestroyEvent(p_Event);
        
        throw MRH_ModuleException("LaunchPackage", e.what());
    }
}

LaunchPackage::~LaunchPackage() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************

void LaunchPackage::HandleEvent(const MRH_Event* p_Event) noexcept
{
    // @NOTE: CanHandleEvent() allows skipping event type check!
    b_AnswerRecieved = true;
    
    MRH_EvD_A_LaunchSOA_S c_Launch;
    
    if (MRH_EVD_ReadEvent(&c_Launch, p_Event->u32_Type, p_Event) < 0)
    {
        MRH_ModuleLogger::Singleton().Log("LaunchPackage", "Failed to read launch package event!",
                                          "LaunchPackage.cpp", __LINE__);
        return;
    }
    else if (strncmp(c_Launch.p_PackagePath, s_PackagePath.c_str(), s_PackagePath.size()) != 0 ||
             c_Launch.s32_LaunchCommandID != s32_LaunchCommandID)
    {
        MRH_ModuleLogger::Singleton().Log("LaunchPackage", "Failed to read launch package event!",
                                          "LaunchPackage.cpp", __LINE__);
        return;
    }
    else if (s_LaunchInput.size() > 0 &&
             strncmp(c_Launch.p_LaunchInput, s_LaunchInput.c_str(), s_LaunchInput.size()) != 0)
    {
        MRH_ModuleLogger::Singleton().Log("LaunchPackage", "Failed to read launch package event!",
                                          "LaunchPackage.cpp", __LINE__);
        return;
    }
    
    b_LaunchSet = true;
}

MRH_Module::Result LaunchPackage::Update()
{
    if (b_AnswerRecieved == true || c_Timer.GetTimerFinished() == true)
    {
        return MRH_Module::FINISHED_POP;
    }
    
    return MRH_Module::IN_PROGRESS;
}

std::shared_ptr<MRH_Module> LaunchPackage::NextModule()
{
    throw MRH_ModuleException("LaunchPackage",
                              "No module to switch to!");
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool LaunchPackage::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    switch (u32_Type)
    {
        case MRH_EVENT_APP_LAUNCH_SOA_S:
            return true;
            
        default:
            return false;
    }
}
