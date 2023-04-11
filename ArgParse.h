#ifndef ARGPARSE_ARGPARSE_H
#define ARGPARSE_ARGPARSE_H

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>


struct Flag
{
public:
    std::string name;
    std::string nice_name;
    int number = 0;
    bool found = false;
    std::string description;

    std::vector<std::string> results;
};


class Parser
{
    std::string program_name;
    std::unordered_map<std::string, std::shared_ptr<Flag>> flags;
    std::vector<std::string> arguments;
public:
    Parser() = default;

    void AddFlag(std::string name, std::string nice_name="", int number=0, std::string description="")
    {
        std::shared_ptr<Flag> f = std::make_shared<Flag>();
        f->name = name;
        flags[name] = f;
        f->nice_name = nice_name;
        f->number = number;
        f->description = description;
    }

    void SetProgramName(std::string name)
    {
        program_name = name;
    }

protected:
    bool IsFlag(std::string a) const
    {
        return (flags.find(a) != flags.end());
    }

    std::shared_ptr<Flag> GetFlag(std::string flag) const
    {
        return flags.at(flag);
    }

public:

    void Parse(int argc, char* argv[], int start=1)
    {
        std::shared_ptr<Flag> current = nullptr;
        for (int i = start; i < argc; i++)
        {
            std::string a = argv[i];
            if (IsFlag(a))
            {
                current = GetFlag(a);
                current->found = true;
            }

            else if (current)
            {
                if (current->number == -1 || current->results.size() < current->number)
                {
                    current->results.push_back(a);
                }
                else
                {
                    arguments.push_back(a);
                }
            }

            else
            {
                arguments.push_back(a);
            }
        }
    }

    bool FlagFound(std::string flag) const
    {
        if (flags.find(flag) == flags.end())
        {
            return false;
        }

        return flags.at(flag)->found;
    }

    std::vector<std::string> FlagResults(std::string flag) const
    {
        if (flags.find(flag) == flags.end())
        {
            return {};
        }

        return flags.at(flag)->results;
    }

    std::string FlagResult(std::string flag) const
    {
        if (flags.find(flag) == flags.end())
        {
            return {};
        }

        return flags.at(flag)->results.at(0);
    }

    std::vector<std::string> Arguments() const
    {
        return arguments;

    }

protected:
    std::set<std::shared_ptr<Flag>> _UniqueFlagList() const
    {
        std::set<std::shared_ptr<Flag>> res;
        for (const auto& f : flags)
        {
            res.insert(f.second);
        }

        return res;
    }

public:

    std::string Doc() const
    {
        std::string doc;
        if (!program_name.empty())
            doc += "Documentation of " + program_name + "\n";

        for (const auto &flag : _UniqueFlagList())
        {
            doc += flag->name;
            if (!flag->nice_name.empty())
            {
                doc += " / " + flag->nice_name;
            }

            doc += " :\n";
            doc += flag->description;
        }

        return doc;

    }
};

#endif //ARGPARSE_ARGPARSE_H
