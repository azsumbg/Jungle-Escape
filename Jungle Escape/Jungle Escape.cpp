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

D2D1_RECT_F b1TxtRect{ 60.0f, 15.0f, scr_width / 3.0f - 50.0f, 45.0f };
D2D1_RECT_F b2TxtRect{ scr_width / 3.0f + 60.0f, 15.0f, scr_width * 2.0f / 3.0f - 50.0f, 45.0f };
D2D1_RECT_F b3TxtRect{ scr_width * 2.0f / 3.0f + 50.0f, 15.0f, scr_width - 50.0f, 45.0f };

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

ID2D1Bitmap* bmpHeroL[4]{ nullptr };
ID2D1Bitmap* bmpHeroR[4]{ nullptr };

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
std::vector<dll::TILE*> vTrees;

std::vector<dll::PLATFORM*>vPlatforms;
std::vector<dll::EVIL*>vEvils;

std::vector<dll::SHOT*>vTomahawks;
std::vector<dll::SHOT*>vArrows;

dll::HERO* Hero{ nullptr };
FPOINT hero_tomb{};

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

	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpHeroL[i]))LogErr(L"Error releasing D2D1 bmpHeroL ! ");
	for (int i = 0; i < 4; ++i)if (!FreeMem(&bmpHeroR[i]))LogErr(L"Error releasing D2D1 bmpHeroR ! ");

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
	for (float t_x = -scr_width; t_x < 2.0f * scr_width; t_x += scr_width)
		vBackgrounds.push_back(D2D1::RectF(t_x, 50.0f, t_x + scr_width, scr_height));

	if (!vTrees.empty())for (int i = 0; i < vTrees.size(); ++i)FreeMem(&vTrees[i]);
	vTrees.clear();

	if (!vPlatforms.empty())for (int i = 0; i < vPlatforms.size(); ++i)FreeMem(&vPlatforms[i]);
	vPlatforms.clear();

	if (!vEvils.empty())for (int i = 0; i < vEvils.size(); ++i)FreeMem(&vEvils[i]);
	vEvils.clear();

	if (!vTomahawks.empty())for (int i = 0; i < vTomahawks.size(); ++i)FreeMem(&vTomahawks[i]);
	vTomahawks.clear();

	if (!vArrows.empty())for (int i = 0; i < vArrows.size(); ++i)FreeMem(&vArrows[i]);
	vArrows.clear();

	if (!vTiles.empty())for (int i = 0; i < vTiles.size(); ++i)FreeMem(&vTiles[i]);
	vTiles.clear();
	float tile_x{ -scr_width };
	for (int i = 0; i < 54; ++i)
	{
		tiles temp_type{ tiles::dirt };
		int trouble = RandIt(0, 100);

		if (trouble == 51)temp_type = tiles::water;
		else if (trouble == 78)temp_type = tiles::dirt_water;
		else if (trouble == 11)temp_type = tiles::trap_axe;
		else if (trouble == 66)temp_type = tiles::trap_bolt;
		else if (trouble == 33)temp_type = tiles::trap_spear;

		if (tile_x >= 0 && tile_x <= 200.0f)temp_type = tiles::dirt;

		if (temp_type != tiles::water && temp_type != tiles::dirt_water)
		{
			vTiles.push_back(dll::TILE::create(temp_type, tile_x, ground, dirs::stop));
			tile_x += 50.0f;
		}
		else
		{
			for (int j = 0; j < 2; ++j)
			{
				vTiles.push_back(dll::TILE::create(temp_type, tile_x, ground, dirs::stop));
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

	case WM_KEYDOWN:
		if (Hero)
		{
			switch (wParam)
			{
			case VK_RIGHT:
				Hero->dir = dirs::right;
				break;

			case VK_LEFT:
				Hero->dir = dirs::left;
				break;

			case VK_UP:
				if (Hero->in_jump || Hero->state == FALLING)break;
				Hero->jump((float)(level));
				break;

			case VK_DOWN:
				Hero->dir = dirs::stop;
				break;

			case VK_SPACE:
				if (!vEvils.empty())
				{
					dll::BAG<FPOINT> evil(vEvils.size());

					for (int i = 0; i < vEvils.size(); ++i)evil.push_back(vEvils[i]->center);

					dll::Sort(evil, Hero->center);

					vTomahawks.push_back(dll::SHOT::create(shots::tomahawk, Hero->center.x, Hero->center.y,
						evil[0].x, evil[0].y));
					vTomahawks.back()->damage = Hero->strenght;
					if (evil[0].x >= Hero->center.x)vTomahawks.back()->dir = dirs::right;
					else vTomahawks.back()->dir = dirs::left;
				}
				else if (sound)mciSendString(L"play .\\res\\snd\\negative.wav", NULL, NULL, NULL);
			}
		}
		break;



	default: return DefWindowProc(hwnd, ReceivedMsg, wParam, lParam);
	}

	return (LRESULT)(FALSE);
}

void CreateResources()
{
	int result{ 0 };

	CheckFile(Ltmp_file, &result);
	if (result == FILE_EXIST)ErrExit(eStarted);
	else
	{
		std::wofstream start{ Ltmp_file };
		start << L"Game started at: " << std::chrono::system_clock::now();
		start.close();
	}

	mainIcon = (HICON)(LoadImage(NULL, L".\\res\\main.ico", IMAGE_ICON, 255, 255, LR_LOADFROMFILE));
	if (!mainIcon)ErrExit(eIcon);
	mainCur = LoadCursorFromFileW(L".\\res\\main.ani");
	outCur = LoadCursorFromFileW(L".\\res\\out.ani");
	if (!mainCur or !outCur)ErrExit(eCursor);

	int win_x{ GetSystemMetrics(SM_CXSCREEN) / 2 - (int)(scr_width / 2.0f) };
	int win_y{ 50 };

	if (GetSystemMetrics(SM_CXSCREEN) < win_x + (int)(scr_width) || GetSystemMetrics(SM_CYSCREEN) < win_y + (int)(scr_height))ErrExit(eScreen);

	bWinClass.lpszClassName = bWinClassName;
	bWinClass.hInstance = bIns;
	bWinClass.lpfnWndProc = &WinProc;
	bWinClass.hbrBackground = CreateSolidBrush(RGB(10, 10, 10));
	bWinClass.hIcon = mainIcon;
	bWinClass.hCursor = mainCur;
	bWinClass.style = CS_DROPSHADOW;

	if (!RegisterClass(&bWinClass))ErrExit(eClass);

	bHwnd = CreateWindow(bWinClassName, L" JUNGLE RUN", WS_CAPTION | WS_SYSMENU, win_x, win_y, (int)(scr_width), (int)(scr_height), NULL, NULL, bIns, NULL);
	if (!bHwnd)ErrExit(eWindow);
	else
	{
		ShowWindow(bHwnd, SW_SHOWDEFAULT);

		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &iFactory);
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 Factory !");
			ErrExit(eD2D);
		}

		if (iFactory)
			hr = iFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(bHwnd,
				D2D1::SizeU((UINT32)(scr_width), (UINT32)(scr_height))), &Draw);
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 HwndRenderTarget !");
			ErrExit(eD2D);
		}

		if (Draw)
		{
			RECT ClR{};
			GetClientRect(bHwnd, &ClR);
			D2D1_SIZE_F RenderR{ Draw->GetSize() };

			x_scale = RenderR.width / (ClR.right - ClR.left);
			y_scale = RenderR.height / (ClR.bottom - ClR.top);

			D2D1_GRADIENT_STOP gStops[2]{};
			ID2D1GradientStopCollection* gColl{ nullptr };

			gStops[0].position = 0;
			gStops[0].color = D2D1::ColorF(D2D1::ColorF::LightSeaGreen);
			gStops[1].position = 1.0f;
			gStops[1].color = D2D1::ColorF(D2D1::ColorF::DarkGreen);

			hr = Draw->CreateGradientStopCollection(gStops, 2, &gColl);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 GradienStopCollection !");
				ErrExit(eD2D);
			}

			if (gColl)
			{
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b1Rect.left +
					(b1Rect.right - b1Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b1Rect.right - b1Rect.left) / 2.0f, 25.0f), gColl, &b1BckgBrush);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b2Rect.left +
					(b2Rect.right - b2Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b2Rect.right - b2Rect.left) / 2.0f, 25.0f), gColl, &b2BckgBrush);
				hr = Draw->CreateRadialGradientBrush(D2D1::RadialGradientBrushProperties(D2D1::Point2F(b3Rect.left +
					(b3Rect.right - b3Rect.left) / 2.0f, 25.0f), D2D1::Point2F(0, 0), (b3Rect.right - b3Rect.left) / 2.0f, 25.0f), gColl, &b3BckgBrush);
				if (hr != S_OK)
				{
					LogErr(L"Error creating D2D1 RadialGradientBrushes !");
					ErrExit(eD2D);
				}
			
				if (!FreeMem(&gColl))LogErr(L"Error releasing D2D1 GradientStopCollection ! ");
			}

			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Brown), &statBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &txtBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::NavajoWhite), &hgltBrush);
			hr = Draw->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::IndianRed), &inactBrush);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 SolidColorBrushes !");
				ErrExit(eD2D);
			}

			bmpArrowL = Load(L".\\res\\img\\assets\\arrow_l.png", Draw);
			if (!bmpArrowL)
			{
				LogErr(L"Error loading bmpArrowL !");
				ErrExit(eD2D);
			}
			bmpArrowR = Load(L".\\res\\img\\assets\\arrow_r.png", Draw);
			if (!bmpArrowR)
			{
				LogErr(L"Error loading bmpArrowR !");
				ErrExit(eD2D);
			}
			bmpTomahawkL = Load(L".\\res\\img\\assets\\Tomahawk_l.png", Draw);
			if (!bmpTomahawkL)
			{
				LogErr(L"Error loading bmpTomahawkL !");
				ErrExit(eD2D);
			}
			bmpTomahawkR = Load(L".\\res\\img\\assets\\Tomahawk_r.png", Draw);
			if (!bmpTomahawkR)
			{
				LogErr(L"Error loading bmpTomahawkR !");
				ErrExit(eD2D);
			}
			bmpChest = Load(L".\\res\\img\\assets\\chest.png", Draw);
			if (!bmpChest)
			{
				LogErr(L"Error loading bmpChest !");
				ErrExit(eD2D);
			}
			bmpCrystal = Load(L".\\res\\img\\assets\\crystal.png", Draw);
			if (!bmpCrystal)
			{
				LogErr(L"Error loading bmpCrystal !");
				ErrExit(eD2D);
			}
			bmpGold = Load(L".\\res\\img\\assets\\gold.png", Draw);
			if (!bmpGold)
			{
				LogErr(L"Error loading bmpGold !");
				ErrExit(eD2D);
			}
			bmpPotion = Load(L".\\res\\img\\assets\\potion.png", Draw);
			if (!bmpPotion)
			{
				LogErr(L"Error loading bmpPotion !");
				ErrExit(eD2D);
			}

			bmpBackground = Load(L".\\res\\img\\field\\background.png", Draw);
			if (!bmpBackground)
			{
				LogErr(L"Error loading bmpBackground !");
				ErrExit(eD2D);
			}
			bmpLogo = Load(L".\\res\\img\\field\\Logo.png", Draw);
			if (!bmpLogo)
			{
				LogErr(L"Error loading bmpLogo !");
				ErrExit(eD2D);
			}
			bmpGameWin = Load(L".\\res\\img\\field\\game_win.png", Draw);
			if (!bmpGameWin)
			{
				LogErr(L"Error loading bmpGameWin !");
				ErrExit(eD2D);
			}
			bmpGameLoose = Load(L".\\res\\img\\field\\game_loose.png", Draw);
			if (!bmpGameLoose)
			{
				LogErr(L"Error loading bmpGameLoose !");
				ErrExit(eD2D);
			}
			bmpGameRecord = Load(L".\\res\\img\\field\\game_record.png", Draw);
			if (!bmpGameRecord)
			{
				LogErr(L"Error loading bmpGameRecord !");
				ErrExit(eD2D);
			}
			bmpRIP = Load(L".\\res\\img\\field\\rip.png", Draw);
			if (!bmpRIP)
			{
				LogErr(L"Error loading bmpRIP !");
				ErrExit(eD2D);
			}
			bmpHouse = Load(L".\\res\\img\\field\\house.png", Draw);
			if (!bmpHouse)
			{
				LogErr(L"Error loading bmpHouse !");
				ErrExit(eD2D);
			}
			bmpPlatform1 = Load(L".\\res\\img\\field\\Platform1.png", Draw);
			if (!bmpPlatform1)
			{
				LogErr(L"Error loading bmpPlatform1 !");
				ErrExit(eD2D);
			}
			bmpPlatform2 = Load(L".\\res\\img\\field\\Platform2.png", Draw);
			if (!bmpPlatform2)
			{
				LogErr(L"Error loading bmpPlatform2 !");
				ErrExit(eD2D);
			}
			bmpPlatform3 = Load(L".\\res\\img\\field\\Platform3.png", Draw);
			if (!bmpPlatform3)
			{
				LogErr(L"Error loading bmpPlatform3 !");
				ErrExit(eD2D);
			}

			bmpDirt = Load(L".\\res\\img\\field\\tiles\\dirt.png", Draw);
			if (!bmpDirt)
			{
				LogErr(L"Error loading bmpDirt !");
				ErrExit(eD2D);
			}
			bmpTrapAxe = Load(L".\\res\\img\\field\\tiles\\trap_axe.png", Draw);
			if (!bmpTrapAxe)
			{
				LogErr(L"Error loading bmpTrapAxe !");
				ErrExit(eD2D);
			}
			bmpTrapBolt = Load(L".\\res\\img\\field\\tiles\\trap_bolt.png", Draw);
			if (!bmpTrapBolt)
			{
				LogErr(L"Error loading bmpTrapBolt !");
				ErrExit(eD2D);
			}
			bmpTrapSpear = Load(L".\\res\\img\\field\\tiles\\trap_spear.png", Draw);
			if (!bmpTrapSpear)
			{
				LogErr(L"Error loading bmpTrapSpear !");
				ErrExit(eD2D);
			}
			bmpWater = Load(L".\\res\\img\\field\\tiles\\Water.png", Draw);
			if (!bmpWater)
			{
				LogErr(L"Error loading bmpWater !");
				ErrExit(eD2D);
			}
			bmpWater2 = Load(L".\\res\\img\\field\\tiles\\Water2.png", Draw);
			if (!bmpWater2)
			{
				LogErr(L"Error loading bmpWater2 !");
				ErrExit(eD2D);
			}
			bmpTree1 = Load(L".\\res\\img\\field\\tiles\\Tree1.png", Draw);
			if (!bmpTree1)
			{
				LogErr(L"Error loading bmpTree1 !");
				ErrExit(eD2D);
			}
			bmpTree2 = Load(L".\\res\\img\\field\\tiles\\Tree2.png", Draw);
			if (!bmpTree2)
			{
				LogErr(L"Error loading bmpTree2 !");
				ErrExit(eD2D);
			}
			bmpTree3 = Load(L".\\res\\img\\field\\tiles\\Tree3.png", Draw);
			if (!bmpTree3)
			{
				LogErr(L"Error loading bmpTree3 !");
				ErrExit(eD2D);
			}

			for (int i = 0; i < 181; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\intro\\0" };
				wchar_t add[5]{ L"\0" };

				if (i < 10)wcscat_s(name, L"00");
				else if (i < 100)wcscat_s(name, L"0");

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpIntro[i] = Load(name, Draw);

				if (!bmpIntro[i])
				{
					LogErr(L"Error loading bmpIntro !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 4; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\hero\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroL[i] = Load(name, Draw);

				if (!bmpHeroL[i])
				{
					LogErr(L"Error loading bmpHeroL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 4; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\hero\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpHeroR[i] = Load(name, Draw);

				if (!bmpHeroR[i])
				{
					LogErr(L"Error loading bmpHeroR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 16; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\evil1\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpFlyerL[i] = Load(name, Draw);

				if (!bmpFlyerL[i])
				{
					LogErr(L"Error loading bmpFlyerL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 16; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\evil1\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpFlyerR[i] = Load(name, Draw);

				if (!bmpFlyerR[i])
				{
					LogErr(L"Error loading bmpFlyerR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 37; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\evil2\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpMushroom[i] = Load(name, Draw);

				if (!bmpMushroom[i])
				{
					LogErr(L"Error loading bmpMushroom !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 32; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\evil3\\l\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpSnailL[i] = Load(name, Draw);

				if (!bmpSnailL[i])
				{
					LogErr(L"Error loading bmpSnailL !");
					ErrExit(eD2D);
				}
			}
			for (int i = 0; i < 32; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\evil3\\r\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpSnailR[i] = Load(name, Draw);

				if (!bmpSnailR[i])
				{
					LogErr(L"Error loading bmpSnailR !");
					ErrExit(eD2D);
				}
			}

			for (int i = 0; i < 36; ++i)
			{
				wchar_t name[150]{ L".\\res\\img\\evil4\\" };
				wchar_t add[5]{ L"\0" };

				wsprintf(add, L"%d", i);
				wcscat_s(name, add);
				wcscat_s(name, L".png");

				bmpOctopus[i] = Load(name, Draw);

				if (!bmpOctopus[i])
				{
					LogErr(L"Error loading bmpOctopus !");
					ErrExit(eD2D);
				}
			}

		}

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&iWriteFactory));
		if (hr != S_OK)
		{
			LogErr(L"Error creating D2D1 iWriteFactory !");
			ErrExit(eD2D);
		}

		if (iWriteFactory)
		{
			hr = iWriteFactory->CreateTextFormat(L"Cascadia code", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
				DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 16.0f, L"", &nrmTxt);
			hr = iWriteFactory->CreateTextFormat(L"Cascadia code", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
				DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"", &midTxt);
			hr = iWriteFactory->CreateTextFormat(L"Cascadia code", NULL, DWRITE_FONT_WEIGHT_EXTRA_BLACK,
				DWRITE_FONT_STYLE_OBLIQUE, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"", &bigTxt);
			if (hr != S_OK)
			{
				LogErr(L"Error creating D2D1 iWriteFactory Text Formats !");
				ErrExit(eD2D);
			}
		}
	}

	PlaySound(L".\\res\\snd\\intro.wav", NULL, SND_ASYNC);

	for (int i = 0; i < 181; ++i)
	{
		Draw->BeginDraw();
		Draw->DrawBitmap(bmpIntro[i], D2D1::RectF(0, 0, scr_width, scr_height));
		Draw->DrawBitmap(bmpLogo, D2D1::RectF(0, 0, scr_width, scr_height));
		Draw->EndDraw();
	}
	Sleep(2000);
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

	CreateResources();

	while (bMsg.message != WM_QUIT)
	{
		if ((bRet = PeekMessage(&bMsg, NULL, NULL, NULL, PM_REMOVE)) != 0)
		{
			if (bRet == -1)ErrExit(eMsg);
			TranslateMessage(&bMsg);
			DispatchMessage(&bMsg);
		}

		if (pause)
		{
			if (show_help)continue;

			if (bigTxt && inactBrush)
			{
				Draw->BeginDraw();
				Draw->DrawBitmap(bmpIntro[GetIntroFrame()], D2D1::RectF(0, 0, scr_width, scr_height));
				Draw->DrawTextW(L"ПАУЗА", 6, bigTxt, D2D1::RectF(scr_width / 2.0f - 100.0f, scr_height / 2.0f - 50.0f,
					scr_width, scr_height), inactBrush);
				Draw->EndDraw();
				continue;
			}
		}

		//////////////////////////////////////////////////////////////

		if (vTrees.size() < 10 && RandIt(0, 100) == 66)
		{
			tiles ttype = static_cast<tiles>(RandIt(6, 8));
			float t_x = scr_width + RandIt(50.0f, 150.0f);
			bool not_permitted = false;

			dll::TILE* temp_tree{ dll::TILE::create(ttype,t_x,ground - 50.0f,dirs::left) };

			if (!vTrees.empty())for (int i = 0; i < vTrees.size(); ++i)
			{
				FRECT in_tree{ vTrees[i]->start.x,vTrees[i]->start.y,vTrees[i]->end.x,vTrees[i]->end.y };
				FRECT candidate{ temp_tree->start.x,temp_tree->start.y,temp_tree->end.x,temp_tree->end.y };
				if (dll::Intersect(in_tree, candidate))
				{
					not_permitted = true;
					break;
				}
			}

			if (!not_permitted)vTrees.push_back(temp_tree);
		}
		if (!vTrees.empty())
		{
			for (std::vector<dll::TILE*>::iterator tree = vTrees.begin(); tree < vTrees.end(); ++tree)
			{
				if (!(*tree)->move(field_dir, (float)(level)))
				{
					vTrees.erase(tree);
					break;
				}
			}
		}

		if (vPlatforms.size() < 4 && RandIt(0, 300) == 66)
		{
			platforms ttype = static_cast<platforms>(RandIt(0, 2));

			float t_x = scr_width + RandIt(50.0f, 150.0f);
			float t_y{ 0 };

			switch (ttype)
			{
			case platforms::flat_platform1:
				t_y = ground - 75.0f;
				break;

			case platforms::flat_platform2:
				t_y = ground - 90.0f;
				break;

			case platforms::high_platform:
				t_y = ground - 90.0f;
				break;

			}
			bool not_permitted = false;

			dll::PLATFORM* temp_tree{ dll::PLATFORM::create(ttype,t_x, t_y,dirs::left) };

			if (!vPlatforms.empty())for (int i = 0; i < vPlatforms.size(); ++i)
			{
				FRECT in_tree{ vPlatforms[i]->start.x,vPlatforms[i]->start.y,vPlatforms[i]->end.x,vPlatforms[i]->end.y };
				FRECT candidate{ temp_tree->start.x,temp_tree->start.y,temp_tree->end.x,temp_tree->end.y };
				if (dll::Intersect(in_tree, candidate))
				{
					not_permitted = true;
					break;
				}
			}

			if (!not_permitted)vPlatforms.push_back(temp_tree);
		}
		if (!vPlatforms.empty())
		{
			for (std::vector<dll::PLATFORM*>::iterator tree = vPlatforms.begin(); tree < vPlatforms.end(); ++tree)
			{
				if (!(*tree)->move(field_dir, (float)(level)))
				{
					vPlatforms.erase(tree);
					break;
				}
			}
		}

		/////////////////////////////////////////////////////////////

		if (Hero)
		{
			switch (Hero->dir)
			{
			case dirs::left:
				field_dir = dirs::right;
				break;

			case dirs::right:
				field_dir = dirs::left;
				break;

			case dirs::stop:
				field_dir = dirs::stop;
				break;
			}

			if (Hero->in_jump)
			{
				Hero->jump((float)(level));
				if (Hero->state == FALLING)Hero->fall((float)(level));
			}
			else if (Hero->state == FALLING)Hero->fall((float)(level));
			else Hero->move((float)(level));
		}
		if (!vBackgrounds.empty())
		{
			switch (field_dir)
			{
			case dirs::left:
				for (int i = 0; i < vBackgrounds.size(); ++i)
				{
					vBackgrounds[i].left -= (float)(level);
					vBackgrounds[i].right -= (float)(level);
					if (vBackgrounds[i].right <= -scr_width)
					{
						vBackgrounds.erase(vBackgrounds.begin() + i);
						need_back_right = true;
						break;
					}
				}
				break;

			case dirs::right:
				for (int i = 0; i < vBackgrounds.size(); ++i)
				{
					vBackgrounds[i].left += (float)(level);
					vBackgrounds[i].right += (float)(level);
					if (vBackgrounds[i].left >= 2.0f * scr_width)
					{
						vBackgrounds.erase(vBackgrounds.begin() + i);
						need_back_left = true;
						break;
					}
				}
				break;
			}
		}
		if (need_back_left)
		{
			need_back_left = false;
			vBackgrounds.insert(vBackgrounds.begin(), D2D1::RectF(-scr_width, 50.0f, 0, scr_height));
		}
		if (need_back_right)
		{
			need_back_right = false;
			vBackgrounds.push_back(D2D1::RectF(scr_width, 50.0, 2.0f * scr_width, scr_height));
		}
		if (!vTiles.empty())
		{
			for (std::vector<dll::TILE*>::iterator atile = vTiles.begin(); atile < vTiles.end(); ++atile)
			{
				if (!(*atile)->move(field_dir, (float)(level)))
				{
					vTiles.erase(atile);
					switch (field_dir)
					{
					case dirs::left:
						need_tile_right = true;
						break;

					case dirs::right:
						need_tile_left = true;
						break;
					}

					break;
				}
			}
		}
		if (need_tile_left)
		{
			need_tile_left = false;

			tiles temp_type{ tiles::dirt };
			int trouble = RandIt(0, 100);

			if (trouble == 51)temp_type = tiles::water;
			else if (trouble == 78)temp_type = tiles::dirt_water;
			else if (trouble == 11)temp_type = tiles::trap_axe;
			else if (trouble == 66)temp_type = tiles::trap_bolt;
			else if (trouble == 33)temp_type = tiles::trap_spear;

			if (temp_type != tiles::water && temp_type != tiles::dirt_water)
				vTiles.insert(vTiles.begin(), dll::TILE::create(temp_type, vTiles.front()->start.x - 50.0f,
					ground, dirs::right));
			else
			{
				for (int i = 0; i < 2; ++i)
					vTiles.insert(vTiles.begin(), dll::TILE::create(temp_type, vTiles.front()->start.x - 50.0f,
						ground, dirs::right));
			}
		}
		if (need_tile_right)
		{
			need_tile_right = false;

			tiles temp_type{ tiles::dirt };
			int trouble = RandIt(0, 100);

			if (trouble == 51)temp_type = tiles::water;
			else if (trouble == 78)temp_type = tiles::dirt_water;
			else if (trouble == 11)temp_type = tiles::trap_axe;
			else if (trouble == 66)temp_type = tiles::trap_bolt;
			else if (trouble == 33)temp_type = tiles::trap_spear;

			if (temp_type != tiles::water && temp_type != tiles::dirt_water)
				vTiles.push_back(dll::TILE::create(temp_type, vTiles.back()->end.x, ground, dirs::left));
			else
			{
				for (int i = 0; i < 2; ++i)
					vTiles.push_back(dll::TILE::create(temp_type, vTiles.back()->end.x, ground, dirs::left));
			}
		}

		// MOVE *****************************************************

		if (Hero && !vPlatforms.empty())
		{
			if (Hero->in_jump || Hero->state == FALLING)
			{
				FRECT heroR{ Hero->start.x,Hero->start.y, Hero->end.x, Hero->end.y };

				for (std::vector<dll::PLATFORM*>::iterator plat = vPlatforms.begin(); plat < vPlatforms.end(); ++plat)
				{
					FRECT platR{ (*plat)->start.x,(*plat)->start.y, (*plat)->end.x,(*plat)->end.y };

					if (dll::Intersect(heroR, platR))
					{
						Hero->set_platform(platR);
						break;
					}
				}

			}
		}

		if (Hero)
		{
			if (Hero->on_platform)
			{
				FRECT HeroR{ Hero->start.x,Hero->start.y,Hero->end.x, Hero->end.y };

				if (!dll::Intersect(HeroR, Hero->platform))
				{
					Hero->state = FALLING;
					Hero->on_platform = false;
					Hero->in_jump = false;
				}
			}
		}

		if (vEvils.size() < 5 + level && RandIt(0, 100) == 6)
		{
			evils temp_type{ static_cast<evils>(RandIt(0, 3)) };
			float temp_y = 0;
			switch (temp_type)
			{
			case evils::flyer:
				temp_y = ground - 43.0f;
				break;

			case evils::mushroom:
				temp_y = ground - 41.0f;
				break;

			case evils::snail:
				temp_y = ground - 45.0f;
				break;

			case evils::octopus:
				temp_y = ground - 51.0f;
				break;
			}
			vEvils.push_back(dll::EVIL::create(temp_type, scr_width + RandIt(0.0f, 200.0f), temp_y));
		}

		if (!vEvils.empty() && Hero)
		{
			dll::BAG<FPOINT> tomahawkBag;
			dll::BAG<FPOINT> obstBag;

			if (!vTomahawks.empty())for (int i = 0; i < vTomahawks.size(); ++i)tomahawkBag.push_back(vTomahawks[i]->center);
			if (!vTiles.empty())
			{
				for (int i = 0; i < vTiles.size(); ++i)
				{
					if (vTiles[i]->type != tiles::dirt)
						obstBag.push_back(vTiles[i]->center);
				}
			}

			for (int i = 0; i < vEvils.size(); ++i)
			{
				dll::EVIL* anEvil{ vEvils[i] };

				switch (dll::AIDispatcher(*anEvil, Hero->center, tomahawkBag, obstBag))
				{
				case RUN:
					vEvils[i]->state = RUN;
					break;

				case FALLING:
					vEvils[i]->state = FALLING;
					break;

				case JUMP_UP:
					if (!vEvils[i]->in_jump)vEvils[i]->jump((float)(level));
					break;

				case SHOOT:
					vEvils[i]->state = SHOOT;
					break;
				}
			}
		}

		if (!vEvils.empty())
		{
			for (int i = 0; i < vEvils.size(); ++i)
			{
				bool evil_gone = false;

				switch (vEvils[i]->state)
				{
				case RUN:
					if (!vEvils[i]->move((float)(level)))
					{
						evil_gone = true;
						vEvils.erase(vEvils.begin() + i);
						break;
					}
					break;

				case FALLING:
					vEvils[i]->fall((float)(level));
					break;

				case JUMP_UP:
					vEvils[i]->jump((float)(level));
					break;

				case JUMP_DOWN:
					vEvils[i]->jump((float)(level));
					break;

				case SHOOT:
				{
					int ready = vEvils[i]->attack();
					if (ready > 0)
					{
						vArrows.push_back(dll::SHOT::create(shots::arrow, vEvils[i]->start.x, vEvils[i]->start.y,
							Hero->center.x, Hero->center.y));
						vArrows.back()->damage = ready;
						if (Hero->center.x >= vEvils[i]->center.x)vArrows.back()->dir = dirs::right;
						else vArrows.back()->dir = dirs::left;
					}
				}
				break;
				
				}

				if (evil_gone)break;
			}
		}

		////////////////////////////////////////////////////////////
		
		// BATTLE *****************************
		
		if (!vEvils.empty() && !vTomahawks.empty())
		{
			bool killed = false;

			for (std::vector<dll::EVIL*>::iterator evil = vEvils.begin(); evil < vEvils.end(); ++evil)
			{
				for (std::vector<dll::SHOT*>::iterator shot = vTomahawks.begin(); shot < vTomahawks.end(); ++shot)
				{
					if (dll::Intersect((*evil)->center, (*shot)->center, (*evil)->x_rad, (*shot)->x_rad,
						(*evil)->y_rad, (*shot)->y_rad))
					{
						(*evil)->lifes -= (*shot)->damage;
						vTomahawks.erase(shot);

						if ((*evil)->lifes <= 0)
						{
							vEvils.erase(evil);
							score += 3 * level;
							killed = true;
						}

						break;
					}
				}

				if (killed)break;
			}
		}
		
		if (!vArrows.empty())
		{
			for (int i = 0; i < vArrows.size(); ++i)
			{
				if (!vArrows[i]->move((float)(level)))
				{
					vArrows.erase(vArrows.begin() + i);
					break;
				}
			}
		}
	
		if (!vTomahawks.empty())
		{
			for (int i = 0; i < vTomahawks.size(); ++i)
			{
				if (!vTomahawks[i]->move((float)(level)))
				{
					vTomahawks.erase(vTomahawks.begin() + i);
					break;
				}
			}
		}

	// DRAW THINGS ***********************************************

		Draw->BeginDraw();
		if (nrmTxt && statBrush && txtBrush && hgltBrush && inactBrush && b1BckgBrush && b2BckgBrush && b3BckgBrush)
		{
			Draw->FillRectangle(D2D1::RectF(0, 0, scr_width, 50.0f), statBrush);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b1Rect, 20.0f, 25.0f), b1BckgBrush);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b2Rect, 20.0f, 25.0f), b2BckgBrush);
			Draw->FillRoundedRectangle(D2D1::RoundedRect(b3Rect, 20.0f, 25.0f), b3BckgBrush);

			if (name_set)Draw->DrawTextW(L"ИМЕ НА ИГРАЧ", 13, nrmTxt, b1TxtRect, inactBrush);
			else
			{
				if (b1Hglt)Draw->DrawTextW(L"ИМЕ НА ИГРАЧ", 13, nrmTxt, b1TxtRect, hgltBrush);
				else Draw->DrawTextW(L"ИМЕ НА ИГРАЧ", 13, nrmTxt, b1TxtRect, txtBrush);
			}

			if (b2Hglt)Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmTxt, b2TxtRect, hgltBrush);
			else Draw->DrawTextW(L"ЗВУЦИ ON / OFF", 15, nrmTxt, b2TxtRect, txtBrush);
			if (b3Hglt)Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTxt, b3TxtRect, hgltBrush);
			else Draw->DrawTextW(L"ПОМОЩ ЗА ИГРАТА", 16, nrmTxt, b3TxtRect, txtBrush);
		}

	///////////////////////////////////////////////////////////////

		if (!vBackgrounds.empty())
		{
			for (int i = 0; i < vBackgrounds.size(); ++i)Draw->DrawBitmap(bmpBackground, vBackgrounds[i]);
		}

		if (!vTiles.empty())
		{
			for (int i = 0; i < vTiles.size(); ++i)
			{
				switch (vTiles[i]->type)
				{
				case tiles::dirt:
					Draw->DrawBitmap(bmpDirt, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
						vTiles[i]->end.x, vTiles[i]->end.y));
					break;

				case tiles::dirt_water:
					Draw->DrawBitmap(bmpWater2, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
						vTiles[i]->end.x, vTiles[i]->end.y));
					break;

				case tiles::water:
					Draw->DrawBitmap(bmpWater, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
						vTiles[i]->end.x, vTiles[i]->end.y));
					break;

				case tiles::trap_axe:
					Draw->DrawBitmap(bmpTrapAxe, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
						vTiles[i]->end.x, vTiles[i]->end.y));
					break;

				case tiles::trap_bolt:
					Draw->DrawBitmap(bmpTrapBolt, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
						vTiles[i]->end.x, vTiles[i]->end.y));
					break;

				case tiles::trap_spear:
					Draw->DrawBitmap(bmpTrapSpear, D2D1::RectF(vTiles[i]->start.x, vTiles[i]->start.y,
						vTiles[i]->end.x, vTiles[i]->end.y));
					break;
				}
			}
		}
		
		if (!vPlatforms.empty())
		{
			for (std::vector<dll::PLATFORM*>::iterator tree = vPlatforms.begin(); tree < vPlatforms.end(); ++tree)
			{
				switch ((*tree)->type)
				{
				case platforms::flat_platform1:
					Draw->DrawBitmap(bmpPlatform1, D2D1::RectF((*tree)->start.x, (*tree)->start.y, 
						(*tree)->end.x, (*tree)->end.y));
					break;

				case platforms::flat_platform2:
					Draw->DrawBitmap(bmpPlatform2, D2D1::RectF((*tree)->start.x, (*tree)->start.y,
						(*tree)->end.x, (*tree)->end.y));
					break;

				case platforms::high_platform:
					Draw->DrawBitmap(bmpPlatform3, D2D1::RectF((*tree)->start.x, (*tree)->start.y,
						(*tree)->end.x, (*tree)->end.y));
					break;
				}
			}
		}

		if (Hero)
		{
			int aframe = Hero->get_frame();

			switch (Hero->dir)
			{
			case dirs::left:
				Draw->DrawBitmap(bmpHeroL[aframe], Resizer(bmpHeroL[aframe], Hero->start.x, Hero->start.y));
				break;

			case dirs::right:
				Draw->DrawBitmap(bmpHeroR[aframe], Resizer(bmpHeroR[aframe], Hero->start.x, Hero->start.y)); 
				break;

			case dirs::stop:
				Draw->DrawBitmap(bmpHeroR[aframe], Resizer(bmpHeroR[aframe], Hero->start.x, Hero->start.y));
				break;
			}

			Draw->DrawLine(D2D1::Point2F(Hero->start.x - 5.0f, Hero->start.y - 8.0f),
				D2D1::Point2F(Hero->start.x + (float)(Hero->lifes / 2), Hero->start.y - 8.0f), txtBrush, 5.0f);
		}

		if (!vEvils.empty())
		{
			for (int i = 0; i < vEvils.size(); ++i)
			{
				dll::EVIL* anEvil{ vEvils[i] };
				int aframe{ 0 };

				if (anEvil->type == evils::flyer)
				{
					switch (anEvil->dir)
					{
					case dirs::left:
						aframe = anEvil->get_frame();
						Draw->DrawBitmap(bmpFlyerL[aframe], Resizer(bmpFlyerL[aframe], anEvil->start.x, anEvil->start.y));
						break;

					case dirs::right:
						aframe = anEvil->get_frame();
						Draw->DrawBitmap(bmpFlyerR[aframe], Resizer(bmpFlyerL[aframe], anEvil->start.x, anEvil->start.y));
						break;
					}
				}
				else if (anEvil->type == evils::mushroom)
					Draw->DrawBitmap(bmpMushroom[aframe], Resizer(bmpFlyerL[aframe], anEvil->start.x, anEvil->start.y));
				else if (anEvil->type == evils::snail)
				{
					switch (anEvil->dir)
					{
					case dirs::left:
						aframe = anEvil->get_frame();
						Draw->DrawBitmap(bmpSnailL[aframe], Resizer(bmpSnailL[aframe], anEvil->start.x, anEvil->start.y));
						break;

					case dirs::right:
						aframe = anEvil->get_frame();
						Draw->DrawBitmap(bmpSnailR[aframe], Resizer(bmpSnailR[aframe], anEvil->start.x, anEvil->start.y));
						break;
					}
				}
				else if (anEvil->type == evils::octopus)
					Draw->DrawBitmap(bmpOctopus[aframe], Resizer(bmpOctopus[aframe], anEvil->start.x, anEvil->start.y));
			}
		}

		if (!vTrees.empty())
		{
			for (std::vector<dll::TILE*>::iterator tree = vTrees.begin(); tree < vTrees.end(); ++tree)
			{
				switch ((*tree)->type)
				{
				case tiles::tree1:
					Draw->DrawBitmap(bmpTree1, D2D1::RectF((*tree)->start.x, (*tree)->start.y, (*tree)->end.x, (*tree)->end.y));
					break;

				case tiles::tree2:
					Draw->DrawBitmap(bmpTree2, D2D1::RectF((*tree)->start.x, (*tree)->start.y, (*tree)->end.x, (*tree)->end.y));
					break;

				case tiles::tree3:
					Draw->DrawBitmap(bmpTree1, D2D1::RectF((*tree)->start.x, (*tree)->start.y, (*tree)->end.x, (*tree)->end.y));
					break;
				}
			}
		}

		if (!vArrows.empty() && Hero)
		{
			for (int i = 0; i < vArrows.size(); ++i)
			{
				dll::SHOT* arrow{ vArrows[i] }; 

				if (arrow->dir == dirs::left)
					Draw->DrawBitmap(bmpArrowL, D2D1::RectF(arrow->start.x, arrow->start.y, arrow->end.x, arrow->end.y));
				else Draw->DrawBitmap(bmpArrowR, D2D1::RectF(arrow->start.x, arrow->start.y, arrow->end.x, arrow->end.y));
			}
		}

		if (!vTomahawks.empty())
		{
			for (int i = 0; i < vTomahawks.size(); ++i)
			{
				dll::SHOT* arrow{ vTomahawks[i] };

				if (arrow->dir == dirs::left)
					Draw->DrawBitmap(bmpTomahawkL, D2D1::RectF(arrow->start.x, arrow->start.y, arrow->end.x, arrow->end.y));
				else Draw->DrawBitmap(bmpTomahawkR, D2D1::RectF(arrow->start.x, arrow->start.y, arrow->end.x, arrow->end.y));
			}
		}


	//////////////////////////////////////////////////////////////

		Draw->EndDraw();

	}

	ClearResources();
	std::remove(tmp_file);

    return (int) bMsg.wParam;
}