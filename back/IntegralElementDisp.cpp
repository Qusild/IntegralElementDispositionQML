#include "back.h"
#include <stdio.h>
#include <iostream>

int main()
{
    int dim_x = 5, dim_y = 10;
    
    Schema test_schema(dim_x, dim_y);

    
    test_schema.clear_map();
    integral_element temp_element(0, 1, 11);
    connection temp_connection(11, 12, 1);
    temp_element.add_connection(temp_connection);
    test_schema.add_element(temp_element);

    integral_element temp_element2(3,3, 12);
    temp_connection = connection(12, 11, 2);
    temp_element2.add_connection(temp_connection);
    test_schema.add_element(temp_element2);
    
    Schema new_schema = Back::genetic_update(&test_schema);

    for (int y = 0; y < dim_y; y++)
    {
        for (int x = 0; x < dim_x; x++)
        {
            printf("%d ", new_schema.schema_map[y][x]);
        }
        printf("\n");
    }
    system("pause");
}