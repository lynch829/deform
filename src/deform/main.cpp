#include "config_file.h"
#include "registration/registration_engine.h"

#include <assert.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


class ArgParser
{
public:
    ArgParser(int argc, char** argv)
    {
        _executable = argv[0];

        std::vector<std::string> tokens;
        for (int i = 1; i < argc; ++i)
            tokens.push_back(argv[i]);

        while (!tokens.empty())
        {
            const std::string& token = tokens.back();
            
            if (token[0] == '-')
            {
                int b = 1;
                if (token[1] == '-')
                {
                    b = 2;
                }

                std::string line = token.substr(b);
                size_t p = line.find('=');
                if (p != std::string::npos)
                {
                    std::string key = line.substr(0, p);
                    std::string value = line.substr(p + 1);
                    _values[key] = value;
                }
                else
                {
                    _values[line] = "";
                }
            }
            else
            {
                _tokens.push_back(token);
            }
            tokens.pop_back();
        }
    }

    bool is_set(const std::string& key) const
    {
        return _values.find(key) != _values.end();
    }
    const std::string& value(const std::string& key) const
    {
        assert(is_set(key));
        return _values.at(key);
    }
    const std::map<std::string, std::string>& values() const
    {
        return _values;
    }

    const std::string& token(int i) const
    {
        return _tokens[i];
    }
    int num_tokens() const
    {
        return (int)_tokens.size();
    }
    const std::string& executable() const
    {
        return _executable;
    }

private:
    std::string _executable;
    std::map<std::string, std::string> _values;
    std::vector<std::string> _tokens;

};



void print_help()
{
    std::cout   << "Arguments:" << std::endl
                << "-p=<filename> : parameter file (obligatory)" << std::endl
                << "-f<i>=<filename> : Filename for the i:th fixed image" << std::endl
                << "-m<i>=<filename> : Filename for the i:th moving image" << std::endl
                << "-h, --help : Show this help section" << std::endl;

}

int main(int argc, char* argv[])
{
    ArgParser args(argc, argv);

    if (args.is_set("help") || args.is_set("h"))
    {
        print_help();
        return 1;
    }

    std::string param_file;
    if (args.is_set("p"))
    {
        param_file = args.value("p");
    }
    else
    {
        print_help();
        return 1;
    }

    ConfigFile cfg(param_file);
    RegistrationEngine engine;

    std::string fi, mi, fixed_file, moving_file;
    for (int i = 0; ; ++i)
    {
        std::stringstream ss;
        ss << "-f" << i;
        fi = ss.str();

        ss.str("");
        ss << "-m" << i;
        mi = ss.str();

        if (args.is_set(fi) && args.is_set(mi))
        {
            // engine.set_fixed_image(i, file);
            // engine.set_moving_image(i, file);
        }
    }



    // if (!engine.initialize(cfg))
    // {
    //     return 1;
    // }


    // engine.shutdown();

    return 0;
}