#include "utils/logger.hpp"
#include <unordered_map>

#include <sstream>

namespace logger
{

    // ==================================================================
    // ===                         Basic Logs                         ===
    // ==================================================================

    std::string const &Log::levelToString(log_level level)
    {
        static std::unordered_map<log_level, std::string> map = {{Trace, "[Trace]"},
                                                                 {Debug, "[Debug]"},
                                                                 {Info, "[Info]"},
                                                                 {Warn, "[Warn]"},
                                                                 {Error, "[Error]"},
                                                                 {Fatal, "[Fatal]"}};

        return map[level];
    }

    /**
         * @brief Constructeur par défaut du log
         * 
         * @param level 
         */
    Log::Log(log_level level) : m_level(level) {}

    /**
         * @brief Permet de définir le niveau du log
         * 
         * @param level 
         */
    void Log::level(log_level level)
    {
        m_level = level;
    }

    /**
         * @brief Permet d'obtenir le niveau de l'objet
         * 
         * @return log_level 
         */
    Log::log_level Log::level() const
    {
        return m_level;
    }

    /**
         * @brief permet d'obtenir le message du log, comprend le timestamp et le préfix si nécessaire
         * 
         * @return std::string 
         */
    std::string Log::message(LoggerConfig const &config) const
    {
        std::stringstream tmp;

        if (config.ts_type != util::time::timestamp_t::None)
            tmp << util::time::timestamp(config.ts_type) << " ";

        tmp << levelToString(m_level) << " " << messageImpl();

        return tmp.str();
    }

    /**
         * @brief Méthode abstraite permettant de récuperer le messages contenu dans le log
         * 
         * @return std::string 
         */
    std::string StringLog::messageImpl() const
    {
        return m_str;
    }

    StringLog::StringLog(log_level level) : Log(level){};

    /**
         * @brief Constructeur de base a partir d'un message
         * 
         * @param message 
         */
    StringLog::StringLog(std::string const &message, log_level level) : Log(level), m_str(message) {}

    /**
         * @brief Constructeur de base a partir d'un message, utilise la sémantique de mouvement
         * 
         * @param message 
         */
    StringLog::StringLog(std::string &&message) : m_str(std::move(message)) {}

    /**
         * @brief Construct de mouvement
         * 
         * @param other 
         */
    StringLog::StringLog(StringLog &&other) : m_str(std::move(other.m_str)) {}

    /**
         * @brief Assignation de mouvement
         * 
         * @param other 
         * @return StringLog& 
         */
    StringLog &StringLog::operator=(StringLog &&other)
    {
        m_str = std::move(other.m_str);
        return *this;
    }

    /**
         * @brief Définit le message
         * 
         * @param message 
         */
    void StringLog::string(std::string const &message)
    {
        m_str = message;
    }

    /**
         * @brief Retourne le message
         * 
         * @return std::string const& 
         */
    std::string const &StringLog::string() const
    {
        return m_str;
    }

    // ==================================================================
    // ===                         Error Logs                         ===
    // ==================================================================

    std::string ErrorLog::messageImpl() const
    {

        std::stringstream tmp;

        tmp << m_str;

        if (m_description != "")
            tmp << "\n\t" << m_description;

        return tmp.str();
    }

    /**
         * @brief Constructeur de base pour une erreurs, prenant une erreur, un code erreur, ainsi qu'une description optionelle
         * 
         * @param error 
         * @param code
         * @param description 
         */
    ErrorLog::ErrorLog(std::string const &error,
                       int code,
                       log_level level,
                       std::string const &description) : StringLog(error, level), m_error_code(code), m_description(description)
    {
    }

    /**
         * @brief Constructeur de copie par mouvement
         * 
         * @param other 
         */
    ErrorLog::ErrorLog(ErrorLog &&other)
    {
        *this = std::move(other);
    }

    /**
         * @brief Assignation de mouvement
         * 
         * @param other 
         * @return ErrorLog& 
         */
    ErrorLog &ErrorLog::operator=(ErrorLog &&other)
    {
        m_str = std::move(other.m_str);
        m_error_code = other.m_error_code;
        m_description = std::move(other.m_description);
        return *this;
    }

    /**
         * @brief Fonction surchargé pour pouvoir afficher correctement l'erreurs, y compris le code d'erreurs et la description
         * 
         * @param config 
         * @return std::string 
         */
    std::string ErrorLog::message(LoggerConfig const &config) const
    {
        std::stringstream tmp;

        if (config.ts_type != util::time::timestamp_t::None)
            tmp << util::time::timestamp(config.ts_type) << " ";

        tmp << levelToString(m_level)
            << "[0x" << std::hex << (int)m_error_code << "] "
            << messageImpl();
        return tmp.str();
    }

    // ==================================================================
    // ===                         Log handling                       ===
    // ==================================================================

    LoggerConfig::LoggerConfig(util::time::timestamp_t ts_type,
                               Log::log_level minimum_level,
                               bool log_to_file,
                               bool use_color,
                               bool enabled)
        : ts_type(ts_type), minimum_level(minimum_level),
          log_to_file(log_to_file), use_color(use_color),
          enabled(enabled) {}

    /**
         * @brief Constructeur du log handler, initialisant correctement la config
         * 
         * @param config 
         */
    LogHandler::LogHandler(LoggerConfig &config)
    {
        m_config = &config;
    }

    /**
         * @brief Permet de changer la config utilisé
         * 
         * @param config 
         */
    void LogHandler::config(LoggerConfig &config)
    {
        m_config = &config;
    }

    /**
         * @brief Permet de récuperer la config utilisé
         * 
         * @return LoggerConfig const& 
         */
    LoggerConfig const &LogHandler::config()
    {
        return *m_config;
    }

    /**
         * @brief Méthode utilitaire pour colorer les messages
         * 
         * @param level 
         * @return std::string 
         */
    std::string TerminalLogHandler::colorize(Log::log_level level)
    {
        static std::unordered_map<Log::log_level, std::string> res = {{Log::Trace, "\e[39;90m"},
                                                                      {Log::Debug, "\e[39;36m"},
                                                                      {Log::Info, "\e[39;34m"},
                                                                      {Log::Warn, "\e[39;33m"},
                                                                      {Log::Error, "\e[39;31m"},
                                                                      {Log::Fatal, "\e[39;35m"}};

        return res[level];
    }

    /**
         * @brief Constructeur d'un logger qui fera apparaitre les logs sur le terminal
         * 
         * @param config 
         */
    TerminalLogHandler::TerminalLogHandler(LoggerConfig &config) : LogHandler(config)
    {
    }

    /**
         * @brief Methode principal de logs, utilisera les couleurs si possible
         * 
         * @param log 
         */
    void TerminalLogHandler::log(Log const &log)
    {
        if (m_config->use_color)
            std::cout << colorize(log.level());

        std::cout << log.message(*m_config) << std::endl;

        if (m_config->use_color)
            std::cout << "\e[0m";
    }

    void FileLogHandler::open()
    {
        m_of.open("logs" + util::time::timestamp(util::time::timestamp_t::Full) + ".txt");

        if (not m_of.good())
        {
            m_config->log_to_file = false;
            Logger::log(ErrorLog("Failed to open file for logging",
                                 error_code::ERR_IO_ERROR,
                                 Log::Error,
                                 "Failed to open file \"logs.txt\". The program may continue, but file logging will be disabled"));
        }
    }

    /**
         * @brief Constructeur d'un logger qui fera apparaitre les logs dans un fichier
         * 
         * @param config 
         */
    FileLogHandler::FileLogHandler(LoggerConfig &config) : LogHandler(config)
    {
    }

    /**
         * @brief Surchage de la méthode recevant les logs
         * 
         * @param log1pl 
         */
    void FileLogHandler::log(Log const &log)
    {
        if (not m_of.good())
            open();

        m_of << log.message(*m_config) << std::endl;
    }

    /**
         * @brief Constructeur privé pour maintenir l'état de singleton
         * 
         */
    Logger::Logger() : m_file_logger(m_config)
    {
        m_loggers.push_back(std::make_unique<TerminalLogHandler>(m_config));
    }

    /**
         * @brief retourne le singleton
         * 
         * @return Logger& 
         */
    Logger &Logger::singleton()
    {
        static Logger singleton;

        return singleton;
    }

    /**
         * @brief Fonction statique utilitaire pour enregistrer un log
         * 
         * @param log 
         * @return Logger& 
         */
    Logger &Logger::log(Log const &log)
    {
        auto &res = singleton();

        if (!res.config().enabled)
            return res;

        res(log);
        return res;
    }

    /**
         * @brief Fonction statique utilitaire pour enregistrer un string
         * 
         * @param msg 
         * @param level 
         * @return Logger& 
         */
    Logger &Logger::log(std::string const &msg, Log::log_level level)
    {
        auto &res = singleton();

        if (!res.config().enabled)
            return res;

        res(msg, level);
        return res;
    }

    /**
         * @brief Fonction permettant d'envoyer un logs au different LogHandlers
         * 
         * @param log 
         * @return Logger& 
         */
    Logger &Logger::operator()(Log const &log)
    {
        if (!m_config.enabled)
            return *this;

        if (log.level() < m_config.minimum_level)
            return *this;

        for (auto &i : m_loggers)
            i->log(log);

        if (m_config.log_to_file)
            m_file_logger.log(log);

        if (log.level() == Log::Fatal)
        {

            std::cout << "\n\nLe programme a rencontré une erreur fatale, et doit se fermer.\n";
            exit(1);
        }

        return *this;
    }

    /**
         * @brief Fonction permettant d'envoyer un message au different LogHandlers
         * 
         * @param msg 
         * @param level 
         * @return Logger& 
         */
    Logger &Logger::operator()(std::string const &msg, Log::log_level level)
    {
        if (!m_config.enabled)
            return *this;

        StringLog tmp(msg, level);
        (*this)(tmp);

        return *this;
    }

    /**
         * @brief Retourne la configuration actuelle du logger
         * 
         * @return LoggerConfig 
         */
    LoggerConfig Logger::config()
    {
        return m_config;
    }

    /**
         * @brief Change la configuration du logger
         * 
         * @param val 
         */
    void Logger::config(LoggerConfig const &val)
    {
        m_config = val;
    }

} // namespace logger