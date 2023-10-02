#include "utils.h"
#include <sstream>

namespace Cylink::StringUtils
{
    /**
     * Tokenizes an input string based on a delimeter and returns a vector of tokens.
     * @param input Is the string to tokenize.
     * @param delim Is the token delimeter for the input string.
     * @return A vector of strings containing the parsed tokens
     */
    std::vector<std::string> tokenize(const std::string& input, char delim)
    {
        std::vector<std::string> result;
        if(input.size() > 0)
        {
            std::string token;
            std::stringstream tokenizer(input);

            while(getline(tokenizer, token, delim))
                result.push_back(token);
        }
        return result;
    }
}
