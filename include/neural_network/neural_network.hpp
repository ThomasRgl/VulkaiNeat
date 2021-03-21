#pragma once

#include <iostream>
#include <vector>

#include <memory>

class Layer
{
private :
    std::vector<double> m_neurons;
    std::vector<double> m_bias;
    std::vector<double> m_weights;

    void initWeigth(); // initialise les poids 
    void initBias();

public :

    Layer(int neurons, Layer* previous_layer = nullptr);

    Layer(Layer const& other);
    Layer(Layer&& other);

    Layer& operator=(Layer const& other);
    Layer& operator=(Layer&& other);

    double const& operator[](unsigned int index) const {
        return m_neurons[index];
    }

    double& operator[](unsigned int index) {
        return m_neurons[index];
    }


    void init();
    void compute(Layer const& previous);
    void compute(std::vector<double> const& inputs);
    void mutate(); //mute un nn 

    std::vector<double>& weights() {
        return m_weights;
    }

    std::vector<double>& bias() {
        return m_bias;
    }

    size_t size() const {
        return m_neurons.size();
    }

};


//
struct NeuralParameters
{
    unsigned int nhiddenlayer;
    unsigned int ninput;
    unsigned int nhidden;
    unsigned int noutput;
};


//
class NeuralNetwork
{
private :


    std::vector<Layer> layers;

    size_t size;
        
    double score;
    double fitness;
    
public :


    NeuralNetwork(NeuralParameters const& params);
    NeuralNetwork(NeuralNetwork const& other);

    NeuralNetwork& operator=(NeuralNetwork const& other);
    NeuralNetwork& operator=(NeuralNetwork&& other);

    Layer& operator[](size_t index);
    Layer const& operator[](size_t index) const;

    void init();

    size_t compute(std::vector<double> const& inputs); //lance le calcul du nn 
    size_t output() const; //va chercher le résultat du calcul

    void score(double score);// set le score d'un nn 
    double score() const;

    void fitness(double fitness);// set le fitness d'un nn 
    double fitness() const;

    void crossover(NeuralNetwork const& first, NeuralNetwork const& second);
    void mutate();//mute un nn
};


//

class Game {
    public :

    virtual bool operator() (NeuralNetwork& nn) = 0;
};

//
class Population
{
private:
    std::vector<NeuralNetwork> *m_curr_population;
    std::vector<NeuralNetwork> *m_old_population;

    std::vector<NeuralNetwork> m_first_population;
    std::vector<NeuralNetwork> m_second_population;

    NeuralParameters m_params;
    NeuralNetwork *  pickOne();//choisi un element aléatoire de la population
    void calculateFitness(); //calcule la fitness de chaque element de la population
    void evolve(); //copulation de toute la popolation


public:
    Population(unsigned population_size, NeuralParameters const &params);

    Population(Population const& other) = delete;
    Population& operator=(Population const& other) = delete;

    void run(Game& game);

    NeuralNetwork& bestElement();
    NeuralNetwork const& bestElement() const;
    
    NeuralNetwork& operator[](size_t index);
    NeuralNetwork const& operator[](size_t index) const;
};

class NeuralPrinter {
    void printNetwork(NeuralNetwork const& nn);//affichage 
    void printPopulaton(Population const& pop);//print population
};


/*
#define TAILLE_POPULATION 1000
#define NB_INPUT 8
#define NB_HIDDEN_LAYER 1
#define NB_NEURONS_HIDDEN 8
#define NB_NEURONS_OUTPUT 4

#define MUTATION_RATE 0.3
#define SIGMA_MUTATION 0.15
#define CROSSOVER_PERCENT 30


prototype
    void initGlobalVar();
    
    double sigmoid(double x);
    
    double rand_gen();
    double normalRandom();
    
    unsigned long long result( NeuralNetwork * nn ); //va chercher le résultat du calcul
    void compute(NeuralNetwork * nn);//lance le calcul du nn 
    void setInput(NeuralNetwork * nn, double * inputList);// set input 
    void printNetwork(NeuralNetwork * nn);//affichage 
    void printPopulaton(NeuralNetwork ** population);//print population
    //void afficherData(NeuralNetwork * nn);
    void initWeigth(NeuralNetwork * nn);// initialise les poids 
    void initLayer(NeuralNetwork * nn, unsigned long long taille );//initialise les layers
    void initNeuralNetwork(NeuralNetwork * nn, unsigned long long nbInput, unsigned long long nbHiddenLayer, unsigned long long nbNeuronsHidden, unsigned long long nbNeuronsOutput );

    void copy(NeuralNetwork * NewNn,  NeuralNetwork * nn );// copy un nn 
    NeuralNetwork ** fuck(NeuralNetwork ** population);//copulation de toute la popolation
    NeuralNetwork *  pickOne(NeuralNetwork ** population);//choisi un element aléatoire de la population
    NeuralNetwork * crossover(NeuralNetwork * a, NeuralNetwork * b);//copulation de 2 élemets
    void calculateFitness(NeuralNetwork ** population);//calcule la fitness de chaque element de la population
    void mutate( NeuralNetwork * nn );//mute un nn 
    NeuralNetwork * bestElement(NeuralNetwork ** population);// trouve le meilleur element 
    void setScore(NeuralNetwork * nn, double score, double nbFruit);// set le score d'un nn 
    


    // FILE* openLog( char *fileName );
    // void writeLogScore ( FILE* fichier,  NeuralNetwork ** population );
    // void writeLogId ( FILE* fichier,  NeuralNetwork ** population );
    // void writeLogFruit( FILE* fichier,  NeuralNetwork ** population );
    // void closeLog( FILE* fichier);


*/