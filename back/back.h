#pragma once
#include <array>
#include <unordered_map>
#include <vector>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
using boost::property_tree::ptree;

struct coordinates
{
public:
    coordinates(int x1, int y1)
        : x(x1), y(y1) {}
    int x, y;
    bool operator ==(coordinates& other)
    {
        return x == other.x && y == other.y;
    }
};

struct connection
{
public:
    connection(int ini, int fin, int id)
        : initial(ini), final(fin), id(id){}
    connection(int ini, int fin, int id, std::string name)
        : initial(ini), final(fin), id(id), name(name) {}

    int initial, final;
    std::string name;
    int id;

public:
    bool operator==(const connection& other) { return this->id == other.id; }
    bool operator==(const connection& other) const { return this->id == other.id; }
};

class integral_element
{
public:
    /*integral_element(int x, int y)
        : coords({ x, y }) {}*/
    integral_element(int x, int y, int id)
        : coords({ x, y }), id(id) {}
    integral_element(int x, int y, int id, std::string n)
        : coords({ x, y }), id(id), name(n) {}

    coordinates coords;
    std::string name;
    int id;
    std::vector<connection> connections;


public:
    bool operator==(const integral_element& other) { return this->id == other.id; }
    bool operator==(const integral_element& other) const { return this->id == other.id; }

    void add_connection(const connection& conn);
    void remove_connection(connection& conn);
};

class Schema
{
public:
    Schema(int x, int y)
        : dimentions_x(x), dimentions_y(y) {
        for (int i = 0; i < dimentions_y; i++)
        {
            std::vector<int> temp_vec;
            for (int j = 0; j < dimentions_x; j++)
                temp_vec.push_back(0);
            schema_map.push_back(temp_vec);
        }
    };

    int dimentions_x, dimentions_y;
    std::vector<integral_element> elements;
    std::vector<std::vector<int>> schema_map;

    void add_element(const integral_element& new_element);
    void remove_element(integral_element& element);


    integral_element* find(int id);

    void clear_map();
};

class Back
{
public:
    Schema* read_file(std::string filename);
    int write_file(Schema);

  

    static Schema genetic_update(Schema*);

private:
    static int A_star(Schema*, connection);
};

Schema* Back::read_file(std::string filename) {

    ptree pt;
    read_xml(filename, pt);
    ptree scheme = pt.get_child("scheme");

    ptree sst = pt.get_child("scheme.size");
    int dim_x = sst.get<int>("cols", 0);
    int dim_y = sst.get<int>("rows", 0);
    //int scale = sst.get<int>("scale", 1);

    Schema schema(dim_x, dim_y);

    ptree sbt = scheme.get_child("bits");
    std::vector<integral_element> Curelems;

    BOOST_FOREACH(ptree::value_type & bit, sbt.get_child("")) {
        std::string id = bit.second.get<std::string>("<xmlattr>.bit_id");
        int id_i = stoi(id);

        int x = bit.second.get<int>("place.x");
        int y = bit.second.get<int>("place.y");

        std::string name = bit.second.get<std::string>("name", id);

        std::vector<connection> Curconns;
        if (id_i < 10000) {
            ptree blinkt = bit.second.get_child("links");
            BOOST_FOREACH(ptree::value_type & link, blinkt.get_child("")) {
                std::string lid = link.second.get<std::string>("<xmlattr>.link_id");
                int lid_i = stoi(lid);

                int initial = id_i;
                int final = link.second.get<int>("to");
                std::string name = link.second.get<std::string>("link_name", lid);
                int id = lid_i;

                connection Curcon(initial, final, id, name);
                Curconns.push_back(Curcon);
            }

        }

        integral_element Curbit(x, y, id_i, name);
        if (id_i < 10000) {
            Curbit.connections = Curconns;
        }
        Curelems.push_back(Curbit);
    }

    schema.elements = Curelems;

    ptree smt = scheme.get_child("map");

    std::vector<std::vector<int>> Curmap(dim_y,
        std::vector<int>(dim_x));

    BOOST_FOREACH(ptree::value_type& row, smt.get_child("")) {
        std::string rid = row.second.get<std::string>("<xmlattr>.y");
        int rid_i = stoi(rid);
        ptree rct = row.second;

        BOOST_FOREACH(ptree::value_type & col, rct.get_child("")) {
            if (col.first == "col") {
                std::string cid = col.second.get<std::string>("<xmlattr>.x");
                int cid_i = stoi(cid);
                Curmap[rid_i-1][cid_i-1] = col.second.get<int>("");
            }
            
        }
    }
    
    schema.schema_map = Curmap;
    
    return &schema;

}

int is_invalid(Schema* schema, coordinates place);
