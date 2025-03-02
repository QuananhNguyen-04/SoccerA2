#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>

__global__ void initializeWeightsKernel(double* weights, int fan_in, int fan_out, unsigned long long seed) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < fan_in * fan_out) {
        curandState state;
        curand_init(seed, idx, 0, &state);
        weights[idx] = curand_normal(&state) * sqrt(2.0 / fan_in);
    }
}

class NeuralNetwork {
public:
    NeuralNetwork(const std::vector<int>& layers);
    ~NeuralNetwork();
    std::vector<std::vector<double>> forward(const std::vector<std::vector<double>>& inputs);
    void backward(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& gradients);
    void saveNetwork(const std::string& filename) const;
    void loadNetwork(const std::string& filename);

private:
    void initializeWeights();
    void initializeBiases();
    void initializeAdam();
    double sigmoid(double x) const;
    double sigmoidDerivative(double x) const;

    std::vector<int> layers;
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> biases;
    std::vector<std::vector<std::vector<double>>> m;
    std::vector<std::vector<std::vector<double>>> v;
    std::vector<std::vector<std::vector<double>>> activations;
    double beta1, beta2, epsilon, learning_rate;
    int t;
};

NeuralNetwork::NeuralNetwork(const std::vector<int>& layers)
    : beta1(0.9), beta2(0.999), epsilon(1e-10), learning_rate(0.001), t(0) {
    if (layers.size() < 2)
        throw std::invalid_argument("Network must have at least one input and one output layer.");

    this->layers = layers;
    initializeWeights();
    initializeBiases();
    initializeAdam();
}

NeuralNetwork::~NeuralNetwork() {
    // Free CUDA memory if allocated
}

void NeuralNetwork::initializeWeights() {
    weights.clear();
    for (size_t i = 0; i < layers.size() - 1; ++i) {
        int fan_in = layers[i];
        int fan_out = layers[i + 1];
        std::vector<std::vector<double>> layerWeights(fan_out, std::vector<double>(fan_in));

        double* d_weights;
        cudaMalloc(&d_weights, fan_in * fan_out * sizeof(double));
        initializeWeightsKernel<<<(fan_in * fan_out + 255) / 256, 256>>>(d_weights, fan_in, fan_out, time(0));
        cudaMemcpy(layerWeights.data(), d_weights, fan_in * fan_out * sizeof(double), cudaMemcpyDeviceToHost);
        cudaFree(d_weights);

        weights.push_back(layerWeights);
    }
}

void NeuralNetwork::initializeBiases() {
    biases.clear();
    for (size_t i = 1; i < layers.size(); ++i) {
        biases.push_back(std::vector<double>(layers[i], 0.0));
    }
}

void NeuralNetwork::initializeAdam() {
    m.clear();
    v.clear();
    for (size_t i = 0; i < weights.size(); ++i) {
        std::vector<std::vector<double>> layerM(layers[i + 1], std::vector<double>(layers[i], 0.0));
        std::vector<std::vector<double>> layerV(layers[i + 1], std::vector<double>(layers[i], 0.0));
        m.push_back(layerM);
        v.push_back(layerV);
    }
}

std::vector<std::vector<double>> NeuralNetwork::forward(const std::vector<std::vector<double>>& inputs) {
    // ...existing code...
}

void NeuralNetwork::backward(const std::vector<std::vector<double>>& inputs, const std::vector<std::vector<double>>& gradients) {
    // ...existing code...
}

void NeuralNetwork::saveNetwork(const std::string& filename) const {
    // ...existing code...
}

void NeuralNetwork::loadNetwork(const std::string& filename) {
    // ...existing code...
}

double NeuralNetwork::sigmoid(double x) const {
    return 1.0 / (1.0 + exp(-x));
}

double NeuralNetwork::sigmoidDerivative(double x) const {
    double sig = sigmoid(x);
    return sig * (1 - sig);
}

class Agent {
public:
    Agent(const std::vector<int>& layers, const std::vector<int>& vlayers, double gamma, double lambda);
    std::vector<int> takeAction(const std::vector<double>& input, bool on_policy);
    double computeAdvantage(const std::vector<double>& rewards, const std::vector<double>& values, const std::vector<double>& next_values, const std::vector<bool>& dones);
    void update(const std::vector<std::vector<double>>& states, const std::vector<double>& rewards, const std::vector<std::vector<double>>& next_states, const std::vector<bool>& dones);
    void saveWeights(const std::string& filename);
    void loadWeights(const std::string& filename);

private:
    NeuralNetwork policyNetwork;
    NeuralNetwork valueNetwork;
    double gamma, lambda;
    int time_step;
    double epsilon;
};

Agent::Agent(const std::vector<int>& layers, const std::vector<int>& vlayers, double gamma, double lambda)
    : policyNetwork(layers), valueNetwork(vlayers), gamma(gamma), lambda(lambda), time_step(0), epsilon(0.999) {
}

std::vector<int> Agent::takeAction(const std::vector<double>& input, bool on_policy) {
    // ...existing code...
}

double Agent::computeAdvantage(const std::vector<double>& rewards, const std::vector<double>& values, const std::vector<double>& next_values, const std::vector<bool>& dones) {
    // ...existing code...
}

void Agent::update(const std::vector<std::vector<double>>& states, const std::vector<double>& rewards, const std::vector<std::vector<double>>& next_states, const std::vector<bool>& dones) {
    // ...existing code...
}

void Agent::saveWeights(const std::string& filename) {
    policyNetwork.saveNetwork(filename + "_policy");
    valueNetwork.saveNetwork(filename + "_value");
}

void Agent::loadWeights(const std::string& filename) {
    policyNetwork.loadNetwork(filename + "_policy");
    valueNetwork.loadNetwork(filename + "_value");
}
