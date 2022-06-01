#pragma once
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
        : coords({ x, y }) {};

    coordinates coords;
    std::string name;
    int id;
    std::vector<connection> connections;
};

class Schema
{
public:
    Schema(int x, int y)
        : dimentions_x(x), dimentions_y(y) {
        for (int i = 0; i < dimentions_x; i++)
        {
            std::vector<int> temp_vec;
            for (int j = 0; j < dimentions_y; j++)
                temp_vec.push_back(0);
            schema_map.push_back(temp_vec);
        }
    };

    int dimentions_x, dimentions_y;
    std::vector<integral_element> elements;
    std::vector<std::vector<int>> schema_map;

    integral_element* find(int id);

    void clear_map();
};

class Back
{
public:
    Schema* read_file(std::string filename);
    int write_file(Schema);

    void add_element(const integral_element& new_element, Schema& schema);
    void remove_element(integral_element& element, Schema& schema);
    void add_connection(const connection& conn, Schema& schema);
    void remove_connection(const integral_element& element, connection& conn);

    static Schema genetic_update(Schema*);

private:
    static int A_star(Schema*, connection);
};