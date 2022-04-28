/**
 *  Launcher.cpp
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

// External
#include <libmrhvt/Output/MRH_OutputGenerator.h>
#include <libmrhvt/String/MRH_LocalisedPath.h>
#include <libmrhvt/String/Compare/MRH_Levenshtein.h>

// Project
#include "./Launcher.h"
#include "./CheckService.h"
#include "./SpeechInput.h"
#include "./SpeechOutput.h"
#include "./LaunchPackage.h"

// Pre-defined
#ifndef PACKAGE_LIST_PATH
    #define PACKAGE_LIST_PATH "/usr/local/etc/mrh/MRH_PackageList.conf"
#endif
#ifndef SPEECH_OUTPUT_DIR
    #define SPEECH_OUTPUT_DIR "Output"
#endif
#ifndef SPEECH_OUTPUT_LIST_PACKAGE_FILE
    #define SPEECH_OUTPUT_LIST_PACKAGE_FILE "ListPackages.mrhog"
#endif
#ifndef SPEECH_OUTPUT_NO_PACKAGE_FILE
    #define SPEECH_OUTPUT_NO_PACKAGE_FILE "NoPackages.mrhog"
#endif


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Launcher::Launcher() noexcept : MRH_Module("Launcher"),
                                e_State(START),
                                s_Input(""),
                                b_LaunchSet(false),
                                b_ServiceAvailable(false),
                                c_PackageList(PACKAGE_LIST_PATH)
{}

Launcher::~Launcher() noexcept
{}

Launcher::Selected::Selected(Package const& c_Package,
                             std::string const s_LaunchInput,
                             MRH_Sint32 s32_LaunchCommandID) noexcept : c_Package(c_Package),
                                                                        s_LaunchInput(s_LaunchInput),
                                                                        s32_LaunchCommandID(s32_LaunchCommandID)
{}

//*************************************************************************************
// Update
//*************************************************************************************

void Launcher::HandleEvent(const MRH_Event* p_Event) noexcept
{}

MRH_Module::Result Launcher::Update()
{
    switch (e_State)
    {
        /**
         *  Start
         */
            
        case START:
        {
            e_State = CHECK_SERVICE_LISTEN;
            return MRH_Module::FINISHED_APPEND;
        }
            
        /**
         *  Check Service
         */
            
        case CHECK_SERVICE_LISTEN:
        {
            e_State = b_ServiceAvailable ? CHECK_SERVICE_SAY : CHECK_SERVICE_LISTEN;
            return MRH_Module::FINISHED_APPEND;
        }
        case CHECK_SERVICE_SAY:
        {
            e_State = b_ServiceAvailable ? CHECK_SERVICE_APP : CHECK_SERVICE_LISTEN;
            return MRH_Module::FINISHED_APPEND;
        }
        case CHECK_SERVICE_APP:
        {
            e_State = b_ServiceAvailable ? INPUT_LAUNCH_TRIGGER : CHECK_SERVICE_LISTEN;
            return MRH_Module::FINISHED_APPEND;
        }
            
        /**
         *  Input
         */
            
        case INPUT_LAUNCH_TRIGGER:
        {
            // No input
            if (s_Input.size() == 0)
            {
                e_State = INPUT_LAUNCH_TRIGGER;
            }
            
            // Select packages
            SelectPackageLaunchTrigger();
            
            if (l_Selected.size() == 0)
            {
                e_State = OUTPUT_NO_PACKAGE;
            }
            else if (l_Selected.size() == 1)
            {
                e_State = LAUNCH_PACKAGE;
            }
            else if (l_Selected.size() > 0)
            {
                e_State = OUTPUT_PACKAGE_LIST;
            }
            
            return MRH_Module::FINISHED_APPEND;
        }
        case INPUT_PACKAGE_NAME:
        {
            // No input, wait for next
            if (s_Input.size() == 0)
            {
                l_Selected.clear();
                e_State = INPUT_LAUNCH_TRIGGER;
            }
            
            // Select packages
            FilterPackageByName();
            
            if (l_Selected.size() == 0)
            {
                e_State = OUTPUT_NO_PACKAGE;
            }
            else
            {
                e_State = LAUNCH_PACKAGE;
            }
            
            return MRH_Module::FINISHED_APPEND;
        }
            
        /**
         *  Output
         */
            
        case OUTPUT_NO_PACKAGE:
        {
            e_State = INPUT_LAUNCH_TRIGGER; // Keep listening for input
            return MRH_Module::FINISHED_APPEND;
        }
        case OUTPUT_PACKAGE_LIST:
        {
            e_State = INPUT_PACKAGE_NAME;
            return MRH_Module::FINISHED_APPEND;
        }
            
        /**
         *  Launch
         */
            
        case LAUNCH_PACKAGE:
        {
            if (b_LaunchSet == false)
            {
                e_State = CHECK_SERVICE_LISTEN; // Failed, check services again
                return MRH_Module::FINISHED_APPEND;
            }
            else
            {
                e_State = CLOSE_APP;
                return MRH_Module::IN_PROGRESS;
            }
        }
            
        /**
         *  Close App
         */
            
        default:
        {
            return MRH_Module::FINISHED_POP;
        }
    }
}

std::shared_ptr<MRH_Module> Launcher::NextModule()
{
    switch (e_State)
    {
        /**
         *  Check Service
         */
            
        case CHECK_SERVICE_LISTEN:
        {
            try
            {
                return std::make_shared<CheckService>(CheckService::LISTEN,
                                                      b_ServiceAvailable);
            }
            catch (MRH_ModuleException& e)
            {
                throw;
            }
        }
        case CHECK_SERVICE_SAY:
        {
            try
            {
                return std::make_shared<CheckService>(CheckService::SAY,
                                                      b_ServiceAvailable);
            }
            catch (MRH_ModuleException& e)
            {
                throw;
            }
        }
        case CHECK_SERVICE_APP:
        {
            try
            {
                return std::make_shared<CheckService>(CheckService::APPLICATION,
                                                      b_ServiceAvailable);
            }
            catch (MRH_ModuleException& e)
            {
                throw;
            }
        }
            
        /**
         *  Input
         */
            
        case INPUT_LAUNCH_TRIGGER:
        case INPUT_PACKAGE_NAME:
        {
            return std::make_shared<SpeechInput>(s_Input);
        }
            
        /**
         *  Output
         */
            
        case OUTPUT_PACKAGE_LIST:
        {
            try
            {
                return std::make_shared<SpeechOutput>(PackageListOutput());
            }
            catch (MRH_ModuleException& e)
            {
                throw;
            }
        }
        case OUTPUT_NO_PACKAGE:
        {
            try
            {
                return std::make_shared<SpeechOutput>(NoPackagesOutput());
            }
            catch (MRH_ModuleException& e)
            {
                throw;
            }
        }
            
        /**
         *  Launch
         */
            
        case LAUNCH_PACKAGE:
        {
            try
            {
                if (l_Selected.size() == 0)
                {
                    throw MRH_ModuleException("Launcher",
                                              "No package to launch!");
                }
                
                auto& Selected = l_Selected.front();
                return std::make_shared<LaunchPackage>(Selected.c_Package.GetPackagePath(),
                                                       Selected.s_LaunchInput,
                                                       Selected.s32_LaunchCommandID,
                                                       b_LaunchSet);
            }
            catch (MRH_ModuleException& e)
            {
                throw;
            }
        }
            
        /**
         *  Unk
         */
            
        default:
        {
            throw MRH_ModuleException("Launcher",
                                      "No module to switch to!");
        }
    }
}

//*************************************************************************************
// Package
//*************************************************************************************

void Launcher::SelectPackageLaunchTrigger() noexcept
{
    std::list<Package> const& l_Package = c_PackageList.GetPackages();
    MRH_InputTrigger::Evaluation c_Current(-1, 0);
    
    for (auto& Package : l_Package)
    {
        // Evaluate with trigger
        MRH_InputTrigger::Evaluation c_Next = Package.GetLaunchTrigger().Evaluate(s_Input, 1);
        
        if (c_Next.first < 0 || c_Next.second < c_Current.second)
        {
            // Invalid, do not use
            continue;
        }
        
        // New highest weight?
        if (c_Next.second > c_Current.second)
        {
            // Highest weight, replace old
            l_Selected.clear();
            
            // New highest
            c_Current = c_Next;
        }
        
        // Can be added
        l_Selected.emplace_back(Package,
                                s_Input,
                                c_Current.first);
    }
    
    // List matching
    MRH_ModuleLogger& c_Logger = MRH_ModuleLogger::Singleton();
    
    if (l_Selected.size() > 0)
    {
        c_Logger.Log("Launcher", "Selected packages by trigger for input " +
                                 s_Input +
                                 ":",
                     "Launcher.cpp", __LINE__);
        
        for (auto& Selected : l_Selected)
        {
            c_Logger.Log("Launcher", Selected.c_Package.GetPackagePath() +
                                     " (Command: " +
                                     std::to_string(Selected.s32_LaunchCommandID) +
                                     ")",
                         "Launcher.cpp", __LINE__);
        }
    }
    else
    {
        c_Logger.Log("Launcher", "No matching packages found.",
                     "Launcher.cpp", __LINE__);
    }
}

void Launcher::FilterPackageByName() noexcept
{
    MRH_ModuleLogger& c_Logger = MRH_ModuleLogger::Singleton();
    bool b_Match;
    
    for (auto It = l_Selected.begin(); It != l_Selected.end();)
    {
        b_Match = MRH_StringCompareLS::ContainedIn(It->c_Package.GetApplicationName(),
                                                   s_Input,
                                                   0.75f);
        
        if (b_Match == false)
        {
            c_Logger.Log("Launcher", It->c_Package.GetPackagePath() +
                                     " removed, name mismatch with input " +
                                     s_Input,
                         "Launcher.cpp", __LINE__);
            
            It = l_Selected.erase(It);
        }
        else
        {
            ++It;
        }
    }
}

//*************************************************************************************
// Output
//*************************************************************************************

std::string Launcher::NoPackagesOutput()
{
    try
    {
        return MRH_OutputGenerator(MRH_LocalisedPath::GetPath(SPEECH_OUTPUT_DIR, 
                                                              SPEECH_OUTPUT_NO_PACKAGE_FILE)).Generate();
    }
    catch (std::exception& e)
    {
        throw MRH_ModuleException("Launcher",
                                  e.what());
    }
}

std::string Launcher::PackageListOutput()
{
    try
    {
        std::string s_PackageList = MRH_OutputGenerator(MRH_LocalisedPath::GetPath(SPEECH_OUTPUT_DIR,
                                                                                   SPEECH_OUTPUT_LIST_PACKAGE_FILE)).Generate();
        s_PackageList += " ";
        
        for (auto It = l_Selected.begin(); It != l_Selected.end(); ++It)
        {
            s_PackageList += It->c_Package.GetApplicationName();
            
            if (It != (--(l_Selected.end())))
            {
                s_PackageList += ", ";
            }
        }
        
        return s_PackageList;
    }
    catch (std::exception& e)
    {
        throw MRH_ModuleException("Launcher",
                                  e.what());
    }
}

//*************************************************************************************
// Getters
//*************************************************************************************

bool Launcher::CanHandleEvent(MRH_Uint32 u32_Type) noexcept
{
    return false;
}
