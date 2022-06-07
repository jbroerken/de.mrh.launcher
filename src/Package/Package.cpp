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
#include <fstream>

// External
#include <libmrhvt/String/MRH_LocalisedPath.h>
#include <libmrhab/Module/Tools/MRH_ModuleLogger.h>

// Project
#include "./Package.h"

// Pre-defined
#define PACKAGE_LAUNCH_TRIGGER_DIRECTORY "LaunchTrigger"
#define PACKAGE_LAUNCH_TRIGGER_FILE "LaunchTrigger.mrhit"
#define PACKAGE_APPLICATION_NAME_DIRECTORY "ApplicationName"
#define PACKAGE_APPLICATION_NAME_FILE "ApplicationName.txt"
#define DEVOTION_LAUNCH_RECOMMENDATION_DIRECTORY "FSRoot/Devotion"
#define DEVOTION_LAUNCH_RECOMMENDATION_FILE "LaunchRecommendation.mrhog"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Package::Package(std::string const& s_PackagePath) : s_PackagePath(s_PackagePath),
                                                     c_LaunchTrigger(MRH_LocalisedPath::GetPath(s_PackagePath + "/" + PACKAGE_LAUNCH_TRIGGER_DIRECTORY,
                                                                                                PACKAGE_LAUNCH_TRIGGER_FILE)),
                                                     s_ApplicationName("")
{
    // Load the application name
    std::ifstream f_File;
    
    f_File.open(MRH_LocalisedPath::GetPath(s_PackagePath + "/" + PACKAGE_APPLICATION_NAME_DIRECTORY,
                                           PACKAGE_APPLICATION_NAME_FILE));
    std::getline(f_File, s_ApplicationName);
    f_File.close();
    
    if (s_ApplicationName.size() == 0)
    {
        throw MRH_VTException("Invalid application name!");
    }
}

Package::~Package() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string const& Package::GetPackagePath() const noexcept
{
    return s_PackagePath;
}

std::string const& Package::GetApplicationName() const noexcept
{
    return s_ApplicationName;
}

MRH_InputTrigger const& Package::GetLaunchTrigger() const noexcept
{
    return c_LaunchTrigger;
}
