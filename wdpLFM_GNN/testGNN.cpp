//
// Created by Kenneth Zhang on 2023-08-09.
// MIT License 2023
//

#include "testGNN.h"
#include <torch/torch.h>
#include <torch/script.h>

struct GNN : torch::nn::Module {
    torch::nn::Linear transform{nullptr}, aggregate{nullptr}, classify{nullptr};

    GNN(int input_features, int hidden_features, int output_features){
        transform = register_module("tranformation_module", torch::nn::Linear(input_features, hidden_features));
        aggregate = register_module("aggregation_module", torch::nn::Linear(hidden_features, hidden_features));
        classify = register_module("classification_module", torch::nn::Linear(hidden_features, output_features));
    }

    torch::Tensor forward(torch::Tensor x, torch::Tensor edge_index){
        x = torch::relu(transform(x));
        x = torch::relu(aggregate(x));
        x =classify(x);
        return torch::sigmoid(x);
    }
};


int main() {
    auto data = load_data(); // Load your graph data and labels
    auto model = std::make_shared<GNN>(/*input_features=*/3, /*hidden_features=*/16, /*output_features=*/1);
    torch::optim::Adam optimizer(model->parameters(), /*lr=*/0.01);

    for (int epoch = 0; epoch < 100; ++epoch) {
        optimizer.zero_grad();
        auto out = model->forward(data.x, data.edge_index);
        auto loss = torch::binary_cross_entropy(out, data.y);
        loss.backward();
        optimizer.step();
        std::cout << "Epoch " << epoch << " | Loss: " << loss.item<float>() << std::endl;
    }

    return 0;
}