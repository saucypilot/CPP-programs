#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

// Define a struct to store flight information
struct Flight
{
    string departureCity;
    string departureTime;
    string departureMeridiem;
    string destinationCity;
};


// Define a struct to store time information
struct Time
{
    int hours;
    int minutes;
    string meridiem;
};

// Define an unordered map to store the time zone differences for various cities
unordered_map<string, int> timeZoneDifference = {
    {"Charlotte", 0},
    {"Orlando", 0},
    {"Houston", -1},
    {"Denver", -2},
    {"San Diego", -3}};

// Function to parse a string into a Time struct
Time parseTime(const string &timeStr)
{
    Time time;
    char trash;
    stringstream ss(timeStr);
    ss >> time.hours >> trash >> time.minutes >> time.meridiem;
    return time;
}

// Function to format a Time struct into a string
string formatTime(const Time &time)
{
    string formattedTime = to_string(time.hours) + ":" + (time.minutes < 10 ? "0" : "") + to_string(time.minutes) + " " + time.meridiem;
    return formattedTime;
}

// Function to calculate the arrival time given the departure time and the time zone difference
Time calculateArrivalTime(const Time &departureTime, int diff, int duration)
{
    Time arrivalTime;
    arrivalTime.hours = departureTime.hours + diff + (duration / 60);
    arrivalTime.minutes = departureTime.minutes + (duration % 60);
    arrivalTime.meridiem = departureTime.meridiem;

    // Handle minute overflow
    if (arrivalTime.minutes >= 60)
    {
        arrivalTime.minutes -= 60;
        arrivalTime.hours++;
    }

    // Handle hour overflow and meridiem change
    int meridiem_changes = 0;
    while (arrivalTime.hours >= 12)
    {
        arrivalTime.hours -= 12;
        meridiem_changes++;
    }

    while (meridiem_changes > 0)
    {
        arrivalTime.meridiem = (arrivalTime.meridiem == "A.M.") ? "P.M." : "A.M.";
        meridiem_changes--;
    }

    return arrivalTime;
}

int main()
{
    // Open the input file
    string fileName;
    cout << "Enter file name: ";
    cin >> fileName;

    ifstream inFile(fileName);
    if (!inFile)
    {
        // If the file cannot be opened, print an error message and exit
        cerr << "Unable to open file: " << fileName << endl;
        return 1;
    }

    // Read the number of flights from the input file
    int numFlights;
    inFile >> numFlights;

    // Create a vector to store flight information for each flight
    vector<Flight> flights(numFlights);
    for (int i = 0; i < numFlights; i++)
    {
        // Read the flight information from the input file
        inFile >> flights[i].departureCity >> flights[i].departureTime >> flights[i].departureMeridiem >> flights[i].destinationCity;
    }

    // Close the input file
    inFile.close();

    // Calculate and print the arrival time for each flight
    vector<int> flightDurations = {181, 89, 83}; // Flight durations in minutes
    for (int i = 0; i < numFlights; i++)
    {
        // Parse the departure time and calculate the time zone difference
        Time departureTime = parseTime(flights[i].departureTime + " " + flights[i].departureMeridiem);
        int diff = timeZoneDifference[flights[i].destinationCity] - timeZoneDifference[flights[i].departureCity];

        // Calculate the arrival time and print the flight information
        Time arrivalTime = calculateArrivalTime(departureTime, diff, flightDurations[i]);

        cout << "Flight " << i + 1 << ": "
             << flights[i].departureCity << " " << formatTime(departureTime) << " "
             << flights[i].destinationCity << " " << formatTime(arrivalTime) << endl;
    }

    return 0;
}