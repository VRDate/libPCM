#include "../Dependencies/BitIO/libBitIO/include/CommandLineIO.h"

#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    enum CommandLineSwitchNames {
        Input   = 0,
        Output  = 1,
        Silence = 2,
        Help    = 3,
        LogFile = 4,
    };
    
    CommandLineIO *SetTrimSilenceOptions(void) {
        CommandLineIO     *CLI = InitCommandLineIO(4);
        
        SetCLIName(CLI, "TrimSilence");
        SetCLIVersion(CLI, TrimSilenceVersion);
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLICopyright(CLI, "2017-2017");
        SetCLIDescription(CLI, "PCM silence remover written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD", NULL, false);
        SetCLILicenseURL(CLI, "https://opensource.org/licenses/BSD-3-Clause");
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input", 5);
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: '-'\n");
        SetCLISwitchResultStatus(CLI, Input, false);
        SetCLISwitchAsMaster(CLI, Input, true);
        
        SetCLISwitchFlag(CLI, Output, "Output", 6);
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: '-'\n");
        SetCLISwitchResultStatus(CLI, Output, false);
        SetCLISwitchAsMaster(CLI, Output, true);
        
        SetCLISwitchFlag(CLI, Silence, "Silence", 7);
        SetCLISwitchDescription(CLI, Silence, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetCLISwitchResultStatus(CLI, Silence, false);
        SetCLISwitchAsMaster(CLI, Silence, true);
        
        SetCLISwitchFlag(CLI, Help, "Help", 4);
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, Help, true);
        SetCLISwitchAsMaster(CLI, Help, true);
        
        SetCLISwitchFlag(CLI, LogFile, "LogFile", 7);
        SetCLISwitchDescription(CLI, LogFile, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, LogFile, true);
        SetCLISwitchAsMaster(CLI, LogFile, true);
        
        return CLI;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineIO *CLI  = SetTrimSilenceOptions();
        ParseCommandLineArguments(CLI, argc, argv);
        BitInput            *BitI  = InitBitInput();
        BitOutput           *BitO  = InitBitOutput();
        PCMFile             *PCM   = InitPCMFile();
        
        OpenInputFile(BitI, GetCLIArgumentResult(CLI, Input), false);
        OpenOutputFile(BitO, GetCLIArgumentResult(CLI, Output));
        
        IdentifyPCMFile(BitI, PCM);
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
