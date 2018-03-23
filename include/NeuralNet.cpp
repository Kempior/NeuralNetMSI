#include "NeuralNet.hpp"
#include <iostream>

NeuralNet::NeuralNet(unsigned int inputs, unsigned int outputs, unsigned int maximumLearningIterations) : inputsWithBiasSize(inputs + 1), outputSize(outputs), maxLearningIterations(maximumLearningIterations) {
	ResetInternals();
}

NeuralNet::~NeuralNet() {}


void NeuralNet::ResetInternals() {
	weights = std::vector<std::vector<float>>(outputSize, std::vector<float>(inputsWithBiasSize));
	history.push_back(weights);
	
	updatesWithoutChanges = 0;
	learningIteration = 0;
}

void NeuralNet::LoadWeights (std::vector<std::vector<float>> newWeights) {
	ResetInternals();
	weights = std::vector<std::vector<float>>(newWeights);
}

std::vector<std::vector<float>> NeuralNet::Weights() const {
	return weights;
}


std::vector<std::vector<std::vector<float>>> NeuralNet::History() const {
	return history;
}

std::vector<std::vector<float>> NeuralNet::History(int elem) const {
	return history[elem];
}


std::vector<float> NeuralNet::PredictBiased(std::vector<float> input) {
	
	// Created the output layer vector
	std::vector<float> output(outputSize, 0);
	
	// Caching values for some reason
	int iSize = input.size();
	int oSize = weights[0].size();
	
	// Checking if input vector size is not WRONG
	if (input.size() != weights[0].size()) {
		return std::vector<float>();
	}
	
	// Calculating the output values
	for(int i = 0; i < iSize; i++) {
		for(int j = 0; j < oSize; j++) {
			output[j] += input[i] * weights[j][i];
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

unsigned int NeuralNet::LearningIteration() const {
	return learningIteration;
}

unsigned int NeuralNet::UpdatesWithoutChanges() const {
	return updatesWithoutChanges;
}


void NeuralNet::LearnStep(std::vector<std::vector<float>> learningValues) {
	
	updatesWithoutChanges++;
	unsigned int currentValue = learningIteration % learningValues.size();
	std::vector<float> prediction = PredictBiased(learningValues[currentValue]);
	
	// Just for the homework, delet this
	std::cout << "Processed point: " << learningValues[currentValue][0] << ", " << learningValues[currentValue][1] << ", (" << learningValues[currentValue][2] << ")" << std::endl;
	std::cout << "Weights:" << std::endl << weights[0][0] << ' ' << weights[0][1] << ' ' << weights[0][2] << std::endl;
	std::cout << weights[1][0] << ' ' << weights[1][1] << ' ' << weights[1][2] << std::endl;
	std::cout << weights[2][0] << ' ' << weights[2][1] << ' ' << weights[2][2] << std::endl << "Predicted output: ";
	// End of delet this
	
	for (unsigned int j = 0; j < prediction.size(); j++) {
		
		// Delet this too
		std::cout << prediction[j] << "\t";
		// End of delet this
		
		// Yes, i'm doing comparisons on floats. Shush. They were assigned manually so it should work.
		if ((j == currentValue && prediction[j] != 1) || 
			(j != currentValue && prediction[j] != -1)) {
			for (unsigned int k = 0; k < weights[0].size(); k++) {
				weights[j][k] -= learningValues[currentValue][k];
				updatesWithoutChanges = 0;
			}
		}
	}
	
	// Also delet this
	std::cout << "\nIteration number: " << learningIteration << ", currentValue =  " << currentValue << ", updatesWithouthanges =  " << updatesWithoutChanges << std::endl << std::endl;
	// End of delet this
	
	learningIteration++;
}

void NeuralNet::Learn(std::vector<std::vector<float>> learningValues) {
	
	// Adding the bias untis
	for (unsigned int i = 0; i < learningValues.size(); i++) {
		learningValues[i].push_back(-1);
	}
	
	// Main learning loop
	for (learningIteration = 0; learningIteration < maxLearningIterations && updatesWithoutChanges < learningValues.size() + 1;) {
		LearnStep(learningValues);
	}
}