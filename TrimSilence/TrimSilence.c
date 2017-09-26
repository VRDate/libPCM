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
        CommandLineIO     *CLI = InitCommandLineIO(4);
        
        SetCLIName(CLI, "TrimSilence");
        SetCLIVersion(CLI, TrimSilenceVersion);
        SetCLIAuthor(CLI, "BumbleBritches57");
        SetCLICopyright(CLI, "2017-2017");
        SetCLIDescription(CLI, "PCM silence remover written from scratch in modern C");
        SetCLILicense(CLI, "Revised BSD", "Permissive open source license", "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCLIMinSwitches(CLI, 3);
        
        SetCLISwitchFlag(CLI, Input, "Input");
        SetCLISwitchDescription(CLI, Input, "Input file or stdin with: '-'\n");
        SetCLISwitchResultStatus(CLI, Input, false);
        SetCLISwitchAsMaster(CLI, Input);
        
        SetCLISwitchFlag(CLI, Output, "Output");
        SetCLISwitchDescription(CLI, Output, "Output file or stdout with: '-'\n");
        SetCLISwitchResultStatus(CLI, Output, false);
        SetCLISwitchAsMaster(CLI, Output);
        
        SetCLISwitchFlag(CLI, LogFile, "LogFile");
        SetCLISwitchDescription(CLI, LogFile, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, LogFile, true);
        SetCLISwitchAsMaster(CLI, LogFile);
        
        SetCLISwitchFlag(CLI, Silence, "Silence");
        SetCLISwitchDescription(CLI, Silence, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetCLISwitchResultStatus(CLI, Silence, false);
        SetCLISwitchAsMaster(CLI, Silence);
        
        SetCLISwitchFlag(CLI, Help, "Help");
        SetCLISwitchDescription(CLI, Help, "Prints all the command line options");
        SetCLISwitchResultStatus(CLI, Help, true);
        SetCLISwitchAsMaster(CLI, Help);
        
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
