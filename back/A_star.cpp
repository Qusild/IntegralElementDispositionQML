#include "back.h"
#include <stack>
#include <QDebug>
inline int manhatton(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

inline bool is_dest(coordinates& current, coordinates& dest)
{
    return current == dest;
}

bool is_invalid(Schema& schema, coordinates place)
{
    int retval = 0;
    if (place.x < 0 || place.y < 0 || place.x > schema.dimentions_x - 1 || place.y >schema.dimentions_y - 1)
    {
        retval = 1;
        retval <<= 1;
    }
    else
    {
        retval ^= schema.schema_map[place.y][place.x] != 0;
        retval <<= 1;
    }
    retval ^= place.x >= schema.dimentions_x;
    retval <<= 1;
    retval ^= place.y >= schema.dimentions_y;
    retval <<= 10;
    return retval>0;
}

int alg_redo(Schema& schema, connection conn);

int Back::A_star(Schema& schema, connection conn)
{
    integral_element* start = schema.find(conn.initial);

    //if (!start) return -10000;
    schema.schema_map[start->coords.y][start->coords.x] = start->id;
    integral_element* end = schema.find(conn.final);
    //if (!end) return -10001;
    schema.schema_map[end->coords.y][end->coords.x] = end->id;

    Schema schem(schema);
    int right = alg_redo(schem, conn);
    Schema schem2(schema);
    connection connect(conn.final, conn.initial, conn.id);
    int inverse = alg_redo(schem2, connect);
    if (right < inverse && right > 0)
    {
        schema = schem;
        return right;
    }
    if(inverse > 0 && inverse <= right)
    {
        schema = schem2;
        return inverse;
    }
    return -10000;
}


int alg_redo(Schema& schema, connection conn)
{

    integral_element* start = schema.find(conn.initial);

    if (!start) return -10000;
    coordinates starting_coord = start->coords;

    integral_element* end = schema.find(conn.final);
    if (!end) return -10001;
    coordinates ending_coord = end->coords;

    /*if (is_invalid(schema, ending_coord) || is_invalid(schema, starting_coord))
        return -191919;*/

    int current_path = 0;
    int count = 0;
    coordinates current_place = starting_coord;
    while (!is_dest(current_place, ending_coord))
    {
        count++;
        int best_path = INT_MAX;
        int choice = 0;
        int p = manhatton(current_place.x, current_place.y - 1, ending_coord.x, ending_coord.y);

        coordinates next_place(current_place.x, current_place.y - 1);
        if ((!is_invalid(schema, next_place) && p + current_path < best_path)
            || is_dest(next_place, ending_coord))
        {
            best_path = p + current_path;
            choice = 1;
        }
        p = manhatton(current_place.x + 1, current_place.y, ending_coord.x, ending_coord.y);
        next_place = coordinates(current_place.x + 1, current_place.y);
        if ((!is_invalid(schema, next_place) && p + current_path < best_path)
            || is_dest(next_place, ending_coord))
        {
            best_path = p + current_path;
            choice = 2;
        }
        p = manhatton(current_place.x, current_place.y + 1, ending_coord.x, ending_coord.y);
        next_place = coordinates(current_place.x, current_place.y + 1);
        if ((!is_invalid(schema, next_place) && p + current_path < best_path)
            || is_dest(next_place, ending_coord))
        {
            best_path = p + current_path;
            choice = 3;
        }
        p = manhatton(current_place.x - 1, current_place.y, ending_coord.x, ending_coord.y);
        next_place = coordinates(current_place.x - 1, current_place.y);
        if ((!is_invalid(schema, next_place) && p + current_path < best_path)
            || is_dest(next_place, ending_coord))
        {
            best_path = p + current_path;
            choice = 4;
        }

        switch (choice)
        {
        case 1:
            schema.schema_map[current_place.y - 1][current_place.x] = conn.id;
            current_place = coordinates(current_place.x, current_place.y - 1);
            current_path += 1;

            break;
        case 2:
            schema.schema_map[current_place.y][current_place.x + 1] = conn.id;
            current_place = coordinates(current_place.x + 1, current_place.y);
            current_path += 1;

            break;
        case 3:
            schema.schema_map[current_place.y + 1][current_place.x] = conn.id;
            current_place = coordinates(current_place.x, current_place.y + 1);
            current_path += 1;

            break;
        case 4:
            schema.schema_map[current_place.y][current_place.x - 1] = conn.id;
            current_path += 1;

            current_place = coordinates(current_place.x - 1, current_place.y);
            break;
        default:
            return -999999;
        }
        if (count > 300) return -11111111;
    }
    return current_path;
}

integral_element* Schema::find(int id)
{
    for (uint i =0;i<elements.size();i++)
        if (this->elements[i].id == id)
            return &elements[i];
    return nullptr;
}

connection *Schema::find_con(int id)
{
    for (uint i =0;i<elements.size();i++)
        for (uint j = 0;j<elements[i].connections.size();j++)
            if (this->elements[i].connections[j].id == id)
            return &elements[i].connections[j];
    return nullptr;
}

void Schema::clear_map()
{
    for (int y = 0; y < dimentions_y; y++)
        for (int x = 0; x < dimentions_x; x++)
            schema_map[y][x] = 0;
}
