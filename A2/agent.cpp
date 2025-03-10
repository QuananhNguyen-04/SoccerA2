#include "agent.hpp"

// Constructor: Initializes layers and calls initialization functions
NeuralNetwork::NeuralNetwork(const std::vector<int> &layers, std::string activator, double lr)
    : beta1(0.9), beta2(0.999), epsilon(1e-6), learning_rate(lr), t(0), activator(activator)
{

    if (layers.size() < 2)
        throw std::invalid_argument("Network must have at least one input and one output layer.");

    this->layers = layers;
    initializeWeights();
    initializeBiases();
    initializeAdam();
}

double NeuralNetwork::activationFunction(double x) const
{
    if (activator == "sigmoid")
    {
        return sigmoid(x);
    }
    if (activator == "tanh")
    {
        return tanh(x);
    }
    if (activator == "leaky_relu")
    {
        return leakyRelu(x);
    }
}

double NeuralNetwork::activationDer(double x) const
{
    if (activator == "sigmoid")
    {
        return sigmoidDerivative(x);
    }
    if (activator == "tanh")
    {
        return tanhDerivative(x);
    }
    if (activator == "leaky_relu")
    {
        return leakyReluDerivative(x);
    }
}

void NeuralNetwork::initializeWeights()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    weights.clear();
    for (size_t i = 0; i < layers.size() - 1; ++i)
    {
        int fan_in = layers[i];      // Number of input neurons
        int fan_out = layers[i + 1]; // Number of output neurons
        // double std_dev = sqrt(2.0 / fan_in); // He initialization std deviation

        // std::normal_distribution<double> dist(0.0, std_dev); // Normal distribution
        // std::normal_distribution<double> dist(0.0, sqrt(2.0 / double(fan_in)));
        std::uniform_real_distribution<double> dist(-sqrt(6.0 / (fan_in + fan_out)), sqrt(6.0 / (fan_in + fan_out)));
        std::vector<std::vector<double>> layerWeights(fan_out, std::vector<double>(fan_in));
        for (int j = 0; j < fan_out; ++j)
        {
            for (int k = 0; k < fan_in; ++k)
            {
                layerWeights[j][k] = dist(gen); // Sample from normal distribution
            }
        }

        weights.push_back(layerWeights);
    }
}

void NeuralNetwork::initializeBiases()
{
    biases.clear();
    for (size_t i = 1; i < layers.size(); ++i)
    {
        biases.push_back(std::vector<double>(layers[i], 0.0));
    }
}

void NeuralNetwork::initializeAdam()
{
    m.clear();
    v.clear();
    for (size_t i = 0; i < weights.size(); ++i)
    {
        std::vector<std::vector<double>> layerM(layers[i + 1], std::vector<double>(layers[i], 0.0));
        std::vector<std::vector<double>> layerV(layers[i + 1], std::vector<double>(layers[i], 0.0));
        m.push_back(layerM);
        v.push_back(layerV);
    }
}

// Forward propagation for a batch of inputs
std::vector<std::vector<double>> NeuralNetwork::forward(const std::vector<std::vector<double>> &inputs)
{
    if (inputs.empty() || inputs[0].size() != static_cast<size_t>(layers[0]))
        throw std::invalid_argument("Input batch size is invalid.");

    size_t batch_size = inputs.size();
    std::vector<std::vector<std::vector<double>>> activations(batch_size, std::vector<std::vector<double>>(layers.size()));

    // Ensure activations are properly initialized
    assert(activations.size() == batch_size && "Mismatch in batch size for activations!");
    assert(activations[0].size() == layers.size() && "Mismatch in layer count for activations!");

    for (size_t i = 0; i < batch_size; ++i)
    {
        assert(inputs[i].size() == layers[0] && "Mismatch in input size and first layer neurons!");
        activations[i][0] = inputs[i]; // Assigning inputs to activations
    }

    for (size_t i = 0; i < weights.size(); ++i)
    {
        assert(weights[i].size() == layers[i + 1] && "Mismatch in weight rows and next layer size!");
        assert(biases[i].size() == layers[i + 1] && "Mismatch in biases and next layer size!");

        for (size_t b = 0; b < batch_size; ++b)
        {
            std::vector<double> z(layers[i + 1], 0.0);
            std::vector<double> a(layers[i + 1], 0.0);

            assert(activations[b][i].size() == layers[i] && "Mismatch in activations shape at layer!");

            for (int j = 0; j < layers[i + 1]; ++j)
            {
                assert(weights[i][j].size() == layers[i] && "Mismatch in weight columns and previous layer size!");

                for (int k = 0; k < layers[i]; ++k)
                {
                    assert(k < activations[b][i].size() && "Index out of bounds for activations!");
                    z[j] += weights[i][j][k] * activations[b][i][k];
                }
                z[j] = std::inner_product(weights[i][j].begin(), weights[i][j].end(), activations[b][i].begin(), biases[i][j]);

                a[j] = activationFunction(z[j]);
            }

            assert(a.size() == layers[i + 1] && "Mismatch in activations output size!");
            activations[b][i + 1] = a;
        }
    }
    std::vector<std::vector<double>> outputs(batch_size);
    for (size_t b = 0; b < batch_size; ++b)
    {
        outputs[b] = activations[b].back();
    }
    this->activations = activations;
    return outputs;
}

// Backpropagation for a batch of input-target pairs
void NeuralNetwork::backward(const std::vector<std::vector<double>> &inputs, const std::vector<std::vector<double>> &gradients)
{

    if (inputs.size() != gradients.size())
    {
        std::cout << "size mismatch\n";
        throw std::invalid_argument("Batch size mismatch batch_size between inputs and targets.");
    }

    size_t batch_size = inputs.size();
    std::vector<std::vector<double>> outputs = forward(inputs);
    std::vector<std::vector<std::vector<double>>> deltas(batch_size);

    // Proper initialization
    for (size_t b = 0; b < batch_size; ++b)
    {
        deltas[b].resize(layers.size()); // Resize to have space for each layer
        for (size_t i = 0; i < layers.size(); ++i)
        {
            deltas[b][i].resize(layers[i], 0.0); // Each layer should have the correct number of neurons
        }
    }
    // Assign last layer's deltas (gradients)
    for (size_t b = 0; b < batch_size; ++b)
    {
        assert(gradients[b].size() == layers.back() && "Gradient shape mismatch!");
        deltas[b].back() = gradients[b]; // Now safe because it's properly initialized
    }
    assert(deltas[0].size() == layers.size() && "Mismatch in deltas size!");
    assert(weights.size() == layers.size() - 1 && "Mismatch in weights size!");
    for (int i = static_cast<int>(weights.size()) - 1; i >= 0; --i)
    {
        for (size_t b = 0; b < batch_size; ++b)
        {
            assert(deltas[b][i + 1].size() == layers[i + 1] && "Mismatch in deltas shape!");
            assert(weights[i].size() == layers[i + 1] && "Mismatch in weight shape!");
            assert(weights[i][0].size() == layers[i] && "Mismatch in weight inner shape!");
            assert(activations[b][i + 1].size() == layers[i + 1] && "Mismatch in activations shape!");

            std::vector<double> delta(layers[i], 0.0);

            assert(layers[i] == activations[b][i].size() && "Index out of bounds for activations!");
            for (int j = 0; j < layers[i]; ++j)
            {
                for (int k = 0; k < layers[i + 1]; ++k)
                {
                    delta[j] += deltas[b][i + 1][k] * weights[i][k][j];
                }
                delta[j] *= (activationDer(activations[b][i][j]) + 1e-9); // Fix: Ensure correct indexing
            }

            // assert(delta.size() == layers[i] && "Mismatch in computed delta size!");
            deltas[b][i] = delta;
        }
    }
    t++;
    auto beta1_t = 1 - pow(beta1, t);
    auto beta2_t = 1 - pow(beta2, t);
    for (size_t i = 0; i < weights.size(); ++i)
    {
        int alert1 = false;
        int alert2 = false;
        int count1 = 0;
        int count2 = 0, count3 = 0;
        for (int j = 0; j < layers[i + 1]; ++j)
        {
            for (int k = 0; k < layers[i]; ++k)
            {
                double grad = 0.0;
                for (size_t b = 0; b < batch_size; ++b)
                {
                    // Ensure activations, weights, and deltas have correct dimensions before multiplication
                    assert(weights[i].size() == layers[i + 1] && "Mismatch in weights first dimension!");
                    assert(weights[i][0].size() == layers[i] && "Mismatch in weights second dimension!");
                    assert(activations[b][i].size() == layers[i] && "Mismatch in activations shape!");
                    assert(deltas[b][i + 1].size() == layers[i + 1] && "Mismatch in deltas shape!");
                    grad += (activations[b][i][k] + 1e-9) * deltas[b][i + 1][j]; // Fix: Use activations[b][i][k] instead of inputs[b][k]
                }
                // if (abs(grad) > 1e6) {
                //     count2++;
                //     alert2 = true;
                // }
                grad /= batch_size;
                if (batch_size > 0)
                {
                    if ((fabs(grad) < 1e-12))
                    {
                        count1++;
                        alert1 = true;
                    }
                    // std::cout << grad << " ";
                }
                m[i][j][k] = beta1 * m[i][j][k] + (1 - beta1) * grad;
                v[i][j][k] = beta2 * v[i][j][k] + (1 - beta2) * grad * grad;
                // std::cout << "grad = " << grad << ", ";
                // std::cout << "m[" << i << "][" << j << "][" << k << "] = " << m[i][j][k] << ", ";
                // std::cout << "v[" << i << "][" << j << "][" << k << "] = " << v[i][j][k] << std::endl;
                double m_hat = m[i][j][k] / (beta1_t + 1e-8);
                double v_hat = v[i][j][k] / (beta2_t + 1e-8);

                weights[i][j][k] -= learning_rate * m_hat / (sqrt(v_hat) + epsilon);
                // assert(abs(weights[i][j][k]) > 1e-18);
            }
        }
        if (alert1)
            std::cerr << "Layers: " << i << " Zero gradient detected: " << count1 / float(layers[i + 1] * layers[i]) * 100.0 << std::endl;
        // if (alert2)
        //     std::cerr << "Layers: " << i << " Exploding gradient detected: " << count2 / float(layers[i + 1] * layers[i]) * 100.0 << std::endl;
    }
}
// Print all weights (for debugging)
void NeuralNetwork::printWeights() const
{
    for (const auto &layer : weights)
    {
        std::cout << "Layer " << (&layer - &weights[0]) << ":" << std::endl;
        for (const auto &neuron : layer)
        {
            for (double weight : neuron)
            {
                std::cout << weight << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
void NeuralNetwork::saveNetwork(const std::string &filename) const
{
    std::string outfilename = filename;
    if (outfilename.find('.') == std::string::npos)
    {
        outfilename += std::string(".bin"); // Default extension if none is provided
    }
    std::ofstream file(outfilename, std::ios::binary);
    size_t num_layers = weights.size();
    file.write(reinterpret_cast<const char *>(&num_layers), sizeof(size_t));

    for (size_t i = 0; i < num_layers; ++i)
    {
        size_t num_neurons = weights[i].size();
        file.write(reinterpret_cast<const char *>(&num_neurons), sizeof(size_t));
        for (const auto &neuron : weights[i])
        {
            size_t num_weights = neuron.size();
            file.write(reinterpret_cast<const char *>(&num_weights), sizeof(size_t));
            file.write(reinterpret_cast<const char *>(neuron.data()), num_weights * sizeof(double));
        }

        file.write(reinterpret_cast<const char *>(biases[i].data()), biases[i].size() * sizeof(double));
    }
}
void NeuralNetwork::loadNetwork(const std::string &filename)
{
    std::string outfilename = filename;
    if (outfilename.find('.') == std::string::npos)
    {
        outfilename += ".bin"; // Default extension if none is provided
    }
    // std::cout << "weights shape: " << weights.size() << " layers" << std::endl;
    // for (size_t i = 0; i < weights.size(); ++i)
    // {
    //     std::cout << "Layer " << i << ": " << weights[i].size() << " neurons, each with " << (weights[i].empty() ? 0 : weights[i][0].size()) << " weights" << std::endl;
    // }
    // std::cout << "load file: " << outfilename << std::endl;
    std::ifstream file(outfilename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file for loading network.");
    }

    size_t num_layers;
    file.read(reinterpret_cast<char *>(&num_layers), sizeof(size_t));

    // Assert if loaded network structure is incompatible
    assert(num_layers == weights.size() && "Mismatch in the number of layers!");

    for (size_t i = 0; i < num_layers; ++i)
    {
        size_t num_neurons;
        file.read(reinterpret_cast<char *>(&num_neurons), sizeof(size_t));

        // Assert neuron count in layer i
        assert(num_neurons == weights[i].size() && "Mismatch in neuron count per layer!");

        for (size_t j = 0; j < num_neurons; ++j)
        {
            size_t num_weights;
            file.read(reinterpret_cast<char *>(&num_weights), sizeof(size_t));

            // Assert the weight count per neuron
            assert(num_weights == weights[i][j].size() && "Mismatch in weight count per neuron!");

            file.read(reinterpret_cast<char *>(weights[i][j].data()), num_weights * sizeof(double));
        }

        // Assert bias count per layer
        assert(num_neurons == biases[i].size() && "Mismatch in bias count per layer!");

        file.read(reinterpret_cast<char *>(biases[i].data()), biases[i].size() * sizeof(double));
    }

    file.close();
}
double NeuralNetwork::leakyRelu(double x) const
{
    return (x > 0) ? x : 0.01 * x; // Small slope for negative inputs
}

double NeuralNetwork::leakyReluDerivative(double x) const
{
    return (x > 0) ? 1.0 : 0.01;
}
// ReLU activation function
double NeuralNetwork::relu(double x) const
{
    return std::max(0.0, x);
}

// Derivative of the ReLU function
double NeuralNetwork::reluDerivative(double x) const
{
    return x > 0.0 ? 1.0 : 0.0;
}

double NeuralNetwork::tanhDerivative(double x) const
{
    double tanh_val = tanh(x);
    return (1.0 - tanh_val * tanh_val);
}

// Sigmoid activation function
double NeuralNetwork::sigmoid(double x) const
{
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of the sigmoid function
double NeuralNetwork::sigmoidDerivative(double x) const
{
    double sig = sigmoid(x);
    return sig * (1 - sig);
}

Agent::Agent(const std::vector<int> &layers, const std::vector<int> &vlayers, double gamma, double lambda)
    : policyNetwork(layers, "tanh", 0.05), valueNetwork(vlayers, "tanh", 0.0001),
      gamma(config::GAMMA_RL), lambda(lambda), time_step(0), epsilon(0.99)
{
}

// 🎯 Selects an action based on the policy network output (stochastic policy)
std::vector<int> Agent::takeAction(const std::vector<double> &input, bool on_policy)
{
    std::vector<std::vector<double>> output = policyNetwork.forward({input});
    std::vector<double> probabilities = output[0];
    // Print probabilities for debugging
    // double sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
    // std::ofstream debug_file("debug.txt", std::ios_base::app);

    double max_prob = *std::max_element(probabilities.begin(), probabilities.end());
    double sum_exp = 0.0;
    for (double &p : probabilities)
    {
        p = exp(p - max_prob); // Subtract max_prob for numerical stability
        sum_exp += p;
    }
    std::cout << "Action prob: ";
    for (double &p : probabilities)
    {
        p /= (sum_exp + 1e-10);
        std::cout << p << " ";
    }
    std::cout << std::endl;
    // debug_file.close();
    // Sample action based on probability distribution
    double r = static_cast<double>(rand()) / RAND_MAX;
    if (r > epsilon or on_policy)
    {
        // for (size_t i = 0; i < probabilities.size(); ++i)
        // {
        //     std::cout << "Action " << i << ": " << probabilities[i] << std::endl;
        // }
        // std::cout << "choosing by policy\n";
        auto max_it = std::max_element(probabilities.begin(), probabilities.end());
        return {static_cast<int>(std::distance(probabilities.begin(), max_it)) % 2 + 2};
    }
    r = static_cast<double>(rand()) / RAND_MAX;
    if (r < 0.5)
    {
        // std::cout << "choosing by randomness\n";
        return {static_cast<int>(rand() % probabilities.size() % 2) + 2};
    }
    else
    {
        r = static_cast<double>(rand()) / RAND_MAX;
        double cumulative = 0.0;
        std::vector<double> reversed_probabilities(probabilities.size());

        // Step 1: Compute reversed probabilities
        double max_prob = *std::max_element(probabilities.begin(), probabilities.end());
        for (size_t i = 0; i < probabilities.size(); ++i)
        {
            reversed_probabilities[i] = max_prob - probabilities[i]; // Flip probability importance
        }

        // Step 2: Normalize to sum to 1
        double sum_reversed = std::accumulate(reversed_probabilities.begin(), reversed_probabilities.end(), 0.0);
        for (double &p : reversed_probabilities)
        {
            p /= (sum_reversed + 1e-10); // Normalize (avoid division by zero)
        }

        // Step 3: Sample action using the reversed probabilities
        for (size_t i = 0; i < reversed_probabilities.size(); ++i)
        {
            cumulative += reversed_probabilities[i];
            if (r <= cumulative)
            {
                return {static_cast<int>(i) % 2 + 2}; // Return the chosen action index
            }
        }

        return {static_cast<int>(reversed_probabilities.size() - 1)}; // Fallback action for floating point error
    }
}

// 🎯 Computes Advantage using Generalized Advantage Estimation (GAE)
std::vector<double> Agent::computeAdvantage(const std::vector<double> &rewards,
                                            const std::vector<double> &values,
                                            const std::vector<double> &next_values,
                                            const std::vector<bool> &dones)
{
    std::vector<double> advantages(rewards.size(), 0.0);
    double gae = 0.0;

    for (int t = rewards.size() - 1; t >= 0; --t)
    {
        // double delta = rewards[t] + (dones[t] ? 0.0 : gamma * next_values[t]) - values[t];
        // gae = delta + gamma * lambda * (dones[t] ? 0.0 : gae);
        // advantages[t] = gae; // Store per-time-step advantage
        double next_advantage = (t == rewards.size() - 1) ? 0.0 : advantages[t + 1];
        double delta = rewards[t] + (dones[t] ? 0.0 : gamma * next_values[t]) - values[t];
        advantages[t] = delta + gamma * lambda * (dones[t] ? 0.0 : next_advantage);
    }

    // // Normalize advantage for stability (zero mean, unit variance)
    double mean = std::accumulate(advantages.begin(), advantages.end(), 0.0) / advantages.size();
    double variance = 0.0;
    for (double adv : advantages)
        variance += (adv - mean) * (adv - mean);
    variance = std::sqrt(variance / advantages.size() + 1e-8); // Prevent division by zero

    for (double &adv : advantages)
        adv = (adv - mean) / variance; // Normalize

    return advantages;
}
bool Agent::compare(Agent other, const std::vector<double> &input)
{
    if (this->takeAction(input, true) != other.takeAction(input, true))
        return false;
    if (this->takeValue(input) != other.takeValue(input))
        return false;
    return true;
}

void Agent::saveWeights(std::string filename)
{
    // std::ofstream file(filename, std::ios::binary);
    // if (!file.is_open())
    // {
    //     throw std::runtime_error("Unable to open file for saving weights.");
    // }

    // Save both policy and value networks
    policyNetwork.saveNetwork(filename + "_policy");
    valueNetwork.saveNetwork(filename + "_value");

    // file.close();
}

void Agent::loadWeights(std::string filename)
{
    policyNetwork.loadNetwork(filename + "_policy");
    valueNetwork.loadNetwork(filename + "_value");
}

std::vector<double> Agent::takeValue(const std::vector<double> &input)
{
    std::vector<std::vector<double>> value = valueNetwork.forward({input});
    return {value[0][0]};
}

// 🎯 Updates both policy and value networks
void Agent::update(const std::vector<std::vector<double>> &states,
                   const std::vector<double> &rewards,
                   const std::vector<std::vector<double>> &next_states,
                   const std::vector<bool> &dones,
                   const std::vector<int> &actions)
{

    size_t batch_size = states.size();
    time_step++;
    std::cout << time_step << std::endl;
    if (time_step > 400 && time_step % 50 == 0) // Start decay after 100 steps
    {
        epsilon = std::max((time_step < 1000 ? 0.5 : 0.1), epsilon * 0.9);
        lambda = std::min(0.8, lambda * 1.2); // Increase gamma by 1.4 times, but cap it at 0.95

        // gamma = std::min(0.95, gamma * 1.4); // Increase gamma by 1.4 times, but cap it at 0.95
        gamma = std::min(0.9, gamma + (0.9 - gamma) * 0.4);
    }
    // Compute value predictions
    std::vector<std::vector<double>> values_batch = valueNetwork.forward(states);
    std::vector<std::vector<double>> next_values_batch = valueNetwork.forward(next_states);

    std::vector<double> values(batch_size), next_values(batch_size);
    for (size_t i = 0; i < batch_size; ++i)
    {
        values[i] = values_batch[i][0];
        next_values[i] = next_values_batch[i][0];
    }
    auto advantages = computeAdvantage(rewards, values, next_values, dones);
    printf("advantage of GAE: %lf\n", advantages.back());
    // Compute Policy Loss (Negative Advantage * log probability)
    std::vector<std::vector<double>> policy_outputs = policyNetwork.forward(states);

    std::vector<std::vector<double>> policy_gradients(batch_size, std::vector<double>(policy_outputs[0].size(), 0.0));

    double gradient, entropy;
    for (size_t i = 0; i < batch_size; ++i)
    {
        entropy = 0.0;
        int action = actions[i];
        std::vector<double> softmax_probs = policy_outputs[i]; // Copy original outputs
        double sum_exp = 0.0;
        double max_prob = *std::max_element(softmax_probs.begin(), softmax_probs.end());

        for (double &p : softmax_probs)
        {
            p = exp(p - max_prob);
            sum_exp += p;
        }

        for (double &p : softmax_probs)
        {
            p /= (sum_exp + 1e-10); // Prevent division by zero
            entropy += -p * log(p + 1e-10);
        }

        // Compute gradient
        // std::cout << "gradient in batch: " << i << " " << gradient << " ";
        if (time_step < 1000 or true)
        {
            for (size_t j = 0; j < softmax_probs.size(); ++j)
            {
                if (j != action)
                {
                    gradient = -advantages[i] * log(softmax_probs[j] + 1e-8);
                    policy_gradients[i][j] = -gradient / 10.0;
                }
                else
                {
                    gradient = -advantages[i] * log(softmax_probs[action] + 1e-8);
                    policy_gradients[i][action] = gradient - (time_step < 2000 ? 0.01 : 0.001) * entropy;
                }
            }
        }
        // else
        // {
        //     gradient = -advantages[i] * log(softmax_probs[action] + 1e-8);
        //     policy_gradients[i][action] = 2 * gradient - (time_step < 2000 ? 0.001 : 0) * entropy;
        // }
    }
    // std::cout << std::endl;
    // Compute Value Loss (Mean Squared Error)
    double target;
    std::cout << "gamma: " << gamma << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    std::vector<std::vector<double>> value_gradients(batch_size, std::vector<double>(1, 0.0));
    for (size_t i = 0; i < batch_size; ++i)
    {
        target = rewards[i] + gamma * next_values[i] * (1 - dones[i]);
        value_gradients[i][0] = 2.0 * (values[i] - target) * (gamma < 0.4 ? 1 : 0.099);
    }

    // Print policy gradients for debugging
    std::cout << "Policy Gradients:" << std::endl;
    for (const auto &grad : policy_gradients)
    {
        for (double g : grad)
        {
            std::cout << g << " ";
        }
        std::cout << std::endl;
        break;
    }
    // Print value gradients for debugging
    std::cout << "Value Gradients:" << std::endl;
    for (const auto &grad : value_gradients)
    {
        for (double g : grad)
        {
            // std::cout << "target: " << rewards[0] + std::max(std::min(gamma * next_values[0] * (1 - dones[0]), 0.5), -0.5) << " ";
            std::cout << g << " ";
        }
        std::cout << std::endl;
        break;
    }
    // Backpropagate and update both networks
    // std::cout << "policy and value\n";
    // policyNetwork.printWeights();
    // std::cout << "\n\n";
    // valueNetwork.printWeights();
    // std::cout << "\n\n";
    // std::cout << "policy backward\n";
    policyNetwork.backward(states, policy_gradients);
    // std::cout << "valuation backward\n";
    // valueNetwork.backward(states, value_gradients);
    if (time_step < 1000)
    {
        valueNetwork.backward(states, value_gradients);
    }
}