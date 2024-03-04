#include "compCarcBmg.h"

/*




*/
// Start Of
/* COPIED FROM NITRO PAINT*/
/*




*/
char *COPIEDlz77decompress(char *buffer, int size, unsigned int *uncompressedSize)
{
    // decompress the input buffer.
    // input is invalid if the size is less than 4.
    if (size < 4)
        return NULL;

    // find the length of the decompressed buffer.
    uint32_t length = *(uint32_t *)(buffer + 1) & 0xFFFFFF;

    // create a buffer for the decompressed buffer
    char *result = (char *)malloc(length);
    if (result == NULL)
        return NULL;
    *uncompressedSize = length;

    // initialize variables
    uint32_t offset = 4;
    uint32_t dstOffset = 0;
    while (1)
    {
        uint8_t head = buffer[offset];
        offset++;
        // loop 8 times
        for (int i = 0; i < 8; i++)
        {
            int flag = head >> 7;
            head <<= 1;

            if (!flag)
            {
                result[dstOffset] = buffer[offset];
                dstOffset++, offset++;
                if (dstOffset == length)
                    return result;
            }
            else
            {
                uint8_t high = buffer[offset++];
                uint8_t low = buffer[offset++];

                // length of uncompressed chunk and offset
                uint32_t offs = (((high & 0xF) << 8) | low) + 1;
                uint32_t len = (high >> 4) + 3;
                for (uint32_t j = 0; j < len; j++)
                {
                    result[dstOffset] = result[dstOffset - offs];
                    dstOffset++;
                    if (dstOffset == length)
                        return result;
                }
            }
        }
    }
    return result;
}

/* char *lz77HeaderDecompress(char *buffer, int size, int *uncompressedSize)
{
    if (size < 8)
        return NULL;
    return lz77decompress(buffer + 4, size - 4, uncompressedSize);
} */

/*




*/
/* COPIED FROM NITRO PAINT*/
// End Of
/*




*/
#define STARTTRACKMSG 45
#define ESBMGCARC 20
#define FRBMGCARC 21
#define GEBMGCARC 22
#define ITBMGCARC 23
#define USBMGCARC 24
unsigned int OffsetToTrackMSG(unsigned int TrackNum, unsigned int *array)
{
    return 0x3c4 + array[TrackNum + STARTTRACKMSG];
}

unsigned int lenMSG(unsigned int TrackNum, unsigned int *array)
{
    return (array[TrackNum + STARTTRACKMSG + 1] - array[TrackNum + STARTTRACKMSG]);
}
unsigned int charToCompatChar(char *input, char *buffer, unsigned int size)
{
    for (unsigned int i = 0; i <= size; i++) // i think the windows function doesnt take the \0 into account so im gonna forcefully add it
    {
        buffer[i * 2] = input[i];
        buffer[i * 2 + 1] = '\0';
    }
    return (size + 1) * 2;
}
ADDRFAT getCFatAddresses(char *decompCarc)
{
    unsigned int numFile = *(unsigned int *)&decompCarc[0x18];
    unsigned int size = numFile * 2 * sizeof(int);
    unsigned int *addresses = malloc(size);
    for (int i = 0; i < numFile; i++)
    {
        addresses[i * 2] = *(unsigned int *)&decompCarc[0x18 + i * 8];
        addresses[i * 2 + 1] = *(unsigned int *)&decompCarc[0x18 + i * 8 + 4];
    }
    ADDRFAT res = {numFile, addresses};
    return res;
}
void moveCAddr(char *decompC, ENTRY bmg, int delta)
{
    ADDRFAT addr = getCFatAddresses(decompC);

    for (int i = 0; i < (addr.numFile * 2); i++)
    {
        if (addr.addresses[i] > bmg.addrStart)
        {
            *(unsigned int *)&decompC[0x18 + i * 4] += delta;
        }
    }
    free(addr.addresses);
}

unsigned int *getAllMSGOffsets(char *bmg)
{

    unsigned int num = getNumBmg(bmg);
    unsigned int *array = calloc(num, sizeof(int));
    while (--num)
    {
        array[num] = *(unsigned int *)&bmg[0x30 + (num * sizeof(int))];
    }
    return array;
}

ENTRY getBmgAddr(char *DecompedCarc)
{
    return *(ENTRY *)&DecompedCarc[0x1c]; // hackish
}

char *getBMGFile(char *DecompedCarc, ENTRY bmg)
{
    unsigned int size = getSize(bmg);
    char *result = malloc(size);
    for (int i = 0; i < size; i++)
    {
        result[i] = DecompedCarc[0x15c + bmg.addrStart + i]; // hackish
    }
    return result;
}

char *ReplMSG(char *bmg, char *buffer, unsigned int trackID, unsigned int charSize, unsigned int bmgSize, unsigned int *array, int *delta)
{
    int deltaS = (int)charSize - (int)lenMSG(trackID, array);
    int NSize = (int)bmgSize + deltaS;
    *delta = deltaS;
    char *result = malloc(NSize);
    for (int i = 0; i < NSize; i++)
    {
        if (i < OffsetToTrackMSG(trackID, array))
        {
            result[i] = bmg[i];
        }
        else if (i < OffsetToTrackMSG(trackID, array) + charSize)
        {
            result[i] = buffer[i - OffsetToTrackMSG(trackID, array)];
        }
        else
        {
            result[i] = bmg[i - deltaS];
        }
    }
    free(bmg);
    return result;
}
char *putBMGinDecompCarc(char *DecompC, char *bmg, int deltaS, ENTRY bmgAddr, int uncompSize)
{
    int NSize = uncompSize + deltaS;
    char *result = malloc(NSize);
    for (int i = 0; i < NSize; i++)
    {
        if (i < 0x15c + bmgAddr.addrStart)
        {
            result[i] = DecompC[i];
        }
        else if (i < 0x15c + bmgAddr.addrStart + getSize(bmgAddr))
        {
            result[i] = bmg[i - (0x15c + bmgAddr.addrStart)];
        }
        else
        {
            result[i] = DecompC[i - deltaS];
        }
    }
    moveCAddr(result, bmgAddr, deltaS);
    free(DecompC);
    return result;
}
void replaceATrackName(unsigned int TrackNum, char *DecompedCarc, char *newName, unsigned int charSize, int uncompSize)
{
    char officialBuffer[512];
    unsigned int newSize = charToCompatChar(newName, officialBuffer, charSize);

    char *bmg = getBMGFile(DecompedCarc, getBmgAddr(DecompedCarc));

    unsigned int bmgSize = getSize(getBmgAddr(DecompedCarc));
    unsigned int *array = getAllMSGOffsets(bmg);
    int deltaS;
    bmg = ReplMSG(bmg, officialBuffer, TrackNum, newSize, bmgSize, array, &deltaS);
    DecompedCarc = putBMGinDecompCarc(DecompedCarc, bmg, deltaS, getBmgAddr(DecompedCarc), uncompSize);
    free(bmg);
    free(array);
}

int main()
{
    FILE *file = fopen("C1.carc", "rb");
    FILE *result = fopen("C2.carc", "wb");
    int uncompSize;
    printf("a\n");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buf = malloc(size);
    fread(buf, size, 1, file);
    printf("a\n");
    char *res = COPIEDlz77decompress(buf, size, &uncompSize);

    printf("a\n");
    fwrite(res, uncompSize, 1, result);

    free(buf);
    free(res);
    fclose(file);
    fclose(result);

    file = fopen("C11.carc", "rb");
    result = fopen("C22.carc", "wb");
    printf("a\n");
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *buf = malloc(size);
    fread(buf, size, 1, file);
    printf("a\n");
    *res = COPIEDlz77decompress(buf, size, &uncompSize);
    printf("a\n");
    fwrite(res, uncompSize, 1, result);

    free(buf);
    free(res);
    fclose(file);
    fclose(result);

    return 0;
}