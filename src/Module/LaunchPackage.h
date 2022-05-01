/**
 *  LaunchPackage.h
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
