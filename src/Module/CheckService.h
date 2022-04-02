/**
 *  CheckService.h
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

#ifndef CheckService_h
#define CheckService_h

// C / C++

// External
#include <libmrhab/Module/MRH_Module.h>

// Project


class CheckService : public MRH_Module
{
public:
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    enum Service
    {
        LISTEN = 0,
        SAY = 1,
        APPLICATION = 2,
        
        SERVICE_MAX = APPLICATION,
        
        SERVICE_COUNT = SERVICE_MAX + 1
    };
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************
    
    /**
     *  Default constructor.
     *
     *  \param e_Service The platform service to check.
     *  \param b_ServiceAvailable The services available flag to set.
     */
    
    CheckService(Service e_Service,
                 bool& b_ServiceAvailable);
    
    /**
     *  Default destructor.
     */
    
    ~CheckService() noexcept;
    
    //*************************************************************************************
    // Update
    //*************************************************************************************
    
    /**
     *  Hand a recieved event to the module.
     *
     *  \param p_Event The recieved event.
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
    
    enum Result
    {
        NOT_SET = 0,
        AVAILABLE = 1,
        UNAVAILABLE = 2,
        
        RESULT_MAX = UNAVAILABLE,
        
        RESULT_COUNT = RESULT_MAX + 1
    };
    
    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    MRH_ModuleTimer c_Timer;
    
    Service e_Service;
    Result e_Result;
    bool& b_ServiceAvailable;
    
protected:
    
};

#endif /* CheckService_h */
