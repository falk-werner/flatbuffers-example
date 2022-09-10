#include "todolist.hpp"

#include <getopt.h>

#include <cstdlib>
#include <string>
#include <iostream>

namespace
{

void print_usage()
{
    std::cout << R"(todo-add, (c) 2022 by Falk Werner <github.com/falk-werner>
adds an todo to the list

Usage:
    todo-add -f <filename> -t <title> [-d <description>] | -h

Options:
    -f, --filename      name of the todo file
    -t, --title         title of the todo
    -d, --description   description of the todo (optional)
    -h, --help          print this message

Examples:
    todo-add -f todo.bin -t play -d 'have fun'
    todo-add -f todo.bin -t work
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
            {"title", required_argument, nullptr, 't'},
            {"description", required_argument, nullptr, 'd'},
            {nullptr, 0, nullptr, 0}
        };

        optind = 0;
        opterr = 0;

        bool finished = false;
        while (!finished)
        {
            int option_index = 0;
            int const c = getopt_long(argc, argv, "hf:t:d:", long_opts, &option_index);
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
                case 't':
                    title = optarg;
                    break;
                case 'd':
                    description = optarg;
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

        if ((show_help == false) && (exit_code != EXIT_FAILURE) && (title.empty()))
        {
            std::cerr << "error: missing title" << std::endl;
            exit_code = EXIT_FAILURE;
        }

    }

    bool show_help;
    int exit_code;
    std::string filename;
    std::string title;
    std::string description;
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
            todos.add(ctx.title, ctx.description);
            todos.save(ctx.filename);
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
