/**
 *  Package.cpp
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
