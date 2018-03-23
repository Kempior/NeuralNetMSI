#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <vector>

class NeuralNet
{
	std::vector<std::vector<float>> weights;
	std::vector<std::vector<std::vector<float>>> history;
	unsigned int learningIteration;
	unsigned int updatesWithoutChanges;
	
	void ResetInternals();
	
public:
	NeuralNet(unsigned int inputs, unsigned int outputs, unsigned int maximumLearningIterations = 100);
	~NeuralNet();
	
	std::vector<float> PredictBiased(std::vector<float> input);
	std::vector<float> Predict(std::vector<float> input);
	
	void LearnStep(std::vector<std::vector<float>> learningValues);
	void Learn(std::vector<std::vector<float>> learningValues);
	unsigned int LearningIteration() const;
	unsigned int UpdatesWithoutChanges() const;
	
	void LoadWeights (std::vector<std::vector<float>> newWeights);
	std::vector<std::vector<float>> Weights() const;
	
	std::vector<std::vector<std::vector<float>>> History() const;
	std::vector<std::vector<float>> History(int elem) const;
	
	const unsigned int inputsWithBiasSize;
	const unsigned int outputSize;
	
	unsigned int maxLearningIterations;
};

#endif // NEURALNET_HPP
