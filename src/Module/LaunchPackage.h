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

#ifndef LaunchPackage_h
#define LaunchPackage_h

// C / C++

// External
#include <libmrhab/Module/MRH_Module.h>

// Project


class LaunchPackage : public MRH_Module
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param s_PackagePath The full path to the package to launch.
     *  \param s_LaunchInput The input to supply when launching.
     *  \param s32_LaunchCommandID The launch command id to supply when launching.
     *  \param b_LaunchSet The launch result.
     */
    
    LaunchPackage(std::string const& s_PackagePath,
                  std::string const& s_LaunchInput,
                  MRH_Sint32 s32_LaunchCommandID,
                  bool& b_LaunchSet);
    
    /**
     *  Default destructor.
     */
    
    ~LaunchPackage() noexcept;
    
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
    // Data
    //*************************************************************************************
    
    MRH_ModuleTimer c_Timer;
    
    std::string s_PackagePath;
    std::string s_LaunchInput;
    MRH_Sint32 s32_LaunchCommandID;
    
    bool& b_LaunchSet;
    bool b_AnswerReceived;
    
protected:
    
};

#endif /* LaunchPackage_h */
