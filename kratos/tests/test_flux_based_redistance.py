import KratosMultiphysics.KratosUnittest as KratosUnittest
import KratosMultiphysics
from KratosMultiphysics import python_linear_solver_factory as linear_solver_factory
import KratosMultiphysics.vtk_output_process as vtk_output_process
import math
import os

def GetFilePath(fileName):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), fileName)

class TestRedistance(KratosUnittest.TestCase):

    def _ExpectedDistance(self,x,y,z):
        d = x
        if( d > 0.2):
            d = 0.2
        if( d < -0.2):
            d = -0.2
        return x
        #return -(math.sqrt(x**2+y**2+z**2) - 0.4)

    def test_model_part_sub_model_parts(self):
        current_model = KratosMultiphysics.Model()

        model_part = current_model.CreateModelPart("Main")
        model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)
        model_part.AddNodalSolutionStepVariable(KratosMultiphysics.VELOCITY)
        model_part.AddNodalSolutionStepVariable(KratosMultiphysics.FLAG_VARIABLE)
        model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_VOLUME)
        KratosMultiphysics.ModelPartIO(GetFilePath("auxiliar_files_for_python_unittest/mdpa_files/coarse_sphere")).ReadModelPart(model_part)
        model_part.SetBufferSize(2)


        for node in model_part.Nodes:
            node.SetSolutionStepValue(KratosMultiphysics.DISTANCE,0, self._ExpectedDistance(node.X,node.Y,node.Z)  )

        linear_solver = linear_solver_factory.ConstructSolver( KratosMultiphysics.Parameters( """ { "solver_type" : "skyline_lu_factorization" } """ ) )

        model_part.CloneTimeStep(1.0)

        settings = KratosMultiphysics.Parameters()
        KratosMultiphysics.FluxBasedRedistanceProcess3D(model_part, linear_solver, settings).Execute()

        max_distance = -1.0
        min_distance = +1.0
        for node in model_part.Nodes:
            d =  node.GetSolutionStepValue(KratosMultiphysics.DISTANCE)
            max_distance = max(max_distance, d)
            min_distance = min(min_distance, d)


        vtk_output_parameters = KratosMultiphysics.Parameters("""{
            "Parameters" : {
                "model_part_name"                    : "Main",
                "file_format"                        : "ascii",
                "output_precision"                   : 8,
                "output_interval"                    : 2,
                "output_sub_model_parts"             : true,
                "output_path"                        : "test_vtk_output",
                "nodal_solution_step_data_variables" : ["DISTANCE"],
                "nodal_data_value_variables"         : ["POTENTIAL_GRADIENT","ADJOINT_SCALAR_1"],
                "nodal_flags"                        : [],
                "element_data_value_variables"       : [],
                "condition_data_value_variables"     : [],
                "condition_flags"                    : []
            }
        }""")

        vtk_output_parameters["Parameters"]["model_part_name"].SetString("Main")
        vtk_output_parameters["Parameters"]["file_format"].SetString("ascii")
        process = vtk_output_process.Factory(vtk_output_parameters, current_model)
        process.ExecuteInitialize()
        process.ExecuteInitializeSolutionStep()
        process.ExecuteFinalizeSolutionStep()
        process.PrintOutput()


        self.assertAlmostEqual(max_distance, 0.49554221316850783)
        self.assertAlmostEqual(min_distance, -0.4950998362886954)

    # def test_variational_redistance_maintain_plane_2d(self):
    #     current_model = KratosMultiphysics.Model()
    #     free_surface_level = 0.25

    #     model_part = current_model.CreateModelPart("Main")
    #     model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)
    #     model_part.AddNodalSolutionStepVariable(KratosMultiphysics.FLAG_VARIABLE)

    #     model_part.CreateNewNode(1, 0.0 , 0.0 , 0.0)
    #     model_part.CreateNewNode(2, 1.0 , 2.0 , 0.0)
    #     model_part.CreateNewNode(3, -1.0 , 1.0 , 0.0)
    #     model_part.CreateNewNode(4, 2.0 , 1.0 , 0.0)

    #     model_part.AddProperties(KratosMultiphysics.Properties(1))

    #     model_part.CreateNewElement("Element2D3N", 1, [1,2,3], model_part.GetProperties()[1])
    #     model_part.CreateNewElement("Element2D3N", 2, [1,4,2], model_part.GetProperties()[1])

    #     for node in model_part.Nodes:
    #         node.SetSolutionStepValue(KratosMultiphysics.DISTANCE, node.Y - free_surface_level)


    #     linear_solver = linear_solver_factory.ConstructSolver( KratosMultiphysics.Parameters( """ { "solver_type" : "skyline_lu_factorization" } """ ) )

    #     model_part.CloneTimeStep(1.0)

    #     max_iterations = 2
    #     KratosMultiphysics.VariationalDistanceCalculationProcess2D(
    #         model_part,
    #         linear_solver,
    #         max_iterations,
    #         KratosMultiphysics.VariationalDistanceCalculationProcess2D.CALCULATE_EXACT_DISTANCES_TO_PLANE).Execute()

    #     for node in model_part.Nodes:
    #         self.assertAlmostEqual(node.GetSolutionStepValue(KratosMultiphysics.DISTANCE), node.Y - free_surface_level, 10 )

    # def test_variational_redistance_maintain_plane_3d(self):
    #     current_model = KratosMultiphysics.Model()
    #     free_surface_level = 0.25

    #     model_part = current_model.CreateModelPart("Main")
    #     model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)
    #     model_part.AddNodalSolutionStepVariable(KratosMultiphysics.FLAG_VARIABLE)

    #     model_part.CreateNewNode(1, 0.0 , 0.0 , 0.0)
    #     model_part.CreateNewNode(2, 1.0 , 2.0 , 0.0)
    #     model_part.CreateNewNode(3, -1.0 , 1.0 , 0.0)
    #     model_part.CreateNewNode(4, 2.0 , 1.0 , 0.0)
    #     model_part.CreateNewNode(5, 2.0 , 2.0 , 1.0)

    #     model_part.AddProperties(KratosMultiphysics.Properties(1))

    #     model_part.CreateNewElement("Element3D4N", 1, [1,2,3,5], model_part.GetProperties()[1])
    #     model_part.CreateNewElement("Element3D4N", 2, [1,4,2,5], model_part.GetProperties()[1])

    #     for node in model_part.Nodes:
    #         node.SetSolutionStepValue(KratosMultiphysics.DISTANCE, node.Y - free_surface_level)

    #     linear_solver = linear_solver_factory.ConstructSolver( KratosMultiphysics.Parameters( """ { "solver_type" : "skyline_lu_factorization" } """ ) )

    #     model_part.CloneTimeStep(1.0)

    #     max_iterations = 2
    #     KratosMultiphysics.VariationalDistanceCalculationProcess3D(
    #         model_part,
    #         linear_solver,
    #         max_iterations,
    #         KratosMultiphysics.VariationalDistanceCalculationProcess3D.CALCULATE_EXACT_DISTANCES_TO_PLANE).Execute()

    #     for node in model_part.Nodes:
    #         self.assertAlmostEqual(node.GetSolutionStepValue(KratosMultiphysics.DISTANCE), node.Y - free_surface_level, 10 )

    # def test_parallel_redistance_maintain_plane_3d(self):
    #     current_model = KratosMultiphysics.Model()
    #     free_surface_level = 0.25

    #     model_part = current_model.CreateModelPart("Main")
    #     model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)
    #     model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_VOLUME)

    #     model_part.CreateNewNode(1, 0.0 , 0.0 , 0.0)
    #     model_part.CreateNewNode(2, 1.0 , 2.0 , 0.0)
    #     model_part.CreateNewNode(3, -1.0 , 1.0 , 0.0)
    #     model_part.CreateNewNode(4, 2.0 , 1.0 , 0.0)
    #     model_part.CreateNewNode(5, 2.0 , 2.0 , 1.0)

    #     model_part.AddProperties(KratosMultiphysics.Properties(1))

    #     model_part.CreateNewElement("Element3D4N", 1, [1,2,3,5], model_part.GetProperties()[1])
    #     model_part.CreateNewElement("Element3D4N", 2, [1,4,2,5], model_part.GetProperties()[1])

    #     for node in model_part.Nodes:
    #         node.SetSolutionStepValue(KratosMultiphysics.DISTANCE, node.Y - free_surface_level)
    #         node.SetValue(KratosMultiphysics.NODAL_VOLUME, 0.0)

    #     for element in model_part.Elements:
    #         el_vol = element.GetGeometry().Area()
    #         for node in element.GetNodes():
    #             nodal_volume = node.GetValue(KratosMultiphysics.NODAL_VOLUME) + el_vol*0.25
    #             node.SetValue(KratosMultiphysics.NODAL_VOLUME, nodal_volume)

    #     model_part.CloneTimeStep(1.0)

    #     distance_calculator = KratosMultiphysics.ParallelDistanceCalculator3D()
    #     distance_calculator.CalculateDistances(
    #         model_part,
    #         KratosMultiphysics.DISTANCE,
    #         KratosMultiphysics.NODAL_VOLUME,
    #         2,
    #         2.0,
    #         KratosMultiphysics.ParallelDistanceCalculator3D.CALCULATE_EXACT_DISTANCES_TO_PLANE)

    #     for node in model_part.Nodes:
    #         self.assertAlmostEqual(node.GetSolutionStepValue(KratosMultiphysics.DISTANCE), node.Y - free_surface_level, 10 )




if __name__ == '__main__':
    KratosUnittest.main()