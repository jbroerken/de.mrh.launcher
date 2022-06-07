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

#ifndef CheckService_h
#define CheckService_h

// C / C++

// External
#include <libmrhab/Module/MRH_Module.h>

// Project


class CheckService : public MRH_Module
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    enum Service
    {
        LISTEN = 0,
        SAY = 1,
        APPLICATION = 2,
        
        SERVICE_MAX = APPLICATION,
        
        SERVICE_COUNT = SERVICE_MAX + 1
    };
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param e_Service The platform service to check.
     *  \param b_ServiceAvailable The services available flag to set.
     */
    
    CheckService(Service e_Service,
                 bool& b_ServiceAvailable);
    
    /**
     *  Default destructor.
     */
    
    ~CheckService() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Hand a received event to the module.
     *
     *  \param p_Event The received event.
     */
    
    void HandleEvent(const MRH_Event* p_Event) noexcept override;
    
    /**
     *  Perform a module update.
     *
     *  \return The module update result.
     */
    
    MRH_Module::Result Update() override;
    
    /**
     *  Get the module to switch to.
     *
     *  \return The module switch information.
     */
    
    std::shared_ptr<MRH_Module> NextModule() override;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if the module can handle a event.
     *
     *  \param u32_Type The type of the event to handle.
     *
     *  \return true if the event can be used, false if not.
     */
    
    bool CanHandleEvent(MRH_Uint32 u32_Type) noexcept override;
    
private:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    enum Result
    {
        NOT_SET = 0,
        AVAILABLE = 1,
        UNAVAILABLE = 2,
        
        RESULT_MAX = UNAVAILABLE,
        
        RESULT_COUNT = RESULT_MAX + 1
    };
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    MRH_ModuleTimer c_Timer;
    
    Service e_Service;
    Result e_Result;
    bool& b_ServiceAvailable;
    
protected:
    
};

#endif /* CheckService_h */
