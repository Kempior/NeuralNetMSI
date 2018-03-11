#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <vector>

class NeuralNet
{
	std::vector<std::vector<float>> *weights;
public:
	NeuralNet(unsigned int inputs, unsigned int outputs);
	~NeuralNet();
	
	void LoadWeights (std::vector<std::vector<float>> newWeights);
	std::vector<std::vector<float>> Weights();
	
	const unsigned int inputsWithBiasSize;
	const unsigned int outputSize;
};

#endif // NEURALNET_HPP
