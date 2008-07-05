/*
 * ArcEmu Failure Server
 * Copyright (C) 2008 <http://www.AscentEmu.com/>
 *
 * This program is stolen software. Please visit www.ascentemu.com
 * for the real owners of this emulator.
 * 
 * ArcEmu is a pathetic piece of software which crashes even more
 * than vanilla Ascent. And don't even put it up against Summit.
 * There's no comparison there.
 *
 * SO, why use a project run by an idiot who can't even code?
 * You don't! :D
 *
 */

#ifndef _CONSOLECOMMANDS_H
#define _CONSOLECOMMANDS_H

bool HandleAnnounceCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleBanAccountCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleCancelCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleCreateAccountCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleInfoCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleGMsCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleKickCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleMOTDCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleOnlinePlayersCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandlePlayerInfoCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleRehashCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleShutDownCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleUnbanAccountCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleWAnnounceCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleWhisperCommand(BaseConsole * pConsole, int argc, const char * argv[]);
bool HandleNameHashCommand(BaseConsole * pConsole, int argc, const char * argv[]);
#endif
