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

#ifndef Package_h
#define Package_h

// C / C++
#include <memory>

// External
#include <libmrhvt/Input/MRH_InputTrigger.h>
#include <libmrhvt/Output/MRH_OutputGenerator.h>

// Project


class Package
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param s_PackagePath The full path to the package.
     */
    
    Package(std::string const& s_PackagePath);
    
    /**
     *  Default destructor.
     */
    
    ~Package() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the full package path.
     *
     *  \return The full package path.
     */
    
    std::string const& GetPackagePath() const noexcept;
    
    /**
     *  Get the application name.
     *
     *  \return The application name.
     */
    
    std::string const& GetApplicationName() const noexcept;
    
    /**
     *  Get the application launch trigger.
     *
     *  \return The application launch trigger.
     */
    
    MRH_InputTrigger const& GetLaunchTrigger() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_PackagePath;
    std::string s_ApplicationName;
    MRH_InputTrigger c_LaunchTrigger;
    
protected:
    
};

#endif /* Package_h */
