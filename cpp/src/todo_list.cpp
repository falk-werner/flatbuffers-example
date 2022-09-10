#include "todolist.hpp"

#include <getopt.h>

#include <cstdlib>
#include <string>
#include <iostream>

namespace
{

void print_usage()
{
    std::cout << R"(todo-list, (c) 2022 by Falk Werner <github.com/falk-werner>
list all todos in the list

Usage:
    todo-list -f <filename>  | -h

Options:
    -f, --filename      name of the todo file
    -h, --help          print this message

Examples:
    todo-list -f todo.bin
)";
}

struct context
{
    context(int argc, char * argv[])
    : show_help(false)
    , exit_code(EXIT_SUCCESS)
    {
        static option const long_opts[] =
        {
            {"help", no_argument, nullptr, 'h'},
            {"filename", required_argument, nullptr, 'f'},
            {nullptr, 0, nullptr, 0}
        };

        optind = 0;
        opterr = 0;

        bool finished = false;
        while (!finished)
        {
            int option_index = 0;
            int const c = getopt_long(argc, argv, "hf:", long_opts, &option_index);
            switch (c)
            {
                case -1:
                    finished = true;
                    break;
                case 'h':
                    show_help = true;
                    finished = true;
                    break;
                case 'f':
                    filename = optarg;
                    break;
                default:
                    std::cerr << "error: unknown option" << std::endl;
                    exit_code = EXIT_FAILURE;
                    finished = true;
                    break;
            }
        }

        if ((show_help == false) && (exit_code != EXIT_FAILURE) && (filename.empty()))
        {
            std::cerr << "error: missing filename" << std::endl;
            exit_code = EXIT_FAILURE;
        }
    }

    bool show_help;
    int exit_code;
    std::string filename;
};

}

int main(int argc, char * argv[])
{
    context ctx(argc, argv);

    if (!ctx.show_help)
    {
        try 
        {
            todolist::todolist todos(ctx.filename);
            for(auto const & todo: todos.all())
            {
                std::cout << todo.title << '\t' << todo.description << std::endl;
            }
        }
        catch (std::exception const & ex)
        {
            std::cout << "error: " << ex.what() << std::endl;
            ctx.exit_code = EXIT_FAILURE;
        }
        catch (...)
        {
            std::cout << "error: something went wrong" << std::endl;
            ctx.exit_code = EXIT_FAILURE;
        }
    }
    else
    {
        print_usage();
    }

    return ctx.exit_code;
}
