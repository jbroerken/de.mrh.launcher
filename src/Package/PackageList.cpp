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
#include <libmrhbf.h>
#include <libmrhab/Module/Tools/MRH_ModuleLogger.h>

// Project
#include "./PackageList.h"

// Pre-defined
namespace
{
    const char* p_BlockIdentifier = "Package";
    const char* p_CountKey = "Count";
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageList::PackageList(std::string const& s_PackageListPath) noexcept
{
    MRH_ModuleLogger& c_Logger = MRH_ModuleLogger::Singleton();
    c_Logger.Log("PackageList", "Reading " +
                                 s_PackageListPath +
                                 " package location config...",
                 "PackageList.cpp", __LINE__);
    
    try
    {
        MRH_BlockFile c_File(s_PackageListPath);
        
        for (auto& Block : c_File.l_Block)
        {
            if (Block.GetName().compare(p_BlockIdentifier) != 0)
            {
                continue;
            }
            
            // Grab the count first
            MRH_Uint32 u32_Count = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_CountKey)));
            
            // Now read the packages
            MRH_ValueBlock::ValueList l_Value = Block.GetValues();
            
            for (auto& Value : l_Value)
            {
                try
                {
                    if (std::stoull(Value.first) < u32_Count && Value.second.size() > 0)
                    {
                        l_Package.emplace_back(Value.second);
                    }
                }
                catch (std::exception& e) // Catch all
                {
                    c_Logger.Log("PackageList", "Failed to load package " +
                                                Value.second +
                                                ": " +
                                                e.what(),
                                 "PackageList.cpp", __LINE__);
                }
            }
        }
    }
    catch (std::exception& e)
    {
        c_Logger.Log("PackageList", "Failed to load package list: " +
                                   std::string(e.what()),
                     "PackageList.cpp", __LINE__);
    }
}

PackageList::~PackageList() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::list<Package> const& PackageList::GetPackages() const noexcept
{
    return l_Package;
}
