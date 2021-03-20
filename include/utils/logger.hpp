#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <memory>
#include "utils/util.h"

namespace logger
{
    struct LoggerConfig;

    // ==================================================================
    // ===                         Basic Logs                         ===
    // ==================================================================

    /**
     * @brief Classe de base pour tout type de log
     * 
     */
    class Log
    {
    public:
        /**
         * @brief Enumération de tout les niveaux de log
         * Les logs critiques entraineront la fin du programme
         * 
         */
        enum log_level
        {
            Trace,
            Debug,
            Info,
            Warn,
            Error,
            Fatal
        };

        /**
         * @brief Méthode permetant d'obtenir le niveau du log formatter
         * 
         * @return const std::string& 
         */
        static std::string const &levelToString(log_level level);

    protected:
        /**
         * @brief niveau du log
         * 
         */
        log_level m_level;

    private:
        /**
         * @brief Méthode abstraite permettant de récuperer le messages contenu dans le log
         * 
         * @return std::string 
         */
        virtual std::string messageImpl() const = 0;

    public:
        /**
         * @brief Constructeur par défaut du log
         * 
         * @param level 
         */
        Log(log_level level = Trace);

        /**
         * @brief Permet de définir le niveau du log
         * 
         * @param level 
         */
        void level(log_level level);

        /**
         * @brief Permet d'obtenir le niveau de l'objet
         * 
         * @return log_level 
         */
        log_level level() const;

        /**
         * @brief permet d'obtenir le message du log, comprend le timestamp et le préfix si nécessaire
         * 
         * @return std::string 
         */
        virtual std::string message(LoggerConfig const &config) const;
    };

    /**
     * @brief Log de base représentant un message sous forme de string
     * 
     */
    class StringLog : public Log
    {
    protected:
        std::string m_str;

        /**
         * @brief Méthode abstraite permettant de récuperer le messages contenu dans le log
         * 
         * @return std::string 
         */
        virtual std::string messageImpl() const override;

    public:
        StringLog(log_level level = Log::Trace);

        /**
         * @brief Constructeur de base a partir d'un message
         * 
         * @param message 
         */
        StringLog(std::string const &message, log_level level = Log::Trace);

        /**
         * @brief Constructeur de base a partir d'un message, utilise la sémantique de mouvement
         * 
         * @param message 
         */
        StringLog(std::string &&message);

        /**
         * @brief Constructeur de copie
         * 
         * @param other 
         */
        StringLog(StringLog const &other) = default;

        /**
         * @brief Construct de mouvement
         * 
         * @param other 
         */
        StringLog(StringLog &&other);

        /**
         * @brief Assignation de copie par défaut
         * 
         * @param other 
         * @return StringLog& 
         */
        StringLog &operator=(StringLog const &other) = default;

        /**
         * @brief Assignation de mouvement
         * 
         * @param other 
         * @return StringLog& 
         */
        StringLog &operator=(StringLog &&other);

        /**
         * @brief Définit le message
         * 
         * @param message 
         */
        void string(std::string const &message);

        /**
         * @brief Retourne le message
         * 
         * @return std::string const& 
         */
        std::string const &string() const;
    };

    // ==================================================================
    // ===                         Error Logs                         ===
    // ==================================================================

    /**
     * @brief Enumération de toutes les erreurs
     * 
     */
    enum error_code
    {
        // Common errors
        ERR_NONE = 0x0000,
        ERR_ALLOCATION_FAILURE,
        IERR_NVALID_PTR,
        ERR_OUT_OF_BOUND,

        // I/O Errors
        ERR_IO_ERROR = 0x1000,
        // To be filled ...

        // Parse Errors
        ERR_PARSE_ERROR = 0x2000,
        // To be filled ...

    };

    /**
     * @brief Log représentant une erreur du programme. Contient un code d'erreur, un message, et une description optionelle
     * 
     */
    class ErrorLog : public StringLog
    {
    protected:
        std::string m_description;

        int m_error_code;

        /**
         * @brief Méthode abstraite permettant de récuperer le messages contenu dans le log
         * 
         * @return std::string 
         */
        virtual std::string messageImpl() const override;

    public:
        /**
         * @brief Constructeur de base pour une erreurs, prenant une erreur, un code erreur, ainsi qu'une description optionelle
         * 
         * @param error 
         * @param code 
         * @param description 
         */
        ErrorLog(std::string const &error,
                 int code = error_code::ERR_NONE,
                 Log::log_level level = Log::Error,
                 std::string const &description = "");

        /**
         * @brief Construct de copie par défaut
         * 
         * @param other 
         */
        ErrorLog(ErrorLog const &other) = default;

        /**
         * @brief Constructeur de copie par mouvement
         * 
         * @param other 
         */
        ErrorLog(ErrorLog &&other);

        /**
         * @brief Assignation de copie par défaut
         * 
         * @param other 
         * @return ErrorLog& 
         */
        ErrorLog &operator=(ErrorLog const &other) = default;

        /**
         * @brief Assignation de mouvement
         * 
         * @param other 
         * @return ErrorLog& 
         */
        ErrorLog &operator=(ErrorLog &&other);

        /**
         * @brief Fonction surchargé pour pouvoir afficher correctement l'erreurs, y compris le code d'erreurs et la description
         * 
         * @param config 
         * @return std::string 
         */
        virtual std::string message(LoggerConfig const &config) const override;
    };

    // ==================================================================
    // ===                         Log handling                       ===
    // ==================================================================

    /**
     * @brief Structure permettant de stocker la configuration du Logger
     * 
     */
    struct LoggerConfig
    {

        /**
         * @brief Type de timestamp a utilisé
         * 
         */
        util::time::timestamp_t ts_type;

        /**
         * @brief Niveau minimum des logs a affiché
         * 
         */
        Log::log_level minimum_level;

        /**
         * @brief Vrai si les logs doivent etre écrit dans un fichier
         * 
         */
        bool log_to_file;

        /**
         * @brief Vrai si on doit utiliser de la couleurs lors du logging
         * 
         */
        bool use_color;

        /**
         * @brief Vrai si le processus de logging doit etre verbose
         * 
         */
        bool enabled;

        LoggerConfig(util::time::timestamp_t ts_type = util::time::timestamp_t::Partial,
                     Log::log_level minimum_level = Log::Trace,
                     bool log_to_file = false,
                     bool use_color = false,
                     bool enabled = true);
    };

    /**
     * @brief Classe abstraite recevant les logs, et les affichant correctement
     * 
     */
    class LogHandler
    {
    protected:
        /**
         * @brief Pointeur vers la config du logger associé
         * 
         */
        LoggerConfig *m_config;

    public:
        /**
         * @brief Constructeur du log handler, initialisant correctement la config
         * 
         * @param config 
         */
        LogHandler(LoggerConfig &config);

        /**
         * @brief Methode principale de logging
         * 
         * @param log 
         */
        virtual void log(Log const &log) = 0;

        /**
         * @brief Permet de changer la config utilisé
         * 
         * @param config 
         */
        void config(LoggerConfig& config);

        /**
         * @brief Permet de récuperer la config utilisé
         * 
         * @return LoggerConfig const& 
         */
        LoggerConfig const &config();
    };

    /**
     * @brief Spécialisation de LogHandler pour prendre en charge la sortie vers le terminal
     * 
     */
    class TerminalLogHandler : public LogHandler
    {
    private :

        /**
         * @brief Méthode utilitaire pour colorer les messages
         * 
         * @param level 
         * @return std::string 
         */
        std::string colorize(Log::log_level level);

    public:
        /**
         * @brief Constructeur d'un logger qui fera apparaitre les logs sur le terminal
         * 
         * @param config 
         */
        TerminalLogHandler(LoggerConfig& config);

        /**
         * @brief Methode principal de logs, utilisera les couleurs si possible
         * 
         * @param log 
         */
        virtual void log(Log const &log);
    };

    /**
     * @brief Spécialisation de LogHandler prenant en charge la sortie vers un fichier de logs
     * 
     */
    class FileLogHandler : public LogHandler
    {
    private:
        /**
         * @brief Flux de sortie vers le fichier
         * 
         */
        std::ofstream m_of;

        void open();

    public:
        /**
         * @brief Constructeur d'un logger qui fera apparaitre les logs dans un fichier
         * 
         * @param config 
         */
        FileLogHandler(LoggerConfig& config);

        /**
         * @brief Surchage de la méthode recevant les logs
         * 
         * @param log1pl 
         */
        virtual void log(Log const &log);
    };

    // ==================================================================
    // ===                         Logger                             ===
    // ==================================================================

    /**
     * @brief Singleton responsable de la bonne gestion des logs
     * 
     */
    class Logger
    {

    private:
        /**
         * @brief File contenant tout les loggers autre que celui de fichier
         * 
         */
        std::list<std::unique_ptr<LogHandler>> m_loggers;

        /**
         * @brief Logger pour les sorties fichiers
         * 
         */
        FileLogHandler m_file_logger;

        /**
         * @brief Configuration du logger
         * 
         */
        LoggerConfig m_config;

        /**
         * @brief Constructeur privé pour maintenir l'état de singleton
         * 
         */
        Logger();

        /**
         * @brief Suppression du constructeur de copie pour maintenir l'état de singleton
         * 
         */
        Logger(Logger const &) = delete;

        /**
         * @brief Suppression du l'assignation de copie pour maintenir l'état de singleton
         * 
         * @return Logger& 
         */
        Logger &operator=(Logger const &) = delete;

    public:
        /**
         * @brief retourne le singleton
         * 
         * @return Logger& 
         */
        static Logger &singleton();

        /**
         * @brief Fonction statique utilitaire pour enregistrer un log
         * 
         * @param log 
         * @return Logger& 
         */
        static Logger &log(Log const &log);

        /**
         * @brief Fonction statique utilitaire pour enregistrer un string
         * 
         * @param msg 
         * @param level 
         * @return Logger& 
         */
        static Logger &log(std::string const &msg, Log::log_level level = Log::Trace);

        /**
         * @brief Fonction permettant d'envoyer un logs au different LogHandlers
         * 
         * @param log 
         * @return Logger& 
         */
        Logger &operator()(Log const &log);

        /**
         * @brief Fonction permettant d'envoyer un message au different LogHandlers
         * 
         * @param msg 
         * @param level 
         * @return Logger& 
         */
        Logger &operator()(std::string const &msg, Log::log_level level = Log::Trace);

        /**
         * @brief Retourne la configuration actuelle du logger
         * 
         * @return LoggerConfig 
         */
        LoggerConfig config();
        
        /**
         * @brief Change la configuration du logger
         * 
         * @param val 
         */
        void config(LoggerConfig const& val);
    };

} // namespace logger