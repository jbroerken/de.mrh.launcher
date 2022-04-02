/**
 *  Package.h
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
