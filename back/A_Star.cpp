#include "back.h"
#include <stack>

inline int manhatton(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

inline bool is_dest(coordinates& current, integral_element* dest)
{
    return current == dest->coords;
}

int is_valid(Schema* schema, coordinates place)
{
    int retval = 0;
    retval = schema->schema_map[place.x][place.y] >= 10;
    retval <<= 1;
    retval ^= place.x < schema->dimentions_x;
    retval <<= 1;
    retval ^= place.y < schema->dimentions_y;
    retval <<= 10;
    return retval;
}

int Back::A_star(Schema *schema, connection conn)
{

    integral_element* start = schema->find(conn.initial);
    integral_element* end = schema->find(conn.final);

    coordinates starting_coord = start->coords;
    coordinates ending_coord = end->coords;

    if(int r = is_valid(schema, ending_coord))
        return r;

    std::stack<coordinates> openList;
    openList.emplace(starting_coord);
    while(!openList.empty())
    {
        coordinates current_place = openList.top();
        openList.pop();
        int best_path = INT_MAX;
        int current_path = 0;
        if(is_valid(schema, coordinates(current_place.x, current_place.y-1)) && )
        schema->schema_map[current_place.x][current_place.y - 1]
    }

}