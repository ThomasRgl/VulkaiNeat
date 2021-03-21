#include "neural_network/neural_network.hpp"

#include <cmath>

namespace neuralnetwork
{   

    //
    double sigmoid(double x) {
        return 1 / (1 + exp(-(x)) ); //default sigmoid
    }

    //
    double rand_gen() {
        // return a uniformly distributed random value
        return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
    }

    //
    double normalRandom() {
        // return a normally distributed random value
        double v1=rand_gen();
        double v2=rand_gen();
        return cos(2*3.14*v2)*sqrt(-2.*log(v1));
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                        LAYER                                           /////
    //////////////////////////////////////////////////////////////////////////////////////////////////

    Layer::Layer(int neurons, Layer* previous_layer){

        m_neurons.resize(neurons, 0);

        if(previous_layer != nullptr){
            m_bias.resize(neurons, 0);
            m_weights.resize(neurons * previous_layer->size(), 0);
            initWeigth(previous_layer->size());
        }
            
    }

    Layer::Layer(Layer const &other) {
        *this = other;
    }

    Layer::Layer(Layer&& other) {
        *this = std::move(other);
    }

    Layer &Layer::operator=(Layer const &other) {
        m_neurons = other.m_neurons;
        m_bias = other.m_bias;
        m_weights = other.m_weights;
        return *this;
    }

    Layer &Layer::operator=(Layer &&other) {
        m_neurons = std::move(other.m_neurons);
        m_bias = std::move(other.m_bias);
        m_weights = std::move(other.m_weights);
        return *this;
    }

    void Layer::initWeigth( int previousLayerSize ){
        for (size_t i = 0; i < size() ; i++) 
            m_bias[i] = ((double)rand() / (double)RAND_MAX) * 2 ;
        

        for (size_t i = 0; i < previousLayerSize * m_neurons.size(); i++ )
            m_weights[i]= (rand() / RAND_MAX)*2 ;
        
    }


    void Layer::compute( Layer const& previous ){
        double s = 0;
        
        size_t tmp = previous.size();

        for (size_t i = 0; i < size() ; i++){
            s = m_bias[i];
            for(size_t j = 0; j < tmp; j++){
                s += m_weights[i * tmp + j] * previous[j]; // stocker ou appeller à chaque fois ?
            }
            m_neurons[i] = sigmoid(s);
        }
    }


    void Layer::compute( std::vector<double> const& inputs ){

        for (size_t i = 0; i < size() ; i++) 
            m_neurons[i] = sigmoid(inputs[i]);
        
    }


    void Layer::mutate(double const mutation_rate) {
        for (auto& i : m_bias) 
            if( mutation_rate > (double) rand() / RAND_MAX) { i += normalRandom() * 0.05;}
            
        for (auto& i : m_weights)
            if( mutation_rate > (double) rand() / (double)RAND_MAX) { i += normalRandom() * 0.05;}
        
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                   NeuralNetwork                                        /////
    //////////////////////////////////////////////////////////////////////////////////////////////////

    NeuralNetwork::NeuralNetwork(NeuralParameters const& params) : m_score(-1), m_fitness(-1) {
        
        
        m_size = 2 + params.nhiddenlayer;
        m_layers.reserve(m_size);


        m_layers.emplace_back(params.ninput);
        
        for(int i = 0; i < params.nhiddenlayer; i++){
            m_layers.emplace_back(params.nhidden, &m_layers.back());
        }
        
        m_layers.emplace_back(params.noutput, &m_layers.back()); //input

    }

    NeuralNetwork::NeuralNetwork(NeuralNetwork const &other) {
        *this = other;
    }

    NeuralNetwork::NeuralNetwork(NeuralNetwork&& other) {
        *this = std::move(other);
    }

    NeuralNetwork &NeuralNetwork::operator=(NeuralNetwork const &other) {
        m_layers = other.m_layers;
        m_score = other.m_score;
        m_fitness = other.m_fitness;
        m_size = other.m_size;
        return *this;
    }

    NeuralNetwork &NeuralNetwork::operator=(NeuralNetwork &&other) {
        m_layers = std::move(other.m_layers);
        m_score = other.m_score;
        m_fitness = other.m_fitness;
        m_size = other.m_size;

        other.m_size = 0;
        other.m_score = 0;
        other.m_fitness = 0;
        return *this;
    }

    size_t NeuralNetwork::compute(std::vector<double> const& inputs){

        m_layers.front().compute(inputs);

        for(int i = 1; i < m_size; i++)
            m_layers[i].compute(m_layers[i - 1]);
        return output();
    } 

    size_t NeuralNetwork::output() const {

        
        auto& tmp = m_layers.back();
        
        double max = tmp[0];
        size_t index = 0;

        for (int i = 0; i < tmp.size(); i++) {

            if (tmp[i] > max)
            {
                max = tmp[i];
                index = i;
            }
        }
        return index;
    }

    void NeuralNetwork::crossover(NeuralNetwork const& first, NeuralNetwork const& second, double const crossover_rate) {
        //size tot de tout les w et bias 
        size_t tot = 0;
        for(int i = 1; i < m_size; i++)
            tot += m_layers[i].weights().size() + m_layers[i].bias().size();
        

        size_t cut = (tot * crossover_rate);
        size_t remains = tot - cut;
        
        int layer = 1;

        while (cut != 0)
        {

            auto& current_layer = m_layers[layer];
            auto& other_layer = first[layer];

            size_t nweights = current_layer.weights().size();
            size_t nbias = current_layer.bias().size();

            auto& current_weigths = current_layer.weights();
            auto& other_weigths = other_layer.weights();

            auto& current_bias = current_layer.weights();
            auto& other_bias = other_layer.weights();

            for (int i = 0; i < nweights && cut != 0; i++) {
                current_weigths[i] = other_weigths[i];
                cut--;
            }

            for (int i = 0; i < nbias && cut != 0; i++) {
                current_bias[i] = other_bias[i];
                cut--;
            }
            layer++;
        }

        layer = m_layers.size() - 1;

        while (remains != 0)
        {
            auto& current_layer = m_layers[layer];
            auto& other_layer = second[layer];

            size_t nweights = current_layer.weights().size();
            size_t nbias = current_layer.bias().size();

            auto& current_weigths = current_layer.weights();
            auto& other_weigths = other_layer.weights();

            auto& current_bias = current_layer.weights();
            auto& other_bias = other_layer.weights();

            for (int i = nbias - 1; i != 0 && remains != 0; i--) {
                current_bias[i] = other_bias[i];
                remains--;
            }

            for (int i = nweights - 1; i != 0 && remains != 0; i--) {
                current_weigths[i] = other_weigths[i];
                remains--;
            }

            layer--;
        }
    }

    void NeuralNetwork::mutate(double const mutation_rate) {
        for(int i = 1; i < m_size; i++)
            m_layers[i].mutate(mutation_rate);
    }



    //////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                     Population                                         /////
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    NeuralNetwork& Population::pickOne(){
        size_t index = 0;

        double r = (double) rand() / (double) RAND_MAX;
        auto& population = *m_curr_population;

        while ( r > 0) {
            r -= population[index].fitness();

            index += 1;
        }
        if (index == 0)
            index = 1;
            
        return population[index - 1];
    }


    void Population::calculateFitness(){
        double sum = 0;

        auto& population = *m_curr_population;

        for( int i = 0; i < m_size; i++)
            sum += population[i].score();

        for( int i = 0; i < m_size; i++)
            population[i].fitness(population[i].score()  / sum);
    }


    void Population::evolve(){
        for( int i = 0; i < m_size; i++){
            NeuralNetwork& tmp = (*m_old_population)[i];

            tmp.crossover(pickOne(), pickOne(), m_params.crossover_rate);
            tmp.mutate(m_params.mutation_rate);
            tmp.score(0);
        }
        std::swap(m_curr_population, m_old_population);
    }


    Population::Population(unsigned population_size, NeuralParameters const &params) : m_size(population_size), m_params(params) {
        NeuralNetwork buffer(params);

        m_first_population.resize(population_size, buffer);
        m_second_population.resize(population_size, buffer);

        m_curr_population = &m_first_population;
        m_old_population = &m_second_population;

    }

    Population::Population(Population const &other) {
        *this = other;
    }

    Population::Population(Population&& other) {
        *this = other;
    }

    Population &Population::operator=(Population const &other) {

        m_first_population = other.m_first_population;
        m_second_population = other.m_second_population;

        m_params = other.m_params;
        return *this;
    }

    Population &Population::operator=(Population&& other) {

        m_first_population = std::move(other.m_first_population);
        m_second_population = std::move(other.m_second_population);

        m_params = other.m_params;
        return *this;
    }

    void Population::run(Game &game){
        auto& population = *m_curr_population;

        for (auto& i : population) {
            game(i);
        }
        calculateFitness();
        evolve();
    }

    NeuralNetwork &Population::bestElement(){
        auto& population = *m_curr_population;

        NeuralNetwork* res = &population[0];
        size_t max = population[0].score();

        for (auto& i : population) {
            if (i.score() > max)
            {
                max = i.score();
                res = &i;
            }
        }
        return *res;
    }

    NeuralNetwork const &Population::bestElement() const{
        return bestElement();
    }
        


}