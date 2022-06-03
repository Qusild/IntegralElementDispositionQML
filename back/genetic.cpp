#include "back.h"

#include <algorithm>
#include <random>
#include <ctime>

#define GENERATION_LENGTH 200
#define MUTATION_PERCENT 40         //"Сила" мутации
#define MUTATION_AMOUNT 70          //Процент от поколения который мутирует
#define CROSSBREED_PERCENT 40       //"Сила" смешивания
#define CROSSBREED_AMOUNT 10        //Процент от поколения который будет спариваться

#define MAX_UNCHANGED  200

/*
    @author WhoLeb
    @param schema схема, какая она сейчас есть.
    @param percent процент изменения особи(целое число)
*/
std::vector<integral_element> individual_mutation(std::vector<integral_element>& elements, int max_x, int max_y, int percent);

/*
    @author WhoLeb
    @param first,second родители
    @param percnet вероятность обмена геном
*/
std::vector<integral_element> crossbreed(std::vector<integral_element>& first, std::vector<integral_element>& second, int percent);
std::vector<std::vector<integral_element>> make_generation(std::vector<std::vector<integral_element>>& previous_generation, int max_x, int max_y);
integral_element* find_by_coords(coordinates& coords, std::vector<integral_element>& elements);


Schema Back::genetic_update(Schema& input_schema)
{
    int best_len = 0;
    for (auto i : input_schema.elements)
    {
         for (auto j : i.connections)
        {
            best_len += A_star(input_schema, j);
        }
    }
    if (best_len < 0) best_len = INT_MAX;
    std::vector<integral_element> primary_individual = input_schema.elements;
    std::vector<integral_element> best_individual = primary_individual;
    std::vector<std::vector<integral_element>> last_generation;

    Schema final_schema(input_schema.dimentions_x, input_schema.dimentions_y);
    Schema working_schema(input_schema.dimentions_x, input_schema.dimentions_y);

    for (int i = 0; i < GENERATION_LENGTH; i++)
    {
        std::vector<integral_element> individual = individual_mutation(input_schema.elements, input_schema.dimentions_x, input_schema.dimentions_y, 100);
        last_generation.emplace_back(individual);
        working_schema.elements = individual;
        int len = 0;
        for (auto j : individual)
        {
            for (auto k : j.connections)
            {
                len += A_star(working_schema, k);
                if (len < 0)
                    len *= -100000;
            }
        }
			if (len < best_len && len > 0)
			{
				best_len = len;
				best_individual = individual;
			}
        //if (best_len < 0) best_len = INT_MAX;
        working_schema.clear_map();
    }
    if (best_len < 0) best_len = INT_MAX;
    int prev_len = best_len;
    int const_gen = 0;
    while (const_gen < MAX_UNCHANGED)
    {
        last_generation = make_generation(last_generation, input_schema.dimentions_x, input_schema.dimentions_y);
        for (auto individual : last_generation)
        {
            working_schema.elements = individual;
            int len = 0;
            for (auto element : individual)
            {
                for (auto connections : element.connections)
                {
                    len += A_star(working_schema, connections);
                    if (len < 0)
                        len *= -100000;
                }
            }
			if (len < best_len && len > 0)
			{
				best_len = len;
				best_individual = individual;
			}
        }
    label2:
        //if (best_len < 0) best_len = INT_MAX;
        working_schema.clear_map();
        if (prev_len == best_len)
            const_gen++;
        else
        {
            prev_len = best_len;
            const_gen = 0;
        }
    }
    working_schema.clear_map();
    working_schema.elements = best_individual;
    for (auto i : best_individual)
        for (auto j : i.connections)
            A_star(working_schema, j);

    for (auto i: best_individual)
        working_schema.schema_map[i.coords.y][i.coords.x] = i.id;
    
    return working_schema;
}

std::vector<integral_element> get_elements(Schema& input_schema)
{
    std::vector<integral_element> ret;
    for (auto i : input_schema.elements)
    {
        ret.emplace_back(i);
    }
    return ret;
}

//Переделать, чтобы не создавалась копия вектора
std::vector<integral_element> individual_mutation(std::vector<integral_element>& elements, int max_x, int max_y, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> individual;
    for (auto i : elements)
    {
        if (rand() % 100 < percent)
        {
            coordinates coords = { 0, 0 };
            do
            {
                coords.x = rand() % max_x;
                coords.y = rand() % max_y;
                i.coords = coords;
            } while (find_by_coords(coords, elements));
        }
        individual.emplace_back(i);
    }
    return individual;
}

std::vector<integral_element> crossbreed(std::vector<integral_element>& first, std::vector<integral_element>& second, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> ret;
    for (int i = 0; i < first.size(); i++)
    {
        if (rand() % 100 < percent)
        {
            ret.emplace_back(second[i]);
        }
        else
            ret.emplace_back(first[i]);
    }
    return ret;
}

std::vector<std::vector<integral_element>> make_generation(std::vector<std::vector<integral_element>>& previous_generation, int max_x, int max_y)
{
    std::vector<std::vector<integral_element>> ret;
    srand(std::time(NULL));
    for (int i = 0; i < GENERATION_LENGTH; i++)
    {
        if (rand() % 100 < MUTATION_AMOUNT)
        {
            std::vector<integral_element> s = individual_mutation(previous_generation[i], max_x, max_y, MUTATION_PERCENT);
            ret.emplace_back(s);
            continue;
        }
        if (rand() % 100 < CROSSBREED_AMOUNT)
        {
            int j = rand() % GENERATION_LENGTH;
            while (j == i)
            {
                j = rand() % GENERATION_LENGTH;
            }
            std::vector<integral_element> s = crossbreed(previous_generation[i], previous_generation[j], CROSSBREED_PERCENT);
            ret.emplace_back(s);
            continue;
        }
        ret.emplace_back(previous_generation[i]);
    }
    return ret;
}

integral_element* find_by_coords(coordinates& coords, std::vector<integral_element>& elements)
{
    for (auto i : elements)
    {
        if (i.coords == coords)
            return &i;
    }
    return nullptr;
}

void Schema::add_element(const integral_element& new_element)
{
    elements.push_back(new_element);
}

void Schema::remove_element(integral_element& element)
{
    if (&element)
        elements.erase(std::find(elements.begin(), elements.end(), element));
}

void integral_element::add_connection(const connection& conn)
{
    connections.push_back(conn);
}


void integral_element::remove_connection(connection& conn)
{
    connections.erase(std::find(connections.begin(), connections.end(), conn));
}