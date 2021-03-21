#pragma once

#include <iostream>
#include <vector>

#include <memory>

namespace neuralnetwork
{

    //
    class Layer
    {
    private:
        std::vector<double> m_neurons;
        std::vector<double> m_bias;
        std::vector<double> m_weights;

        void initWeigth(int previousLayerSize); // initialise les poids
        //void initBias();

    public:
        Layer(int neurons, Layer *previous_layer = nullptr);

        Layer(Layer const &other);
        Layer(Layer&& other);

        Layer &operator=(Layer const &other);
        Layer &operator=(Layer &&other);

        double const &operator[](unsigned int index) const
        {
            return m_neurons[index];
        }

        double &operator[](unsigned int index)
        {
            return m_neurons[index];
        }

        //void init();
        void compute(Layer const &previous);
        void compute(std::vector<double> const &inputs);
        void mutate(double const mutation_rate); //mute un nn

        std::vector<double> &weights()
        {
            return m_weights;
        }

        std::vector<double> const &weights() const
        {
            return m_weights;
        }

        std::vector<double> &bias()
        {
            return m_bias;
        }

        std::vector<double> const &bias() const
        {
            return m_bias;
        }

        size_t size() const
        {
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
        double crossover_rate;
        double mutation_rate;
    };

    //
    class NeuralNetwork
    {
    private:
        std::vector<Layer> m_layers;

        size_t m_size;

        double m_score;
        double m_fitness;

    public:
        NeuralNetwork(NeuralParameters const &params);
        NeuralNetwork(NeuralNetwork const &other);
        NeuralNetwork(NeuralNetwork&& other);

        NeuralNetwork &operator=(NeuralNetwork const &other);
        NeuralNetwork &operator=(NeuralNetwork &&other);

        Layer &operator[](size_t index) {
            return m_layers[index];
        }

        Layer const &operator[](size_t index) const {
            return m_layers[index];
        }

        //void init();

        size_t compute(std::vector<double> const &inputs); //lance le calcul du nn
        size_t output() const;                             //va chercher le résultat du calcul

        void score(double score) {
            m_score = score;
        } // set le score d'un nn

        double score() const {
            return m_score;
        }

        void fitness(double fitness) {
            m_fitness = fitness;
        } // set le fitness d'un nn

        double fitness() const {
            return m_fitness;
        }

        void crossover(NeuralNetwork const &first, NeuralNetwork const &second, double const crossover_rate);
        void mutate(double const mutation_rate); //mute un nn
    };

    //

    class Game
    {
    public:
        virtual bool operator()(NeuralNetwork &nn) = 0;
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

        size_t m_size;

        NeuralNetwork& pickOne(); //choisi un element aléatoire de la population
        void calculateFitness();  //calcule la fitness de chaque element de la population
        void evolve();            //copulation de toute la popolation

    public:
        Population(unsigned population_size, NeuralParameters const &params);

        Population(Population const &other);
        Population(Population&& other);

        Population &operator=(Population const &other);
        Population &operator=(Population&& other);

        void run(Game &game);

        NeuralNetwork &bestElement();
        NeuralNetwork const &bestElement() const;

        NeuralNetwork &operator[](size_t index) {
            return m_curr_population->at(index);
        }

        NeuralNetwork const &operator[](size_t index) const {
            return m_curr_population->at(index);
        }
    };

    //
    class NeuralPrinter
    {
        void printNetwork(NeuralNetwork const &nn); //affichage
        void printPopulaton(Population const &pop); //print population
    };

} // namespace neuralnetwork
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