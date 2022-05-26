#include "back.h"
#include <stack>

inline int manhatton(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

inline bool is_dest(coordinates& current, coordinates& dest)
{
    return current == dest;
}

int is_invalid(Schema* schema, coordinates place)
{
    int retval = 0;
    if(place.x > 0 && place.y > 0 && place.x < schema->dimentions_x-1 && place.y < schema->dimentions_y - 1)
    retval = schema->schema_map[place.x][place.y] != 0;
    retval <<= 1;
    retval ^= place.x >= schema->dimentions_x;
    retval <<= 1;
    retval ^= place.y >= schema->dimentions_y;
    retval <<= 10;
    return -retval;
}

int Back::A_star(Schema* schema, connection conn)
{

    integral_element* start = schema->find(conn.initial);
    coordinates starting_coord = start->coords;

    integral_element* end = schema->find(conn.final);
    coordinates ending_coord = end->coords;

    if (int r = is_invalid(schema, ending_coord))
        return r;

    int current_path = 0;
    coordinates current_place = starting_coord;
    while (!is_dest(current_place, ending_coord))
    {
        int best_path = INT_MAX;
        int choice;
        int p = manhatton(current_place.x, current_place.y - 1, ending_coord.x, ending_coord.y);
        if (!is_invalid(schema, coordinates(current_place.x, current_place.y - 1)) && p + current_path < best_path)
        {
            best_path = p + current_path;
            choice = 1;
        }
        p = manhatton(current_place.x + 1, current_place.y, ending_coord.x, ending_coord.y);
        if (!is_invalid(schema, coordinates(current_place.x + 1, current_place.y)) && p + current_path < best_path)
        {
            best_path = p + current_path;
            choice = 2;
        }
        p = manhatton(current_place.x, current_place.y + 1, ending_coord.x, ending_coord.y);
        if (!is_invalid(schema, coordinates(current_place.x, current_place.y + 1)) && p + current_path < best_path)
        {
            best_path = p + current_path;
            choice = 3;
        }
        p = manhatton(current_place.x - 1, current_place.y, ending_coord.x, ending_coord.y);
        if (!is_invalid(schema, coordinates(current_place.x - 1, current_place.y)) && p + current_path < best_path)
        {
            best_path = p + current_path;
            choice = 4;
        }

        switch (choice)
        {
        case 1:
            schema->schema_map[current_place.x][current_place.y - 1] = conn.id;
            current_place = coordinates(current_place.x, current_place.y - 1);
            current_path += 1;
            break;
        case 2:
            schema->schema_map[current_place.x + 1][current_place.y] = conn.id;
            current_place = coordinates(current_place.x + 1, current_place.y);
            current_path += 1;
            break;
        case 3:
            schema->schema_map[current_place.x][current_place.y + 1] = conn.id;
            current_place = coordinates(current_place.x, current_place.y + 1);
            current_path += 1;
            break;
        case 4:
            schema->schema_map[current_place.x - 1][current_place.y] = conn.id;
            current_path += 1;
            current_place = coordinates(current_place.x - 1, current_place.y);
            break;
        default:
            return -999999;
        }
    }
    return current_path;
}

integral_element* Schema::find(int id)
{
    for (integral_element i : elements)
        if (i.id == id)
            return &i;
    return nullptr;
}

void Schema::clear_map()
{
    for (auto i : schema_map)
        for (auto j : i)
            j = 0;
}