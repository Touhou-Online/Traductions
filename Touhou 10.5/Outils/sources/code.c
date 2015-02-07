#include	<Windows.h>

HFONT WINAPI MyCreateFontA(int nHeight, int nWidth, int nEscapement, int nOrientation,
				int fnWeight,
				DWORD fdwItalic, DWORD fdwUnderline,
				DWORD fdwStrikeOut, DWORD fdwCharSet,
				DWORD fdwOutputPrecision,
				DWORD fdwClipPrecision,
				DWORD fdwQuality,
				DWORD fdwPitchAndFamily,
				LPCSTR lpszFace)
{
	(void)fdwCharSet;
	return CreateFontA(nHeight, nWidth, nEscapement, nOrientation,
				fnWeight,
				fdwItalic, fdwUnderline,
				fdwStrikeOut, ANSI_CHARSET,
				fdwOutputPrecision,
				fdwClipPrecision,
				fdwQuality,
				fdwPitchAndFamily,
				lpszFace);
}

HFONT WINAPI MyCreateFontW(int nHeight, int nWidth, int nEscapement, int nOrientation,
				int fnWeight,
				DWORD fdwItalic, DWORD fdwUnderline,
				DWORD fdwStrikeOut, DWORD fdwCharSet,
				DWORD fdwOutputPrecision,
				DWORD fdwClipPrecision,
				DWORD fdwQuality,
				DWORD fdwPitchAndFamily,
				LPCWSTR lpszFace)
{
	(void)fdwCharSet;
	if (wcscmp(lpszFace, L"MS ゴシック") == 0)
		lpszFace = L"MonoSpatialModSWR";
	return CreateFontW(nHeight, nWidth, nEscapement, nOrientation,
				fnWeight,
				fdwItalic, fdwUnderline,
				fdwStrikeOut, ANSI_CHARSET,
				fdwOutputPrecision,
				fdwClipPrecision,
				fdwQuality,
				fdwPitchAndFamily,
				lpszFace);
}



static const char*	text_table[] =
{
	// DINPUT
	"Failed to set properties",
	"Failed to create DirectInput object",
	"Failed to initialize keyboard",
	"Failed to set data format",
	"Failed to set cooperation level",
	"Failed to queue device",
	"Failed to initialize mouse",
	"Failed to set data format",
	"Failed to set cooperation level",

	// DIRECTX
	"Failed to create D3D object",
	"Failed to get display mode",
	"Failed to create D3D device",

	// DSOUND
	"Failed to get DirectSoundBuffer8",
	"Failed to get DirectSound3DBuffer8",
	"Failed to get DirectSoundBuffer8",
	"Failed to lock buffer" ,
	"Failed to lock buffer" ,
	"Failed to create DirectSound object",
	"Failed to initialize DirectSound object",
	"Failed to set cooperation level",
	"Failed to create primary sound buffer",
	"Failed to create listener",
	"Failed to create temporary buffer",

	// WINSOCK? func 41d370 looks okay though.

	// ERROR
	"Failed to initialize",

	// Replay
	"Returning to select. Save replay?",
	"Returning to title. Save replay?",
	"Replay saved.",
	"Replay saved.", // same as previous
	"Replay saved.", // same as previous

	// Netplay
	"Select from saved addresses...",
	"Connect to previous address...", // same as previous
	"1\x82\xA0Input port number...",
	"2Connect to address...",
	"Allow spectating?",
	"Awaiting connection...",
	"0 1 2 3 4 5 6 7 8 9<br>^<br>     .      .      .      : ",
	"Spectating is disallowed.",
	"Game is not available.",
	"Connection failed.",
	"Connection failed.", // same as previous
	"Connection failed.", // same as previous
	"Connecting...", // ?
	"Connect for a match?",
	" is the address?",

	"Already playing.",
	"Already playing. Spectate?",

	// Replay/continue/etc
	"Continue?  %d left",
	"Save replay?",
	"Save replay?",
	"<color 808080>? ? ? ? ? ? ? ? </color>",
	"<color 808080>? ? ? ? ? ? ? ? </color>", // same as previous
	"Delete this replay?",

	// Profile stuff
	"Failed to delete replay.",
	"Failed to delete profile.", // ? same as prev?
	"Replay deleted.",
	"Profile deleted.", // same as previous
	"<color 808080>? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? </color>",
	"Save changes?",
	"A deck must have 20 cards.",
	" creation failed.",
	" creation failed.", // same as previous
	" created.",
	"Profile copied.",
	"Profile copy failed.",
	"Name change failed.",
	"Name change failed.", // same as previous
	"Name changed.",
	" can be overwritten?",
	" can be overwritten?", // same as previous
	" will be copied.",
	" will be deleted.",

	// stupid spellcard clip fix
	"<br><br>",
	"<br><br>",

	//"%d    <color FF8080>%d</color>     / %d <color FF8080>/ %d</color><br>",

	"%-52s %s  %03d/%03d  Time %02d.%02d",

	NULL
};

static const char*	patch_windowname = "Touhou Hisouten - Scarlet Weather Rhapsody ver1.06a (french dev build)";

BOOL WINAPI	DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  DWORD 	save;
  DWORD 	dummy;
  int		size;
  int		i;

  (void)hinstDLL;
  (void)lpvReserved;

  if (fdwReason != DLL_PROCESS_ATTACH)
    return TRUE;

  for (size = 0; text_table[size]; size++);
  VirtualProtect((void *)0x6F5B169C, size * 8, PAGE_EXECUTE_READWRITE, &save);
  for (i = 0; text_table[i]; i++)
    *(const char**)(0x6F5B169C + i * 8 + 4) = text_table[i];
  VirtualProtect((void *)0x6F5B169C, size * 8, save, &dummy);
  FlushInstructionCache(GetCurrentProcess(), (void *)0x6F5B169C, size * 8);

  VirtualProtect((void *)0x6F581828, 4, PAGE_EXECUTE_READWRITE, &save);
  *(const char**)0x6F581828 = patch_windowname;
  VirtualProtect((void *)0x6F581828, 4, save, &dummy);
  FlushInstructionCache(GetCurrentProcess(), (void *)0x6F581828, 4);
  return TRUE;
}
