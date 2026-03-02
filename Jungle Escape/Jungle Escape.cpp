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

float x_scale{ 0 };
float y_scale{ 0 };

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
ID2D1Bitmap* bmpHouse{ nullptr };

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

///////////////////////////////////////////////////////

dll::RANDIT RandIt{};

std::vector<D2D1_RECT_F> vBackgrounds;
std::vector<dll::TILE*> vTiles;
dll::HERO* Hero{ nullptr };

dirs field_dir = dirs::stop;

bool need_back_left = false;
bool need_back_right = false;

bool need_tile_left = false;
bool need_tile_right = false;

D2D1_RECT_F House{ scr_width, ground - 150.0f, scr_width + 147.0f, ground };
bool house_active = false;


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
	if (!FreeMem(&bmpHouse))LogErr(L"Error releasing D2D1 bmpHouse ! ");

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

void GameOver()
{
	PlaySound(NULL, NULL, NULL);
	KillTimer(bHwnd, bTimer);



	bMsg.message = WM_QUIT;
	bMsg.wParam = 0;
}
void InitGame()
{
	wcscpy_s(current_player, L"TARLYO");
	level = 1;
	score = 0;
	mins = 0;
	secs = 300;

	hero_killed = false;
	name_set = false;

	house_active = false;
	House.left = scr_width;
	House.right = scr_width + 147.0f;
	House.top = ground - 150.0f;
	House.bottom = ground;

	need_back_left = false;
	need_back_right = false;

	need_tile_left = false;
	need_tile_right = false;

	field_dir = dirs::stop;

	vBackgrounds.clear();
	for (float t_x = -scr_width; t_x < 2 * scr_width; t_x += scr_width)
		vBackgrounds.push_back(D2D1::RectF(t_x, 50.0f, scr_width, scr_height));

	if (!vTiles.empty())for (int i = 0; i < vTiles.size(); ++i)FreeMem(&vTiles[i]);
	vTiles.clear();
	float tile_x{ -scr_width };
	for (int i = 0; i < 54; ++i)
	{
		tiles temp_type{ static_cast<tiles>(RandIt(0,5)) };
		if ((temp_type == tiles::trap_axe|| temp_type == tiles::trap_bolt|| temp_type == tiles::trap_spear) 
			&& RandIt(0, 3) == 1)temp_type = tiles::dirt;

		if (temp_type != tiles::water && temp_type != tiles::dirt_water)
		{
			vTiles.push_back(dll::TILE::create(temp_type, tile_x, ground - 50.0f, dirs::stop));
			tile_x += 50.0f;
		}
		else
		{
			for (int j = 0; j < 2; ++j)
			{
				vTiles.push_back(dll::TILE::create(temp_type, tile_x, ground - 50.0f, dirs::stop));
				tile_x += 50.0f;
			}
		}
	}

	if (Hero)FreeMem(&Hero);
	Hero = dll::HERO::create(100.0f, ground - 51.0f);

}

INT_PTR CALLBACK DlgProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_INITDIALOG:
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)(mainIcon));
		return true;

	case WM_CLOSE:
		EndDialog(hwnd, IDCANCEL);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;

		case IDOK:
			if (GetDlgItemText(hwnd, IDC_NAME, current_player, 16) < 1)
			{
				wcscpy_s(current_player, L"TARLYO");
				if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);

				MessageBox(bHwnd, L"Ха, ха, ха ! Забрави си името !", L"Забраватор !", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);

				EndDialog(hwnd, IDCANCEL);
				break;
			}
			EndDialog(hwnd, IDOK);
			break;
		}
	}

	return (INT_PTR)(FALSE);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT ReceivedMsg, WPARAM wParam, LPARAM lParam)
{
	switch (ReceivedMsg)
	{
	case WM_CREATE:
		if (bIns)
		{
			SetTimer(hwnd, bTimer, 1000, NULL);

			bBar = CreateMenu();
			bMain = CreateMenu();
			bStore = CreateMenu();

			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bMain), L"Основно меню");
			AppendMenu(bBar, MF_POPUP, (UINT_PTR)(bStore), L"Меню за данни");

			AppendMenu(bMain, MF_STRING, mNew, L"Нова игра");
			AppendMenu(bMain, MF_STRING, mLvl, L"Следващо ниво");
			AppendMenu(bMain, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bMain, MF_STRING, mExit, L"Изход");

			AppendMenu(bStore, MF_STRING, mSave, L"Запази игра");
			AppendMenu(bStore, MF_STRING, mLoad, L"Зареди игра");
			AppendMenu(bStore, MF_SEPARATOR, NULL, NULL);
			AppendMenu(bStore, MF_STRING, mHoF, L"Зала на славата");
			SetMenu(hwnd, bBar);
			
			InitGame();
		}
		break;

	case WM_CLOSE:
		pause = true;
		if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
		if (MessageBox(hwnd, L"Ако излезеш, губиш прогреса по тази игра !\n\nНаистина ли излизаш ?",
			L"Изход !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
		{
			pause = false;
			break;
		}
		GameOver();
		break;

	case WM_TIMER:
		if (pause)break;
		--secs;
		mins = secs / 60;
		break;

	case WM_PAINT:
		PaintDC = BeginPaint(hwnd, &bPaint);
		FillRect(PaintDC, &bPaint.rcPaint, CreateSolidBrush(RGB(10, 10, 10)));
		EndPaint(hwnd, &bPaint);
		break;

	case WM_SETCURSOR:
		GetCursorPos(&cur_pos);
		ScreenToClient(hwnd, &cur_pos);
		if (LOWORD(lParam) == HTCLIENT)
		{
			if (!in_client)
			{
				in_client = true;
				pause = false;
			}

			if (cur_pos.y * y_scale <= 50)
			{
				if (cur_pos.x * x_scale >= b1Rect.left && cur_pos.x * x_scale <= b1Rect.right)
				{
					if (!b1Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = true;
						b2Hglt = false;
						b3Hglt = false;
					}
				}
				else if (cur_pos.x * x_scale >= b2Rect.left && cur_pos.x * x_scale <= b2Rect.right)
				{
					if (!b2Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = true;
						b3Hglt = false;
					}
				}
				else if (cur_pos.x * x_scale >= b3Rect.left && cur_pos.x * x_scale <= b3Rect.right)
				{
					if (!b3Hglt)
					{
						if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
						b1Hglt = false;
						b2Hglt = false;
						b3Hglt = true;
					}
				}
				else if (b1Hglt || b2Hglt || b3Hglt)
				{
					if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
					b1Hglt = false;
					b2Hglt = false;
					b3Hglt = false;
				}

				SetCursor(outCur);

				return true;
			}
			else
			{
				if (b1Hglt || b2Hglt || b3Hglt)
				{
					if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
					b1Hglt = false;
					b2Hglt = false;
					b3Hglt = false;
				}
			}

			SetCursor(mainCur);

			return true;
		}
		else
		{
			if (in_client)
			{
				in_client = false;
				pause = true;
			}
			if (b1Hglt || b2Hglt || b3Hglt)
			{
				if (sound)mciSendString(L"play .\\res\\snd\\click.wav", NULL, NULL, NULL);
				b1Hglt = false;
				b2Hglt = false;
				b3Hglt = false;
			}

			SetCursor(LoadCursorW(NULL, IDC_ARROW));

			return true;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case mNew:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Ако рестартираш, губиш прогреса по тази игра !\n\nНаистина ли рестартираш ?",
				L"Рестарт !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
			{
				pause = false;
				break;
			}
			InitGame();
			break;

		case mLvl:
			pause = true;
			if (sound)mciSendString(L"play .\\res\\snd\\exclamation.wav", NULL, NULL, NULL);
			if (MessageBox(hwnd, L"Ако прескочиш нивото, губиш прогреса по него !\n\nНаистина ли прескачаш ?",
				L"Следващо ниво !", MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION) == IDNO)
			{
				pause = false;
				break;
			}
			//LevelUp();
			break;

		case mExit:
			SendMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;


		}
		break;






	default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
	}

	return (LRESULT)(FALSE);
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