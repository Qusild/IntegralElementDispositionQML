#pragma once
#include <array>
#include <unordered_map>
#include <vector>
#include <QObject>

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
    int id;
    std::string name;

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
    int id;
    std::string name;
    std::vector<connection> connections;


public:
    bool operator==(const integral_element& other) { return this->id == other.id; }
    bool operator==(const integral_element& other) const { return this->id == other.id; }

    void add_connection(const connection& conn);
    void remove_connection(connection& conn);
};

class Schema : public QObject
{
    Q_OBJECT
public:
    Schema(const Schema &other):QObject(other.parent())
    {
        dimentions_x= other.dimentions_x;
        dimentions_y= other.dimentions_y;
        elements = other.elements;
        schema_map = other.schema_map;
    }
    Schema operator=(const Schema &other)
    {
        dimentions_x= other.dimentions_x;
        dimentions_y= other.dimentions_y;
        elements = other.elements;
        schema_map = other.schema_map;
        return *this;
    }
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
    connection* find_con(int id);
    int scale;
    void clear_map();
    int setID(int type);
public slots:
    void addConnection(int init,int fin);
    void addElement(QString ElemName, qint32 ini);
    void delElement(int ini);
    void startUpdate();
    void updateGrid();
    void readFile(QString filename);
    void writeFile(QString filename);
    void delConnection(int init, int fin);
signals:
    void sendCell(int id, int pos, int type, QString name, QString con_1, QString con_2, QString con_3, QString con_4);
    void sendCon(int pos, QString con);
    void updModel(int dim_x, int dim_y);
};

class Back : public QObject
{
    Q_OBJECT
public:
    static Schema read_file(std::string filename, Schema& schema);
    static void write_file(Schema& schema, std::string filename);

    static Schema genetic_update(Schema&);
    static int A_star(Schema&, connection);


public slots:
    void decypher(std::string filename);

signals:
    void sendProps();
};

int is_invalid(Schema* schema, coordinates place);
