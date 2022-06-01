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

public:
    bool operator==(const connection& other) { return this->id == other.id; }
    bool operator==(const connection& other) const { return this->id == other.id; }
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


public:
    bool operator==(const integral_element& other) { return this->id == other.id; }
    bool operator==(const integral_element& other) const { return this->id == other.id; }

    void add_connection(const connection& conn);
    void remove_connection(connection& conn);
};

class Schema
{
public:
    Schema(int x, int y)
        : dimentions_x(x), dimentions_y(y) {
        for (int i = 0; i < dimentions_y; i++)
        {
            std::vector<int> temp_vec;
            for (int j = 0; j < dimentions_x; j++)
                temp_vec.push_back(0);
            schema_map.push_back(temp_vec);
        }
    };

    int dimentions_x, dimentions_y;
    std::vector<integral_element> elements;
    std::vector<std::vector<int>> schema_map;

    void add_element(const integral_element& new_element);
    void remove_element(integral_element& element);


    integral_element* find(int id);

    void clear_map();
};

class Back
{
public:
    Schema* read_file(std::string filename);
    int write_file(Schema);

  

    static Schema genetic_update(Schema*);

private:
    static int A_star(Schema*, connection);
};