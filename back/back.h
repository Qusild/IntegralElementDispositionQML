#include <array>
#include <unordered_map>
#include <vector>


struct coordinates
{
public:
    coordinates(int x1, int y1)
        : x(x1), y(y1) {}
    int x, y;
    bool operator ==(coordinates& other)
    {
        return x == other.x && y == other.y;
    }
};

struct connection
{
public:
    int initial, final;
    std::string name;
    int id;
    // std::vector<coordinates> path;
};

class integral_element
{
public:
    integral_element(int x, int y)
        : coords({x, y}){};

    coordinates coords;
    std::string name;
    int id;
    std::vector<connection> connections;
};

class Schema
{
public:
    Schema(int x, int y)
        : dimentions_x(x), dimentions_y(y){};

    int dimentions_x, dimentions_y;
    std::vector<integral_element> elements;
    std::vector<std::vector<int>> schema_map;

    integral_element *find(int id);

    void clear_map();
};

void Schema::clear_map()
{
    for(auto i: schema_map)
        for(auto j: i)
            j = 0;
}

integral_element* Schema::find(int id)
{
    for(auto i: elements)
        if(i.id == id)
            return &i;
}

class Back
{
public:
    Schema *read_file(std::string filename);
    int write_file(Schema);

    Schema genetic_update(Schema*);

private:
    int A_star(Schema*, connection);
};