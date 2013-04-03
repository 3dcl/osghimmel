
// Copyright (c) 2011-2012, Daniel M�ller <dm@g4t3.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright 
//     notice, this list of conditions and the following disclaimer in the 
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the 
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its 
//     contributors may be used to endorse or promote products derived from 
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#pragma once
#ifndef __LOG_H__
#define __LOG_H__

#include <QMutex>
#include <QDateTime>
#include <QString>
#include <QQueue>
#include <QSet>
#include <QVector>

class QFile;
class QTextStream;


class LogEntry
{
public:

    enum e_LogEntryType
    {
        UNDEFINED_LOG   = 1 << 0
    ,   MESSAGE_LOG     = 1 << 1
    ,   INFORMATION_LOG = 1 << 2
    ,   WARNING_LOG     = 1 << 3
    ,   ERROR_LOG       = 1 << 4
    ,   DEBUG_LOG       = 1 << 5
    ,   WORKFLOW_LOG    = 1 << 6
    ,   EMPTYLINE_LOG   = 1 << 7
    };


public:
    LogEntry(e_LogEntryType type, const QString &message);
    LogEntry(const LogEntry &entry);
    ~LogEntry();

    inline void setLine(const int line)
    {
        m_line = line;
    }


    inline void setFile(const QString &filename)
    {
        m_file = filename;
    }


    inline e_LogEntryType type() const
    {
        return m_type;
    }


    inline const QString message() const
    {
        return m_message;
    }


    inline const QDateTime timestamp() const
    {
        return m_timestamp;
    }


    inline const QString file() const
    {
        return m_file;
    }


    inline const int line() const
    {
        return m_line;
    }

protected:

    e_LogEntryType m_type;

    QString m_message;
    QDateTime m_timestamp;

    QString m_file;
    int m_line;
};




class LogOutput
{
public:
    virtual void print(const LogEntry &entry) = 0;
};




class StdLogOutput : public LogOutput
{
public:
    virtual void print(const LogEntry &entry);
};




class FileLogOutput : public LogOutput
{
public:
    // Initializes and tries to open the file named filename. CAUTION: will 
    // throw an exception if the file cannot be open in write mode.
    FileLogOutput(const QString &filename);
    virtual ~FileLogOutput();

protected:
    QFile *m_file;
    QTextStream *m_stream;

    bool m_opened;

protected:
    virtual void print(const LogEntry &entry);
};




class LogDispatcher
{
public:

    LogDispatcher();
    virtual ~LogDispatcher();

    void append(const LogEntry &entry);

protected:

    QSet<LogOutput*> m_outputs;

    QVector<LogEntry*> m_cachedEntries;
    bool m_cache;

protected:

    // As long caching is enabled (that should be as long as not all 
    // outputs have been attached).
    void cache(const LogEntry &entry);
    void flushCache(LogOutput *output);

    // Should be used by sub classing from LogDispatcher.
    void attachLogOutput(LogOutput *output);
    void detachLogOutput(LogOutput *output);

    // Use this after attaching all outputs and no caching is needed anymore.
    void stopCaching();
};




class Log
{
public:
    Log();
    ~Log();

    void attachDispatcher(LogDispatcher *dispatcher);
    LogDispatcher *dispatcher();

    void appendMessage(const char *message, ...);
    void appendMessage(const QString &message);

    void appendInformation(const char *message, ...);
    void appendInformation(const QString &message);

    void appendWorkflow(const char *message, ...);
    void appendWorkflow(const QString &message);

    void appendWarning(const char *message, ...);
    void appendWarning(const QString &message);

    void appendError(const char *message, ...);
    void appendError(const QString &message);

    void appendEmptyLine();

    // This is a little workaround to obtain a simple to use macro LOG_DEBUG
    // This is basically a split up into two functions: debug and append 
    // debug. One should always use the debug first and the appendDebug 
    // afterwards... The macro handles the convenient call of both functions.
    void debug(const char *file, const int line);
    
    // Any debug message - only visible in debug mode.
    void appendDebug(const char *message, ...);
    void appendDebug(const QString &message);

protected:

    // Stores debug information passed by debug(...).
    QString m_debug_file;
    int m_debug_line;

    LogDispatcher *m_dispatcher;

    // Stores entries that are appended before a valid dispatcher 
    // is assigned.
    QQueue<LogEntry*> m_earlyEntries;

protected:

    // Final log action - this creates the LogEntry.
    void append(LogEntry::e_LogEntryType type, const QString &message, const bool debug = false);

    // For thread safety overload this method in your derived class then, 
    // lock the your mutex in this method (done by ThreadSafeLog).
    virtual void lock(); 

    // For thread safety overload this method in your derived class then,
    // unlock your mutex in this method (done by ThreadSafeLog).
    virtual void unlock();
};




class ThreadSafeLog : public Log
{
protected:
    QMutex m_mutex;

protected:
    virtual void lock();
    virtual void unlock();
};




// Singleton that encapsulates a single log 
// instance and supports its global usage.
class GlobalLog : public ThreadSafeLog
{
public:
    static Log *instance();

protected:
    static Log *m_log;

private:
    GlobalLog();
    virtual ~GlobalLog();
};




#define _LOG GlobalLog::instance()

// Using the do..while trick, we can execute two statements in one macro 
// without having to worry about what happens when we to a conditional 
// LOG_DEBUG without enclosing braces.

// From http://cnicholson.net/2009/02/stupid-c-tricks-adventures-in-assert/ 
#ifndef NDEBUG
#define _LOG_DEBUG(...)    do { \
    _LOG->debug(__FILE__, __LINE__); \
    _LOG->appendDebug(__VA_ARGS__); \
} while(false)
#else
#define _LOG_DEBUG(...) do { } while(0)
#endif

#define _LOG_MESSAGE     _LOG->appendMessage
#define _LOG_INFO        _LOG->appendInformation
#define _LOG_WARNING     _LOG->appendWarning
#define _LOG_ERROR       _LOG->appendError

#define _LOG_EMPTYLINE   _LOG->appendEmptyLine


#endif // __LOG_H__