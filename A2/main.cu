#include "agent.cu"

int main() {
    std::vector<int> policy_layers = {4, 128, 2};  // Example layer sizes
    std::vector<int> value_layers = {4, 128, 1};
    double gamma = 0.99;
    double lambda = 0.95;

    Agent agent(policy_layers, value_layers, gamma, lambda);

    // Example usage
    std::vector<std::vector<double>> states = {{1, 2, 3, 4}, {4, 3, 2, 1}};
    std::vector<double> rewards = {1, 0};
    std::vector<std::vector<double>> next_states = {{2, 3, 4, 5}, {5, 4, 3, 2}};
    std::vector<bool> dones = {false, true};

    agent.update(states, rewards, next_states, dones);
    agent.saveWeights("agent_weights");
    agent.loadWeights("agent_weights");

    return 0;
}
