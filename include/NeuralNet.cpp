#include "NeuralNet.hpp"

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
