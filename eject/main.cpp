#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <winioctl.h>

// Finds the device interface for the CDROM drive with the given interface number.
DEVINST GetDrivesDevInstByDeviceNumber(long DeviceNumber)
{
    const GUID *guid = &GUID_DEVINTERFACE_CDROM;

    // Get device interface info set handle
    // for all devices attached to system
    HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if(hDevInfo == INVALID_HANDLE_VALUE)
        return 0;

    // Retrieve a context structure for a device interface of a device information set.
    BYTE                             buf[1024];
    PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)buf;
    SP_DEVICE_INTERFACE_DATA         spdid;
    SP_DEVINFO_DATA                  spdd;
    DWORD                            dwSize;

    spdid.cbSize = sizeof(spdid);

    // Iterate through all the interfaces and try to match one based on
    // the device number.
    for(DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL,guid, i, &spdid); i++)
    {
        // Get the device path.
        dwSize = 0;
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL);
        if(dwSize == 0 || dwSize > sizeof(buf))
            continue;

        pspdidd->cbSize = sizeof(*pspdidd);
        ZeroMemory((PVOID)&spdd, sizeof(spdd));
        spdd.cbSize = sizeof(spdd);
        if(!SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd,
            dwSize, &dwSize, &spdd))
            continue;

        // Open the device.
        HANDLE hDrive = CreateFile(pspdidd->DevicePath,0,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, 0, NULL);
        if(hDrive == INVALID_HANDLE_VALUE)
            continue;

        // Get the device number.
        STORAGE_DEVICE_NUMBER sdn;
        dwSize = 0;
        if(DeviceIoControl(hDrive,
            IOCTL_STORAGE_GET_DEVICE_NUMBER,
            NULL, 0, &sdn, sizeof(sdn),
            &dwSize, NULL))
        {
            // Does it match?
            if(DeviceNumber == (long)sdn.DeviceNumber)
            {
                CloseHandle(hDrive);
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return spdd.DevInst;
            }
        }
        CloseHandle(hDrive);
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return 0;
}


// Returns true if the given device instance belongs to the USB device with the given VID and PID.
bool matchDevInstToUsbDevice(DEVINST device, DWORD vid, DWORD pid)
{
    // This is the string we will be searching for in the device harware IDs.
    TCHAR hwid[64];
    _stprintf(hwid, _T("VID_%04X&PID_%04X"), vid, pid);

    // Get a list of hardware IDs for all USB devices.
    ULONG ulLen;
    CM_Get_Device_ID_List_Size(&ulLen, NULL, CM_GETIDLIST_FILTER_NONE);
    TCHAR *pszBuffer = new TCHAR[ulLen];
    CM_Get_Device_ID_List(NULL, pszBuffer, ulLen, CM_GETIDLIST_FILTER_NONE);

    // Iterate through the list looking for our ID.
    for(LPTSTR pszDeviceID = pszBuffer; *pszDeviceID; pszDeviceID += _tcslen(pszDeviceID) + 1)
    {
        // Some versions of Windows have the string in upper case and other versions have it
        // in lower case so just make it all upper.
        for(int i = 0; pszDeviceID[i]; i++)
            pszDeviceID[i] = toupper(pszDeviceID[i]);

        if(_tcsstr(pszDeviceID, hwid))
        {
            // Found the device, now we want the grandchild device, which is the "generic volume"
            DEVINST MSDInst = 0;
            if(CR_SUCCESS == CM_Locate_DevNode(&MSDInst, pszDeviceID, CM_LOCATE_DEVNODE_NORMAL))
            {
                DEVINST DiskDriveInst = 0;
                if(CR_SUCCESS == CM_Get_Child(&DiskDriveInst, MSDInst, 0))
                {
                    // Now compare the grandchild node against the given device instance.
                    if(device == DiskDriveInst)
                        return true;
                }
            }
        }
    }

    return false;
}

// Eject the given drive.
void ejectDrive(TCHAR driveletter)
{
    TCHAR devicepath[16];
    _tcscpy(devicepath, _T("\\\\.\\?:"));
    devicepath[4] = driveletter;

    DWORD dwRet = 0;
    HANDLE hVol = CreateFile(devicepath, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if(hVol == INVALID_HANDLE_VALUE)
        return;

    if(!DeviceIoControl(hVol, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &dwRet, 0))
        return;

    if(!DeviceIoControl(hVol, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &dwRet, 0))
        return;

    DeviceIoControl(hVol, IOCTL_STORAGE_EJECT_MEDIA, 0, 0, 0, 0, &dwRet, 0);

    CloseHandle(hVol);
}

// Find a USB device by it's Vendor and Product IDs. When found, eject it.
void usbEjectDevice(unsigned vid, unsigned pid)
{
    TCHAR devicepath[8];
    _tcscpy(devicepath, _T("\\\\.\\?:"));

    TCHAR drivepath[4];
    _tcscpy(drivepath, _T("?:\\"));

    // Iterate through every drive letter and check if it is our device.
    for(TCHAR driveletter = _T('A'); driveletter <= _T('Z'); driveletter++)
    {
        // We are only interested in CDROM drives.
        drivepath[0] = driveletter;
        if(DRIVE_CDROM != GetDriveType(drivepath))
            continue;

        // Get the "storage device number" for the current drive.
        long DeviceNumber = -1;
        devicepath[4]     = driveletter;
        HANDLE hVolume    = CreateFile(devicepath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, 0, NULL);
        if(INVALID_HANDLE_VALUE == hVolume)
            continue;

        STORAGE_DEVICE_NUMBER sdn;
        DWORD dwBytesReturned = 0;
        if(DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER,
            NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL))
            DeviceNumber = sdn.DeviceNumber;
        CloseHandle(hVolume);
        if(DeviceNumber < 0)
            continue;

        // Use the data we have collected so far on our drive to find a device instance.
        DEVINST DevInst = GetDrivesDevInstByDeviceNumber(DeviceNumber);

        // If the device instance corresponds to the USB device we are looking for, eject it.
        if(DevInst)
        {
            if(matchDevInstToUsbDevice(DevInst, vid, pid))
                ejectDrive(driveletter);
        }
    }
}


int main(int argc, char *argv[])
{
    usbEjectDevice (0x22F4, 0x2000);
}
