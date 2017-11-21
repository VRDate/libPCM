#include "../Dependencies/BitIO/libBitIO/include/BitIO.h"
#include "../Dependencies/BitIO/libBitIO/include/CommandLineIO.h"

#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    typedef enum CommandLineSwitches { // CommandLineSwitchNames
        Input         = 0,
        Output        = 1,
        LogFile       = 2,
        SilenceLevel  = 3,
        Help          = 4,
        NumSwitches   = 5,
    } CommandLineSwitches;
    
    static CommandLineIO *SetTrimSilenceOptions(void) {
        CommandLineIO *CLI = CommandLineIO_Init(NumSwitches);
        
        CLISetName(CLI, "TrimSilence");
        CLISetVersion(CLI, TrimSilenceVersion);
        CLISetAuthor(CLI, "BumbleBritches57");
        CLISetCopyright(CLI, "2017 - 2017");
        CLISetDescription(CLI, "Removes leading and trailing silence");
        CLISetLicense(CLI, "Revised BSD", "Permissive license", "https://opensource.org/licenses/BSD-3-Clause", No);
        CLISetMinOptions(CLI, 3);
        
        CLISetSwitchFlag(CLI, Input, "Input");
        CLISetSwitchDescription(CLI, Input, "Input file or stdin");
        CLISetSwitchType(CLI, Input, SwitchCantHaveSlaves);
        
        CLISetSwitchFlag(CLI, Output, "Output");
        CLISetSwitchDescription(CLI, Output, "Output file or stdout");
        CLISetSwitchType(CLI, Output, SwitchCantHaveSlaves);
        
        CLISetSwitchFlag(CLI, LogFile, "LogFile");
        CLISetSwitchDescription(CLI, LogFile, "Where should the logs be written? default is STDERR");
        CLISetSwitchType(CLI, LogFile, SwitchCantHaveSlaves);
        
        CLISetSwitchFlag(CLI, SilenceLevel, "SilenceLevel");
        CLISetSwitchDescription(CLI, SilenceLevel, "Set the threshhold by absolute value");
        CLISetSwitchType(CLI, SilenceLevel, SwitchCantHaveSlaves);
        
        CLISetSwitchFlag(CLI, Help, "Help");
        CLISetSwitchDescription(CLI, Help, "Prints all the command line options");
        CLISetSwitchType(CLI, Help, ExistentialSwitch);
        CLISetHelpSwitch(CLI, Help);
        
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
    
    int64_t ConvertSilenceLevel2Integer(char *Level) {
        return atoll(Level);
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineIO *CLI          = SetTrimSilenceOptions();
        BitInput      *BitI         = BitInput_Init();
        BitOutput     *BitO         = BitOutput_Init();
        PCMFile       *PCM          = PCMFile_Init();
        BitBuffer     *BitB         = BitBuffer_Init(40);
        
        //BitIOLog(BitIOLog_DEBUG, BitIOLibraryName, __func__, "CommandLineIO Size", sizeof(CommandLineIO));
        
        ParseCommandLineOptions(CLI, argc, argv);
        //PrintCommandLineOptions(CLI);
        
        int64_t InputFileOption     = CLIGetOptionNum(CLI, Input, 0, NULL);
        int64_t OutputFileOption    = CLIGetOptionNum(CLI, Output, 0, NULL);
        int64_t LogFileOption       = CLIGetOptionNum(CLI, LogFile, 0, NULL);
        int64_t SilenceLevelOption  = CLIGetOptionNum(CLI, SilenceLevel, 0, NULL);
        
        char *InputPath             = CLIGetOptionResult(CLI, InputFileOption);
        char *OutputPath            = CLIGetOptionResult(CLI, OutputFileOption);
        char *OutputExtension       = GetExtensionFromPath(OutputPath);
        char *LogFilePath           = CLIGetOptionResult(CLI, LogFileOption);
        char *SilenceLevelString    = CLIGetOptionResult(CLI, SilenceLevelOption);
        int64_t SilenceValue        = ConvertSilenceLevel2Integer(SilenceLevelString);
        
        BitInput_OpenFile(BitI, InputPath);
        BitIOLog_OpenFile(LogFilePath);
        BitOutput_OpenFile(BitO, OutputPath);
         
        
        if (strcasecmp(OutputExtension, ".wav") == 0) {
            PCMSetOutputFileType(PCM, WAVFormat);
        } else if (strcasecmp(OutputExtension, ".w64") == 0) {
            PCMSetOutputFileType(PCM, W64Format);
        } else if ((strcasecmp(OutputExtension, ".aif") || strcasecmp(OutputExtension, ".aiff") || strcasecmp(OutputExtension, ".aifc"))  == 0) {
            PCMSetOutputFileType(PCM, AIFFormat);
        } else {
            BitIOLog(BitIOLog_ERROR, "TrimSilence", __func__, "Unrecognized Output file extension: %s", OutputExtension);
        }
        
        // So now we go ahead and mess around with the samples, looking for empty SampleGroups, then write it all out with the generic Write functions that I need to write.
        
        PCMFile_Identify(PCM, BitB);
        PCMFile_ParseMetadata(PCM, BitB);
        
        // Honestly, fuck this; I'm just gonna read all the samples in at once.
        
        uint64_t NumSamples         = PCMGetNumSamples(PCM);
        uint64_t NumChannels        = PCMGetNumChannels(PCM);
        uint8_t  BitDepth           = PCMGetBitDepth(PCM);
        
        uint32_t **ExtractedSamples = calloc(NumSamples * NumChannels, Bits2Bytes(BitDepth, Yes));
        
        PCM_ExtractSamples(PCM, BitB, PCMGetNumSamples(PCM), ExtractedSamples);
        
        free(ExtractedSamples);
        
        return EXIT_SUCCESS;
    }
    
#ifdef __cplusplus
}
#endif
