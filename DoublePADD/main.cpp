#include "experimental/xrt_bo.h"
#include "experimental/xrt_device.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_ip.h"
#include <fstream>
#include <stdio.h>
#include <time.h>
#include "Zp.h"

int main()
{
    unsigned int device_index = 0;
    auto device = xrt::device(device_index);
    auto uuid = device.load_xclbin("PippengerU250.xclbin");
    auto ip = xrt::ip(device, uuid, "PippengerU250");

    xrt::bo bufIn = xrt::bo(device, 192l * (1 << 26), xrt::bo::flags::normal, 1);
    xrt::bo bufOut = xrt::bo(device, 256 * 9, xrt::bo::flags::normal, 1);
    char *bufInData = bufIn.map<char *>();
    unsigned int *bufOutData = bufOut.map<unsigned int *>();
    unsigned long long bufInAddr = bufIn.address();
    unsigned long long bufOutAddr = bufOut.address();

    ip.write_register(0x20, bufInAddr);
    ip.write_register(0x24, bufInAddr >> 32);

    std::ifstream inFile("parameters.dat", std::ios::binary);
    inFile.read(bufInData, 192l * (1 << 26));
    bufIn.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    inFile.close();

    printf("Calculate start.\n");

    clock_t start;
    clock_t end;
    float costTime;

    for (int i = 0; i < 9; i++)
    {
        ip.write_register(0x18, (1 << (18 + i)) - 1);
        ip.write_register(0x28, bufOutAddr + 256 * i);
        ip.write_register(0x2c, (bufOutAddr + 256 * i) >> 32);

        start = clock();
        ip.write_register(0x10, 0x1);
        while (ip.read_register(0x10))
            ;
        end = clock();
        costTime = float(end - start) * 1000 / CLOCKS_PER_SEC;
        printf("N = 2 ^ %d, cost time is %fms.\n", 18 + i, costTime);
    }

    printf("Calculate over.\n");

    std::ifstream goldenFile("golden.dat", std::ios::binary);
    unsigned int golden[64 * 9];
    goldenFile.read((char *)golden, 256 * 9);
    goldenFile.close();

    bufOut.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    unsigned int zInv[12];

    for (int i = 0; i < 9; i++)
    {
        zp.minv(bufOutData + i * 64 + 32, zInv);
        zp.mmul(bufOutData + i * 64 + 0, zInv, bufOutData + i * 64 + 0);
        zp.mmul(bufOutData + i * 64 + 16, zInv, bufOutData + i * 64 + 16);
        zp.mmul(bufOutData + i * 64 + 32, zInv, bufOutData + i * 64 + 32);
        zp.mmul(bufOutData + i * 64 + 48, zInv, bufOutData + i * 64 + 48);

        printf("N = 2 ^ %d\n", 18 + i);
        printf("Golden is: \nX = 0x");
        print(golden + i * 64 + 0, 12);
        printf("\nY = 0x");
        print(golden + i * 64 + 16, 12);
        printf("\nT = 0x");
        print(golden + i * 64 + 48, 12);
        printf("\nOutput is: \nX = 0x");
        print(bufOutData + i * 64 + 0, 12);
        printf("\nY = 0x");
        print(bufOutData + i * 64 + 16, 12);
        printf("\nT = 0x");
        print(bufOutData + i * 64 + 48, 12);
        printf("\n");
    }

    if (memcmp(golden, bufOutData, 256 * 9) == 0)
        std::cout << "They are identical.\n";
    else
        std::cout << "They are not identical.\n";
}