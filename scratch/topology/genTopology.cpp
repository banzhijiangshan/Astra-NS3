#include <iostream>
using namespace std;

int
main()
{
    // devices, routers, and links
    cout << "176 48 1024" << endl;
    // routers id
    cout << "128 129 130 131 132 133 134 135 136 137 138 139 140 141 142 143 144 145 146 147 148 "
            "149 150 151 152 153 154 155 156 157 158 159 160 161 162 163 164 165 166 167 168 169 "
            "170 171 172 173 174 175"
         << endl;
    // links
    for (int i = 128; i < 160; i++)
    {
        switch (i % 8)
        {
        case 0:
            for (int j = 0; j < 16; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 1:
            for (int j = 16; j < 32; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 2:
            for (int j = 32; j < 48; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 3:
            for (int j = 48; j < 64; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 4:
            for (int j = 64; j < 80; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 5:
            for (int j = 80; j < 96; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 6:
            for (int j = 96; j < 112; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        case 7:
            for (int j = 112; j < 128; j++)
            {
                cout << j << " " << i << " 100Gbps 0.005ms 0" << endl;
            }
            break;
        }
    }
    for (int i = 160; i < 176; i++)
    {
        for (int j = 128; j < 160; j++)
        {
            cout << i << " " << j << " 200Gbps 0.05ms 0" << endl;
        }
    }
    return 0;
}