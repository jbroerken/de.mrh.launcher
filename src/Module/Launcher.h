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

#ifndef Launcher_h
#define Launcher_h

// C / C++

// External
#include <libmrhab/Module/MRH_Module.h>

// Project
#include "../Package/PackageList.h"


class Launcher : public MRH_Module
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     */
    
    Launcher() noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~Launcher() noexcept;
    
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
    
    struct Selected
    {
    public:
        
        //*************************************************************************************
        // Constructor
        //*************************************************************************************
        
        /**
         *  Default constructor.
         *  
         *  \param c_Package The selected package.
         *  \param s_LaunchInput The input used for launch.
         *  \param s32_LaunchCommandID The chosen launch command id.
         */
        
        Selected(Package const& c_Package,
                 std::string const s_LaunchInput,
                 MRH_Sint32 s32_LaunchCommandID) noexcept;
        
        //*************************************************************************************
        // Data
        //*************************************************************************************
        
        Package const& c_Package;
        std::string s_LaunchInput;
        MRH_Sint32 s32_LaunchCommandID;
    };
    
    enum State
    {
        START = 0,
        
        CHECK_SERVICE_LISTEN = 1,
        CHECK_SERVICE_SAY = 2,
        CHECK_SERVICE_APP = 3,
        
        INPUT_LAUNCH_TRIGGER = 4, // Listen for launch triggers
        INPUT_PACKAGE_NAME = 6, // Check package name
        
        OUTPUT_PACKAGE_LIST = 5, // List packages for triggers
        OUTPUT_NO_PACKAGE = 7, // No package for input (launch trigger or name)
        
        LAUNCH_PACKAGE = 8, // Set launch
        
        CLOSE_APP = 9, // Close
        
        STATE_MAX = CLOSE_APP,
        
        STATE_COUNT = STATE_MAX + 1
    };
    
    //*************************************************************************************
    // Package
    //*************************************************************************************
    
    /**
     *  Select packages by launch trigger.
     */
    
    void SelectPackageLaunchTrigger() noexcept;
    
    /**
     *  Filter selected packages by application name.
     */
    
    void FilterPackageByName() noexcept;
    
    //*************************************************************************************
    // Output
    //*************************************************************************************
    
    /**
     *  Generate no packages output.
     *  
     *  \return The generated output.
     */
    
    std::string NoPackagesOutput();
    
    /**
     *  Generate package list output.
     *  
     *  \return The generated output.
     */
    
    std::string PackageListOutput();
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    // Application state
    State e_State;

    // Module information
    bool b_ServiceAvailable;
    std::string s_Input;
    bool b_LaunchSet;
    
    // Packages
    PackageList c_PackageList;
    std::list<Selected> l_Selected;
    
protected:

};

#endif /* Launcher_h */
