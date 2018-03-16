#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <vector>

class NeuralNet
{
	std::vector<std::vector<float>> *weights;
public:
	NeuralNet(unsigned int inputs, unsigned int outputs);
	~NeuralNet();
	
	std::vector<float> PredictBiased(std::vector<float> input);
	std::vector<float> Predict(std::vector<float> input);
	void Learn(std::vector<std::vector<float>> learningValues);
	
	void LoadWeights (std::vector<std::vector<float>> newWeights);
	std::vector<std::vector<float>> Weights();
	
	const unsigned int inputsWithBiasSize;
	const unsigned int outputSize;
};

#endif // NEURALNET_HPP
