//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                     Kratos default license: kratos/license.txt
//
//  Main authors:    Alejandro Cornejo
//                   Riccardo Rossi
//
//


#if !defined(KRATOS_PROPERTY_ACCESSOR_H_INCLUDED)
#define KRATOS_PROPERTY_ACCESSOR_H_INCLUDED

// System includes

// External includes

// Project includes


namespace Kratos
{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/**
 * PropertyAccessor sets a proper way of returning a value from the properties
 * It is used to properly interpolate material properties according to temperature, tables, etc...
*/
class KRATOS_API(KRATOS_CORE) PropertyAccessor
{
public:
    ///@name Type Definitions
    ///@{
    
    typedef std::function<double(const std::string&, Properties&) > AccessorType;

    /// Pointer definition of ProcessInfo
    KRATOS_CLASS_POINTER_DEFINITION(PropertyAccessor);


    ///@}
    ///@name Life Cycle
    ///@{



    ///@}
    ///@name Operators
    ///@{

    PropertyAccessor(double Value)
    {
        mValue = Value;
    }



    ///@}
    ///@name Operations
    ///@{


    ///@}
    ///@name Solution Step Data
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    double mValue;
    std::map< std::string, AccessorType* > mListOfAccessors;

    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{

    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save(Serializer& rSerializer) const override;

    void load(Serializer& rSerializer) override;

    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{


    ///@}

}; // Class ProcessInfo

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}


}  // namespace Kratos.

#endif //   defined

