#include "/usr/local/Packages/libBitIO/include/BitIO.h"

#include "../../include/libPCM.h"
#include "../../include/W64Common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    // To encode W64 i'll need to read the values from the struct and write it to the file
    
    void WriteW64FMT(W64Header *W64, BitOutput *BitO) {
        WriteBits(BitO, W64->FormatType, 16, false);
        WriteBits(BitO, W64->Channels,   16, false);
        WriteBits(BitO, W64->SampleRate, 32, false);
        WriteBits(BitO, W64->ByteRate,   32, false);
        WriteBits(BitO, W64->BlockAlign, 16, false);
        WriteBits(BitO, W64->BitDepth,   16, false);
    }
    
    void WriteW64Header(W64Header *W64, BitOutput *BitO) {
        WriteUUID(BitO, RIFF_UUID);
        // Write the size of the file including all header fields
        uint64_t W64Size = (W64->NumSamples * W64->Channels * W64->BitDepth) + W64HeaderSize;
        WriteBits(BitO, SwapEndian64(W64Size), 64, false); // little endian field
        WriteUUID(BitO, WAVE_UUID);
        WriteUUID(BitO, FMT_UUID);
        uint64_t FMTSize = 40;
        WriteBits(BitO, SwapEndian64(FMTSize), 64, false); // little endian
        WriteW64FMT(W64, BitO);
        WriteUUID(BitO, DATA_UUID);
        WriteBits(BitO, SwapEndian64(W64->NumSamples), 64, false); // little endian
    }
    
#ifdef __cplusplus
}
#endif