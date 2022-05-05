#include "back.h"
#include <random>
#include <ctime>

#define GENERATION_LENGTH 200
#define MUTATION_PERCENT 30         //"Сила" мутации
#define MUTATION_AMOUNT 20          //Процент от поколения который мутирует
#define CROSSBREED_PERCENT 30       //"Сила" смешивания
#define CROSSBREED_AMOUNT 20        //Процент от поколения который будет спариваться


/*
    @author WhoLeb
    @param schema схема, какая она сейчас есть.
    @param percent процент изменения особи(целое число)
*/
std::vector<integral_element> individual_mutation(Schema* schema, int percent);
std::vector<integral_element> get_elements(Schema *input_schema);

/*
    @author WhoLeb
    @param first,second родители
    @param percnet вероятность обмена геном
*/
std::vector<integral_element> crossbreed(std::vector<integral_element>* first, std::vector<integral_element>* second, int percent);
std::vector<std::vector<integral_element>> make_generation(std::vector<std::vector<integral_element>>* previous_generation);

Schema Back::genetic_update(Schema* input_schema)
{
    int primary_len = 0;
    for(auto i: input_schema->elements)
    {
        for (auto j: i.connections)
        {
            primary_len = A_star(input_schema, j);
        }
    }

    std::vector<std::vector<integral_element>> last_generation;
    int min_gen_len = INT_MAX;
    for (int i = 0; i < GENERATION_LENGTH; i++)
    {
        std::vector<integral_element> individual = individual_mutation(input_schema, 100);
        last_generation.emplace_back();
        for (auto j: individual)
        {
            for(auto k: j.connections)
            {
                int len = A_star(input_schema, k);
                if(len < min_gen_len)
                    min_gen_len = len;
            }
        }
    }


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

std::vector<integral_element> individual_mutation(Schema* schema, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> individual;
    std::vector<integral_element> schem_elements = get_elements(schema);
    for (auto i: schem_elements)
    {
        if(rand()%100 < percent)
        {
            i.coords.x = rand() % schema->dimentions_x;
            i.coords.y = rand() % schema->dimentions_y;
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

std::vector<std::vector<integral_element>> make_generation(Schema* input_schema)
{
    std::vector<std::vector<integral_element>> ret;
    srand(std::time(NULL));
    for (int i = 0; i < GENERATION_LENGTH; i++)
    {
        
    }
}