#include "back.h"
#include <QDebug>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#define GENERATION_LENGTH 200
#define MUTATION_PERCENT 40         //"����" �������
#define MUTATION_AMOUNT 70          //������� �� ��������� ������� ��������
#define CROSSBREED_PERCENT 40       //"����" ����������
#define CROSSBREED_AMOUNT 10        //������� �� ��������� ������� ����� �����������
#define MIN_ELEMENT 11
#define MAX_ELEMENT 10000
#define MIN_WIRE 10001
#define MAX_WIRE 100000

#define MAX_UNCHANGED  1000

/*
    @author WhoLeb
    @param schema �����, ����� ��� ������ ����.
    @param percent ������� ��������� �����(����� �����)
*/
std::vector<integral_element> individual_mutation(std::vector<integral_element>& elements, int max_x, int max_y, int percent);

/*
    @author WhoLeb
    @param first,second ��������
    @param percnet ����������� ������ �����
*/
std::vector<integral_element> crossbreed(std::vector<integral_element>& first, std::vector<integral_element>& second, int percent);
std::vector<std::vector<integral_element>> make_generation(std::vector<std::vector<integral_element>>& previous_generation, int max_x, int max_y);
integral_element* find_by_coords(coordinates& coords, std::vector<integral_element>& elements);


Schema Back::genetic_update(Schema& input_schema)
{

    int best_len = 0;
    for (auto i : input_schema.elements)
    {
         for (auto j : i.connections)
        {
            best_len += A_star(input_schema, j);
        }
    }

    if (best_len < 0) best_len = INT_MAX;
    std::vector<integral_element> primary_individual = input_schema.elements;
    std::vector<integral_element> best_individual = primary_individual;
    std::vector<std::vector<integral_element>> last_generation;

    Schema final_schema(input_schema.dimentions_x, input_schema.dimentions_y);
    Schema working_schema(input_schema.dimentions_x, input_schema.dimentions_y);

    for (int i = 0; i < GENERATION_LENGTH; i++)
    {
        std::vector<integral_element> individual = individual_mutation(input_schema.elements, input_schema.dimentions_x, input_schema.dimentions_y, 100);
        last_generation.emplace_back(individual);
        working_schema.elements = individual;
        int len = 0;
        for (auto j : individual)
        {
            for (auto k : j.connections)
            {
                len += A_star(working_schema, k);
                if (len < 0)
                    len *= -100000;
            }
        }
            if (len < best_len && len > 0)
            {
                best_len = len;
                best_individual = individual;
            }
        //if (best_len < 0) best_len = INT_MAX;
        working_schema.clear_map();
    }
    if (best_len < 0) best_len = INT_MAX;
    int prev_len = best_len;
    int const_gen = 0;
    while (const_gen < MAX_UNCHANGED)
    {
        last_generation = make_generation(last_generation, input_schema.dimentions_x, input_schema.dimentions_y);
        for (auto individual : last_generation)
        {
            working_schema.elements = individual;
            int len = 0;
            for (auto element : individual)
            {
                for (auto connections : element.connections)
                {
                    len += A_star(working_schema, connections);
                    if (len < 0)
                        len *= -100000;
                }
            }
            if (len < best_len && len > 0)
            {
                best_len = len;
                best_individual = individual;
            }
        }
        //if (best_len < 0) best_len = INT_MAX;
        working_schema.clear_map();
        if (prev_len == best_len)
            const_gen++;
        else
        {
            prev_len = best_len;
            const_gen = 0;
        }
    }
    working_schema.clear_map();
    working_schema.elements = best_individual;
    for (auto i : best_individual)
        for (auto j : i.connections)
            A_star(working_schema, j);

    for (auto i: best_individual)
        working_schema.schema_map[i.coords.y][i.coords.x] = i.id;
    for (int y = 0; y < working_schema.dimentions_y; y++)
    {
        for (int x = 0; x < working_schema.dimentions_x; x++)
        {
            input_schema.schema_map[y][x]= working_schema.schema_map[y][x];
        }
    }
    return working_schema;
}

std::vector<integral_element> get_elements(Schema& input_schema)
{
    std::vector<integral_element> ret;
    for (auto i : input_schema.elements)
    {
        ret.emplace_back(i);
    }
    return ret;
}

//����������, ����� �� ����������� ����� �������
std::vector<integral_element> individual_mutation(std::vector<integral_element>& elements, int max_x, int max_y, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> individual;
    for (auto i : elements)
    {
        if (rand() % 100 < percent)
        {
            coordinates coords = { 0, 0 };
            do
            {
                coords.x = rand() % max_x;
                coords.y = rand() % max_y;
                i.coords = coords;
            } while (find_by_coords(coords, elements));
        }
        individual.emplace_back(i);
    }
    return individual;
}

std::vector<integral_element> crossbreed(std::vector<integral_element>& first, std::vector<integral_element>& second, int percent)
{
    srand(std::time(NULL));
    std::vector<integral_element> ret;
    for (uint i = 0; i < first.size(); i++)
    {
        if (rand() % 100 < percent)
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
        if (rand() % 100 < MUTATION_AMOUNT)
        {
            std::vector<integral_element> s = individual_mutation(previous_generation[i], max_x, max_y, MUTATION_PERCENT);
            ret.emplace_back(s);
            continue;
        }
        if (rand() % 100 < CROSSBREED_AMOUNT)
        {
            int j = rand() % GENERATION_LENGTH;
            while (j == i)
            {
                j = rand() % GENERATION_LENGTH;
            }
            std::vector<integral_element> s = crossbreed(previous_generation[i], previous_generation[j], CROSSBREED_PERCENT);
            ret.emplace_back(s);
            continue;
        }
        ret.emplace_back(previous_generation[i]);
    }
    return ret;
}

integral_element* find_by_coords(coordinates& coords, std::vector<integral_element>& elements)
{
    for (uint i=0;i<elements.size();i++)
    {
        if (elements[i].coords == coords)
            return &elements[i];
    }
    return nullptr;
}

void Schema::add_element(const integral_element& new_element)
{
    elements.push_back(new_element);
}

void Schema::remove_element(integral_element& element)
{
        elements.erase(std::find(elements.begin(), elements.end(), element));
}

void integral_element::add_connection(const connection& conn)
{
    connections.push_back(conn);
}


void integral_element::remove_connection(connection& conn)
{
    connections.erase(std::find(connections.begin(), connections.end(), conn));
}

void Schema::addConnection(int init, int fin)
{
    integral_element *tmp = find(init);
    tmp->add_connection(connection(init,fin,setID(2)));
    startUpdate();
}


void Schema::delConnection(int init, int fin)
{
        {
            integral_element *tmp = find(init);
            for (uint i = 0;i<tmp->connections.size();i++)
                if (tmp->connections[i].final == fin)
                {
                    tmp->remove_connection(tmp->connections[i]);
                    startUpdate();
                    return;
                }
    }
}

int Schema::setID(int type)
{
    if (type==1)
    {
        for (int i = MIN_ELEMENT;i<=MAX_ELEMENT;i++)
            if (!Schema::find(i))
                return i;
    }
    else if (type==2)
    {
        for (int i = MIN_WIRE;i<=MAX_WIRE;i++)
            if (!Schema::find_con(i))
                return i;
    }
    return -1;
}
void Schema::addElement(QString ElemName, int ini)
{
    integral_element *tmp = new integral_element(0,0,setID(1),ElemName.toStdString());
    tmp->coords.y = ini/dimentions_y;
    tmp->coords.x = ini%dimentions_y;
    add_element(*tmp);
    updateGrid();
}

void Schema::delElement(int ini)
{
    integral_element *tmp = find(ini);
    for (uint i = 0;i<tmp->connections.size();i++)
        tmp->remove_connection(tmp->connections[i]);
    for (uint i =0;i<elements.size();i++)
    {
        for (uint j = 0;j<elements[i].connections.size();j++)
            if (elements[i].connections[j].final == ini)
                elements[i].remove_connection(elements[i].connections[j]);
    }
    remove_element(*tmp);
    Schema::updateGrid();
}



void Schema::startUpdate()
{
    *this = Back::genetic_update(*this);
    Schema::updateGrid();
}

void Schema::updateGrid()
{
    emit(updModel(dimentions_x,dimentions_y));
    for (int i = 0;i<dimentions_y;i++)
        for (int j =0;j<dimentions_x;j++)
            emit(sendCell(0, i*dimentions_x+j,0,"","none","none","none","none"));
    for (uint i = 0;i<this->elements.size();i++)
    {

        if(this->elements[i].connections.size()==4)
        {    emit(sendCell(this->elements[i].id,this->elements[i].coords.y*dimentions_x+this->elements[i].coords.x,1,
                      QString::fromStdString(this->elements[i].name),
                          QString::fromStdString(find(this->elements[i].connections[0].final)->name),
                            QString::fromStdString(find(this->elements[i].connections[1].final)->name),
                              QString::fromStdString(find(this->elements[i].connections[2].final)->name),
                                QString::fromStdString(find(this->elements[i].connections[3].final)->name)));
        }
        else if(this->elements[i].connections.size()==3)
        {    emit(sendCell(this->elements[i].id,this->elements[i].coords.y*dimentions_x+this->elements[i].coords.x,1,
                      QString::fromStdString(this->elements[i].name),
                          QString::fromStdString(find(this->elements[i].connections[0].final)->name),
                            QString::fromStdString(find(this->elements[i].connections[1].final)->name),
                              QString::fromStdString(find(this->elements[i].connections[2].final)->name),
                                "none"));
        }
        else if(this->elements[i].connections.size()==2)
        {    emit(sendCell(this->elements[i].id,this->elements[i].coords.y*dimentions_x+this->elements[i].coords.x,1,
                      QString::fromStdString(this->elements[i].name),
                          QString::fromStdString(find(this->elements[i].connections[0].final)->name),
                            QString::fromStdString(find(this->elements[i].connections[1].final)->name),
                              "none",
                                "none"));

        }
        else if(this->elements[i].connections.size()==1)
        {
            emit(sendCell(this->elements[i].id,this->elements[i].coords.y*dimentions_x+this->elements[i].coords.x,1,
                      QString::fromStdString(this->elements[i].name),
                          QString::fromStdString(find(this->elements[i].connections[0].final)->name),
                            "none",
                              "none",
                                "none"));
        }
        else if(this->elements[i].connections.size()==0)
            emit(sendCell(this->elements[i].id,this->elements[i].coords.y*dimentions_x+this->elements[i].coords.x,1,
                      QString::fromStdString(this->elements[i].name),"none","none","none","none"));
    }
    for (uint i=0;i<this->elements.size();i++)
    {
        if (elements[i].connections.size()==1)
            emit(sendCon(find(this->elements[i].connections[0].final)->coords.y*dimentions_x
                    +find(this->elements[i].connections[0].final)->coords.x,
                    QString::fromStdString(this->elements[i].name)));
        else if (elements[i].connections.size()==2)
            for (int k = 0;k<2;k++)
            {
                emit(sendCon(find(this->elements[i].connections[k].final)->coords.y*dimentions_x
                    +find(this->elements[i].connections[k].final)->coords.x,
                    QString::fromStdString(this->elements[i].name)));
            }
        else if (elements[i].connections.size()==3)
            for (int k = 0;k<2;k++)
            {
                emit(sendCon(find(this->elements[i].connections[k].final)->coords.y*dimentions_x
                    +find(this->elements[i].connections[k].final)->coords.x,
                    QString::fromStdString(this->elements[i].name)));
            }
        else if (elements[i].connections.size()==4)
            for (int k = 0;k<4;k++)
            {
                emit(sendCon(find(this->elements[i].connections[k].final)->coords.y*dimentions_x
                    +find(this->elements[i].connections[k].final)->coords.x,
                    QString::fromStdString(this->elements[i].name)));
            }
    }
    for (uint i = 0;i<this->elements.size();i++)
    {
        for (uint j = 0; j<this->elements[i].connections.size();j++)
        {
            std::vector<int> path;
            for (int k = 0;k<dimentions_y;k++)
                for (int l =0;l<dimentions_x;l++)
                    if (this->schema_map[k][l]==this->elements[i].connections[j].id)//Проверка на провод
                        path.push_back(k*dimentions_x+l);
            path.push_back(elements[i].coords.y*dimentions_x+elements[i].coords.x);
            path.push_back(find(elements[i].connections[j].final)->coords.y*dimentions_x+find(elements[i].connections[j].final)->coords.x);
            for (uint l = 0;l<path.size()-2;l++)
            {
                if (std::find(path.begin(),path.end(),path[l]-1)!=path.end()
                        &&std::find(path.begin(),path.end(),path[l]+1)!=path.end()) //Горизонтальный провод
                    emit(sendCell(this->elements[i].connections[j].id, path[l],3,
                          QString::fromStdString(this->elements[i].connections[j].name),"none","none","none","none"));
                else if (std::find(path.begin(),path.end(),path[l]-dimentions_x)!=path.end()
                        &&std::find(path.begin(),path.end(),path[l]+dimentions_x)!=path.end()) //Вертикальныйпровод
                    emit(sendCell(this->elements[i].connections[j].id, path[l],2,
                          QString::fromStdString(this->elements[i].connections[j].name),"none","none","none","none"));
                else if (std::find(path.begin(),path.end(),path[l]-1)!=path.end()
                        &&std::find(path.begin(),path.end(),path[l]-dimentions_x)!=path.end()) //Верх-лево провод
                    emit(sendCell(this->elements[i].connections[j].id, path[l],4,
                          QString::fromStdString(this->elements[i].connections[j].name),"none","none","none","none"));
                else if (std::find(path.begin(),path.end(),path[l]-dimentions_x)!=path.end()
                        &&std::find(path.begin(),path.end(),path[l]+1)!=path.end()) //Верх-право провод
                    emit(sendCell(this->elements[i].connections[j].id, path[l],5,
                          QString::fromStdString(this->elements[i].connections[j].name),"none","none","none","none"));
                else if (std::find(path.begin(),path.end(),path[l]-1)!=path.end()
                        &&std::find(path.begin(),path.end(),path[l]+dimentions_x)!=path.end()) //Низ-лево провод
                    emit(sendCell(this->elements[i].connections[j].id, path[l],6,
                          QString::fromStdString(this->elements[i].connections[j].name),"none","none","none","none"));
                else if (std::find(path.begin(),path.end(),path[l]+dimentions_x)!=path.end()
                        &&std::find(path.begin(),path.end(),path[l]+1)!=path.end()) //Низ-право провод
                    emit(sendCell(this->elements[i].connections[j].id, path[l],7,
                          QString::fromStdString(this->elements[i].connections[j].name),"none","none","none","none"));
            }
            path.clear();

        }
    }

}

void Schema::readFile(QString filename)
{
    std::string profiler;
    QString tmp = "file:";
    if (filename.contains(tmp))
        for(int i =8;i<filename.size();i++)
            profiler.push_back(filename[i].toLatin1());
    else
        for(int i =0;i<filename.size();i++)
            profiler.push_back(filename[i].toLatin1());
    Back::read_file(profiler,*this);
    updateGrid();
}

void Schema::writeFile(QString filename)
{
    std::string profiler;
    QString tmp = "file:";
    if (filename.contains(tmp))
        for(int i =8;i<filename.size();i++)
            profiler.push_back(filename[i].toLatin1());
    else
        for(int i =0;i<filename.size();i++)
            profiler.push_back(filename[i].toLatin1());
    Back::write_file(*this,profiler);
}
