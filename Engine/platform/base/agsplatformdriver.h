//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
// AGS Cross-Platform Header
//
//=============================================================================

#ifndef __AGS_EE_PLATFORM__AGSPLATFORMDRIVER_H
#define __AGS_EE_PLATFORM__AGSPLATFORMDRIVER_H

#include <errno.h>
#include <vector>
#include "ac/datetime.h"
#include "debug/outputhandler.h"
#include "util/ini_util.h"

namespace AGS
{
    namespace Common { class Stream; }
    namespace Engine { struct DisplayMode; }
}
using namespace AGS; // FIXME later

enum eScriptSystemOSID
{
    eOS_DOS = 1,
    eOS_Win,
    eOS_Linux,
    eOS_Mac,
    eOS_Android,
    eOS_iOS,
    eOS_PSP
};

enum SetupReturnValue
{
    kSetup_Cancel,
    kSetup_Done,
    kSetup_RunGame
};

struct AGSPlatformDriver
    // be used as a output target for logging system
    : public AGS::Common::IOutputHandler
{
    virtual void AboutToQuitGame();
    virtual void Delay(int millis) = 0;
    virtual void DisplayAlert(const char*, ...) = 0;
    virtual int  GetLastSystemError() { return errno; }
    // Get root directory for storing per-game shared data
    virtual const char *GetAllUsersDataDirectory() { return "."; }
    // Get root directory for storing per-game saved games
    virtual const char *GetUserSavedgamesDirectory() { return "."; }
    // Get root directory for storing per-game user configuration files
    virtual const char *GetUserConfigDirectory() { return "."; }
    // Get directory for storing all-games user configuration files
    virtual const char *GetUserGlobalConfigDirectory()  { return "."; }
    // Get default directory for program output (logs)
    virtual const char *GetAppOutputDirectory() { return "."; }
    // Returns array of characters illegal to use in file names
    virtual const char *GetIllegalFileChars() { return "\\/"; }
    virtual const char *GetDiskWriteAccessTroubleshootingText();
    virtual const char *GetGraphicsTroubleshootingText() { return ""; }
    virtual unsigned long GetDiskFreeSpaceMB() = 0;
    virtual const char* GetNoMouseErrorString() = 0;
    // Tells whether build is capable of controlling mouse movement properly
    virtual bool IsMouseControlSupported(bool windowed) { return false; }
    // Tells whether this platform's backend library deals with mouse cursor
    // virtual->real coordinate transformation itself (otherwise AGS engine should do it)
    virtual bool IsBackendResponsibleForMouseScaling() { return false; }
    virtual const char* GetAllegroFailUserHint();
    virtual eScriptSystemOSID GetSystemOSID() = 0;
    virtual void GetSystemTime(ScriptDateTime*);
    virtual void PlayVideo(const char* name, int skip, int flags) = 0;
    virtual void InitialiseAbufAtStartup();
    virtual void PostAllegroInit(bool windowed);
    virtual void PostAllegroExit() = 0;
    virtual void FinishedUsingGraphicsMode();
    virtual SetupReturnValue RunSetup(const Common::ConfigTree &cfg_in, Common::ConfigTree &cfg_out);
    virtual void SetGameWindowIcon();
    // Formats message and writes to standard platform's output;
    // Always adds trailing '\n' after formatted string
    virtual void WriteStdOut(const char *fmt, ...);
    virtual void YieldCPU();
    virtual void DisplaySwitchOut();
    virtual void DisplaySwitchIn();
    // Returns a list of supported display modes
    virtual void GetSystemDisplayModes(std::vector<Engine::DisplayMode> &dms);
    // Switch to system fullscreen mode; store previous mode parameters
    virtual bool EnterFullscreenMode(const Engine::DisplayMode &dm);
    // Return back to the mode was before switching to fullscreen
    virtual bool ExitFullscreenMode();
    // Adjust application window's parameters to suit fullscreen mode
    virtual void AdjustWindowStyleForFullscreen();
    // Restore application window to normal parameters
    virtual void RestoreWindowStyle();
    virtual void RegisterGameWithGameExplorer();
    virtual void UnRegisterGameWithGameExplorer();
    virtual int  ConvertKeycodeToScanCode(int keyCode);
    // Adjust window size to ensure it is in the supported limits
    virtual void ValidateWindowSize(int &x, int &y, bool borderless) const {}

    virtual int  InitializeCDPlayer() = 0;  // return 0 on success
    virtual int  CDPlayerCommand(int cmdd, int datt) = 0;
    virtual void ShutdownCDPlayer() = 0;

    virtual bool LockMouseToWindow();
    virtual void UnlockMouse();

    static AGSPlatformDriver *GetDriver();

    //-----------------------------------------------
    // IOutputHandler implementation
    //-----------------------------------------------
    // Writes to the standard platform's output, prepending "AGS: " prefix to the message
    virtual void PrintMessage(const AGS::Common::DebugMessage &msg);

private:
    static AGSPlatformDriver *instance;
};

#if defined (AGS_HAS_CD_AUDIO)
int cd_player_init();
int cd_player_control(int cmdd, int datt);
#endif

// [IKM] What is a need to have this global var if you can get AGSPlatformDriver
// instance by calling AGSPlatformDriver::GetDriver()?
extern AGSPlatformDriver *platform;

#endif // __AGS_EE_PLATFORM__AGSPLATFORMDRIVER_H
