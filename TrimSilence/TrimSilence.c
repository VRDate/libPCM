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
        CLISetDescription(CLI, "PCM silence remover written from scratch in modern C");
        CLISetLicense(CLI, "Revised BSD", "Permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        CLISetMinOptions(CLI, 3);
        
        CLISetSwitchFlag(CLI, Input, "Input");
        CLISetSwitchDescription(CLI, Input, "Input file or stdin");
        CLISetSwitchType(CLI, Input, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, Output, "Output");
        CLISetSwitchDescription(CLI, Output, "Output file or stdout");
        CLISetSwitchType(CLI, Output, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, LogFile, "LogFile");
        CLISetSwitchDescription(CLI, LogFile, "Where should the logs be written? default is STDERR");
        CLISetSwitchType(CLI, LogFile, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, SilenceLevel, "SilenceLevel");
        CLISetSwitchDescription(CLI, SilenceLevel, "Set the threshhold by absolute value");
        CLISetSwitchType(CLI, SilenceLevel, SingleSwitchWithResult);
        
        CLISetSwitchFlag(CLI, Help, "Help");
        CLISetSwitchDescription(CLI, Help, "Prints all the command line options");
        CLISetSwitchType(CLI, Help, SingleSwitchNoResult);
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
    
    static void PrintARGV(int argc, const char **argv) {
        printf("Argv Arguments: %d%s", argc, BitIOLineEnding);
        for (int32_t Argument = 0L; Argument < argc; Argument++) {
            printf("Argument Number %d, ArgumentString %s%s", Argument, argv[Argument], BitIOLineEnding);
        }
    }
    
    int main(int argc, const char *argv[]) {
        CommandLineIO *CLI          = SetTrimSilenceOptions();
        BitInput      *BitI         = BitInput_Init();
        BitOutput     *BitO         = BitOutput_Init();
        PCMFile       *PCM          = PCMFile_Init();
        BitBuffer     *BitB         = BitBuffer_Init(40);
        
        // PrintARGV(argc, argv);
        
        ParseCommandLineOptions(CLI, argc, argv);
        
        PrintCommandLineOptions(CLI);
        
        uint64_t InputFileArg       = CLIGetOptionNum(CLI, Input, 0, NULL);
        uint64_t OutputFileArg      = CLIGetOptionNum(CLI, Output, 0, NULL);
        uint64_t LogFileArg         = CLIGetOptionNum(CLI, LogFile, 0, NULL);
        
        char *InputPath             = CLIGetOptionResult(CLI, InputFileArg);
        char *OutputPath            = CLIGetOptionResult(CLI, OutputFileArg);
        char *LogFilePath           = CLIGetOptionResult(CLI, LogFileArg);
        char *OutputExtension       = GetExtensionFromPath(OutputPath);
        
        BitInput_OpenFile(BitI, InputPath);
        BitIOLog_OpenFile(LogFilePath);
        BitOutput_OpenFile(BitO, OutputPath);
         
        
        if (strcasecmp(OutputExtension, "wav") == 0) {
            PCMSetOutputFileType(PCM, WAVFormat);
        } else if (strcasecmp(OutputExtension, "w64") == 0) {
            PCMSetOutputFileType(PCM, W64Format);
        } else if ((strcasecmp(OutputExtension, "aif") || strcasecmp(OutputExtension, "aiff") || strcasecmp(OutputExtension, "aifc"))  == 0) {
            PCMSetOutputFileType(PCM, AIFFormat);
        } else {
            BitIOLog(LOG_ERROR, "TrimSilence", __func__, "Unrecognized Output file extension: %s", OutputExtension);
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
