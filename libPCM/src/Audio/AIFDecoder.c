#include "../../include/libPCM.h"
#include "../../include/Private/libPCMTypes.h"
#include "../../include/Private/Audio/AIFCommon.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
    /*
     Like WAV, Chunks are padded if their size is not even, and the ChunkSize field contains the real size without the padding
     the ChunkSize field does NOT include the ChunkID or ChunkSize fields.
     */
    
    void AIFParseCOMMChunk(PCMFile *PCM, BitBuffer *BitB) {
        PCM->AUD->NumChannels           = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 16);
        PCM->AUD->NumSamples            = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32); // A SampleFrame is simply a single sample from all channels.
        PCM->AUD->BitDepth              = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 16);
        uint16_t SampleRateExponent     = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 16) - 16446;
        uint64_t SampleRateMantissa     = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 64);
        if (SampleRateExponent >= 0) {
            PCM->AUD->SampleRate        = SampleRateMantissa << SampleRateExponent;
        } else {
            PCM->AUD->SampleRate        = (SampleRateMantissa + ((1ULL << (-SampleRateExponent - 1)) >> (-SampleRateExponent)));
        }
    }
    
    void AIFParseNameChunk(PCMFile *PCM, BitBuffer *BitB) {
        uint32_t AIFFNameSize           = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
        char *SongTitleTagString        = calloc(1, AIFFNameSize * sizeof(char));
        for (uint32_t TagByte = 0UL; TagByte < AIFFNameSize; TagByte++) {
            SongTitleTagString[TagByte] = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 8);
        }
        PCM->AUD->Meta->SongTitleTag    = SongTitleTagString;
    }
    
    void AIFParseAuthorChunk(PCMFile *PCM, BitBuffer *BitB) {
        uint32_t AIFFAuthSize           = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
        char *AuthorString              = calloc(1, AIFFAuthSize * sizeof(char));
        for (uint32_t TagByte = 0UL; TagByte < AIFFAuthSize; TagByte++) {
            AuthorString[TagByte] = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 8);
        }
        PCM->AUD->Meta->ArtistTag       = AuthorString;
    }
    
    void AIFParseAnnotationChunk(PCMFile *PCM, BitBuffer *BitB) {
        uint32_t AIFFAnnoSize           = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
        char *AnnotationString          = calloc(1, AIFFAnnoSize * sizeof(char));
        for (uint32_t TagByte = 0UL; TagByte < AIFFAnnoSize; TagByte++) {
            AnnotationString[TagByte]   = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 8);
        }
        PCM->AUD->Meta->NumANNOChunks  += 1;
        PCM->AUD->Meta->AnnoChunks[PCM->AUD->Meta->NumANNOChunks - 1] = AnnotationString;
    }
    
    void AIFParseMetadata(PCMFile *PCM, BitBuffer *BitB) {
        PCM->AUD->FileSize                              = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
        uint32_t AIFFChunkID                            = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
        if (AIFFChunkID != AIF_AIFF || AIFFChunkID != AIF_AIFC) {
            BitIOLog(LOG_ERROR, libPCMLibraryName, __func__, "Invalid ChunkID %s, It should be AIFF or AIFC", AIFFChunkID);
        } else {
            uint32_t AIFFSubChunkID                     = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
            uint32_t AIFFSubChunkIDSize                 = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
            if (AIFFSubChunkID == AIF_COMM) {
                AIFParseCOMMChunk(PCM, BitB);
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_ID3) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_NAME) {
                AIFParseNameChunk(PCM, BitB);
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_AUTH) {
                AIFParseAuthorChunk(PCM, BitB);
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_MARK) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_INST) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_MIDI) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_AESD) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_AAPL) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_COMT) {
                BitBufferSkip(BitB, Bytes2Bits(AIFFSubChunkIDSize));
                if (IsOdd(AIFFSubChunkIDSize) == Yes) { // Skip the IFF container padding byte
                    BitBufferSkip(BitB, 8);
                    PCM->DataLeft -= 1;
                }
                PCM->DataLeft -= AIFFSubChunkIDSize;
            } else if (AIFFSubChunkID == AIF_SSND) {
                PCM->AUD->AIFOffset    = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
                PCM->AUD->AIFBlockSize = ReadBits(BitIOMSByte, BitIOLSBit, BitB, 32);
                BitBufferSkip(BitB, Bytes2Bits(PCM->AUD->AIFOffset));
            }
        }
    }
    
    uint32_t **AIFExtractSamples(PCMFile *PCM, BitBuffer *BitB, uint64_t NumSamples2Extract) {
        uint64_t   ExtractedSampleSize = NumSamples2Extract * PCM->AUD->NumChannels * PCM->AUD->BitDepth;
        uint32_t **ExtractedSamples    = calloc(1, NumSamples2Extract * sizeof(uint32_t));
        if (ExtractedSamples == NULL) {
            BitIOLog(LOG_ERROR, libPCMLibraryName, __func__, "Not enough memory to allocate a buffer for the extracted samples, %d", ExtractedSampleSize);
        } else {
            for (uint16_t Channel = 0; Channel < PCM->AUD->NumChannels; Channel++) {
                for (uint32_t Sample = 0UL; Sample < NumSamples2Extract; Sample++) {
                    ExtractedSamples[Channel][Sample] = ReadBits(BitIOMSByte, BitIOMSBit, BitB, PCM->AUD->BitDepth);
                    BitBufferSkip(BitB, 8 - (PCM->AUD->BitDepth % 8)); // Skip the Zero'd bits
                }
            }
            if (PCM->AUD->NumSamples == 0 && PCM->AUD->FileSize > 0) {
                BitBufferSkip(BitB, 8); // Skip the IFF container padding byte
            }
        }
        return ExtractedSamples;
    }
    
#ifdef __cplusplus
}
#endif
