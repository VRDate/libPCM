#include "../Dependencies/BitIO/libBitIO/include/CommandLineIO.h"

#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    enum CommandLineSwitchNames {
        Input   = 0,
        Output  = 1,
        LogFile = 2,
        Silence = 3,
        Help    = 4,
    };
    
    CommandLineIO *SetTrimSilenceOptions(void) {
        CommandLineIO     *CLI = CommandLineIOInit(4);
        
        SetCLIName(CLI, "TrimSilence");
        SetCLIVersion(CLI, TrimSilenceVersion);
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLICopyright(CLI, "2017-2017");
        SetCLIDescription(CLI, "PCM silence remover written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD", "Permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCLIMinArguments(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input");
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: '-'\n");
        SetCLISwitchAsIndependent(CLI, Input);
        
        SetCLISwitchFlag(CLI, Output, "Output");
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: '-'\n");
        SetCLISwitchAsIndependent(CLI, Output);
        
        SetCLISwitchFlag(CLI, LogFile, "LogFile");
        SetCLISwitchDescription(CLI, LogFile, "Where should the logs be written? if unspecified, logs are written to STDERR");
        SetCLISwitchAsIndependent(CLI, LogFile);
        
        SetCLISwitchFlag(CLI, Silence, "Silence");
        SetCLISwitchDescription(CLI, Silence, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetCLISwitchAsIndependent(CLI, Silence);
        
        SetCLISwitchFlag(CLI, Help, "Help");
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchAsIndependent(CLI, Help);
        
        return CLI;
    }
    
    void RemoveEmptySamples(uint32_t NumChannels, uint32_t NumSamples, uint32_t **AudioSamples) {
        uint32_t  CurrentSampleIndex = 0UL;
        uint32_t *CurrentSampleValue = calloc(NumChannels, sizeof(uint32_t));
        for (uint32_t Channel = 0UL; Channel < NumChannels; Channel++) {
            for (uint32_t Sample = 0UL; Sample < NumSamples; Sample++) {
                // Ok, so we need to check each sample to see if it is 0, and if it is, make sure all samples in that group are zer0, then loop until we find a non-zero sample.
                while (AudioSamples[Channel][Sample] == 0x0) {
                    CurrentSampleIndex += 1;
                }
            }
        }
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineIO *CLI         = SetTrimSilenceOptions();
        ParseCommandLineArguments(CLI, argc, argv);
        BitInput            *BitI  = BitInputInit();
        BitOutput           *BitO  = BitOutputInit();
        PCMFile             *PCM   = PCMFileInit();
        BitBuffer           *BitB  = BitBufferInit(40);
        
        uint64_t InputFileArg      = GetCLIArgumentNumWithIndependentAndDependents(CLI, Input, 0);
        uint64_t OutputFileArg     = GetCLIArgumentNumWithIndependentAndDependents(CLI, Output, 0);
        
        char *OutputPath           = GetCLIArgumentResult(CLI, OutputFileArg);
        char *OutputExtension      = GetExtensionFromPath(OutputPath);
        
        if (strcasecmp(OutputExtension, "wav") == 0) {
            PCMSetOutputFileType(PCM, WAVFormat);
        } else if (strcasecmp(OutputExtension, "w64") == 0) {
            PCMSetOutputFileType(PCM, W64Format);
        } else if (strcasecmp(OutputExtension, "aif") == 0) {
            PCMSetOutputFileType(PCM, AIFFormat);
        } else if (strcasecmp(OutputExtension, "aiff") == 0) {
            PCMSetOutputFileType(PCM, AIFFormat);
        } else if (strcasecmp(OutputExtension, "aifc") == 0) {
            PCMSetOutputFileType(PCM, AIFFormat);
        } else {
            BitIOLog(LOG_ERROR, "TrimSilence", "main", "Unrecognized extension: %s", OutputExtension);
        }
        
        // So now we go ahead and mess around with the samples, looking for empty SampleGroups, then write it all out with the generic Write functions that I need to write.
        
        BitInputOpenFile(BitI,  GetCLIArgumentResult(CLI, InputFileArg));
        BitOutputOpenFile(BitO, GetCLIArgumentResult(CLI, OutputFileArg));
        
        IdentifyPCMFile(PCM, BitB);
        ParsePCMMetadata(PCM, BitB);
        
        uint32_t **AudioSamples = NULL; // Lets read 4096 samples at a time.
        AudioSamples = ExtractSamples(PCM, BitB, 4096);
        
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
