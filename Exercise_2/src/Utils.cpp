#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <limits>


namespace PolygonalLibrary {

bool ImportMesh(const string& filepath, PolygonalMesh& mesh, double &resizing)
{

    //Importazione celle 0D
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh))
    {
        cerr << "Unable to import cell 0D" << endl;
        return false;
    }
    else
    {
        //Test 1:Memorizzazione corretta
        cout << "Test 1: " << endl;
        cout << "Cell0D marker: " << endl;

        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "Key: " << it -> first << " Values: ";

            for(const unsigned int id : it -> second)                
                cout << " " << id;

            cout << endl;
        }
        cout << endl;
    }



    //Importazione celle 1D
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh))
    {
        cerr << "Unable to import cell 1D" << endl;
        return false;
    }
    else
    {
        //Test 2:Memorizzazione corretta
        cout << "Test 2: " << endl;
        cout << "Cell1D marker: " << endl;

        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "Key: " << it -> first << " Values: ";

            for(const unsigned int id : it -> second)
                cout << " " << id;

            cout << endl;
        }
        cout << endl;
    }



    //Controllo lati di lunghezza non nulla

    cout << "Test 3: ";
    cout.flush();

    if(!CheckLength(mesh, resizing))
        return false;

    cout << "Valid length" << endl;

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",mesh))
        return false;

    cout << "Import of 2D cells was successful." << endl;
    cout << endl;



    // Corrispondenza tra i lati 1D e 2D.

    cout << "Test 4: ";
    cout.flush();
    if(!CheckVertices(mesh))
        return false;

    cout << "Valid correspondence between 1D and 2D sides." << endl;
    cout << endl;



    // Aree poligoni non nulle.

    cout << "Test 5: ";
    cout.flush();
    if(!CheckArea(mesh, resizing))
        return false;

    cout << "Valid polygon area." << endl;
    cout << endl;


    return true;
}



bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))

        listLines.push_back(line);


    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string token;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        getline(converter,token,';');
        istringstream(token)>>id;

        getline(converter,token,';');
        istringstream(token)>>marker;

        getline(converter,token,';');
        istringstream(token)>>coord(0);

        getline(converter,token,';');
        istringstream(token)>>coord(1);;

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if( marker != 0)
        {
            auto result = mesh.Cell0DMarkers.insert({marker, {id}});

            if(!result.second)
                (result.first)->second.push_back(id);
        }

    }

    file.close();

    return true;
}





bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string token;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        getline(converter,token,';');
        istringstream(token)>>id;

        getline(converter,token,';');
        istringstream(token)>>marker;

        getline(converter,token,';');
        istringstream(token)>>vertices(0);

        getline(converter,token,';');
        istringstream(token)>>vertices(1);;

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if( marker != 0)
        {          
            auto result = mesh.Cell1DMarkers.insert({marker, {id}});

            if(!result.second)

                (result.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}



bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;

    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string token;
        unsigned int id;
        unsigned int sizeVer;
        unsigned int sizeEdg;
        VectorXi vertices;
        VectorXi edges;

        getline(converter,token,';');
        istringstream(token)>>id;

        getline(converter,token,';');

        getline(converter,token,';');
        istringstream(token)>>sizeVer;
        vertices.resize(sizeVer);

        for(unsigned int i=0; i<sizeVer; i++)
        {
            getline(converter,token,';');
            istringstream(token)>>vertices(i);
        }

        getline(converter,token,';');
        istringstream(token)>>sizeEdg;
        edges.resize(sizeEdg);

        for(unsigned int i=0; i<sizeEdg; i++)
        {
            getline(converter,token,';');
            istringstream(token)>>edges(i);
        }

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }

    file.close();

    return true;
}


bool CheckLength(PolygonalMesh& mesh, double &resizing)
{

    double epsilon = numeric_limits<double>::epsilon() * resizing;

    for(const auto& points : mesh.Cell1DVertices)
    {
        Vector2d coord_1 = mesh.Cell0DCoordinates[points(0)];
        Vector2d coord_2 = mesh.Cell0DCoordinates[points(1)];


        if(abs(coord_1(0) - coord_2(0)) < epsilon && abs(coord_1(1) - coord_2(1)) < epsilon)
        {
            cerr << "There are sides of zero length." << endl;

            return false;
        }
    }

    return true;
}




bool CheckVertices(PolygonalMesh& mesh)
{
    for(const auto& vertices : mesh.Cell2DVertices)
    {
        bool found = false;

        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            // Confronto
            if(i != vertices.size() - 1)
            {
                Vector2i to_find (vertices(i), vertices(i + 1));
                for(const auto& vertices_couple : mesh.Cell1DVertices)
                {

                    Vector2i swapped(vertices_couple[1],vertices_couple[0]);
                    if(to_find == vertices_couple || to_find == swapped)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    cerr << "2D sides do not match 1D sides." << endl;

                    return false;
                }
            }

            // Confronto
            else
            {
                Vector2i to_find(vertices(i), vertices(0));
                for(const auto& vertices_couple : mesh.Cell1DVertices)
                {
                    Vector2i swapped(vertices_couple[1],vertices_couple[0]);
                    if(to_find == vertices_couple || to_find == swapped)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    cerr << "2D sides do not match 1D sides." << endl;

                    return false;
                }
            }
        }
    }
    return true;
}

bool CheckArea(PolygonalMesh& mesh, double &resizing)
{
    double epsilon = numeric_limits<double>::epsilon() * resizing;


    for(const auto& edges : mesh.Cell2DEdges)
    {
        unsigned int counter_zeros = 0;
        for(unsigned int i = 0; i < edges.size(); i++)
        {
            if(i != edges.size() - 1)
            {
                Vector2i points_edge_1 = mesh.Cell1DVertices[edges(i)];
                Vector2d coordinate_A = mesh.Cell0DCoordinates[points_edge_1(0)];
                Vector2d coordinate_B = mesh.Cell0DCoordinates[points_edge_1(1)];
                Vector2d length_1 = coordinate_B - coordinate_A;

                Vector2i points_edge_2 = mesh.Cell1DVertices[edges(i+1)];
                Vector2d coordinate_C = mesh.Cell0DCoordinates[points_edge_2(0)];
                Vector2d coordinate_D = mesh.Cell0DCoordinates[points_edge_2(1)];
                Vector2d length_2 = coordinate_D-coordinate_C;

                double cross_product = length_1.x() * length_2.y() - length_1.y() * length_2.x();
                if(abs(cross_product) < epsilon)
                    counter_zeros ++;

            }


            else
            {
                Vector2i points_edge_1 = mesh.Cell1DVertices[edges(i)];
                Vector2d coordinate_A = mesh.Cell0DCoordinates[points_edge_1(0)];
                Vector2d coordinate_B = mesh.Cell0DCoordinates[points_edge_1(1)];
                Vector2d length_1 = coordinate_B - coordinate_A;

                Vector2i points_edge_2 = mesh.Cell1DVertices[edges(0)];
                Vector2d coordinate_D = mesh.Cell0DCoordinates[points_edge_2(0)];
                Vector2d coordinate_C = mesh.Cell0DCoordinates[points_edge_2(1)];
                Vector2d length_2 = coordinate_C-coordinate_D;

                double cross_product = length_1.x() * length_2.y() - length_1.y() * length_2.x();
                if(abs(cross_product) < epsilon)
                    counter_zeros ++;

            }

            if(counter_zeros == edges.size())
            {
                cerr << "There are polygons with zero area." << endl;

                return false;
            }

        }
    }

    return true;
}


}

