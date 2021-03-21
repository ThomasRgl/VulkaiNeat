#include "neural_network/neural_network.hpp"


namespace nn
{   

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                        LAYER                                           /////
    //////////////////////////////////////////////////////////////////////////////////////////////////

    Layer::Layer(int neurons, Layer* previous_layer = nullptr){
        m_neurons = [neurons];

        if(previous_layer != nullptr){
            m_bias = [neurons];
            m_weights = [neurons * previousLayerSize];
            initWeigth(previous_layer.size());
        }
            
    }


    void Layer::initWeigth( int previousLayerSize ){
        for (unsigned int i = 0; i < size() ; i++) {
            m_bias[i] = random(-1,1) ;
            for (unsigned int j = 0; j < previousLayerSize; j++ )
                m_weights[i * previousLayerSize + j] = random(0,1);
        }
    }


    void Layer::compute( Layer const& previous ){
        double s = 0.0;
        for (unsigned int i = 0; i < size() ; i++){
            s = m_bias[i];
            for(unsigned int j = 0; j < previous.size(); j++){
                s += m_weights[i * previous.size() + j] * previous[j] // stocker ou appeller à chaque fois ?
            }
            m_neurons = sigmoid(s);
        }
    }


    void Layer::compute( std::vector<double> const& inputs ){

        for (unsigned int i = 0; i < size() ; i++){
            m_neurons = sigmoid(inputs[i]);
            }
            
        }
    }


    void Layer::mutate(){
        for (unsigned int i = 0; i < m_bias.size() ; i++) 
            if( MUTATION_RATE > (double) rand()/RAND_MAX){ m_bias[i] += normalRandom()*0.05;}
            
        for (unsigned int j = 0; j < m_weights.size(); j++ )
                if( MUTATION_RATE > (double) rand()/RAND_MAX){ m_weights[j] += normalRandom()*0.05;}
        
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                   NeuralNetwork                                        /////
    //////////////////////////////////////////////////////////////////////////////////////////////////

    NeuralNetwork::NeuralNetwork(NeuralParameters const& params){
        
        
        size = 2 + nhiddenlayer;
        nhiddenlayer = [size];


        nhiddenlayer[0] = Layer(params.ninput); //input
        
        for(int i = 0; i < params.nhiddenlayer; i++){
            nhiddenlayer[i+1] = Layer(params.nhidden);
        }

        nhiddenlayer[ params.nhiddenlayer+1 ] = Layer(params.noutput, nhiddenlayer[ params.nhiddenlayer ]); //input

        score = -1;
        fitness = -1;

    }

    size_t compute(std::vector<double> const& inputs){
        nhiddenlayer[i+1].compute( inputs );

        for(int i = 1; i < size; i++){
            nhiddenlayer[i+1].compute();
        }
        return output();
    } 

    size_t output() const;{

    }

    void crossover(NeuralNetwork const& first, NeuralNetwork const& second);
    void mutate();//mute un nn
}