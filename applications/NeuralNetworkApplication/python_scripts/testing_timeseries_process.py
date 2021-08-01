import KratosMultiphysics as KM
from KratosMultiphysics.NeuralNetworkApplication.neural_network_process import NeuralNetworkProcess
from KratosMultiphysics.NeuralNetworkApplication.data_loading_utilities import ImportDataFromFile
import numpy as np
import matplotlib.pyplot as plt
import keras.models


def Factory(settings):
    if not isinstance(settings, KM.Parameters):
        raise Exception("expected input shall be a parameters object, encapsulating a json string")
    return TestingTimeseriesProcess(settings["parameters"])

class TestingTimeseriesProcess(NeuralNetworkProcess):

    def __init__(self,parameters):

        super().__init__()


        default_settings = KM.Parameters("""{
            "model"               : "",
            "input_file"          : "",
            "target_file"         : "",
            "timesteps"           : 100,
            "lookback"            : 2,
            "echo"                : 0,
            "predictions_file"    : "",
            "save_format"         : "ascii"            
        }""")
        parameters.ValidateAndAssignDefaults(default_settings)

        self.model = parameters["model"].GetString()
        self.input_file = parameters["input_file"].GetString()
        self.target_file = parameters["target_file"].GetString()
        self.timesteps = parameters["timesteps"].GetInt()
        self.lookback = parameters["lookback"].GetInt()
        self.echo = parameters["echo"].GetInt()
        self.predictions_file = parameters["predictions_file"].GetString()
        self.save_format = parameters["save_format"].GetString()
        if  not (self.save_format == "ascii") and  not (self.save_format == "npy"):
            raise Exception("Saving format not supported. Supported formats are ascii and npy.")

    def ExecuteFinalize(self):

        print("Testing process started... \n")
        input = ImportDataFromFile(self.input_file, "InputData")
        target = ImportDataFromFile(self.target_file, "OutputData")
        # The compiling options of the model do not matter, as it will not be trained
        model = keras.models.load_model(self.model)
        model.compile()
        predictions = []
        initial_length = len(input[:])
        for i in range(self.timesteps):
            test = np.reshape(input[i],(1, input[i].shape[0],input[i].shape[1]))
            predictions.append(model.predict(test)[0])
            if i > initial_length - 2:
                new_timestep = []
                for j in reversed(range(self.lookback-1)):
                     new_timestep.append(input[i-j][self.lookback-1])
                new_timestep.append(predictions[-1].tolist())
                new_timestep = np.array(new_timestep)
                new_timestep = np.reshape(new_timestep, (1, new_timestep.shape[0],new_timestep.shape[1]))
                input = np.vstack([input,new_timestep])
                if self.echo > 0:
                    print('Predicted: ' + str(predictions[i]))
            else:
                if self.echo > 0:
                    print('Predicted: ' + str(predictions[i]))
                    print('Ground truth: ' + str(target[i]))
        
        if self.save_format == "ascii":
            self.predictions_file = self.predictions_file + '.csv'
            np.savetxt(self.predictions_file, predictions)
        
        if self.save_format == "npy":
            self.predictions_file = self.predictions_file + '.npy'
            np.save(self.predictions_file, predictions)
            

