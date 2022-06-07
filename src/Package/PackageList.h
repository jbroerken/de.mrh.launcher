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

#ifndef PackageList_h
#define PackageList_h

// C / C++
#include <list>

// External

// Project
#include "./Package.h"


class PackageList
{
public:
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param s_PackageListPath The full path to the package list file.
     */
    
    PackageList(std::string const& s_PackageListPath) noexcept;
    
    /**
     *  Default destructor.
     */
    
    ~PackageList() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the packages.
     *
     *  \return The packages.
     */
    
    std::list<Package> const& GetPackages() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::list<Package> l_Package;
    
protected:
    
};

#endif /* PackageList_h */
