#include "back.h"
#include <stdio.h>
#include <iostream>

int main()
{
    int dim_x = 5, dim_y = 10;
    
    Schema test_schema(dim_x, dim_y);

    
    test_schema.clear_map();
    integral_element* element = new integral_element(0, 0, 11);
    element->add_connection(connection(11, 12, 1));
    test_schema.add_element(*element);
    delete element;

    element = new integral_element(3, 8, 12);
    element->add_connection(connection(12, 13, 2));
    test_schema.add_element(*element);
    delete element;

    element = new integral_element(3, 5, 13);
    element->add_connection(connection(13, 11, 3));
    test_schema.add_element(*element);
    delete element;

    Schema new_schema = Back::genetic_update(test_schema);

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