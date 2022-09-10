#ifndef TODOLIST_HPP
#define TODOLIST_HPP

#include <vector>
#include <string>

namespace todolist
{

struct todo
{
    std::string const title;
    std::string const description;
};

class todolist
{
public:
    todolist();
    explicit todolist(std::string const &filename);
    ~todolist() = default;
    void add(std::string const & title, std::string const & description = "");
    std::vector<todo> const & all() const;
    void save(std::string const & filename) const;
private:
    std::vector<todo> todos;
};

}

#endif
