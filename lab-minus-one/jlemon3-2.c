#include <stdio.h>

int main() {
    // unsigned char cp[] = {0x6b624277, 0x46476e6f, 0x6b456a6a, 0x61754d47, 0x586e4359, 0x444b7378, 0x4f466c00, 0x5babe4e0, 0x48725571, 0x4a68435a, 0x51444974, 0x00664445};
    unsigned int ip[] = {
        0x6b624277,
        0x46476e6f,
        0x6b456a6a,
        0x61754d47,
        0x586e4359,
        0x444b7378,
        0x4f466c00,
        0x5babe4e0,
        0x48725571,
        0x4a68435a,
        0x51444974,
        0x00664445};
    unsigned int x, y;
    unsigned char w, z;

    printf("0 0x%08x\n", ip[8]);
    printf("1 0x%08x\n", ip[11]);
    printf("2 0x%02x\n", 0x6a);
    printf("3 0x%02x\n", 0x73);
    printf("4 0x%02x\n", 0x4f);
    printf("5 0x%02x\n", 0x49);
    printf("6 0x%08lx\n", (unsigned long)(0x5babe4c0 + (4 * 5)));
    printf("7 0x%08lx\n", (unsigned long)(0x5babe4c0 + (4 * 10)));
    printf("8 0x%08lx\n", (unsigned long)(0x5babe4c0 + (1 * 19)));
    printf("9 0x%08lx\n", (unsigned long)(0x5babe4c0 + (1 * 25)));

    // 6e 58 78 73 4b 44 00
    // nXxsKD
    printf("10 %s\n", "nXxsKD");

    // 49 44 51 45 44 66 00
    // IDQEDf
    printf("11 %s\n", "IDQEDf");

    w = 0xb8;
    z = 0xdd;
    printf("12 0x%02x\n", (w & z));

    w = 0xb8;
    z = 0xdd;
    printf("13 0x%02x\n", (w | z));

    w = 0x15;
    z = 0xfe;
    printf("14 0x%02x\n", (w ^ z));

    w = 0xf9;
    z = 0x52;
    z = ~z;
    printf("15 0x%02x\n", (w ^ z));

    w = 0xda;
    z = (w << 3);
    printf("16 0x%02x\n", z);

    w = 0xba;
    z = (w >> 3);
    printf("17 0x%02x\n", z);

    x = 0x0cb74c68;
    y = (x >> 12);
    printf("18 0x%08x\n", y);

    x = 0x7daf99f9;
    y = (x << 12);
    printf("19 0x%08x\n", y);

    x = 0x90bcd784;
    y = 0x0ff0f0f0;
    printf("20 0x%08x\n", x | y);

    x = 0x90bcd784;
    y = 0x0ff0f0f0;
    printf("21 0x%08x\n", x & y);

    x = 0x90bcd784;
    y = 0x0ff0f0f0;
    printf("22 0x%08x\n", x & (~y));

    x = 0x90bcd784;
    y = 0x0ff0f0f0;
    printf("23 0x%08x\n", x ^ y);

    printf("24 0x%08x\n", (0x5babe4c0 + (4 * 7)));
}