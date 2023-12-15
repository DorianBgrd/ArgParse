#ifndef ARGPARSE_ARGPARSE_H
#define ARGPARSE_ARGPARSE_H

#include <iostream>
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

    const std::string help = "-h";
    const std::string help_nice = "--help";

public:
    Parser()
    {
        AddFlag(help, help_nice, 0, "Show this help.");
    }

    /**
     * Adds a new flag for the parser.
     * @param name: flag short name.
     * @param nice_name: flag long name.
     * @param number: number of possible occurence, -1 meaning any number.
     * @param description: description for the flag.
     */
    void AddFlag(std::string name, std::string nice_name="", int number=0, std::string description="")
    {
        std::shared_ptr<Flag> f = std::make_shared<Flag>();
        f->name = name;
        flags[name] = f;

        f->nice_name = nice_name;
        if (!nice_name.empty())
        {
            flags[nice_name] = f;
        }

        f->number = number;
        f->description = description;
    }

    void SetProgramName(std::string name)
    {
        program_name = name;
    }

protected:
    /**
     * Returns whether argument is a flag.
     * @param a: argument.
     * @return is a flag.
     */
    bool IsFlag(std::string a) const
    {
        return (flags.find(a) != flags.end());
    }

    /**
     * Returns flag ptr for flag name.
     * @param flag: flag name.
     * @return flag ptr.
     */
    std::shared_ptr<Flag> GetFlag(std::string flag) const
    {
        return flags.at(flag);
    }

public:

    /**
     * Parses command line arguments.
     * @param argc: argument count.
     * @param argv: argument values.
     * @param start: start.
     */
    bool Parse(int argc, char* argv[], int start=1)
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

                current = nullptr;
            }

            else
            {
                arguments.push_back(a);
            }
        }

        if (HelpRequested())
        {
            ShowHelp();
            return false;
        }

        return true;
    }

    /**
     * Returns if flag was found at least one time
     * during parsing.
     * @param flag: flag name.
     * @return flag was found.
     */
    bool FlagFound(std::string flag) const
    {
        if (flags.find(flag) == flags.end())
        {
            return false;
        }

        return flags.at(flag)->found;
    }

    /**
     * Returns all values for specified flag.
     * @param flag: flag name.
     * @return vector of values.
     */
    std::vector<std::string> FlagResults(std::string flag) const
    {
        if (flags.find(flag) == flags.end())
        {
            return {};
        }

        return flags.at(flag)->results;
    }

    /**
     * Returns first value for specified flag.
     * @param flag: flag to query.
     * @return first value, empty string if none.
     */
    std::string FlagResult(std::string flag) const
    {
        if (flags.find(flag) == flags.end())
        {
            return {};
        }

        auto fres = flags.at(flag)->results;
        if (fres.empty())
        {
            return {};
        }

        return fres.at(0);
    }

    /**
     * Returns default arguments.
     * @return arguments.
     */
    std::vector<std::string> Arguments() const
    {
        return arguments;

    }

protected:
    /**
     * Returns unique flags ptr list.
     * @return set of flag ptr.
     */
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

    /**
     * Formats documentation for the parser flags.
     * @return doc.
     */
    std::string Doc() const
    {
        std::string doc;
        if (!program_name.empty())
            doc += "Documentation of " + program_name + "\n";

        for (const auto &flag : _UniqueFlagList())
        {
            doc += "\n";
            doc += flag->name;
            if (!flag->nice_name.empty())
            {
                doc += " / " + flag->nice_name;
            }

            doc += " :\n";
            doc += flag->description;
            doc += "\n";
        }

        return doc;
    }

    bool HelpRequested() const
    {
        return FlagFound(help);
    }

    void ShowHelp() const
    {
        std::cout << Doc();
    }
};

#endif //ARGPARSE_ARGPARSE_H
