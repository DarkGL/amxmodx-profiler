/* AMX Mod X
*
* by the AMX Mod X Development Team
*  originally developed by OLO
*
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software Foundation,
*  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*  In addition, as a special exception, the author gives permission to
*  link the code of this program with the Half-Life Game Engine ("HL
*  Engine") and Modified Game Libraries ("MODs") developed by Valve,
*  L.L.C ("Valve"). You must obey the GNU General Public License in all
*  respects for all of the code used other than the HL Engine and MODs
*  from Valve. If you modify this file, you may extend this exception
*  to your version of the file, but you are not obligated to do so. If
*  you do not wish to do so, delete this exception statement from your
*  version.
*/

#ifndef AMXMODX_H
#define AMXMODX_H

#ifdef __linux__
#include <unistd.h>
#include <stdlib.h>
#include "sclinux.h"
#endif
#include <ctype.h>				//tolower, etc
#include "string.h"
#include <extdll.h>
#include <meta_api.h>

#ifdef _MSC_VER
	// MSVC8 - replace POSIX functions with ISO C++ conformant ones as they are deprecated
	#if _MSC_VER >= 1400
		#define unlink _unlink	
		#define mkdir _mkdir
		#define strdup _strdup
	#endif
#endif

#include "md5.h"
#include "CVector.h"
#include "CList.h"
#include "CQueue.h"
#include "modules.h"
#include "CString.h"
#include "CPlugin.h"
#include "CMisc.h"
#include "CVault.h"
#include "CModule.h"
#include "CTask.h"
#include "CLogEvent.h"
#include "CForward.h"
#include "CCmd.h"
#include "CEvent.h"
#include "CLang.h"
#include "fakemeta.h"
#include "amxxlog.h"

#define AMXXLOG_Log g_log.Log
#define AMXXLOG_Error g_log.LogError

extern AMX_NATIVE_INFO core_Natives[];
extern AMX_NATIVE_INFO time_Natives[];
extern AMX_NATIVE_INFO power_Natives[];
extern AMX_NATIVE_INFO amxmodx_Natives[];
extern AMX_NATIVE_INFO file_Natives[];
extern AMX_NATIVE_INFO float_Natives[];
extern AMX_NATIVE_INFO string_Natives[];
extern AMX_NATIVE_INFO vault_Natives[];
extern AMX_NATIVE_INFO msg_Natives[];
extern AMX_NATIVE_INFO vector_Natives[];
extern AMX_NATIVE_INFO g_SortNatives[];
extern AMX_NATIVE_INFO g_DataStructNatives[];

#ifndef __linux__
#define DLLOAD(path) (DLHANDLE)LoadLibrary(path)
#define DLPROC(m, func) GetProcAddress(m, func)
#define DLFREE(m) FreeLibrary(m)
#else
#define DLLOAD(path) (DLHANDLE)dlopen(path, RTLD_NOW)
#define DLPROC(m, func) dlsym(m, func)
#define DLFREE(m) dlclose(m)
#endif

#if defined __GNUC__
	#include <stdint.h>
	typedef intptr_t _INT_PTR;
#else
	#if defined AMD64
		typedef __int64 _INT_PTR;
	#else
		typedef __int32 _INT_PTR;
	#endif
#endif

#ifndef __linux__
	typedef HINSTANCE DLHANDLE;
#else
	typedef void* DLHANDLE;
	#define INFINITE 0xFFFFFFFF
#endif

#ifndef __linux__
	#define PATH_SEP_CHAR		'\\'
	#define ALT_SEP_CHAR		'/'
#else
	#define PATH_SEP_CHAR		'/'
	#define ALT_SEP_CHAR		'\\'
#endif

#ifndef GETPLAYERAUTHID
#define GETPLAYERAUTHID     (*g_engfuncs.pfnGetPlayerAuthId)
#endif
#define ANGLEVECTORS        (*g_engfuncs.pfnAngleVectors)
#define CLIENT_PRINT        (*g_engfuncs.pfnClientPrintf)
#define CVAR_DIRECTSET      (*g_engfuncs.pfnCvar_DirectSet)
#define GETCLIENTLISTENING  (*g_engfuncs.pfnVoice_GetClientListening)
#define RUNPLAYERMOVE       (*g_engfuncs.pfnRunPlayerMove)
#define SETCLIENTLISTENING  (*g_engfuncs.pfnVoice_SetClientListening)
#define SETCLIENTMAXSPEED   (*g_engfuncs.pfnSetClientMaxspeed)

char* UTIL_SplitHudMessage(register const char *src);
int UTIL_ReadFlags(const char* c);

void UTIL_ClientPrint(edict_t *pEntity, int msg_dest, char *msg);
void UTIL_FakeClientCommand(edict_t *pEdict, const char *cmd, const char *arg1 = NULL, const char *arg2 = NULL);
void UTIL_GetFlags(char* flags, int flag);
void UTIL_HudMessage(edict_t *pEntity, const hudtextparms_t &textparms, char *pMessage);
void UTIL_IntToString(int value, char *output);
void UTIL_ShowMOTD(edict_t *client, char *motd, int mlen, const char *name);
void UTIL_ShowMenu(edict_t* pEntity, int slots, int time, char *menu, int mlen);

char *UTIL_VarArgs(const char *fmt, ...);


#define GET_PLAYER_POINTER(e)   (&g_players[ENTINDEX(e)])
//#define GET_PLAYER_POINTER(e)   (&g_players[(((int)e-g_edict_point)/sizeof(edict_t))])
#define GET_PLAYER_POINTER_I(i) (&g_players[i])

struct WeaponsVault
{
	String fullName;
	short int iId;
	short int ammoSlot;
};

struct fakecmd_t
{
	char args[256];
	const char *argv[3];
	int argc;
	bool fake;
};

extern CLog g_log;
extern CPluginMngr g_plugins;
extern CTaskMngr g_tasksMngr;
extern CPlayer g_players[33];
extern CPlayer* mPlayer;
extern CmdMngr g_commands;
extern CList<CCVar> g_cvars;
extern CList<ForceObject> g_forcemodels;
extern CList<ForceObject> g_forcesounds;
extern CList<ForceObject> g_forcegeneric;
extern CList<CModule, const char *> g_modules;
extern CList<CScript, AMX*> g_loadedscripts;
extern CList<CPlayer*> g_auth;
extern EventsMngr g_events;
extern Grenades g_grenades;
extern LogEventsMngr g_logevents;
extern CLangMngr g_langMngr;
extern String g_log_dir;
extern String g_mod_name;
extern TeamIds g_teamsIds;
extern Vault g_vault;
extern CForwardMngr g_forwards;
extern WeaponsVault g_weaponsData[MAX_WEAPONS];
extern XVars g_xvars;
extern bool g_bmod_cstrike;
extern bool g_bmod_dod;
extern bool g_dontprecache;
extern int g_srvindex;
extern cvar_t* amxmodx_version;
extern cvar_t* hostname;
extern cvar_t* mp_timelimit;
extern fakecmd_t g_fakecmd;
extern float g_game_restarting;
extern float g_game_timeleft;
extern float g_task_time;
extern float g_auth_time;
extern bool g_NewDLL_Available;
extern hudtextparms_t g_hudset;
//extern int g_edict_point;
extern int g_players_num;
extern int mPlayerIndex;
extern int mState;
extern void (*endfunction)(void*);
extern void (*function)(void*);

typedef void (*funEventCall)(void*);
extern funEventCall modMsgsEnd[MAX_REG_MSGS];
extern funEventCall modMsgs[MAX_REG_MSGS];

extern int gmsgAmmoPickup;
extern int gmsgAmmoX;
extern int gmsgBattery;
extern int gmsgCurWeapon;
extern int gmsgDamage;
extern int gmsgDeathMsg;
extern int gmsgHealth;
extern int gmsgMOTD;
extern int gmsgScoreInfo;
extern int gmsgSendAudio;
extern int gmsgServerName;
extern int gmsgShowMenu;
extern int gmsgTeamInfo;
extern int gmsgTextMsg;
extern int gmsgVGUIMenu;
extern int gmsgWeapPickup;
extern int gmsgWeaponList;
extern int gmsgintermission;
extern int gmsgResetHUD;
extern int gmsgRoundTime;

void Client_AmmoPickup(void*);
void Client_AmmoX(void*);
void Client_CurWeapon(void*);
void Client_ScoreInfo(void*);
void Client_ShowMenu(void*);
void Client_TeamInfo(void*);
void Client_TextMsg(void*);
void Client_VGUIMenu(void*);
void Client_WeaponList(void*);
void Client_DamageEnd(void*);
void Client_DeathMsg(void*);

void amx_command();
void plugin_srvcmd();

const char* stristr(const char* a, const char* b);
char *strptime(const char *buf, const char *fmt, struct tm *tm, short addthem);

int loadModules(const char* filename, PLUG_LOADTIME now);
void detachModules();
void detachReloadModules();

#ifdef FAKEMETA
	void attachModules();
#endif

// Count modules
enum CountModulesMode
{
	CountModules_Running = 0,
	CountModules_All,
	CountModules_Stopped
};

int countModules(CountModulesMode mode);
void modules_callPluginsLoaded();
void modules_callPluginsUnloaded();
void modules_callPluginsUnloading();

cell* get_amxaddr(AMX *amx, cell amx_addr);
char* build_pathname(char *fmt, ...);
char* build_pathname_r(char *buffer, size_t maxlen, char *fmt, ...);
char* format_amxstring(AMX *amx, cell *params, int parm, int& len);
AMX* get_amxscript(int, void**, const char**);
const char* get_amxscriptname(AMX* amx);
char* get_amxstring(AMX *amx, cell amx_addr, int id, int& len);
extern "C" size_t get_amxstring_r(AMX *amx, cell amx_addr, char *destination, int maxlen);

int amxstring_len(cell* cstr);
int load_amxscript(AMX* amx, void** program, const char* path, char error[64], int debug);
int load_amxscript_plugin(AMX *amx, void **program, const char *filename, char error[64], int debug, CPluginMngr::CPlugin* pluginptr = NULL); // Added to pass the CPlugin pointer to it; so that the plugin field in userdata will be set (for profiler usage during JIT compilation)
int set_amxnatives(AMX* amx, char error[64]);
int set_amxstring(AMX *amx, cell amx_addr, const char *source, int max);
int unload_amxscript(AMX* amx, void** program);

void copy_amxmemory(cell* dest, cell* src, int len);
void get_modname(char*);
void print_srvconsole(char *fmt, ...);
void report_error(int code, char* fmt, ...);
void* alloc_amxmemory(void**, int size);
void free_amxmemory(void **ptr);
// get_localinfo
const char* get_localinfo(const char* name, const char* def);
cell AMX_NATIVE_CALL require_module(AMX *amx, cell *params);
extern "C" void LogError(AMX *amx, int err, const char *fmt, ...);

enum ModuleCallReason
{
	ModuleCall_NotCalled = 0,					// nothing
	ModuleCall_Query,							// in Query func
	ModuleCall_Attach,							// in Attach func
	ModuleCall_Detach,							// in Detach func
};

extern ModuleCallReason g_ModuleCallReason;		// modules.cpp
extern CModule *g_CurrentlyCalledModule;		// modules.cpp
extern const char *g_LastRequestedFunc;			// modules.cpp

void Module_CacheFunctions();
void Module_UncacheFunctions();

void *Module_ReqFnptr(const char *funcName);	// modules.cpp

// standard forwards
// defined in meta_api.cpp
extern int FF_ClientCommand;
extern int FF_ClientConnect;
extern int FF_ClientDisconnect;
extern int FF_ClientInfoChanged;
extern int FF_ClientPutInServer;
extern int FF_PluginInit;
extern int FF_PluginCfg;
extern int FF_PluginPrecache;
extern int FF_PluginLog;
extern int FF_PluginEnd;
extern int FF_InconsistentFile;
extern int FF_ClientAuthorized;
extern int FF_ChangeLevel;
extern bool g_coloredmenus;

typedef void (*AUTHORIZEFUNC)(int player, const char *authstring);

#define MM_CVAR2_VERS	13

struct func_s
{
	void *pfn;
	const char *desc;
};

enum AdminProperty
{
	Admin_Auth = 0,
	Admin_Password,
	Admin_Access,
	Admin_Flags
};

extern enginefuncs_t *g_pEngTable;

#endif // AMXMODX_H
