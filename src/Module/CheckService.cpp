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

// External

// Project
#include "./CheckService.h"

// Pre-defined
#ifndef SPEECH_OUTPUT_TIMEOUT_MS
    #define SPEECH_OUTPUT_TIMEOUT_MS 5000
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

CheckService::CheckService(Service e_Service,
                           bool& b_ServiceAvailable) : MRH_Module("CheckService"),
                                                       c_Timer(SPEECH_OUTPUT_TIMEOUT_MS),
                                                       e_Service(e_Service),
                                                       b_ServiceAvailable(b_ServiceAvailable),
                                                       e_Result(NOT_SET)
{
    this->b_ServiceAvailable = false;
    
    MRH_EventStorage& c_Storage = MRH_EventStorage::Singleton();
    
    switch (e_Service)
    {
        case LISTEN:
            c_Storage.Add(MRH_EVD_CreateEvent(MRH_EVENT_LISTEN_AVAIL_U, NULL, 0));
            break;
        case SAY:
            c_Storage.Add(MRH_EVD_CreateEvent(MRH_EVENT_SAY_AVAIL_U, NULL, 0));
            break;
        case APPLICATION:
            c_Storage.Add(MRH_EVD_CreateEvent(MRH_EVENT_APP_AVAIL_U, NULL, 0));
            break;
            
        default:
            throw MRH_ModuleException("CheckService", 
                                      "Unknown service to check!");
    }
}

CheckService::~CheckService() noexcept
{}

//*************************************************************************************
// Update
//*************************************************************************************

void CheckService::HandleEvent(const MRH_Event* p_Event) noexcept
{
    MRH_EvD_Base_ServiceAvail_S_t c_ServiceAvail;
    
    if (MRH_EVD_ReadEvent(&c_ServiceAvail, p_Event->u32_Type, p_Event) < 0)
    {
        MRH_ModuleLogger::Singleton().Log("CheckService", "Failed to read service available event!",
                                          "CheckService.cpp", __LINE__);
        return;
    }
    
    switch (p_Event->u32_Type)
    {
        case MRH_EVENT_LISTEN_AVAIL_S:
            if (e_Service != LISTEN)
            {
                return;
            }
            break;
        case MRH_EVENT_SAY_AVAIL_S:
            if (e_Service != SAY)
            {
                return;
            }
            break;
        case MRH_EVENT_APP_AVAIL_S:
            if (e_Service != APPLICATION)
            {
                return;
            }
            break;
            
        default:
            return;
    }
    
    if (c_ServiceAvail.u8_Available == MRH_EVD_BASE_RESULT_SUCCESS)
    {
        MRH_ModuleLogger::Singleton().Log("CheckService", "Service " +
                                                          std::to_string(e_Service) +
                                                          " available!",
                                          "CheckService.cpp", __LINE__);
        e_Result = AVAILABLE;
    }
    else
    {
        MRH_ModuleLogger::Singleton().Log("CheckService", "Service " +
                                                          std::to_string(e_Service) +
                                                          " not available!",
                                          "CheckService.cpp", __LINE__);
        e_Result = UNAVAILABLE;
    }
}

MRH_Module::Result CheckService::Update()
{
    if (e_Result != NOT_SET || c_Timer.GetTimerFinished() == true)
    {
        b_ServiceAvailable = (e_Result == AVAILABLE ? true : false);
        return MRH_Module::FINISHED_POP;
    }
    
    return MRH_Module::IN_PROGRESS;
}

std::shared_ptr<MRH_Module> CheckService::NextModule()
{
    throw MRH_ModuleException("CheckService",
                              "No module to switch to!");
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool CheckService::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    switch (u32_Type)
    {
        case MRH_EVENT_LISTEN_AVAIL_S:
        case MRH_EVENT_SAY_AVAIL_S:
        case MRH_EVENT_APP_AVAIL_S:
            return true;
            
        default:
            return false;
    }
}
