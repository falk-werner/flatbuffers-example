#include "todolist.hpp"

#include "todolist_generated.h"

#include <fstream>

using namespace fbs::todolist;

namespace todolist
{

todolist::todolist()
{
}

todolist::todolist(std::string const &filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (file.good())
    {
        file.seekg(0, file.end);
        int length = file.tellg();
        file.seekg(0, file.beg);

        std::vector<char> contents(length);
        file.read(contents.data(), length);

        auto root = GetToDoList((uint8_t*) contents.data());
        auto fbs_todos = root->todos();
        for (int i = 0; i < fbs_todos->size(); i++)
        {
            auto todo = fbs_todos->Get(i);
            todos.push_back({todo->title()->str(), todo->description()->str()});
        }
    }
}

void todolist::add(std::string const & title, std::string const & description)
{
    todos.push_back({title, description});
}

std::vector<todo> const & todolist::all() const
{
    return todos;
}

void todolist::save(std::string const & filename) const
{
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<ToDo>> fb_todos;
    for(auto const &todo: todos)
    {
        auto title = builder.CreateString(todo.title);
        auto description = builder.CreateString(todo.description);

        auto fb_todo = CreateToDo(builder, title, description);
        fb_todos.push_back(fb_todo);
    }

    auto fbs = CreateToDoList(builder, builder.CreateVector(fb_todos));
    builder.Finish(fbs);
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    file.write((char*)builder.GetBufferPointer(), builder.GetSize());
}

}