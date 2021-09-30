/*
  ==============================================================================

    NocturneDSP.h
    Created: 23 Sep 2021 6:32:55pm
    Author:  Stephane P. Pericat

  ==============================================================================
*/

#pragma once
#include <RTNeural/RTNeural.h>

using Vec2d = std::vector<std::vector<float>>;

struct NocturneDSP
{
    void reset()
    {
        model.reset();
    }
    
    Vec2d transpose(const Vec2d& x)
    {
        auto outer_size = x.size();
        auto inner_size = x[0].size();
        Vec2d y(inner_size, std::vector<float>(outer_size, 0.0f));

        for (size_t i = 0; i < outer_size; ++i)
        {
            for (size_t j = 0; j < inner_size; ++j)
                y[j][i] = x[i][j];
        }

        return y;
    }
    
    void load_binary(const char* binary)
    {
        nlohmann::json weights_json = nlohmann::json::parse(binary);
        
        load_model(weights_json);
    }

    void load_json(const char* filePath)
    {
        std::ifstream i2(filePath);
        nlohmann::json weights_json;
        i2 >> weights_json;
        
        load_model(weights_json);
    }
    
    void load_model(nlohmann::json& weights)
    {
        auto& lstm = model.get<0>();
        auto& dense = model.get<1>();

        Vec2d lstm_weights_ih = weights["/state_dict/rec.weight_ih_l0"_json_pointer];
        lstm.setWVals(transpose(lstm_weights_ih));

        Vec2d lstm_weights_hh = weights["/state_dict/rec.weight_hh_l0"_json_pointer];
        lstm.setUVals(transpose(lstm_weights_hh));

        std::vector<float> lstm_bias_ih = weights["/state_dict/rec.bias_ih_l0"_json_pointer];
        std::vector<float> lstm_bias_hh = weights["/state_dict/rec.bias_hh_l0"_json_pointer];
        for (int i = 0; i < 128; ++i)
            lstm_bias_hh[i] += lstm_bias_ih[i];
        lstm.setBVals(lstm_bias_hh);

        Vec2d dense_weights = weights["/state_dict/lin.weight"_json_pointer];
        dense.setWeights(dense_weights);

        std::vector<float> dense_bias = weights["/state_dict/lin.bias"_json_pointer];
        dense.setBias(dense_bias.data());
        
        model.reset();
    }

    void process(const float* inData, float* outData, int numSamples)
    {
        for (int i = 0; i < numSamples; ++i)
            outData[i] = model.forward(inData + i) + inData[i];
    }
    
    RTNeural::ModelT<float, 1, 1, RTNeural::LSTMLayerT<float, 1, 32>, RTNeural::DenseT<float, 32, 1>> model;
};
