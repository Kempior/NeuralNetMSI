#ifndef NEURALNET_HPP
#define NEURALNET_HPP

#include <vector>

class NeuralNet
{
	std::vector<std::vector<float>> learningValues;
	std::vector<std::vector<float>> weights;
	std::vector<std::vector<std::vector<float>>> history;
	unsigned int learningIteration;
	unsigned int updatesWithoutChanges;
	
	void ResetInternals();
	
	std::vector<float> PredictBiased(const std::vector<float> &input);
	
public:
	NeuralNet(unsigned int inputs, unsigned int outputs, unsigned int maximumLearningIterations = 100);
	~NeuralNet();
	
	std::vector<float> Predict(std::vector<float> input);
	
	void LearnStep();
	void Learn();
	unsigned int LearningIteration() const { return learningIteration; }
	unsigned int UpdatesWithoutChanges() const { return updatesWithoutChanges; }
	
	void LoadLearningValues(const std::vector<std::vector<float>> &newLearningValues);
	std::vector<std::vector<float>> LearningValues() const { return learningValues; }
	
	void LoadWeights (std::vector<std::vector<float>> newWeights);
	std::vector<std::vector<float>> Weights() const { return weights; }
	
	std::vector<std::vector<std::vector<float>>> History() const;
	std::vector<std::vector<float>> History(int elem) const;
	
	const unsigned int inputsWithBiasSize;
	const unsigned int outputSize;
	
	unsigned int maxLearningIterations;
};

#endif // NEURALNET_HPP
