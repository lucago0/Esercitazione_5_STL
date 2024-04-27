#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Eigen>
#include "PolygonalMesh.hpp"

double square(double x){
    return x*x;
}

namespace PolMeshLibrary {
bool ImportMesh(const string& filepath, PolygonalMesh& mesh){
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh)){
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.MarkerCell0Ds.begin(); it != mesh.MarkerCell0Ds.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh)){
        return false;
    }
    else{
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.MarkerCell1Ds.begin(); it != mesh.MarkerCell1Ds.end(); it++){
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh)){
        return false;
    }
    else
    {
        for(unsigned int c = 0; c < mesh.NumberCell2Ds; c++){
            const unsigned int numEdges = mesh.EdgesCell2Ds[c].size();
            VectorXi edges = mesh.EdgesCell2Ds[c];

            for(unsigned int e = 0; e < numEdges; e++){
                const unsigned int origin = mesh.VerticesCell1Ds[edges[e]][0];
                const unsigned int end = mesh.VerticesCell1Ds[edges[e]][1];

                auto findOrigin = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), origin);
                if(findOrigin == mesh.VerticesCell2Ds[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), end);
                if(findEnd == mesh.VerticesCell2Ds[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }
            }
        }
    }
    return true;
}

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh){
    ifstream file;
    file.open(filename);
    if(file.fail()){
        return false;
    }
    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }
    file.close();
    listLines.pop_front();
    mesh.NumberCell0Ds = listLines.size();

    if (mesh.NumberCell0Ds == 0){
        cerr << "There is no cell 0D" << endl;
        return false;
    }
    mesh.IdCell0Ds.reserve(mesh.NumberCell0Ds);
    mesh.CoordinateCell0Ds.reserve(mesh.NumberCell0Ds);

    for (const string& line : listLines){
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        char separator;

        converter >> id >> separator >> marker >> separator >> coord[0] >> separator >> coord[1];
        mesh.IdCell0Ds.push_back(id);
        mesh.CoordinateCell0Ds.push_back(coord);
        if(marker != 0){
            if (mesh.MarkerCell0Ds.find(marker) == mesh.MarkerCell0Ds.end()){
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else{
                mesh.MarkerCell0Ds[marker].push_back(id);
            }
        }
    }
    file.close();
    return true;
}

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh){
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }
    file.close();
    listLines.pop_front();
    mesh.NumberCell1Ds = listLines.size();
    if (mesh.NumberCell1Ds == 0){
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    mesh.IdCell1Ds.reserve(mesh.NumberCell1Ds);
    mesh.VerticesCell1Ds.reserve(mesh.NumberCell1Ds);

    for (const string& line : listLines){
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;
        char separator;
        converter >>  id >> separator >> marker >> separator >> vertices[0] >> separator >> vertices[1];
        mesh.IdCell1Ds.push_back(id);
        mesh.VerticesCell1Ds.push_back(vertices);

        if(marker != 0){
            if (mesh.MarkerCell1Ds.find(marker) == mesh.MarkerCell1Ds.end()){
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else{
                mesh.MarkerCell1Ds[marker].push_back(id);
            }
        }
    }
    return true;
}

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh){
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    file.close();
    listLines.pop_front();

    mesh.NumberCell2Ds = listLines.size();

    if (mesh.NumberCell2Ds == 0){
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.IdCell2Ds.reserve(mesh.NumberCell2Ds);
    mesh.VerticesCell2Ds.reserve(mesh.NumberCell2Ds);
    mesh.EdgesCell2Ds.reserve(mesh.NumberCell2Ds);

    for (const string& line : listLines){
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        unsigned int numVertices;
        char separator;
        converter >> id >> separator >> marker >> separator >> numVertices;
        VectorXi vertices;
        vertices.resize(numVertices);
        for(unsigned int i = 0; i < numVertices; i++)
            converter >> separator >> vertices[i];
        unsigned int numEdges;
        converter >> separator >> numEdges;
        VectorXi edges;
        edges.resize(numEdges);
        for(unsigned int i = 0; i < numEdges; i++)
            converter >> separator >> edges[i];
        mesh.IdCell2Ds.push_back(id);
        mesh.VerticesCell2Ds.push_back(vertices);
        mesh.EdgesCell2Ds.push_back(edges);
    }
    return true;
}

bool TestMesh(PolygonalMesh &mesh){
    bool output = true;
    bool flag = false;

    for(VectorXi v : mesh.VerticesCell1Ds){
        unsigned int point1 = v[0];
        unsigned int point2 = v[1];
        double d = sqrt(square(mesh.CoordinateCell0Ds[point1][0]-mesh.CoordinateCell0Ds[point2][0])+
                        square(mesh.CoordinateCell0Ds[point1][1]-mesh.CoordinateCell0Ds[point2][1]));
        if (d < mesh.tau) {
            cerr << "Error: the distance between point " << point1 << " and point " << point2 << " is zero." << endl;
            flag = true;
            output = false;
        }
    }
    if (!flag){
        cout << "The lengths are all nonzero." << endl;
    }

    flag = false;
    for(unsigned int pol = 0; pol < mesh.NumberCell2Ds; pol++){
        VectorXi vertices = mesh.VerticesCell2Ds[pol];
        double area = 0;
        unsigned int n = vertices.size();
        vector<double> x;
        vector<double> y;
        x.reserve(n);
        y.reserve(n);
        for (unsigned int id : vertices){
            x.push_back(mesh.CoordinateCell0Ds[id][0]);
            y.push_back(mesh.CoordinateCell0Ds[id][1]);
        }
        for (unsigned int i = 0; i < n; i++){
            area += x[i]*y[(i+1)%n] - x[(i+1)%n]*y[i];
        }
        area = 0.5*abs(area);
        if (area < mesh.tol){
            cerr << "Error: the area of the polygon " << pol << " is zero." << endl;
            flag = true;
            output = false;
        }
    }
    if (!flag){
        cout << "The areas are all nonzero." << endl;
    }
    return output;
}
}
