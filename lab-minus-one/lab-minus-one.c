#include <stdio.h>

int main() {
    // unsigned char cp[] = {0x6b624277, 0x46476e6f, 0x6b456a6a, 0x61754d47, 0x586e4359, 0x444b7378, 0x4f466c00, 0x5babe4e0, 0x48725571, 0x4a68435a, 0x51444974, 0x00664445};
    unsigned int ip[] = {0x4d495450, 0x6d6a6c54, 0x796d766b, 0x446d5255, 0x7a716352, 0x78794576, 0x534e7000, 0x505b7e18, 0x6b776147, 0x65517474, 0x564b7645, 0x00737843};
    unsigned int x, y;
    unsigned char w, z;

    unsigned int ip1 = 0x446d5255;
    printf("6 0x%08lx\n", (unsigned long)(0x505b7df0 + (4 * 3)));
    printf("7 0x%08lx\n", (unsigned long)(0x505b7df0 + (4 * 8)));
    printf("8 0x%08lx\n", (unsigned long)(0x505b7df0 + (1 * 13)));
    printf("9 0x%08lx\n", (unsigned long)(0x505b7df0 + (1 * 29)));
    //printf("10: %s\n", cp + 18);
    //71 7a 76 45 79 78 00
    //qzvEy

    // 0x564b76
    //  0x00737843
    //printf("11: %s\n", cp + 41);
    //76 4b 56 43 78 73 00
    //vKVCxs

    w = 0x18;
    z = 0xb6;
    printf("12 0x%02x\n", (w & z));

    w = 0x18;
    z = 0xb6;
    printf("13 0x%02x\n", (w | z));

    w = 0x97;
    z = 0xd2;
    printf("14 0x%02x\n", (w ^ z));

    w = 0x39;
    z = 0x78;
    z = ~z;
    printf("15 0x%02x\n", (w ^ z));

    w = 0x39;
    z = (w << 3);
    printf("16 0x%02x\n", z);

    w = 0x89;
    z = (w >> 3);
    printf("17 0x%02x\n", z);

    x = 0x468b6eed;
    y = (x >> 12);
    printf("18 0x%08x\n", y);

    x = 0x7b04d5f6;
    y = (x << 12);
    printf("19 0x%08x\n", y);

    x = 0x8b2b7afc;
    y = 0xf00f0ff0;
    printf("20 0x%08x\n", x | y);

    x = 0x8b2b7afc;
    y = 0xf00f0ff0;
    printf("21 0x%08x\n", x & y);

    x = 0x8b2b7afc;
    y = 0xf00f0ff0;
    printf("22 0x%08x\n", x & (~y));

    x = 0x8b2b7afc;
    y = 0xf00f0ff0;
    printf("23 0x%08x\n", x ^ y);

    unsigned int ip2 = (unsigned int)0x505b7e18;
    printf("24 0x%08x\n", ip2);
}