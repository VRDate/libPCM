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
        CommandLineIO     *CLI = CommandLineIO_Init(5);
        
        CLISetName(CLI, "TrimSilence");
        CLISetVersion(CLI, TrimSilenceVersion);
        CLISetAuthor(CLI, "BumbleBritches57");
        CLISetCopyright(CLI, "2017 - 2017");
        CLISetDescription(CLI, "PCM silence remover written from scratch in modern C");
        CLISetLicense(CLI, "Revised BSD", "Permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        CLISetMinArguments(CLI, 3);
        CLISetHelpSwitch(CLI, Help);
        
        CLISetSwitchFlag(CLI, Input, "Input");
        CLISetSwitchDescription(CLI, Input, "Input file or stdin with: -Input -");
        CLISetSwitchType(CLI, Input, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, Output, "Output");
        CLISetSwitchDescription(CLI, Output, "Output file or stdout with: -Output -");
        CLISetSwitchType(CLI, Output, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, LogFile, "LogFile");
        CLISetSwitchDescription(CLI, LogFile, "Where should the logs be written? if unspecified, logs are written to STDERR");
        CLISetSwitchType(CLI, LogFile, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, Silence, "Silence");
        CLISetSwitchDescription(CLI, Silence, "Set the threshhold in dB or absolute value");
        CLISetSwitchType(CLI, Silence, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, Help, "Help");
        CLISetSwitchDescription(CLI, Help, "Prints all the command line options");
        CLISetSwitchType(CLI, Help, SingleSwitchNoResult);
        
        return CLI;
    }
    
    void RemoveEmptySamples(PCMFile *PCM, uint32_t NumChannels, uint32_t NumSamples, uint32_t **AudioSamples) {
        uint32_t  CurrentSampleIndex = 0UL;
        uint8_t   BitDepth           = PCMGetBitDepth(PCM);
        uint32_t *CurrentSampleValue = calloc(NumChannels, NumSamples * Bits2Bytes(BitDepth, Yes));
        for (uint32_t Channel = 0UL; Channel < NumChannels; Channel++) {
            for (uint32_t Sample = 0UL; Sample < NumSamples; Sample++) {
                // Ok, so we need to check each sample to see if it is 0, and if it is, make sure all samples in that group are zer0, then loop until we find a non-zero sample.
                while (AudioSamples[Channel][Sample] == 0x0) {
                    CurrentSampleIndex += 1;
                }
            }
        }
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineIO *CLI   = SetTrimSilenceOptions();
        BitInput      *BitI  = BitInput_Init();
        BitOutput     *BitO  = BitOutput_Init();
        PCMFile       *PCM   = PCMFileInit();
        BitBuffer     *BitB  = BitBuffer_Init(40);
        
        ParseCommandLineArguments(CLI, argc, argv);
        
        uint64_t InputFileArg      = CLIGetMatchingArgumentNum(CLI, 1, Input, 0, NULL);
        uint64_t OutputFileArg     = CLIGetMatchingArgumentNum(CLI, 1, Output, 0, NULL);
        
        char *InputPath            = CLIGetArgumentResult(CLI, InputFileArg);
        char *OutputPath           = CLIGetArgumentResult(CLI, OutputFileArg);
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
            BitIOLog(LOG_ERROR, "TrimSilence", __func__, "Unrecognized extension: %s", OutputExtension);
        }
        
        // So now we go ahead and mess around with the samples, looking for empty SampleGroups, then write it all out with the generic Write functions that I need to write.
        
        BitInput_OpenFile(BitI, InputPath);
        BitOutput_OpenFile(BitO, OutputPath);
        
        IdentifyPCMFile(PCM, BitB);
        ParsePCMMetadata(PCM, BitB);
        
        uint32_t **AudioSamples = NULL; // Lets read 4096 samples at a time.
        AudioSamples = ExtractSamples(PCM, BitB, 4096);
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
