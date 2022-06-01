#include "back.h"
#include <stdio.h>
#include <iostream>

int main()
{
    Schema test_schema(5, 10);

    
    test_schema.clear_map();
    integral_element temp_element(0, 1);
    temp_element.id = 11;
    connection temp_connection;
    temp_connection.initial = 11;
    temp_connection.final = 12;
    temp_connection.id = 1;
    temp_element.connections.push_back(temp_connection);
    test_schema.elements.push_back(temp_element);
    integral_element temp_element2(3,3);
    temp_element2.id = 12;
    temp_connection.initial = 12;
    temp_connection.final = 11;
    temp_connection.id = 2;
    temp_element2.connections.push_back(temp_connection);
    test_schema.elements.push_back(temp_element2);
    

    Schema new_schema = Back::genetic_update(&test_schema);

    for (auto i : new_schema.schema_map)
    {
        for (auto j : i)
        {
            printf("%d ", j);
        }
        printf("\n");
    }
    system("pause");
}