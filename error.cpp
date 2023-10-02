#include "error.h"

using namespace std;
using namespace Cylink;

/* Definte the ErrorData structure */
struct Error::ErrorData
{
    string errCode;     /**< A code used to identify the specific error */
    string errFilter;   /**< Identifyer for the general processing area raising the error */
    string errFile;     /**< The source file where error was generated. Use __FILE__ */
    size_t errLine;     /**< The error line in the file. Use __LINE__ */
};

/**
 * Constructor creates and initializes an Error object.
 * @param code Is the error code.
 * @param filter Is a user supplied identifier for product module.
 * @param msg The error message.
 * @param file Is the soure file where error was raised eg __FILE__.
 * @param line Is the line in the source file eg __LINE__.
 */
Error::Error(const string& msg, const string& code, const string& filter, const string& file, size_t line)
:logic_error(msg)
{
    pData = new ErrorData;
    if(pData != nullptr)
    {
        pData->errLine = line;
        pData->errCode = code;
        pData->errFilter = filter;
        pData->errFile = file;
    }
}

/**
 * Copy constructor creates a new Error object based on the argument.
 * @param err Is the Error object used to initialize the newly created object.
 */
Error::Error(const Error& err)
:logic_error(err.what())
{
    pData = new ErrorData;
    if(pData != nullptr)
    {
        pData->errLine = err.pData->errLine;
        pData->errCode = err.pData->errCode;
        pData->errFilter = err.pData->errFilter;
        pData->errFile = err.pData->errFile;
    }
}

/**
 * Move constructor creates a new object that takes ownership of the internel resources of it's
 * argument. The parameter passed is left in the state of having been newly constructed.
 * @param err Is the rvalue reference providing resources for the current object.
 */
Error::Error(Error&& err)
:logic_error(err.what())
{
    pData = err.pData;
    err.pData = NULL;
}

/**
 * Release all allocated resources.
 */
Error::~Error()
{
    delete pData;
    pData = NULL;
}

/**
 * Assignment operator copies the values in argument object to current object 
 * making the two equivalent.
 * @param err Is object that is assigned to current object.
 * @return A reference to the current object.
 */
Error& Error::operator = (const Error& err)
{
    if(this != &err)
    {
        if(pData == nullptr)
            pData = new ErrorData;
        
        if(pData != nullptr && err.pData != nullptr)
        {
            pData->errLine = err.pData->errLine;
            pData->errCode = err.pData->errCode;
            pData->errFilter = err.pData->errFilter;
            pData->errFile = err.pData->errFile;
        }
    }
    return *this;
}

/**
 * Move assignment steals the internal data members from the object that's assigned to it.
 * @param err Is object that is assigned to current object.
 * @return A reference to the current object.
 */
Error& Error::operator = (Error&& err)
{
    if(this != &err)
    {
        if(pData != nullptr)
            delete pData;
        pData = err.pData;
        err.pData = nullptr;
    }
    return *this;
}

/**
 * Constant function to obtain the error code.
 * @return The error code
 */
string Error::getErrorCode() const
{
    return (pData != nullptr) ? pData->errCode : "";    
}

/**
 * Constant function to obtain the module.
 * @return The module generating the error.
 */
string Error::getErrorFilter() const
{
    return (pData != nullptr) ? pData->errFilter : "";
}

/**
 * Constant function to return the source.
 * @return The file originating the error.
 */
string Error::getSourceFile() const
{
    return (pData != nullptr) ? pData->errFile : "";
}

/**
 * Constant function to return the error line.
 * @return The line number
 */
size_t Error::getSourceLine() const
{
    return (pData != nullptr) ? pData->errLine : 0;
}

