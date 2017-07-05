#include "../libPCM/include/libPCM.h"
#include "../Dependencies/BitIO/libBitIO/include/CommandLineInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    enum CommandLineSwitchNames {
        Input   = 0,
        Output  = 1,
        Silence = 2,
        Help    = 3,
    };
    
    CommandLineInterface *SetTrimSilenceOptions(void) {
        CommandLineInterface     *CLI = InitCommandLineInterface(4);
        
        SetCLIName(CLI, "TrimSilence");
        SetCLIVersion(CLI, TrimSilenceVersion);
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLICopyright(CLI, "2017-2017");
        SetCLIDescription(CLI, "PCM silence remover written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD (3 clause)", false);
        SetCLILicenseURL(CLI, "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input", 5);
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: '-'\n");
        SetCLISwitchResultStatus(CLI, Input, false);
        
        SetCLISwitchFlag(CLI, Output, "Output", 6);
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: '-'\n");
        SetCLISwitchResultStatus(CLI, Output, false);
        
        SetCLISwitchFlag(CLI, Silence, "Silence", 7);
        SetCLISwitchDescription(CLI, Silence, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetCLISwitchResultStatus(CLI, Silence, false);
        
        SetCLISwitchFlag(CLI, Help, "Help", 4);
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, Help, true);
        
        return CLI;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineInterface *CLI  = SetTrimSilenceOptions();
        ParseCommandLineArguments(CLI, argc, argv);
        BitInput            *BitI  = InitBitInput();
        BitOutput           *BitO  = InitBitOutput();
        PCMFile             *PCM   = InitPCMFile();
        
        OpenInputFile(BitI, GetCLIArgumentResult(CLI, Input));
        OpenOutputFile(BitO, GetCLIArgumentResult(CLI, Output));
        
        IdentifyPCMFile(BitI, PCM);
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
