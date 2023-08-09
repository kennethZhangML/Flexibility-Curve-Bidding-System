//
// Created by Kenneth Zhang on 2023-08-09.
// MIT License 2023
//

#include "testGNN.h"
#include <torch/torch.h>
#include <torch/script.h>

struct GraphData {
    torch::Tensor x;          // Node features
    torch::Tensor edge_index; // Edges
    torch::Tensor y;          // Labels
};

GraphData load_data() {
    GraphData data;
    data.x = torch::tensor({
                                   {0.5, 10.0, 2.0},  // Bid 1
                                   {0.7, 8.0,  3.0},  // Bid 2
                                   {0.8, 9.0,  1.0},  // Bid 3
                                   {0.6, 7.0,  2.5},  // Bid 4
                                   {0.9, 11.0, 1.5}   // Bid 5
                           });
    data.edge_index = torch::tensor({
                                            {0, 1, 2, 3, 3, 4},
                                            {1, 2, 3, 4, 2, 2}
                                    }, torch::kInt64);

    data.y = torch::tensor({
                                   1,
                                   0,
                                   1,
                                   0,
                                   1
                           });
    return data;
}


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


int compute_graphs() {
    auto data = load_data();
    auto model = std::make_shared<GNN>(3, 16, 1);
    torch::optim::Adam optimizer(model->parameters(), 0.01);

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