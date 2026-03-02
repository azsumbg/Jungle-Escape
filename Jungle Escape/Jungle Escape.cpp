#include "framework.h"
#include "Jungle Escape.h"
#include <mmsystem.h>
#include <d2d1.h>
#include <dwrite.h>
#include "ErrH.h"
#include "FCheck.h"
#include "D2BMPLOADER.h"
#include "gifresizer.h"
#include "Jungle.h"
#include <chrono>
#include <clocale>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "errh.lib")
#pragma comment(lib, "fcheck.lib")
#pragma comment(lib, "d2bmploader.lib")
#pragma comment(lib, "gifresizer.lib")
#pragma comment(lib, "jungle.lib")

constexpr wchar_t bWinClassName[]{ L"JungleRun" };
constexpr char tmp_file[]{ ".\\res\\data\\temp.dat" };
constexpr wchar_t Ltmp_file[]{ L".\\res\\data\\temp.dat" };
constexpr wchar_t record_file[]{ L".\\res\\data\\record.dat" };
constexpr wchar_t save_file[]{ L".\\res\\data\\save.dat" };
constexpr wchar_t help_file[]{ L".\\res\\data\\help.dat" };
constexpr wchar_t sound_file[]{ L".\\res\\snd\\main.wav" };

constexpr int mNew{ 1001 };
constexpr int mLvl{ 1002 };
constexpr int mExit{ 1003 };
constexpr int mLoad{ 1004 };
constexpr int mSave{ 1005 };
constexpr int mHoF{ 1006 };

constexpr int no_record{ 2001 };
constexpr int first_record{ 2002 };
constexpr int record{ 2003 };

WNDCLASS bWinClass{};
HINSTANCE bIns{ nullptr };
HMENU bBar{ nullptr };
HMENU bMain{ nullptr };
HMENU bStore{ nullptr };
HICON mainIcon{ nullptr };
HCURSOR mainCur{ nullptr };
HCURSOR outCur{ nullptr };
HWND bHwnd{ nullptr };
PAINTSTRUCT bPaint{};
HDC PaintDC{ nullptr };
POINT cur_pos{};
UINT bTimer{};
MSG bMsg{};
BOOL bRet{};

D2D1_RECT_F b1Rect{ 20.0f, 5.0f, scr_width / 3.0f - 50.0f, 45.0f };
D2D1_RECT_F b2Rect{ scr_width / 3.0f + 20.0f, 5.0f, scr_width * 2.0f / 3.0f - 50.0f, 45.0f };
D2D1_RECT_F b3Rect{ scr_width * 2.0f / 3.0f + 20.0f, 5.0f, scr_width - 50.0f, 45.0f };

D2D1_RECT_F b1TxtRect{ 40.0f, 10.0f, scr_width / 3.0f - 50.0f, 45.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 30.0f, 10.0f, scr_width * 2.0f / 3.0f - 50.0f, 45.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2.0f / 3.0f + 25.0f, 10.0f, scr_width - 50.0f, 45.0f };

bool pause = false;
bool show_help = false;
bool sound = true;
bool in_client = true;
bool b1Hglt = false;
bool b2Hglt = false;
bool b3Hglt = false;
bool name_set = false;

bool hero_killed{ false };

wchar_t current_player[16]{ L"TARLYO" };

int level = 1;
int score = 0;
int mins = 0;
int secs = 0;

ID2D1Factory* iFactory{ nullptr };
ID2D1HwndRenderTarget* Draw{ nullptr };

ID2D1RadialGradientBrush* b1BckgBrush{ nullptr };
ID2D1RadialGradientBrush* b2BckgBrush{ nullptr };
ID2D1RadialGradientBrush* b3BckgBrush{ nullptr };

ID2D1SolidColorBrush* statBrush{ nullptr };
ID2D1SolidColorBrush* txtBrush{ nullptr };
ID2D1SolidColorBrush* hgltBrush{ nullptr };
ID2D1SolidColorBrush* inactBrush{ nullptr };

IDWriteFactory* iWriteFactory{ nullptr };
IDWriteTextFormat* nrmTxt{ nullptr };
IDWriteTextFormat* midTxt{ nullptr };
IDWriteTextFormat* bigTxt{ nullptr };

ID2D1Bitmap* bmpArrowL{ nullptr };
ID2D1Bitmap* bmpArrowR{ nullptr };

ID2D1Bitmap* bmpTomahawkL{ nullptr };
ID2D1Bitmap* bmpTomahawkR{ nullptr };

ID2D1Bitmap* bmpChest{ nullptr };
ID2D1Bitmap* bmpCrystal{ nullptr };
ID2D1Bitmap* bmpGold{ nullptr };
ID2D1Bitmap* bmpPotion{ nullptr };

ID2D1Bitmap* bmpBackground{ nullptr };
ID2D1Bitmap* bmpGameLoose{ nullptr };
ID2D1Bitmap* bmpGameWin{ nullptr };
ID2D1Bitmap* bmpGameRecord{ nullptr };
ID2D1Bitmap* bmpLogo{ nullptr };
ID2D1Bitmap* bmpPlatform1{ nullptr };
ID2D1Bitmap* bmpPlatform2{ nullptr };
ID2D1Bitmap* bmpPlatform3{ nullptr };
ID2D1Bitmap* bmpRIP{ nullptr };

ID2D1Bitmap* bmpDirt{ nullptr };
ID2D1Bitmap* bmpTrapAxe{ nullptr };
ID2D1Bitmap* bmpTrapBolt{ nullptr };
ID2D1Bitmap* bmpTrapSpear{ nullptr };
ID2D1Bitmap* bmpTree1{ nullptr };
ID2D1Bitmap* bmpTree2{ nullptr };
ID2D1Bitmap* bmpTree3{ nullptr };
ID2D1Bitmap* bmpWater{ nullptr };
ID2D1Bitmap* bmpWater2{ nullptr };

ID2D1Bitmap* bmpIntro[181]{ nullptr };

ID2D1Bitmap* bmpFlyerL[16]{ nullptr };
ID2D1Bitmap* bmpFlyerR[16]{ nullptr };

ID2D1Bitmap* bmpMushroom[37]{ nullptr };

ID2D1Bitmap* bmpSnailL[32]{ nullptr };
ID2D1Bitmap* bmpSnailR[32]{ nullptr };

ID2D1Bitmap* bmpOctopus[36]{ nullptr };

//////////////////////////////////////////////////////

template<typename T>concept HasRelease = requires(T check)
{
	check.Release();
};
template<HasRelease T>bool FreeMem(T** var)
{
	if (*var)
	{
		(*var)->Release();
		(*var) = nullptr;
		return true;
	}

	return false;
};
void LogErr(const wchar_t* what)
{
	std::wofstream err{ L".\\res\\data\\error.log",std::ios::app };
	err << what << L" Time stamp: " << std::chrono::system_clock::now() << std::endl;
	err.close();
}

int GetIntroFrame()
{
	static int frame{ -1 };

	++frame;
	if (frame > 180)frame = 0;

	return frame;
}
void ClearResources()
{
	if (!FreeMem(&iFactory))LogErr(L"Error releasing D2D1 iFactory ! ");
	if (!FreeMem(&Draw))LogErr(L"Error releasing D2D1 HwndRenderTarget ! ");
	if (!FreeMem(&b1BckgBrush))LogErr(L"Error releasing D2D1 b1BckgBrush ! ");
	if (!FreeMem(&b2BckgBrush))LogErr(L"Error releasing D2D1 b2BckgBrush ! ");
	if (!FreeMem(&b3BckgBrush))LogErr(L"Error releasing D2D1 b3BckgBrush ! ");
	if (!FreeMem(&statBrush))LogErr(L"Error releasing D2D1 statBrush ! ");
	if (!FreeMem(&txtBrush))LogErr(L"Error releasing D2D1 txtBrush ! ");
	if (!FreeMem(&inactBrush))LogErr(L"Error releasing D2D1 inactBrush ! ");
	if (!FreeMem(&hgltBrush))LogErr(L"Error releasing D2D1 hgltBrush ! ");

	if (!FreeMem(&iWriteFactory))LogErr(L"Error releasing D2D1 iWriteFactory ! ");
	if (!FreeMem(&nrmTxt))LogErr(L"Error releasing D2D1 nrmTxt ! ");
	if (!FreeMem(&midTxt))LogErr(L"Error releasing D2D1 midTxt ! ");
	if (!FreeMem(&bigTxt))LogErr(L"Error releasing D2D1 bigTxt ! ");

	if (!FreeMem(&bmpArrowL))LogErr(L"Error releasing D2D1 bmpArrowL ! ");
	if (!FreeMem(&bmpArrowR))LogErr(L"Error releasing D2D1 bmpArrowR ! ");

	if (!FreeMem(&bmpTomahawkL))LogErr(L"Error releasing D2D1 bmpTomahawkL ! ");
	if (!FreeMem(&bmpTomahawkR))LogErr(L"Error releasing D2D1 bmpTomahawkR ! ");

	if (!FreeMem(&bmpChest))LogErr(L"Error releasing D2D1 bmpChest ! ");
	if (!FreeMem(&bmpCrystal))LogErr(L"Error releasing D2D1 bmpCrystal ! ");
	if (!FreeMem(&bmpGold))LogErr(L"Error releasing D2D1 bmpGold ! ");
	if (!FreeMem(&bmpPotion))LogErr(L"Error releasing D2D1 bmpPotion ! ");

	if (!FreeMem(&bmpBackground))LogErr(L"Error releasing D2D1 bmpBackground ! ");
	if (!FreeMem(&bmpGameLoose))LogErr(L"Error releasing D2D1 bmpGameLoose ! ");
	if (!FreeMem(&bmpGameWin))LogErr(L"Error releasing D2D1 bmpGameWin ! ");
	if (!FreeMem(&bmpGameRecord))LogErr(L"Error releasing D2D1 bmpGameRecord ! ");
	if (!FreeMem(&bmpLogo))LogErr(L"Error releasing D2D1 bmpLogo ! ");
	if (!FreeMem(&bmpPlatform1))LogErr(L"Error releasing D2D1 bmpPlatform1 ! ");
	if (!FreeMem(&bmpPlatform2))LogErr(L"Error releasing D2D1 bmpPlatform2 ! ");
	if (!FreeMem(&bmpPlatform3))LogErr(L"Error releasing D2D1 bmpPlatform3 ! ");
	if (!FreeMem(&bmpRIP))LogErr(L"Error releasing D2D1 bmpRIP ! ");

	if (!FreeMem(&bmpDirt))LogErr(L"Error releasing D2D1 bmpDirt ! ");
	if (!FreeMem(&bmpTrapAxe))LogErr(L"Error releasing D2D1 bmpTrapAxe ! ");
	if (!FreeMem(&bmpTrapBolt))LogErr(L"Error releasing D2D1 bmpTrapBolt ! ");
	if (!FreeMem(&bmpTrapSpear))LogErr(L"Error releasing D2D1 bmpTrapSpear ! ");
	if (!FreeMem(&bmpTree1))LogErr(L"Error releasing D2D1 bmpTree1 ! ");
	if (!FreeMem(&bmpTree2))LogErr(L"Error releasing D2D1 bmpTree2 ! ");
	if (!FreeMem(&bmpTree3))LogErr(L"Error releasing D2D1 bmpTree3 ! ");
	if (!FreeMem(&bmpWater))LogErr(L"Error releasing D2D1 bmpWater ! ");
	if (!FreeMem(&bmpWater2))LogErr(L"Error releasing D2D1 bmpWater2 ! ");

	for (int i = 0; i < 181; ++i)if (!FreeMem(&bmpIntro[i]))LogErr(L"Error releasing D2D1 bmpIntro ! ");

	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpFlyerL[i]))LogErr(L"Error releasing D2D1 bmpFlyerL ! ");
	for (int i = 0; i < 16; ++i)if (!FreeMem(&bmpFlyerR[i]))LogErr(L"Error releasing D2D1 bmpFlyerR ! ");

	for (int i = 0; i < 32; ++i)if (!FreeMem(&bmpSnailL[i]))LogErr(L"Error releasing D2D1 bmpSnailL ! ");
	for (int i = 0; i < 32; ++i)if (!FreeMem(&bmpSnailR[i]))LogErr(L"Error releasing D2D1 bmpSnailR ! ");

	for (int i = 0; i < 37; ++i)if (!FreeMem(&bmpMushroom[i]))LogErr(L"Error releasing D2D1 bmpMushroomR ! ");
	for (int i = 0; i < 36; ++i)if (!FreeMem(&bmpOctopus[i]))LogErr(L"Error releasing D2D1 bmpOctopus ! ");
}
void ErrExit(int what_happened)
{
	MessageBeep(MB_ICONERROR);
	MessageBox(NULL, ErrHandle(what_happened), L"Критична грешка !", MB_OK | MB_APPLMODAL | MB_ICONERROR);

	ClearResources();
	std::remove(tmp_file);
	exit(1);
}












int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	_wsetlocale(LC_ALL, L"");
	bIns = hInstance;
	if (!bIns)
	{
		LogErr(L"Error obtaining hInstance from Windows !");
		ErrExit(eClass);
	}












	ClearResources();
	std::remove(tmp_file);

    return (int) bMsg.wParam;
}