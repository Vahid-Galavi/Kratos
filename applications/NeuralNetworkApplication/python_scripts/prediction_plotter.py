from tensorflow.python.ops.variables import Variable
import KratosMultiphysics as KM
from KratosMultiphysics.NeuralNetworkApplication.neural_network_process import NeuralNetworkProcess
from KratosMultiphysics.NeuralNetworkApplication.data_loading_utilities import ImportDataFromFile
import numpy as np
import matplotlib.pyplot as plt



def Factory(settings):
    if not isinstance(settings, KM.Parameters):
        raise Exception("expected input shall be a parameters object, encapsulating a json string")
    return PredictionPlotterProcess(settings["parameters"])

class PredictionPlotterProcess(NeuralNetworkProcess):

    def __init__(self,parameters):

        super().__init__()


        default_settings = KM.Parameters("""{
            "predictions_file"     : "",
            "input_file"          : "",
            "target_file"         : "",
            "input_variable"      : "",
            "input_variable_id"   : 0,
            "variables"           : [],
            "axis"                : "plot",
            "output_name"         : "",
            "output_format"       : "png"          
        }""")
        parameters.ValidateAndAssignDefaults(default_settings)

        self.predictions_file = parameters["predictions_file"].GetString()
        self.input_file = parameters["input_file"].GetString()
        self.target_file = parameters["target_file"].GetString()
        self.input_variable = parameters["input_variable"].GetString()
        self.input_variable_id = parameters["input_variable_id"].GetInt()
        self.variables = parameters["variables"].GetStringArray()
        self.output_format = parameters["output_format"].GetString()
        self.output_name = parameters["output_name"].GetString()
        self.axis = parameters["axis"].GetString()

    def Plot(self):

        input = ImportDataFromFile(self.input_file, "InputData")
        target = ImportDataFromFile(self.target_file, "OutputData")
        if self.predictions_file.endswith('.npy'):
            predictions = np.load(self.predictions_file)
        else:
            predictions = np.genfromtxt(self.predictions_file)

        for variable in self.variables:
            figure, ax = plt.subplots()
            getattr(ax,self.axis)(input[:,self.input_variable_id],target[:,self.variables.index(variable)],'.',label='Ground Truth')
            if isinstance(predictions[0],(list, tuple, np.ndarray)): 
                getattr(ax,self.axis)(input,predictions[:,self.variables.index(variable)],'.',label='Prediction')
            else:
                getattr(ax,self.axis)(input[:,self.input_variable_id],predictions[:],'.',label='Prediction')
            ax.set_xlabel(self.input_variable)
            ax.set_ylabel(variable)
            ax.legend()
            manager = plt.get_current_fig_manager()
            manager.resize(*manager.window.maxsize())
            figure.show()
            figure.savefig(self.output_name + "_" + variable + "." + self.output_format, bbox_inches='tight')

            

