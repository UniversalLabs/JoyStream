#ifndef CATEGORY_LOGGER_MANAGER_HPP
#define CATEGORY_LOGGER_MANAGER_HPP

#include <QString>
#include <QFile>
#include <QLoggingCategory>
#include <QtGlobal> // QtMessageHandler
#include <QMutex>

class LoggerManager {

public:

    // Collects all resources for a single logging category
    struct Category {

        // Name
        const char * name;

        // File where messages are outputted
        QFile * file;

        // Logger for category
        QLoggingCategory * qCategory;

        // Whether standard handler should be used
        bool chainStandardHandler;

        // Whether message should also output message to stdout/stderr
        bool useStandardOutput;
    };

    // Map name of category to category
    // (we use pointer since QLoggingCategory member of Logger cannot be copied around)
    std::map<QString, LoggerManager::Category> loggers;

    // Defualt message handler
    QtMessageHandler defaultHandler;

    // Synchronizes all threads accessing public routines
    QMutex mutex;

    // Constructor
    LoggerManager();

    // Destructor
    ~LoggerManager();

    // Creates a new category
    QLoggingCategory * createLogger(const char * name, bool chainStandardHandler, bool useStandardOutput);

};

// Global logger manager used by all
extern LoggerManager global_log_manager;

#endif // CATEGORY_LOGGER_MANAGER_HPP