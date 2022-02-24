//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		BSD License
//					Kratos default license: kratos/license.txt
//
//  Main authors:    Ilaria Iaconeta, Laura Moreno
//


#if !defined(KRATOS_UPDATED_LAGRANGIAN_UP_VMS_H_INCLUDED )
#define  KRATOS_UPDATED_LAGRANGIAN_UP_VMS_H_INCLUDED

// System includes

// External includes

// Project includes
#include "custom_elements/updated_lagrangian.hpp"

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

/// Large Displacement Lagrangian Element for 3D and 2D geometries. (base class)

/**
 * Implements a Large Displacement Lagrangian definition for structural analysis.
 * This works for arbitrary geometries in 3D and 2D (base class)
 */

class UpdatedLagrangianUPVMS
    : public UpdatedLagrangian
{
public:

    ///@name Type Definitions
    ///@{
    ///Reference type definition for constitutive laws
    typedef ConstitutiveLaw ConstitutiveLawType;
    ///Pointer type for constitutive laws
    typedef ConstitutiveLawType::Pointer ConstitutiveLawPointerType;
    ///StressMeasure from constitutive laws
    typedef ConstitutiveLawType::StressMeasure StressMeasureType;
    ///Type definition for integration methods
    typedef GeometryData::IntegrationMethod IntegrationMethod;

    /// Counted pointer of LargeDisplacementElement
    KRATOS_CLASS_INTRUSIVE_POINTER_DEFINITION( UpdatedLagrangianUPVMS );
    ///@}


public:


    ///@name Life Cycle
    ///@{

    /// Empty constructor needed for serialization
    UpdatedLagrangianUPVMS();


    /// Default constructors
    UpdatedLagrangianUPVMS(IndexType NewId, GeometryType::Pointer pGeometry);

    UpdatedLagrangianUPVMS(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties);

    ///Copy constructor
    UpdatedLagrangianUPVMS(UpdatedLagrangianUPVMS const& rOther);

    /// Destructor.
    ~UpdatedLagrangianUPVMS() override;

    ///@}
    ///@name Operators
    ///@{

    /// Assignment operator.
    UpdatedLagrangianUPVMS& operator=(UpdatedLagrangianUPVMS const& rOther);

    ///@}
    ///@name Operations
    ///@{

    /**
     * creates a new element pointer
     * @param NewId: the ID of the new element
     * @param ThisNodes: the nodes of the new element
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes, PropertiesType::Pointer pProperties) const override;

    Element::Pointer Create(IndexType NewId, GeometryType::Pointer pGeom, PropertiesType::Pointer pProperties) const override;

    /**
     * clones the selected element variables, creating a new one
     * @param NewId: the ID of the new element
     * @param ThisNodes: the nodes of the new element
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Clone(IndexType NewId, NodesArrayType const& ThisNodes) const override;


    //************* GETTING METHODS

    /**
     * Sets on rElementalDofList the degrees of freedom of the considered element geometry
     */
    void GetDofList(DofsVectorType& rElementalDofList, const ProcessInfo& rCurrentProcessInfo) const override;

    /**
     * Sets on rResult the ID's of the element degrees of freedom
     */
    void EquationIdVector(EquationIdVectorType& rResult, const ProcessInfo& rCurrentProcessInfo) const override;

    /**
     * Sets on rValues the nodal displacements
     */
    void GetValuesVector(Vector& rValues, int Step = 0) const override;

    /**
     * Sets on rValues the nodal velocities
     */
    void GetFirstDerivativesVector(Vector& rValues, int Step = 0) const override;

    /**
     * Sets on rValues the nodal accelerations
     */
    void GetSecondDerivativesVector(Vector& rValues, int Step = 0) const override;

    //************* STARTING - ENDING  METHODS

    /**
      * Called to initialize the element.
      * Must be called before any calculation is done
      */
    void Initialize(const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * Called at the beginning of each solution step
     */
    void InitializeSolutionStep(const ProcessInfo& rCurrentProcessInfo) override;


    //************* COMPUTING  METHODS

    /**
      * this is called during the assembling process in order
      * to calculate the elemental mass matrix
      * @param rMassMatrix: the elemental mass matrix
      * @param rCurrentProcessInfo: the current process info instance
      */
    void CalculateMassMatrix(MatrixType& rMassMatrix,
                             const ProcessInfo& rCurrentProcessInfo) override;


    //************************************************************************************
    //************************************************************************************
    /**
     * This function provides the place to perform checks on the completeness of the input.
     * It is designed to be called only once (or anyway, not often) typically at the beginning
     * of the calculations, so to verify that nothing is missing from the input
     * or that no common error is found.
     * @param rCurrentProcessInfo
     */
    int Check(const ProcessInfo& rCurrentProcessInfo) const override;


    ///@}
    ///@name Access
    ///@{

    void CalculateOnIntegrationPoints(const Variable<double>& rVariable,
        std::vector<double>& rValues,
        const ProcessInfo& rCurrentProcessInfo) override;

    void SetValuesOnIntegrationPoints(
        const Variable<double>& rVariable,
        const std::vector<double>& rValues,
        const ProcessInfo& rCurrentProcessInfo) override;

    ///@}
    ///@name Inquiry
    ///@{
    ///@}
    ///@name Input and output
    ///@{
    /// Turn back information as a string.
    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "MPM Element #" << Id();
        return buffer.str();
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "MPM Element #" << Id();
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
        GetGeometry().PrintData(rOStream);
    }
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

    static const unsigned int msIndexVoigt3D6C [6][2];
    static const unsigned int msIndexVoigt2D4C [4][2];
    static const unsigned int msIndexVoigt2D3C [3][2];



    SizeType GetNumberOfDofs() override {
        return GetGeometry().WorkingSpaceDimension() + 1;
    }

    /*
        Compute Element Size
    */

   void ComputeElementSize(double& ElementSize);

   // double ComputeElementSize(BoundedMatrix<double,TNumNodes, TDim>& DN_DX );

       /**
     * Calculates the elemental contributions
     * \f$ K^e = w\,B^T\,D\,B \f$ and
     * \f$ r^e \f$
     */
    void CalculateElementalSystem(
        MatrixType& rLeftHandSideMatrix,
        VectorType& rRightHandSideVector,
        const ProcessInfo& rCurrentProcessInfo,
        const bool CalculateStiffnessMatrixFlag,
        const bool CalculateResidualVectorFlag) override;


    ///@}
    ///@name Protected Operations
    ///@{
    void FinalizeStepVariables(GeneralVariables & rVariables, const ProcessInfo& rCurrentProcessInfo) override;

    /*
     * Calculation of stabilization parameters 
     */

    void CalculateTaus(const int& stabilization_type, GeneralVariables& rVariables);

     /*
     * To compute identity tensor 
     */

    void CalculateTensorIdentityMatrix (GeneralVariables& rVariables, Matrix& rTensorIdentityMatrix);

    double& TensorIdentityComponent (double& rCabcd, GeneralVariables& rVariables,
    const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d);

    // To compute vector in voigt notation to multiply

    void ConvertPressureGradient(Vector& PressureGradientVoigt,Vector& PressureGradient);
    /*
     * Calculation of Specific variables: pressure and gradient pressure 
     */

    void SetSpecificVariables(GeneralVariables& rVariables);


    /**
     * Calculation and addition of the matrices of the LHS
     */

    void CalculateAndAddLHS(MatrixType& rLeftHandSideMatrix,
                            GeneralVariables& rVariables,
                            const double& rIntegrationWeight,
                            const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * Calculation and addition of the vectors of the RHS
     */

    void CalculateAndAddRHS(VectorType& rRightHandSideVector,
                            GeneralVariables& rVariables,
                            Vector& rVolumeForce,
                            const double& rIntegrationWeight,
                            const ProcessInfo& rCurrentProcessInfo) override;


    /**
     * Calculation of the Material Stiffness Matrix. Kuum = BT * C * B
     */

    void CalculateAndAddKuum(MatrixType& rLeftHandSideMatrix,
                             GeneralVariables& rVariables,
                             const double& rIntegrationWeight) override;

    /**
     * Calculation of the Geometric Stiffness Matrix. Kuug = BT * S
     */
    void CalculateAndAddKuugUP(MatrixType& rLeftHandSideMatrix,
                               GeneralVariables& rVariables,
                               const double& rIntegrationWeight);

    /**
     * Calculation of the Kup matrix
     */
    virtual void CalculateAndAddKup (MatrixType& rK,
                                     GeneralVariables & rVariables,
                                     const double& rIntegrationWeight
                                    );

    /**
     * Calculation of the Kpu matrix
     */
    virtual void CalculateAndAddKpu(MatrixType& rK,
                                    GeneralVariables & rVariables,
                                    const double& rIntegrationWeight
                                   );

    /**
     * Calculation of the Kpp matrix
     */
    virtual void CalculateAndAddKpp(MatrixType& rK,
                                    GeneralVariables & rVariables,
                                    const double& rIntegrationWeight
                                   );


    /**
     * Calculation of the Kuu Stabilization Term matrix
     */


    virtual void CalculateAndAddKuuStab(MatrixType& rK,
                                        GeneralVariables & rVariables,
                                        const double& rIntegrationWeight
                                       );

    /**
     * Calculation of the Kup Stabilization Term matrix
     */


    virtual void CalculateAndAddKupStab(MatrixType& rK,
                                        GeneralVariables & rVariables,
                                        const double& rIntegrationWeight
                                       );

    /**
     * Calculation of the Kup Stabilization Term matrix
     */


    virtual void CalculateAndAddKpuStab(MatrixType& rK,
                                        GeneralVariables & rVariables,
                                        const double& rIntegrationWeight
                                       );                                     

    /**
     * Calculation of the Kpp Stabilization Term matrix
     */
    virtual void CalculateAndAddKppStab(MatrixType& rK,
                                        GeneralVariables & rVariables,
                                        const double& rIntegrationWeight
                                       );
    /**
     * Calculation of the External Forces Vector. Fe = N * t + N * b
     */
    void CalculateAndAddExternalForces(VectorType& rRightHandSideVector,
                                       GeneralVariables& rVariables,
                                       Vector& rVolumeForce,
                                       const double& rIntegrationWeight) override;

    /**
      * Calculation of the Internal Forces Vector. Fi = B * sigma
      */
    void CalculateAndAddInternalForces(VectorType& rRightHandSideVector,
                                       GeneralVariables & rVariables,
                                       const double& rIntegrationWeight) override;

    /**
     * Calculation of the Internal Forces due to Pressure-Balance
     */
    virtual void CalculateAndAddPressureForces(VectorType& rRightHandSideVector,
            GeneralVariables & rVariables,
            const double& rIntegrationWeight
                                              );

    /**
     * Calculation of the Internal Forces due to Pressure-Balance
     */
    virtual void CalculateAndAddStabilizedPressure(VectorType& rRightHandSideVector,
            GeneralVariables & rVariables,
            Vector& rVolumeForce,
            const double& rIntegrationWeight
                                                  );


    /**
     * Calculation stabilization terms for the momentum equation
     */
    virtual void CalculateAndAddStabilizedDisplacement(VectorType& rRightHandSideVector,
            GeneralVariables & rVariables,
            Vector& rVolumeForce,
            const double& rIntegrationWeight
                                                  );


    /**
     * Calculate Element Kinematics
     */
    void CalculateKinematics(GeneralVariables& rVariables, const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * Initialize Element General Variables
     */
    void InitializeGeneralVariables(GeneralVariables & rVariables, const ProcessInfo& rCurrentProcessInfo) override;
    /**
     * Update the position of the MP or Gauss point when Finalize Element Internal Variables is called
     */

    void UpdateGaussPoint(GeneralVariables & rVariables, const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * Get the Historical Deformation Gradient to calculate after finalize the step
     */
    void GetHistoricalVariables( GeneralVariables& rVariables) override;

    /**
     * Calculation of the Deformation Matrix  BL
     */
    using UpdatedLagrangian::CalculateDeformationMatrix;
    void CalculateDeformationMatrix(Matrix& rB,
                                    Matrix& rF,
                                    Matrix& rDN_DX);

    /**
     * Calculation of the Volume Change of the Element
     */
    double& CalculateVolumeChange(double& rVolumeChange, GeneralVariables& rVariables) override;

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

    double m_mp_pressure;

    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{
    ///@}

    ///@}
    ///@name Serialization
    ///@{
    friend class Serializer;

    // A private default constructor necessary for serialization

    void save(Serializer& rSerializer) const override;

    void load(Serializer& rSerializer) override;


    ///@name Private Inquiry
    ///@{
    ///@}
    ///@name Un accessible methods
    ///@{
    ///@}

}; // Class UpdatedLagrangian

///@}
///@name Type Definitions
///@{
///@}
///@name Input and output
///@{
///@}

} // namespace Kratos.
#endif // KRATOS_UPDATED_LAGRANGIAN_H_INCLUDED  defined