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
        
        SetCMDSwitchFlag(CMD, 0, "Input", 5);
        SetCMDSwitchDescription(CMD, 0, "Input file or stdin with: '-'\n");
        SetCMDSwitchResultStatus(CMD, 0, false);
        
        SetCMDSwitchFlag(CMD, 1, "Output", 6);
        SetCMDSwitchDescription(CMD, 1, "Output file or stdout with: '-'\n");
        SetCMDSwitchResultStatus(CMD, 1, false);
        
        SetCMDSwitchFlag(CMD, 2, "Silence", 7);
        SetCMDSwitchDescription(CMD, 2, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetCMDSwitchResultStatus(CMD, 2, false);
        
        SetCMDSwitchFlag(CMD, 3, "Help", 4);
        SetCMDSwitchDescription(CMD, 6, "Prints all the command line options");
        SetCMDSwitchResultStatus(CMD, 6, true);
        
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
