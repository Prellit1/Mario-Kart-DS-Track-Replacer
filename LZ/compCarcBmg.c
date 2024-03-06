#include "compCarcBmg.h"

/*




*/
// Start Of--------------------------------
/* COPIED FROM NITRO PAINT*/
/*




*/
int COPIEDcompareMemory(char *b1, char *b2, int nMax, int nAbsoluteMax)
{
    int nSame = 0;
    if (nMax > nAbsoluteMax)
        nMax = nAbsoluteMax;
    // count up to nMax. If all match, count 0x12-nMax bytes. The b1 just starts over.
    for (int i = 0; i < nMax; i++)
    {
        if (*(b1++) == *(b2++))
            nSame++;
        else
            break;
    }
    if (nSame == nMax)
    {
        b1 -= nMax;
        for (int i = 0; i < nAbsoluteMax - nMax; i++)
        {
            if (*(b1++) == *(b2++))
                nSame++;
            else
                break;
        }
    }
    return nSame;
}
unsigned char *COPIEDlz77compress(char *buffer, int size, unsigned int *compressedSize)
{
    int compressedMaxSize = 4 + 9 * ((size + 7) >> 3);
    char *compressed = (char *)malloc(compressedMaxSize);
    char *compressedBase = compressed;
    *(unsigned *)compressed = size << 8;
    *compressed = 0x10;
    int nProcessedBytes = 0;
    int nSize = 4;
    compressed += 4;
    while (1)
    {
        // make note of where to store the head for later.
        char *headLocation = compressed;
        compressed++;
        nSize++;
        // initialize the head.
        char head = 0;

        // set when the end of the file is reached, and the result needs to be zero-padded.
        int isDone = 0;

        // repeat 8x (8 bits per byte)
        for (int i = 0; i < 8; i++)
        {
            head <<= 1;

            if (isDone)
            {
                *(compressed++) = 0;
                nSize++;
                continue;
            }

            // search backwards up to 0xFFF bytes.
            int maxSearch = 0x1000;
            if (maxSearch > nProcessedBytes)
                maxSearch = nProcessedBytes;

            // the biggest match, and where it was
            int biggestRun = 0, biggestRunIndex = 0;

            // begin searching backwards.
            for (int j = 2; j < maxSearch; j++)
            {
                // compare up to 0xF bytes, at most j bytes.
                int nCompare = 0x12;
                if (nCompare > j)
                    nCompare = j;
                int nBytesLeft = size - nProcessedBytes;
                int nAbsoluteMaxCompare = 0x12;
                if (nAbsoluteMaxCompare > nBytesLeft)
                    nAbsoluteMaxCompare = nBytesLeft;
                int nMatched = COPIEDcompareMemory(buffer - j, buffer, nCompare, nAbsoluteMaxCompare);
                if (nMatched > biggestRun)
                {
                    if (biggestRun == 0x12)
                        break;
                    biggestRun = nMatched;
                    biggestRunIndex = j;
                }
            }

            // if the biggest run is at least 3, then we use it.
            if (biggestRun >= 3)
            {
                head |= 1;
                nProcessedBytes += biggestRun;
                // encode the match. First byte has high nybble as length minus 3, low nybble as the high byte of the offset.
                *(compressed++) = ((biggestRun - 3) << 4) | (((biggestRunIndex - 1) >> 8) & 0xF);
                *(compressed++) = (biggestRunIndex - 1) & 0xFF;
                // advance the buffer
                buffer += biggestRun;
                nSize += 2;
                if (nProcessedBytes >= size)
                    isDone = 1;
            }
            else
            {
                *(compressed++) = *(buffer++);
                nProcessedBytes++;
                nSize++;
                if (nProcessedBytes >= size)
                    isDone = 1;
            }
        }
        *headLocation = head;
        if (nProcessedBytes >= size)
            break;
    }
    *compressedSize = nSize;
    return realloc(compressedBase, nSize);
}

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
// End Of---------------------------
/*




*/
#define STARTTRACKMSG 45
#define ESBMGCARC 22
#define FRBMGCARC 23
#define GEBMGCARC 24
#define ITBMGCARC 25
#define USBMGCARC 26
unsigned int OffsetToTrackMSG(unsigned int TrackNum, unsigned int *array)
{
    return 0x3c4 - 0x15c + array[(TrackNum + STARTTRACKMSG)];
}

unsigned int lenMSG(unsigned int TrackNum, unsigned int *array)
{
    return (array[TrackNum + STARTTRACKMSG + 1] - array[TrackNum + STARTTRACKMSG]);
}
char *charToCompatChar(char *input, char *buffer, unsigned int *size)
{
    for (unsigned int i = 0; i <= *size; i++) // i think the windows function doesnt take the \0 into account so im gonna forcefully add it
    {
        buffer[i * 2] = input[i];
        buffer[i * 2 + 1] = '\0';
    }
    *size = (*size + 1) * 2;
    return buffer;
}
ADDRFAT getCFatAddresses(char *decompCarc)
{
    unsigned int numFile = *(unsigned int *)&decompCarc[0x18];
    unsigned int size = numFile * 2 * sizeof(int);
    unsigned int *addresses = malloc(size);
    for (int i = 0; i < numFile; i++)
    {
        addresses[i * 2] = *(unsigned int *)&decompCarc[0x1C + i * 8];
        addresses[i * 2 + 1] = *(unsigned int *)&decompCarc[0x1C + i * 8 + 4];

        // printf("%d   %d\n", addresses[i * 2], addresses[i * 2 + 1]);
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
            *(unsigned int *)&decompC[0x1C + i * 4] += delta;
        }
    }
    free(addr.addresses);
}

unsigned int *getAllMSGOffsets(char *bmg)
{

    unsigned int num = getNumBmg(bmg);
    unsigned int *array = calloc(num, sizeof(int));
    while (--num + 1)
    {
        array[num] = *(unsigned int *)&bmg[0x30 + (num * sizeof(int))];
        // printf("%x  %d\n", array[num], num);
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
void modifyArrayOffset(unsigned int *array, unsigned int TrackID, int DeltaS, char *bmg)
{
    unsigned int num = getNumBmg(bmg);
    while (--num + 1)
    {
        if (array[num] > array[TrackID + STARTTRACKMSG])
        {
            array[num] = array[num] + DeltaS;
            *(unsigned int *)&bmg[0x30 + (num * sizeof(int))] = array[num];
        }
    }
}
char *ReplMSG(char *bmg, char *buffer, unsigned int trackID, unsigned int charSize, unsigned int bmgSize, unsigned int *array, int *delta)
{
    int deltaS = (int)charSize - (int)lenMSG(trackID, array);
    int NSize = (int)bmgSize + deltaS;
    *delta = deltaS;
    modifyArrayOffset(array, trackID, deltaS, bmg);
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
        else if (i < 0x15c + bmgAddr.addrStart + (getSize(bmgAddr) + deltaS)) // kibda old size plus the size diff
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
/// ///  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////   THOSE FUNCTIONS ARE SHITTILY WRITTEN
char *replaceATrackName(unsigned int TrackNum, char *DecompedCarc, char *newName, unsigned int charSize, unsigned int *uncompSize)
{
    char *officialBuffer = malloc(512);
    officialBuffer = charToCompatChar(newName, officialBuffer, &charSize);
    unsigned int newSize = charSize;

    char *bmg = getBMGFile(DecompedCarc, getBmgAddr(DecompedCarc));

    unsigned int bmgSize = getSize(getBmgAddr(DecompedCarc));
    unsigned int *array = getAllMSGOffsets(bmg);
    int deltaS;
    bmg = ReplMSG(bmg, officialBuffer, TrackNum, newSize, bmgSize, array, &deltaS);
    DecompedCarc = putBMGinDecompCarc(DecompedCarc, bmg, deltaS, getBmgAddr(DecompedCarc), *uncompSize);
    *uncompSize += deltaS;

    free(bmg);
    free(array);
    free(officialBuffer);
    return DecompedCarc;
}

void MKDSReplTrack(FILE *mkds, int curs, int TrackID, unsigned int charLength, char *charl)
{
    getFat(mkds);
    int language = USBMGCARC - curs;

    fseek(mkds, allTheFat[language].addressEnd, SEEK_SET);
    int size = ftell(mkds);
    fseek(mkds, allTheFat[language].addressStart, SEEK_SET);
    size -= ftell(mkds);

    char *buf = malloc(size);
    fread(buf, size, 1, mkds);

    unsigned int uncompSize;
    char *res = COPIEDlz77decompress(buf, size, &uncompSize);
    res = replaceATrackName(TrackID, res, charl, charLength, &uncompSize);
    unsigned int compSize;
    res = COPIEDlz77compress(res, uncompSize, &compSize);
    putDataInRom(compSize, res, mkds, getCourse(language));

    // free(res);

    free(buf);
}

/* int main()
{

    FILE *file = fopen("EMKDSEU.nds", "rb");
    getFat(file);
    FILE *result = fopen("C2.carc", "wb");
    int uncompSize;
    printf("a\n");
    fseek(file, allTheFat[ITBMGCARC].addressEnd, SEEK_SET);
    int size = ftell(file);
    fseek(file, allTheFat[ITBMGCARC].addressStart, SEEK_SET);
    size -= ftell(file);
    char *buf = malloc(size);
    fread(buf, size, 1, file);
    printf("a\n");
    char *res = COPIEDlz77decompress(buf, size, &uncompSize);
    res = replaceATrackName(0, res, "abcd", 4, &uncompSize);
    int compSize;
    res = COPIEDlz77compress(res, uncompSize, &compSize);

    printf("b\n");
    fwrite(res, uncompSize, 1, result);

    free(buf);
    free(res);
    fclose(file);
    fclose(result);

    return 0;
} */