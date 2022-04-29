#include "back.h"
#include <random>
#include <ctime>

std::vector<coordinates> generate_individual(int len,int limit_x, int limit_y);

Schema Back::genetic_update(Schema* input_schema)
{
    std::vector<coordinates> elements_coordinates;
    int k = 0;
    for(auto i: input_schema->elements)
    {
        elements_coordinates.push_back(i.coords);
        k++;
    }
    std::vector<std::vector<connection>> connects;
    k = 0;
    for(auto i: input_schema->elements)
    {
        connects.push_back(i.connections);
        k++;
    }
    int number_of_elements = k+1;

    Schema temp_schema = *input_schema;

    int initial_length = 0;
    for (int i = 0; i < k; i++)
    {
        for(auto j : connects[i])
        {
            A_star_ret temp_ret = A_star(temp_schema, j);
            initial_length += temp_ret.path_len;
            temp_schema = temp_ret.schema;
        }
    }
    
    std::vector<std::vector<coordinates>> initial_generation;
    std::vector<A_star_ret> individual_path_and_length;
    for (int i = 0; i < 200; i++)
    {
        temp_schema = *input_schema;
        std::vector<coordinates> individual = generate_individual(k, input_schema->dimentions_x, input_schema->dimentions_y);
        initial_generation.push_back(individual);

        int c = 0;
        for(auto j: individual)
        {
            temp_schema.elements[c].coords = j;
            c++;
        }
        int temp_individual_length = 0;
        for (int l = 0; l < k; l++)
            for (auto j : connects[l])
            {
                A_star_ret temp_ret = A_star(temp_schema, j);
                temp_individual_length += temp_ret.path_len;
                temp_schema = temp_ret.schema;
            }
        individual_path_and_length.push_back({temp_schema, temp_individual_length});
    }

    int min_path = 100000;
    int min_path_place;
    for (int i = 0; i < 200; i++)
    {
        if(individual_path_and_length[i].path_len < min_path)
        {
            min_path_place = i;
            min_path = individual_path_and_length[i].path_len;
        }
    }
    return individual_path_and_length[min_path_place].schema;
}

std::vector<coordinates> generate_individual(int len, int limit_x, int limit_y)
{
    srand(std::time(NULL));
    std::vector<coordinates> individual;
    for(int i = 0; i < len; i++)
    {
        int x = rand() % limit_x;
        int y = rand() % limit_y;
        individual.push_back({x, y});
    }
    return individual;
}