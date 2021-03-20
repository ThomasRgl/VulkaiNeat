#pragma once

#include <iostream>
#include <sstream>
#include <chrono>



namespace util {

    namespace time {

        static const inline std::chrono::high_resolution_clock::time_point program_start = std::chrono::high_resolution_clock::now();

        enum class timestamp_t {
            None,
            Delta,
            Partial,
            Full
        };

        std::string timestamp(timestamp_t tst);


        class Chrono {
            private :

            std::chrono::high_resolution_clock::time_point m_begin;
            std::chrono::duration<double> m_current_duration;
            bool m_paused;

            public :

            Chrono();

            Chrono& resume();
            Chrono& pause();
            Chrono& restart();

            std::chrono::duration<double> get();


            template<typename TUnit>
            TUnit getAs() {
                
                return std::chrono::duration_cast<TUnit>(get());
            }

            friend std::ostream& operator<<(std::ostream& os, Chrono& val);
            operator std::string();
        };
    }

}