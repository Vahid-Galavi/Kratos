//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//

// System includes
#include <mutex>

// External includes

// Project includes
#include "includes/registry.h"

namespace Kratos
{

namespace {
    std::once_flag flag_once;
}

    RegistryItem* Registry::mspRootRegistryItem = nullptr;

    RegistryItem& Registry::GetItem(std::string const& ItemFullName){
        auto item_path = SplitFullName(ItemFullName);
        
        RegistryItem* p_current_item = &GetRootRegistryItem();

        for(std::size_t i = 0 ; i < item_path.size() ; i++){
            auto& item_name = item_path[i];
            if(p_current_item->HasItem(item_name)){
                p_current_item = &p_current_item->GetItem(item_name);
            }
            else{
                KRATOS_ERROR << "The item \"" << ItemFullName << "\" is not found in the registry. The item \"" << p_current_item->Name() << "\" does not have \"" << item_name << "\"" << std::endl;
            }
        }

        return *p_current_item;
    }

    bool Registry::HasItem(std::string const& ItemFullName){
        auto item_path = SplitFullName(ItemFullName);
        
        RegistryItem* p_current_item = &GetRootRegistryItem();

        for(std::size_t i = 0 ; i < item_path.size() ; i++){
            auto& item_name = item_path[i];
            if(p_current_item->HasItem(item_name)){
                p_current_item = &p_current_item->GetItem(item_name);
            }
            else{
               return false;
            }
        }
        return true;    
    }

    std::string Registry::Info() const{
        return "Registry";
    }

    void Registry::PrintInfo(std::ostream &rOStream) const{
        rOStream << Info();
    }

    void Registry::PrintData(std::ostream &rOStream) const{
    }

    std::string Registry::ToJson(std::string const& Indentation) const {
        return GetRootRegistryItem().ToJson(Indentation);
    }

    RegistryItem& Registry::GetRootRegistryItem(){
        if (!mspRootRegistryItem) {
            std::call_once(flag_once, [](){
                static RegistryItem root_item("Registry");
                mspRootRegistryItem = &root_item;
            });
        }

    return *mspRootRegistryItem;

    }

    std::vector<std::string> Registry::SplitFullName(std::string const& FullName){
        std::istringstream iss(FullName);
        std::vector<std::string> result;
        std::string name;

        while (std::getline(iss, name, '.')){
            result.push_back(name);
        }

        KRATOS_ERROR_IF(result.empty()) << "The item full name is empty" << std::endl;


        return result;
    }


} // namespace Kratos.