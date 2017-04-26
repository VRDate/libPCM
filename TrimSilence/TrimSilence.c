#include "../libPCM/include/libPCM.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#define TrimSilenceVersion "0.1.0"
    
    CommandLineOptions *SetModernPNGOptions(void) {
        CommandLineOptions     *CMD = InitCommandLineOptions(4);
        
        // test the logger
        //Log(LOG_ERR, "libBitIO", "SetSwitchResultStatus", "SwitchNum: %d, should be between 0 and %d\n", 0, 43);
        
        SetCMDName(CMD, "ModernPNG");
        SetCMDVersion(CMD, TrimSilenceVersion);
        SetCMDAuthor(CMD, "BumbleBritches57");
        SetCMDCopyright(CMD, "2017-2017");
        SetCMDDescription(CMD, "PNG encoder/decoder written from scratch in modern C");
        SetCMDLicense(CMD, "Revised BSD (3 clause)", false);
        SetCMDLicenseURL(CMD, "https://opensource.org/licenses/BSD-3-Clause", false);
        SetCMDMinSwitches(CMD, 3);
        
        SetSwitchFlag(CMD, 0, "Input", 6);
        SetSwitchDescription(CMD, 0, "Input file or stdin with: '-'\n");
        SetSwitchResultStatus(CMD, 0, false);
        
        SetSwitchFlag(CMD, 1, "Output", 7);
        SetSwitchDescription(CMD, 1, "Output file or stdout with: '-'\n");
        SetSwitchResultStatus(CMD, 1, false);
        
        SetSwitchFlag(CMD, 2, "Silence", 8);
        SetSwitchDescription(CMD, 2, "Set the threshhold, in dB or absolute value like: (-|--|/)Silence 12dB or: (-|--|/)Silence 0");
        SetSwitchResultStatus(CMD, 2, false);
        
        SetSwitchFlag(CMD, 3, "Help", 5);
        SetSwitchDescription(CMD, 6, "Prints all the command line options");
        SetSwitchResultStatus(CMD, 6, true);
        
        // Switch 6 is NULL, aka the auto Help option. Fuck it, the user will just have to code that themselves.
        
        return CMD;
    }
    
    int main(int argc, const char * argv[]) {
        CommandLineOptions *CMD  = SetModernPNGOptions();
        BitInput           *BitI = InitBitInput();
        BitOutput          *BitO = InitBitOutput();
        PCMFile            *PCM  = InitPCMFile();
        
        IdentifyPCMFile(BitI, PCM);
        
        ReadWAVHeader(IO, WAV);
        ReadPCM(IO, WAV, PCM);
        FindFirstGoodSample(WAV, Silence, SilenceThreshold, PCM);
        FindLastGoodSample(WAV, Silence, SilenceThreshold, PCM);
        CoalesceChannels(WAV, Silence);
        WriteWAVHeader(IO, WAV, Silence);
        WritePCM(IO, WAV, Silence, PCM);
    }
    
    return EXIT_SUCCESS;
}

#ifdef __cplusplus
}
#endif
