/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Taras Demchyna
 * 912310109
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement
#include <sstream>
#include <string>
#include <iterator>

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;

    bool operator==(Point point)
    {
        return (x == point.x) && (y == point.y);
    }
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {

    //YOUR CODE HERE
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {

    //YOUR CODE HERE
    return p1.x < p2.x;

}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {

    //YOUR CODE HERE
    return p1.y < p2.y;

}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {

    //YOUR CODE HERE
    pair<Point, Point> closest = { points.front(),points.at(1)};
    for (int i = start; i < end; i++)
    {
        for (int j = i + 1; j <= end; j++)
        {
            if (distance(points[i], points[j]) < distance(closest.first, closest.second))
            {
                closest = { points[i], points[j] };
            }
        }
    }
    return closest;
}
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {
    if (end - start <= 3) {
        return bruteForceClosestPair(points, start, end);
    }

    int mid = (start + end) / 2;

    pair<Point, Point> closestLeft = closestPair(points, start, mid);
    pair<Point, Point> closestRight = closestPair(points, mid, end);

    double sigma = min(distance(closestLeft.first, closestLeft.second), distance(closestRight.first, closestRight.second));

    pair<Point, Point> closest = sigma == distance(closestLeft.first, closestLeft.second) ? closestLeft : closestRight;

    vector<Point> strip;
    double midX = points[mid].x;
    for (const Point& point : points) {
        if (abs(point.x - midX) < sigma) {
            strip.push_back(point);
        }
    }
    sort(strip.begin(), strip.end(), compareY);

    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < sigma; ++j) {
            double currentDist = distance(strip[i], strip[j]);
            if (currentDist < sigma) {
                sigma = currentDist;
                closest = { strip[i], strip[j] };
            }
        }
    }

    return closest;
}



// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {

    //YOUR CODE HERE
    vector<Point> result;
    for (int i = 0; i < point_vector.size(); i++)
    {
        if (!(point_vector[i] == point_pair.first || point_vector[i] == point_pair.second))
        {
            result.push_back(point_vector[i]);
        }
    }
    return result;
}

void fixPair(pair<Point, Point>& pair)
{
    if ((pair.first.y > pair.second.y) || (pair.first.y == pair.second.y && pair.first.x > pair.second.x))
    {
        Point buf = pair.first;
        pair.first = pair.second;
        pair.second = buf;
    }
}
// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    vector<pair<Point,Point>> pairs; //add your pairs to this vector
    Point unconnected = {-1,-1}; //set this as the unconnected point if it exists, leave as is otherwise.

    //YOUR CODE HERE
    while (points.size() > 0)
    {
        pair<Point, Point> pairFound = closestPair(points, 0, (int)points.size() - 1);
        pairs.push_back(pairFound);
        points = removePairFromVector(points, pairFound);

        if (points.size() == 1)
        {
            unconnected = points[0];
            points.pop_back();
        }
    }

    //before printing, please make sure that within each pair, the city with the smaller y coordinate is printed first...
    //...if the y coordinates are the same, print the city with the smaller x coordinate first.

    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        fixPair(pairs[i]);
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }


}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
void readCoordinatesFromFile(const string& filename,vector<Point>& points)
{
    //YOUR CODE HERE
    ifstream openFile(filename);
    if (!openFile)
    {
        cerr << "Error opening file!" << endl;
    }
    string line;
    while (getline(openFile, line))
    {
        istringstream stream(line);
        double x, y;
        if (stream >> x >> y)
        {
            Point newPoint = { x,y };
            points.push_back(newPoint);
        }
        else
        {
            cerr << "Error parsing the line: " << line << endl;
        }
    }
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    //vector<Point> points = readCoordinatesFromFile(argv[1]);
    vector<Point> points;
    readCoordinatesFromFile("case0.txt",points);
    findClosestPairOrder(points);
    return 0;
}
