#ifndef AGENT_HPP
#define AGENT_HPP

#include <vector>
#include <fstream>
#include "config.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include <random>

class NeuralNetwork
{
private:
    std::vector<int> layers;
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;
    std::vector<std::vector<std::vector<double>>> activations;
    std::vector<std::vector<double>> z_values;
    std::vector<std::vector<double>> deltas;
    std::vector<std::vector<std::vector<double>>> m;
    std::vector<std::vector<std::vector<double>>> v;
    double beta1;
    double beta2;
    double epsilon;
    double learning_rate;
    int t;
    std::string activator;
    double activationFunction(double x) const;
    double activationDer(double x) const;
    void initializeWeights();
    void initializeBiases();
    void initializeAdam();
    double sigmoid(double x) const;
    double sigmoidDerivative(double x) const;
    double relu(double x) const;
    double reluDerivative(double x) const;
    double tanhDerivative(double x) const;
    double leakyRelu(double x) const;
    double leakyReluDerivative(double x) const;

public:
    explicit NeuralNetwork(const std::vector<int> &layers, std::string activator);
    std::vector<std::vector<double>> forward(const std::vector<std::vector<double>> &input);
    void backward(const std::vector<std::vector<double>> &inputs, const std::vector<std::vector<double>> &gradients);
    void printWeights() const;
    void saveNetwork(const std::string &filename) const;
    void loadNetwork(const std::string &filename);
};

class Agent
{
private:
    double gamma;  // discount factor
    double lambda; // GAE parameter
    uint16_t time_step;
    double epsilon;

public:
    NeuralNetwork policyNetwork;
    NeuralNetwork valueNetwork;
    explicit Agent(const std::vector<int> &layers, const std::vector<int> &vlayers, double gamma = 0.99, double lambda = 0.95);
    std::vector<int> takeAction(const std::vector<double> &input, bool on_policy = false);
    void update(const std::vector<std::vector<double>> &states, const std::vector<double> &rewards, const std::vector<std::vector<double>> &next_states, const std::vector<bool> &dones, const std::vector<int> &actions);
    double computeAdvantage(const std::vector<double> &rewards, const std::vector<double> &values, const std::vector<double> &next_values, const std::vector<bool> &dones);
    bool compare(Agent other, const std::vector<double> &input);
    void saveWeights(std::string filename);
    void loadWeights(std::string filename);
    std::vector<double> takeValue(const std::vector<double> &input);
};

#endif