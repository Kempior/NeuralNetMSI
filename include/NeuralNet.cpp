#include "NeuralNet.hpp"
#include <iostream>

NeuralNet::NeuralNet(unsigned int inputs, unsigned int outputs, unsigned int maximumLearningIterations, float newLearningRate)
		: learningRate(newLearningRate), inputsWithBiasSize(inputs + 1), outputSize(outputs), maxLearningIterations(maximumLearningIterations) {
	weights = std::vector<std::vector<float>>(inputsWithBiasSize, std::vector<float>(outputs));
	ResetInternals();
}

NeuralNet::~NeuralNet() {}


void NeuralNet::ResetInternals() {
	history.clear();
	history.push_back(weights);
	
	updatesWithoutChanges = 0;
	learningIteration = 0;
}

void NeuralNet::LoadLearningValues(const std::vector<std::vector<float>> &newLearningValues) {
	learningValues = newLearningValues;
	
	for (unsigned int i = 0; i < learningValues.size(); i++) {
		learningValues[i].push_back(-1);
	}
	
	ResetInternals();
}

void NeuralNet::LoadWeights (std::vector<std::vector<float>> newWeights) {
	weights = newWeights;
	ResetInternals();
}


void NeuralNet::LearningRate(float newLearningRate) {
	ResetInternals();
	
	learningRate = newLearningRate;
}


std::vector<std::vector<std::vector<float>>> NeuralNet::History() const {
	return history;
}

std::vector<std::vector<float>> NeuralNet::History(int elem) const {
	return history[elem];
}


std::vector<float> NeuralNet::PredictBiased(const std::vector<float> &input) {
	
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


void NeuralNet::LearnStep() {
	
	// Updating some variables used throughout the function
	updatesWithoutChanges++;
	
	// Sanity check. Will stop iterating after some time thanks to updatesWithoutChanges++ above
	if (learningValues.size() == 0)
		return;
	
	unsigned int currentValue = learningIteration % learningValues.size();
	
	// Generating the prediction
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
				weights[j][k] -= prediction[j] * learningRate * learningValues[currentValue][k];
			}
			updatesWithoutChanges = 0;
		}
	}
	
	// Also delet this
	std::cout << "\nIteration number: " << learningIteration << ", currentValue =  " << currentValue << ", updatesWithouthanges =  " << updatesWithoutChanges << std::endl << std::endl;
	// End of delet this
	
	// Pushing back current weights to history
	history.push_back(weights);
	
	// Also pretty obvious
	learningIteration++;
}

void NeuralNet::Learn() {
	// Main learning loop
	while (learningIteration < maxLearningIterations && updatesWithoutChanges < learningValues.size() + 1) {
		LearnStep();
	}
}