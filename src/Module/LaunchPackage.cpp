/**
 *  Copyright (C) 2021 - 2022 The MRH Project Authors.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
                                                  b_AnswerReceived(false)
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
    b_AnswerReceived = true;
    
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
    if (b_AnswerReceived == true || c_Timer.GetTimerFinished() == true)
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
