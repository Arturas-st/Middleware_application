#include <cstdint>
#include <cstring>
#include <pthread.h>
#include <iostream>
#include <asio.hpp>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

//Defines the object data structure
//Struct stores the incoming object’s full state
struct Object {
    long long id;             // 64-bit
    int x, y;                 // 32-bit each
    unsigned char type;       // 8-bit
    unsigned char color[3];   // RGB: 3 bytes
};

//Map container
//No two mapped values can have the same keys
//When an object update arrives with the same id, we overwrite the older one
map<long long, Object> object_map;

//Function to apply color rules
//Sets the RGB color using hex values
void set_color(Object &obj)
{
    //Calculates how far the point (x, y) is from (150, 150)
    double dist = sqrt(pow(obj.x - 150, 2) + pow(obj.y - 150, 2));

    if (obj.type == 1) {
        if (dist < 50)
            //Copies 3 bytes of data from the string into the obj.color array
            memcpy(obj.color, "\x5B\x31\x6D", 3); // RED
        else if (dist < 75)
            memcpy(obj.color, "\x5B\x34\x6D", 3); // BLUE
    } else if (obj.type == 2) {
        if (dist < 100)
            memcpy(obj.color, "\x5B\x31\x6D", 3); // RED
        else
            memcpy(obj.color, "\x5B\x33\x6D", 3); // YELLOW
    } else if (obj.type == 3) {
        if (dist < 50)
            memcpy(obj.color, "\x5B\x33\x6D", 3); // YELLOW
        else
            memcpy(obj.color, "\x5B\x34\x6D", 3); // BLUE
    }
}

//Outputs the objects in binary format
void output_binary() {
    uint32_t preamble = 0xFEE0;     //Marker to indicate the start of a binary message
    uint32_t count = object_map.size(); //Lets the client know how many objects to expect in the stream

    cout.write((char*)&preamble, sizeof(preamble));
    cout.write((char*)&count, sizeof(count));

    //Iterates over each object in map
    for (auto &[id, obj] : object_map) {
        cout.write((char*)&obj.id, sizeof(obj.id));
        cout.write((char*)&obj.x, sizeof(obj.x));
        cout.write((char*)&obj.y, sizeof(obj.y));
        cout.write((char*)&obj.type, sizeof(obj.type));
        cout.write((char*)obj.color, 3);
    }

    cout.flush(); //Ensures all the buffered binary data is sent to the pipe(or over the network)
}
int main()
{
    //TCP connection. Server will send object data as plain text via TCP.
    //Connects to the server and reads data from it using ASIO library.
    asio::ip::tcp::iostream input("localhost", "5463");
    if (!input) {
        clog << "Failed to connect to server.\n";
        return 1;
    }


    string line;
    auto last_output_time = chrono::steady_clock::now();

    //Continuously reads input
    while (getline(input, line)) {
        Object obj;
        //Reads formatted data
        sscanf(line.c_str(), "ID=%lld;X=%d;Y=%d;TYPE=%hhu", &obj.id, &obj.x, &obj.y, &obj.type);
        set_color(obj);
        //Updates objects
        object_map[obj.id] = obj;

        auto now = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(now - last_output_time).count();
        //Outputs binary every 1.67s
        if (elapsed >= 1670) {
            output_binary();
            //Updates last output time
            last_output_time = now;
        }
    }

    return 0;
}
