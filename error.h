 /**
 * @file error.h
 * File contains definition of main exception object.
 */
#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <stdexcept>

/**
 * @namespace Cylink
 * General project namespace
 */
namespace Cylink
{
    /**
     * @class Error
     * Class defines a throwable object that stores error information
     */
    class Error : public std::logic_error 
    {
        private:
            /**
             * @struct ErrorData
             * Structure of internal Error members.
             */
            struct ErrorData;
            
        public:
            Error(const std::string& msg, const std::string& code, const std::string& filter, 
                const std::string& file="", size_t line = 0);
            Error(const Error& err);
            Error(Error&& err);
            ~Error();

            Error& operator = (const Error& err);
            Error& operator = (Error&& err);

            std::string getErrorCode() const;
            std::string getErrorFilter() const;
            std::string getSourceFile() const;
            size_t getSourceLine() const;
            
        private:
            ErrorData* pData; /**< Oblique pointer to ErrorData */
    };
}

#endif
