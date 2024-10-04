// TSP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <vector>
#include <math.h>

#define RADIUS_OF_EARTH 6371

using namespace std;


struct City
{
    char name[10];
    double lat, lon;
};
double convertToRadian(double val)
{
    return val * 3.14 / 180;
}
double findDistance(City city1, City city2)
{
    double distance = 0;
    double a, c;
    double latdiff = city2.lat - city1.lat;
    double longdiff = city2.lat - city1.lat;
    a = cos(city1.lat) * cos(city2.lat) * sin((city1.lon - city2.lon) / 2) * sin((city1.lon - city2.lon) / 2);
    a += sin(latdiff / 2) * sin(latdiff / 2);
    c = 2 * atan2(sqrt(a), sqrt(1 - a));
    distance = c * RADIUS_OF_EARTH;
    return distance;
}

double calcTotalDist(vector<City> cities)
{
    double distance = 0;
    for(int i = 0; i<cities.size()-1;i++)
    {
        cout << cities[i].name << " -> ";
        distance += findDistance(cities[i], cities[i + 1]);
    }
    cout <<  cities[cities.size() - 1].name << endl;
    cout << "distance : " << distance << endl <<endl<< flush;
    return distance;
}

double combo(vector<City> v, vector<City> v1)
{
    double mindist = INT_MAX, dist;

    if (0 == v.size())
    {
        dist = calcTotalDist(v1);
        return dist;
    }
    for (int i = 0; i < v.size(); i++)
    {
        vector<City> t = v;
        v1.push_back(*(t.begin() + i));
        t.erase(t.begin() + i);

        dist = combo(t, v1);
        if (dist < mindist)mindist = dist;
        v1.pop_back();
    }
    return mindist;
}

int main()
{
    vector<City> cities, temp;
    City c;
    strcpy(c.name, "Pune");
    c.lat = convertToRadian(18.5204);
    c.lon = convertToRadian(73.8567);
    cities.push_back(c);

    strcpy(c.name, "Mumbai");
    c.lat = convertToRadian(19.0760);
    c.lon = convertToRadian(72.8777);
    cities.push_back(c);

    strcpy(c.name, "Goa");
    c.lat = convertToRadian(15.2993);
    c.lon = convertToRadian(74.1240);
    cities.push_back(c);

    strcpy(c.name, "Bokaro");
    c.lat = convertToRadian(23.6693);
    c.lon = convertToRadian(86.1511);
    cities.push_back(c);

    strcpy(c.name, "Bangalore");
    c.lat = convertToRadian(12.9716);
    c.lon = convertToRadian(77.5946);
    cities.push_back(c);

    /*strcpy(c.name, "Hyderabad");
    c.lat = convertToRadian(17.3840);
    c.lon = convertToRadian(78.4564);
    cities.push_back(c);*/
    
    std::cout << "Min Distance : "<< combo(cities, temp)<<endl<<flush;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
