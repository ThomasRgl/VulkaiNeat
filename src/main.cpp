#include <iostream>

#include "utils/logger.hpp"
#include "neural_network/neural_network.hpp"
#include <ctime>


using namespace logger;
using namespace util;
using namespace neuralnetwork;

class testGame : public Game {
private :


public :
    bool operator()(NeuralNetwork& val) {

        std::vector<double> tableau;

        tableau.resize(12, 0.5);

        val.compute(tableau);

        val.score(10);

        return false;
    }

};


int main() {
    LoggerConfig config;
    config.log_to_file = false;
    config.use_color = true;
    config.ts_type = time::timestamp_t::Partial;

    Logger::singleton().config(config);
    Logger::log(StringLog("Message"));

    std::srand(std::time(nullptr));

    Layer test(10);

    NeuralParameters tmp;

    tmp.nhidden = 8;
    tmp.noutput = 4;
    tmp.ninput = 8;
    tmp.nhiddenlayer = 1;
    tmp.crossover_rate = 0.3;
    tmp.mutation_rate = 0.3;
    
    
    
    NeuralNetwork testNn(tmp);

    Population test_populace(1000, tmp);

    testGame mabite;

    for (int i = 0; i < 2000; i++)
    {
        test_populace.run(mabite);
        if (i % 10 == 0)
            std::cout << i << '\n';
    }

    return 0;
}
