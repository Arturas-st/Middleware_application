#include <iostream>
#include <cstdint>

using namespace std;

int main() {
    //Client receives and reads the binary data from middleware at fixed intervals
    while(true)
    {
        uint32_t preamble, count;
        cin.read((char*)&preamble, sizeof(preamble));
        cin.read((char*)&count, sizeof(count));

        cout << "Preamble: " << hex << preamble << endl;
        cout << "Object count: " << dec << count << endl;

        for (uint32_t i = 0; i < count; ++i) {
            long long id;
            int x, y;
            unsigned char type, color[3];
            cin.read((char*)&id, sizeof(id));
            cin.read((char*)&x, sizeof(x));
            cin.read((char*)&y, sizeof(y));
            cin.read((char*)&type, sizeof(type));
            cin.read((char*)color, 3);
            cout << "Object " << i + 1 << ": ID=" << id << ", X=" << x << ", Y=" << y
                    << ", TYPE=" << (int)type << ", COLOR=["
                    << hex << (int)color[0] << " " << (int)color[1] << " " << (int)color[2] << "]\n";
        }
        
    }
    clog << "Stream ended or error occurred.\n";
    return 0;
    
}