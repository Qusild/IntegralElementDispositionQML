#include "back.h"
#include <random>
#include <ctime>

#define GENERATION_LENGTH 200
#define MUTATION_PERCENT 30         //"Сила" мутации
#define MUTATION_AMOUNT 20          //Процент от поколения который мутирует
#define CROSSBREED_PERCENT 30       //"Сила" смешивания
#define CROSSBREED_AMOUNT 20        //Процент от поколения который будет спариваться

#define MAX_UNCHANGED 20

/*
    @author WhoLeb
    @param schema схема, какая она сейчас есть.
    @param percent процент изменения особи(целое число)
*/
std::vector<integral_element> individual_mutation(std::vector<integral_element> elements, int max_x, int max_y, int percent);
std::vector<integral_element> get_elements(Schema *input_schema);

/*
    @author WhoLeb
    @param first,second родители
    @param percnet вероятность обмена геном
*/
std::vector<integral_element> crossbreed(std::vector<integral_element>* first, std::vector<integral_element>* second, int percent);
std::vector<std::vector<integral_element>> make_generation(std::vector<std::vector<integral_element>>* previous_generation, int max_x, int max_y);

Schema Back::genetic_update(Schema* input_schema)
{
    int best_len = 0;
    for(auto i: input_schema->elements)
    {
        for (auto j: i.connections)
        {
            best_len = A_star(input_schema, j);
        }
    }
    std::vector<integral_element> primary_individual = input_schema->elements;
    std::vector<integral_element> best_individual = primary_individual;
    std::vector<std::vector<integral_element>> last_generation;
    Schema working_schema(input_schema->dimentions_x, input_schema->dimentions_y);

    for (int i = 0; i < GENERATION_LENGTH; i++)
    {
        std::vector<integral_element> individual = individual_mutation(input_schema->elements, input_schema->dimentions_x, input_schema->dimentions_y,100);
        last_generation.emplace_back(individual);
        working_schema.elements = individual;
        for (auto j: individual)
        {
            for(auto k: j.connections)
            {
                int len = A_star(&working_schema, k);
                if(len < 0)
                    goto label;
                if(len < best_len)
                {
                    best_len = len;
                    best_individual = individual;
                }
            }
        }
    label:
        working_schema.clear_map();
    }
    int prev_len = best_len;
    int const_gen = 0;
    while(const_gen < MAX_UNCHANGED)
    {
        working_schema.clear_map();
        last_generation = make_generation(last_generation, input_schema->dimentions_x, input_schema->dimentions_y);
        for(auto individual: last_generation)
        {
            working_schema.elements = individual;
            for(auto element: individual)
            {
                for(auto connections: element.connections)
                {
                    int len = A_star(&working_schema, connections);
                    if(len < 0)
                        goto label2;
                    if(len < best_len)
                    {
                        best_len = len;
                        best_individual = individual;
                    }
                }
            }
        }
    label2:
        working_schema.clear_map();
        if (prev_len == best_len)
            const_gen++;
        else
        {
            prev_len = best_len;
            const_gen = 0;
        }
    }
    return working_schema;
}

std::vector<integral_element> get_elements(Schema* input_schema)
{
    std::vector<integral_element> ret;
    for(auto i:input_schema->elements)
    {
        ret.emplace_back(i);
    }
    return ret;
}

//Переделать, чтобы не создавалась копия вектора
std::vector<integral_element> individual_mutation(std::vector<integral_element> *elements, int max_x, int max_y, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> individual;
    for (auto i: *elements)
    {
        if(rand()%100 < percent)
        {
            i.coords.x = rand() % max_x;
            i.coords.y = rand() % max_y;
        }
        individual.emplace_back(i);
    }
    return individual;
}

std::vector<integral_element> crossbreed(std::vector<integral_element>* first, std::vector<integral_element>* second, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> ret;
    for (int i = 0; i < first->size(); i++)
    {
        if(rand()%100 < percent)
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
        if(rand() % 100 < MUTATION_AMOUNT)
        {
            std::vector<integral_element> s = individual_mutation(&previous_generation[i], max_x, max_y, MUTATION_PERCENT);
            ret.emplace_back(s);
            continue;
        }
        if(rand()%100 < CROSSBREED_AMOUNT)
        {
            int j = rand() % GENERATION_LENGTH;
            while(j == i)
            {
                int j = rand() % GENERATION_LENGTH;
            }
            std::vector<integral_element> s = crossbreed(&previous_generation[i], &previous_generation[j], CROSSBREED_PERCENT);
            ret.emplace_back(s);
            continue;
        }
        ret.emplace_back(previous_generation[i]);
    }
    return ret;
}