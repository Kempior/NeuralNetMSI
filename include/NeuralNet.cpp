#include "NeuralNet.hpp"
#include <iostream>

NeuralNet::NeuralNet(unsigned int inputs, unsigned int outputs) : inputsWithBiasSize(inputs + 1), outputSize(outputs) {
	weights = new std::vector<std::vector<float>>(outputSize, std::vector<float>(inputsWithBiasSize));
}

NeuralNet::~NeuralNet() {
	delete weights;
}

void NeuralNet::LoadWeights (std::vector<std::vector<float>> newWeights) {
	delete weights;
	weights = new std::vector<std::vector<float>>(newWeights);
}

std::vector<std::vector<float>> NeuralNet::Weights() {
	return *weights;
}

std::vector<float> NeuralNet::PredictBiased(std::vector<float> input) {
	
	// Created the output layer vector
	std::vector<float> output(outputSize, 0);
	
	// Caching values for some reason
	int iSize = input.size();
	int oSize = weights[0].size();
	
	// Checking if input vector size is not WRONG
	if (input.size() != (*weights)[0].size()) {
		return std::vector<float>();
	}
	
	// Calculating the output values
	for(int i = 0; i < iSize; i++) {
		for(int j = 0; j < oSize; j++) {
			output[j] += input[i] * (*weights)[j][i];
		}
	}
	
	// Implementing the signum function
	for (int i = 0; i < iSize; i++) {
		output[i] = output[i] < 0 ? -1 : 1;
	}
	
	return output;
}

std::vector<float> NeuralNet::Predict(std::vector<float> input) {
	
	// Added the bias node
	input.push_back(-1);
	
	return PredictBiased(input);
}

void NeuralNet::Learn(std::vector<std::vector<float>> learningValues) {
	
	// Adding the bias untis
	for (unsigned int i = 0; i < learningValues.size(); i++) {
		learningValues[i].push_back(-1);
	}
	
	// Flag for the main learning loop
	unsigned int updatesWithoutChanges = 0;
	
	// Main learning loop
	for (unsigned int i = 0; i < 20 && updatesWithoutChanges < learningValues.size() + 1; i++) {
		updatesWithoutChanges++;
		
		unsigned int currentValue = i % learningValues.size();
		
		std::vector<float> prediction = PredictBiased(learningValues[currentValue]);
		
		for (unsigned int j = 0; j < prediction.size(); j++) {
			std::cout << prediction[j] << "  \t";
			
			// Yes, i'm doing comparisons on floats. Shush.
			if ((j == currentValue && prediction[j] != 1) || 
				(j != currentValue && prediction[j] != -1)) {
				for (unsigned int k = 0; k < weights[0].size(); k++) {
					(*weights)[j][k] -= learningValues[currentValue][k];
					updatesWithoutChanges = 0;
				}
			}
		}
		
		std::cout << std::endl << (*weights)[0][0] << ' ' << (*weights)[0][1] << ' ' << (*weights)[0][2] << std::endl;
		std::cout << (*weights)[1][0] << ' ' << (*weights)[1][1] << ' ' << (*weights)[1][2] << std::endl;
		std::cout << (*weights)[2][0] << ' ' << (*weights)[2][1] << ' ' << (*weights)[2][2] << std::endl;
		
		std::cout << "currentValue =  " << currentValue << ", updatesWithouthanges =  " << updatesWithoutChanges << std::endl << std::endl;
	}
}