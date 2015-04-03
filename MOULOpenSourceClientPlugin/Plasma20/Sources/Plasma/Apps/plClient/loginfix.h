/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011 Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/

////////////////////////////////////////////////////////////////////////////////
// The stuff below seems to be missing fron WinIoCtl.h in MSVC2003 but is
// probably present in later versions. This was robbed from MSDN to fill
// the gap.

#if (_MSC_VER < 1500) // For VS2005 and earlier only (might need to make it VS2003 only (1310))

#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

  // retrieve the storage device descriptor data for a device.
  typedef struct _STORAGE_DEVICE_DESCRIPTOR {
    ULONG  Version;
    ULONG  Size;
    UCHAR  DeviceType;
    UCHAR  DeviceTypeModifier;
    BOOLEAN  RemovableMedia;
    BOOLEAN  CommandQueueing;
    ULONG  VendorIdOffset;
    ULONG  ProductIdOffset;
    ULONG  ProductRevisionOffset;
    ULONG  SerialNumberOffset;
    STORAGE_BUS_TYPE  BusType;
    ULONG  RawPropertiesLength;
    UCHAR  RawDeviceProperties[1];
  } STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

  // retrieve the properties of a storage device or adapter.
  typedef enum _STORAGE_QUERY_TYPE {
    PropertyStandardQuery = 0,
    PropertyExistsQuery,
    PropertyMaskQuery,
    PropertyQueryMaxDefined
  } STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;

  // retrieve the properties of a storage device or adapter.
  typedef enum _STORAGE_PROPERTY_ID {
    StorageDeviceProperty = 0,
    StorageAdapterProperty,
    StorageDeviceIdProperty
  } STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;

  // retrieve the properties of a storage device or adapter.
  typedef struct _STORAGE_PROPERTY_QUERY {
    STORAGE_PROPERTY_ID  PropertyId;
    STORAGE_QUERY_TYPE  QueryType;
    UCHAR  AdditionalParameters[1];
  } STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;
#endif
// End of stuff taken from MSDN
