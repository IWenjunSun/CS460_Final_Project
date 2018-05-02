#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#define SDL_MAIN_HANDLED
#pragma comment(lib, "psapi.lib")
#define PSAPI_VERSION 1
#define PI 3.1415926535897

using namespace std;
const DWORD player_offset = 0x3D24D4;
const DWORD team_offset = 0x22C;
const DWORD x_rot_off = 0x326EC0;
const DWORD y_rot_off = 0x326EBC;
const DWORD crouch_offset = 0x36C;
const DWORD entity_diff = 0x10;
const DWORD player_x_off = 0x348;
const DWORD player_y_off = 0x34C;
const DWORD player_z_off = 0x350;
const float ground_to_head = 50;
const float ground_to_gun = 48;
const float gun_to_head = 0;

struct Player {
	float x, y, z;
	float x_rot, y_rot;
	int team;
};
struct Enemy {
	float x, y, z;
	float abs_dist, dist;
	int number;
	int team;
	bool below;
};

HMODULE GetBaseAddressByName(DWORD processId, TCHAR *processName) {
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

	if (NULL != hProcess) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod), &cbNeeded, LIST_MODULES_32BIT | LIST_MODULES_64BIT)) {
			GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
			if (!_tcsicmp(processName, szProcessName)) {
				CloseHandle(hProcess);
				return hMod;
			}
		}
	}

	CloseHandle(hProcess);
	return 0;
}

int main(int argc, char* argv[]) {
	HWND hwnd = FindWindowA(NULL, "Counter-Strike Source");
	if (hwnd == NULL) {
		cout << "Cannot find window" << endl;
		Sleep(3000);
		exit(-1);
	}
	else {
		DWORD procID;
		DWORD size = 100;
		LPTSTR buff;
		GetWindowThreadProcessId(hwnd, &procID);
		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
		if (procID == NULL) {
			cout << "Cannot obtain process." << endl;
			Sleep(3000);
			exit(-1);
		}
		else {
			HMODULE addr = GetBaseAddressByName(procID, (TCHAR*)"hl2.exe");
			DWORD engine_offset = (DWORD)addr + 0x1FC00000;
			DWORD server_offset = (DWORD)addr + 0x21C00000;
			DWORD player_addr, user_addr;
			int num_players;
			float x_rot, y_rot;
			float diff_y, diff_x;
			Player player;
			//get number of players, excluding self
			ReadProcessMemory(handle, (LPVOID)(engine_offset + 0x77BB94), &num_players, sizeof(num_players), 0);
			//get team user is on
			ReadProcessMemory(handle, (LPVOID)(server_offset + player_offset), &user_addr, sizeof(user_addr), 0);
			ReadProcessMemory(handle, (LPVOID)(user_addr + team_offset), &player.team, sizeof(player.team), 0);
			int toggle = 1;
			bool hold = false;
			while (1) {
				if (GetAsyncKeyState(0x56) && !hold) {
					toggle = (toggle + 1) % 2;
					hold = true;
				}
				else if (hold && !GetAsyncKeyState(0x56))
					hold = false;
				if (toggle) {
					float crouch;
					float crouch_diff = 0;
					float target_crouch_diff = 0;
					//god mode disabled
					/*int hpp = 100;
					WriteProcessMemory(handle, (LPVOID)(user_addr + 0x9c), &hpp, sizeof(hpp), 0);*/
					//get crouch
					ReadProcessMemory(handle, (LPVOID)(user_addr + crouch_offset), &crouch, sizeof(crouch), 0);
					if ((int)crouch < 50) {
						crouch_diff = 53.5 - crouch;
					}
					Enemy target;
					Enemy temp;
					target.abs_dist = INT_MAX;
					//get player rotation
					ReadProcessMemory(handle, (LPVOID)(engine_offset + x_rot_off), &player.x_rot, sizeof(player.x_rot), 0);
					ReadProcessMemory(handle, (LPVOID)(engine_offset + y_rot_off), &player.y_rot, sizeof(player.y_rot), 0);
					//get player position
					ReadProcessMemory(handle, (LPVOID)(server_offset + player_offset), &player_addr, sizeof(player_addr), 0);
					ReadProcessMemory(handle, (LPVOID)(player_addr + player_x_off), &player.x, sizeof(player.x), 0);
					ReadProcessMemory(handle, (LPVOID)(player_addr + player_y_off), &player.y, sizeof(player.y), 0);
					ReadProcessMemory(handle, (LPVOID)(player_addr + player_z_off), &player.z, sizeof(player.z), 0);
					//get the enemy that is closest to user
					for (int i = 1; i < num_players; i++) {
						ReadProcessMemory(handle, (LPVOID)(server_offset + player_offset + 0x10 * i), &player_addr, sizeof(player_addr), 0);
						//check that entity is an enemy
						ReadProcessMemory(handle, (LPVOID)(player_addr + team_offset), &temp.team, sizeof(temp.team), 0);

						//DEBUG PURPOSE
						/*int kill = 0;
						WriteProcessMemory(handle, (LPVOID)(player_addr + 0x9c), &kill, sizeof(kill), 0);*/
						//DEBUG END
						int hp_target;
						ReadProcessMemory(handle, (LPVOID)(player_addr + 0x9c), &hp_target, sizeof(hp_target), 0);
						//team for testing purpose
						if (temp.team != player.team && hp_target > 1) {
							ReadProcessMemory(handle, (LPVOID)(player_addr + player_x_off), &temp.x, sizeof(temp.x), 0);
							ReadProcessMemory(handle, (LPVOID)(player_addr + player_y_off), &temp.y, sizeof(temp.y), 0);
							ReadProcessMemory(handle, (LPVOID)(player_addr + player_z_off), &temp.z, sizeof(temp.z), 0);
							//find out if enemy is crouched
							ReadProcessMemory(handle, (LPVOID)(player_addr + crouch_offset), &crouch, sizeof(crouch), 0);
							if ((int)crouch < 50) {
								target_crouch_diff = 53.5 - crouch;
							}
							//absolute distance is distance from player's gun to enemies head
							temp.number = i;
							temp.abs_dist = sqrt((temp.x - player.x)*(temp.x - player.x) + (temp.y - player.y)*(temp.y - player.y) + (temp.z - player.z + gun_to_head + crouch_diff - target_crouch_diff)*(temp.z - player.z + gun_to_head + crouch_diff - target_crouch_diff));
							temp.dist = sqrt((temp.x - player.x)*(temp.x - player.x) + (temp.y - player.y)*(temp.y - player.y));
							if (player.z + ground_to_gun - crouch_diff > temp.z + ground_to_head - target_crouch_diff)
								temp.below = false;
							else
								temp.below = true;
							//select closest shot
							if (temp.abs_dist < target.abs_dist) {
								target = temp;
							}
						}
					}
					//now have a target
					//calculate the x and y rotation values to set 
					//if distances are close, implies it it mostly in a line, can assume 0 on y rotation
					if (target.abs_dist != INT_MAX) {
						//use trig to get y's angle
						y_rot = (acos(target.dist / target.abs_dist)*(180)) / PI;
						if (target.below)
							y_rot = y_rot * -1;
						//x rot is a bit harder
						//-180 to 179 circle of rotation
						//using vector math
						//convert abs_dist into a vector originating at player
						diff_x = target.x - player.x;
						diff_y = target.y - player.y;
						x_rot = (acos(diff_x / target.dist)*(180)) / PI;
						if (diff_y < 0)
							x_rot = x_rot * -1;

						//doesnt let you fire again at exact same rotation
						WriteProcessMemory(handle, (LPVOID)(engine_offset + x_rot_off), &x_rot, sizeof(x_rot), 0);
						WriteProcessMemory(handle, (LPVOID)(engine_offset + y_rot_off), &y_rot, sizeof(y_rot), 0);
						Sleep(2);

					}
				}
			}
		}
	}
	return 0;
}