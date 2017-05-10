#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    CommandLineOptions *SetTrimSilenceOptions(void) {
        CommandLineOptions     *CMD = InitCommandLineOptions(4);
        
        SetCMDName(CMD, "TrimSilence");
        SetCMDVersion(CMD, TrimSilenceVersion);
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDDescription(CMD, "PCM silence remover written from scratch in modern C");
        SetCMDLicense(CMD, "Revised BSD (3 clause)", false);
        SetCMDLicenseURL(CMD, "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCMDMinSwitches(CMD, 3);
        
        SetSwitchFlag(CMD, 0, "Input", 5);
        SetSwitchDescription(CMD, 0, "Input file or stdin with: '-'\n");
        SetSwitchResultStatus(CMD, 0, false);
        
        SetSwitchFlag(CMD, 1, "Output", 6);
        SetSwitchDescription(CMD, 1, "Output file or stdout with: '-'\n");
        SetSwitchResultStatus(CMD, 1, false);
        
        SetSwitchFlag(CMD, 2, "Silence", 7);
        SetSwitchDescription(CMD, 2, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetSwitchResultStatus(CMD, 2, false);
        
        SetSwitchFlag(CMD, 3, "Help", 4);
        SetSwitchDescription(CMD, 6, "Prints all the command line options");
        SetSwitchResultStatus(CMD, 6, true);
        
        return CMD;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineOptions *CMD  = SetTrimSilenceOptions();
        BitInput           *BitI = InitBitInput();
        BitOutput          *BitO = InitBitOutput();
        PCMFile            *PCM  = InitPCMFile();
        
        IdentifyPCMFile(BitI, PCM);
        
        return 0;
    }
    
#ifdef __cplusplus
}
#endif
