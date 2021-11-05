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

// External includes

// Project includes
#include "includes/registry_item.h"

namespace Kratos
{

    std::string RegistryItem::Info() const{
        return mName + " RegistryItem ";
    }

    void RegistryItem::PrintInfo(std::ostream &rOStream) const{
        rOStream << Info();
    }

    void RegistryItem::PrintData(std::ostream &rOStream) const{
        for(auto& item : mSubRegistryItem){
            rOStream << *(item.second) << std::endl;
        }
    }

    std::string RegistryItem::ToJson(std::string const& Indentation) const {
        KRATOS_ERROR_IF(HasValue()) << "For storing a value you should use the RegistryValueItem" << std::endl;

        std::stringstream buffer;
        buffer  << Indentation << "\"" << mName << "\" : {" << std::endl;
        for(auto& item : mSubRegistryItem){
            buffer << item.second->ToJson( Indentation + "    ");
        }
        buffer << "}" << std::endl;

        return buffer.str();
    }


    RegistryItem const& RegistryItem::GetItem(std::string const& ItemName) const {
        auto iterator = mSubRegistryItem.find(ItemName);
        KRATOS_ERROR_IF(iterator == mSubRegistryItem.end()) << "The RegistryItem " << this->Name() << " does not have an item with name " << ItemName << std::endl;
        return *(iterator->second);
    }

    RegistryItem& RegistryItem::GetItem(std::string const& ItemName) {
        auto iterator = mSubRegistryItem.find(ItemName);
        KRATOS_ERROR_IF(iterator == mSubRegistryItem.end()) << "The RegistryItem " << this->Name() << " does not have an item with name " << ItemName << std::endl;
        return *(iterator->second);
    }

    void RegistryItem::RemoveItem(std::string const& ItemName) {
        auto iterator = mSubRegistryItem.find(ItemName);
        KRATOS_ERROR_IF(iterator == mSubRegistryItem.end()) << "The RegistryItem " << this->Name() << " does not have an item with name " << ItemName << std::endl;
        mSubRegistryItem.erase(iterator);
    }


} // namespace Kratos.