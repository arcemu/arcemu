/*
 *	sfmpq.h header file for mpqe c++ version
 */

#ifndef _SFMPQ_H_

// General error codes
#define MPQ_ERROR_MPQ_INVALID 0x85200065
#define MPQ_ERROR_FILE_NOT_FOUND 0x85200066
// MPQ_ERROR_DISK_FULL Physical write file to MPQ failed. Not sure of exact meaning
#define MPQ_ERROR_DISK_FULL 0x85200068
#define MPQ_ERROR_HASH_TABLE_FULL 0x85200069
#define MPQ_ERROR_ALREADY_EXISTS 0x8520006A
// MPQ_ERROR_BAD_OPEN_MODE When MOAU_READ_ONLY is used without MOAU_OPEN_EXISTING
#define MPQ_ERROR_BAD_OPEN_MODE 0x8520006C
#define MPQ_ERROR_COMPACT_ERROR 0x85300001

// MpqOpenArchiveForUpdate flags
#define MOAU_CREATE_NEW 0x00
#define MOAU_CREATE_ALWAYS 0x08
#define MOAU_OPEN_EXISTING 0x04
#define MOAU_OPEN_ALWAYS 0x20
// MOAU_READ_ONLY Must be used with MOAU_OPEN_EXISTING
#define MOAU_READ_ONLY 0x10
#define MOAU_MAINTAIN_LISTFILE 0x01

// MpqAddFileToArchive flags
// MAFA_EXISTS Will be added if not present
#define MAFA_EXISTS 0x80000000
#define MAFA_UNKNOWN40000000 0x40000000
#define MAFA_MODCRYPTKEY 0x00020000
#define MAFA_ENCRYPT 0x00010000
#define MAFA_COMPRESS 0x00000200
#define MAFA_COMPRESS2 0x00000100
#define MAFA_REPLACE_EXISTING 0x00000001

// MpqAddFileToArchiveEx compression flags
// MAFA_COMPRESS_STANDARD Standard PKWare DCL compression
#define MAFA_COMPRESS_STANDARD 0x08
// MAFA_COMPRESS_DEFLATE ZLib's deflate compression
#define MAFA_COMPRESS_DEFLATE 0x02
// MAFA_COMPRESS_WAVE Standard wave compression
#define MAFA_COMPRESS_WAVE 0x81
// MAFA_COMPRESS_WAVE2 Unused wave compression
#define MAFA_COMPRESS_WAVE2 0x41

// Flags for individual compression types used for wave compression
// Main compressor for standard wave compression
#define MAFA_COMPRESS_WAVECOMP1 0x80
// Main compressor for unused wave compression
#define MAFA_COMPRESS_WAVECOMP2 0x40
// Secondary compressor for wave compression
#define MAFA_COMPRESS_WAVECOMP3 0x01

// ZLib deflate compression level constants (used with MpqAddFileToArchiveEx and MpqAddFileFromBufferEx)
#define Z_NO_COMPRESSION 0
#define Z_BEST_SPEED 1
#define Z_BEST_COMPRESSION 9
#define Z_DEFAULT_COMPRESSION (-1)

// MpqAddWaveToArchive quality flags
#define MAWA_QUALITY_HIGH 1
#define MAWA_QUALITY_MEDIUM 0
#define MAWA_QUALITY_LOW 2

// SFileGetFileInfo flags
// Block size in MPQ
#define SFILE_INFO_BLOCK_SIZE 0x01
// Hash table size in MPQ
#define SFILE_INFO_HASH_TABLE_SIZE 0x02
// Number of files in MPQ
#define SFILE_INFO_NUM_FILES 0x03
// Is int a file or an MPQ?
#define SFILE_INFO_TYPE 0x04
// Size of MPQ or uncompressed file
#define SFILE_INFO_SIZE 0x05
// Size of compressed file
#define SFILE_INFO_COMPRESSED_SIZE 0x06
// File flags (compressed, etc.), file attributes if a file not in an archive
#define SFILE_INFO_FLAGS 0x07
//int of MPQ that file is in
#define SFILE_INFO_PARENT 0x08
//Position of file pointer in files
#define SFILE_INFO_POSITION 0x09
//Locale ID of file in MPQ
#define SFILE_INFO_LOCALEID 0x0A
//Priority of open MPQ
#define SFILE_INFO_PRIORITY 0x0B
//Hash index of file in MPQ
#define SFILE_INFO_HASH_INDEX 0x0C

// SFileListFiles flags
// Specifies that lpFilelists is a file list from memory, rather than being a list of file lists
#define SFILE_LIST_MEMORY_LIST 0x01
// Only list files that the function finds a name for
#define SFILE_LIST_ONLY_KNOWN 0x02
// Only list files that the function does not find a name for
#define SFILE_LIST_ONLY_UNKNOWN 0x04

#define SFILE_TYPE_MPQ 0x01
#define SFILE_TYPE_FILE 0x02

// Open archive without regard to the drive type it resides on
#define SFILE_OPEN_HARD_DISK_FILE 0x0000
// Open the archive only if it is on a CD-ROM
#define SFILE_OPEN_CD_ROM_FILE 0x0001
// Open file with write access
#define SFILE_OPEN_ALLOW_WRITE 0x8000

// Used with SFileOpenFileEx only the archive with the int specified will be searched for the file
#define SFILE_SEARCH_CURRENT_ONLY 0x00
// SFileOpenFileEx will look through all open archives for the file
#define SFILE_SEARCH_ALL_OPEN 0x01

#ifndef uint32
#define uint32 DWORD
#endif

#ifndef uint16
#define uint16 unsigned short int
#endif

struct mpqeOptions
{
	bool  option_verbose;
	char* option_outdir;
	bool  option_lowercase;
	char* option_baseMPQ;
	char* option_searchglob;
	bool  option_skipdirstructure;
};

struct FileList
{
	LARGE_INTEGER fwtime;
	char*  fname;
};

struct mpqLCID
{
	char lcLocale[4];
};

struct SFMPQVERSION
{
	uint16 Major;
	uint16 Minor;
	uint16 Revision;
	uint16 Subrevision;
};

struct FILELISTENTRY
{
	uint32 dwFileExists;     // Nonzero if this entry is used
	char lcLocale[4];        // Locale ID of file
	uint32 dwCompressedSize; // Compressed size of file
	uint32 dwFullSize;       // Uncompressed size of file
	uint32 dwFlags;          // Flags for file
	char szFileName[260];
};

struct MPQHEADER 
{
	uint32 dwMPQID; //"MPQ\x1A" for mpq's, "BN3\x1A" for bncache.dat
	uint32 dwHeaderSize; // Size of this header
	uint32 dwMPQSize; //The size of the mpq archive
	uint16 wUnused0C; // Seems to always be 0
	uint16 wBlockSize; // Size of blocks in files equals 512 << wBlockSize
	uint32 dwHashTableOffset; // Offset to hash table
	uint32 dwBlockTableOffset; // Offset to block table
	uint32 dwHashTableSize; // Number of entries in hash table
	uint32 dwBlockTableSize; // Number of entries in block table
};

//Archive ints may be typecasted to this struct so you can access
//some of the archive's properties and the decrypted hash table and
//block table directly.
struct MPQARCHIVE 
{
// Arranged according to priority with lowest priority first
	int* lpNextArc; // Pointer to the next ARCHIVEREC struct. Pointer to addresses of first and last archives if last archive
	int* lpPrevArc; // Pointer to the previous ARCHIVEREC struct. 0xEAFC5E23 if first archive
	char szFileName[260]; // Filename of the archive
	uint32 hFile; // The archive's file int
	uint32 dwFlags1; // Some flags, bit 1 (0 based) seems to be set when opening an archive from a CD
	uint32 dwPriority; // Priority of the archive set when calling SFileOpenArchive
	int* lpLastReadFile; // Pointer to the last read file's FILEREC struct. Only used for incomplete reads of blocks
	uint32 dwUnk; // Seems to always be 0
	uint32 dwBlockSize; // Size of file blocks in bytes
	int* lpLastReadBlock; // Pointer to the read buffer for archive. Only used for incomplete reads of blocks
	uint32 dwBufferSize; // Size of the read buffer for archive. Only used for incomplete reads of blocks
	uint32 dwMPQStart; // The starting offset of the archive
	int* lpMPQHeader; // Pointer to the archive header
	int* lpBlockTable; // Pointer to the start of the block table
	int* lpHashTable; // Pointer to the start of the hash table
	uint32 dwFileSize; // The size of the file in which the archive is contained
	uint32 dwOpenFiles; // Count of files open in archive + 1
	MPQHEADER MpqHeader;
	uint32 dwFlags; //The only flag that should be changed is MOAU_MAINTAIN_LISTFILE
	char* lpFileName;
};

//ints to files in the archive may be typecasted to this struct
//so you can access some of the file's properties directly.
struct MPQFILE 
{
	int* lpNextFile; // Pointer to the next FILEREC struct. Pointer to addresses of first and last files if last file
	int* lpPrevFile; // Pointer to the previous FILEREC struct. 0xEAFC5E13 if first file
	char szFileName[260]; // Filename of the archive
	uint32 hPlaceHolder; // Always 0xFFFFFFFF
	int* lpParentArc; // Pointer to the ARCHIVEREC struct of the archive in which the file is contained
	int* lpBlockEntry; // Pointer to the file's block table entry
	uint32 dwCryptKey; // Decryption key for the file
	uint32 dwFilePointer; // Position of file pointer in the file
	uint32 dwUnk1; // Seems to always be 0
	uint32 dwBlockCount; // Number of blocks in file
	int* lpdwBlockOffsets; // Offsets to blocks in file. There are 1 more of these than the number of blocks
	uint32 dwReadStarted; // Set to 1 after first read
	uint32 dwUnk2; // Seems to always be 0
	int* lpLastReadBlock; // Pointer to the read buffer for file. Only used for incomplete reads of blocks
	uint32 dwBytesRead; // Total bytes read from open file
	uint32 dwBufferSize; // Size of the read buffer for file. Only used for incomplete reads of blocks
	uint32 dwConstant; // Seems to always be 1
	int* lpHashEntry;
	char* lpFileName;
};

struct BLOCKTABLEENTRY 
{
	uint32 dwFileOffset; // Offset to file
	uint32 dwCompressedSize; // Compressed size of file
	uint32 dwFullSize; // Uncompressed size of file
	uint32 dwFlags; // Flags for file
};

struct HASHTABLEENTRY 
{
	uint32 dwNameHashA; // First name hash of file
	uint32 dwNameHashB; // Second name hash of file
	char lcLocale[4]; // Locale ID of file
	uint32 dwBlockTableIndex; // Index to the block table entry for the file
};

typedef char* (WINAPI* Function_MpqGetVersionString)(void);
typedef float (WINAPI* Function_MpqGetVersion)(void);
typedef char* (WINAPI* Function_SFMpqGetVersionString)(void);
typedef uint32 (WINAPI* Function_SFMpqGetVersionString2)(char* lpBuffer, uint32 dwBufferLength);
typedef SFMPQVERSION (WINAPI* Function_SFMpqGetVersion)(void);
typedef int (WINAPI* Function_SFileOpenArchive)(char* lpFileName, uint32 dwPriority, uint32 dwFlags, int* hMPQ);
typedef int (WINAPI* Function_SFileCloseArchive)(int hMPQ);
typedef int (WINAPI* Function_SFileGetArchiveName)(int hMPQ, char* lpBuffer, uint32 dwBufferLength);
typedef int (WINAPI* Function_SFileOpenFile)(char* lpFileName, int* hFile);
typedef int (WINAPI* Function_SFileOpenFileEx)(int hMpq, char* lpFileName, uint32 dwSearchScope, int* hFile);
typedef int (WINAPI* Function_SFileCloseFile)(int hFile);
typedef uint32 (WINAPI* Function_SFileGetFileSize)(int hFile, uint32* lpFileSizeHigh);
typedef int (WINAPI* Function_SFileGetFileArchive)(int hFile, int* hMPQ);
typedef int (WINAPI* Function_SFileGetFileName)(int hFile, char* lpBuffer, uint32 dwBufferLength);
typedef int (WINAPI* Function_SFileSetFilePointer)(int hFile, int lDistanceToMove, int* lplDistanceToMoveHigh, uint32 dwMoveMethod);
typedef int (WINAPI* Function_SFileReadFile)(int hFile, byte* lpBuffer, uint32 nNumberOfBytesToRead, uint32* lpNumberOfBytesRead, int* lpOverlapped);
typedef mpqLCID (WINAPI* Function_SFileSetLocale)(mpqLCID nNewLocale);
typedef int (WINAPI* Function_SFileGetBasePath)(char* lpBuffer, uint32 dwBufferLength);
typedef int (WINAPI* Function_SFileSetBasePath)(char* lpNewBasePath);
typedef int (WINAPI* Function_SFileGetFileInfo)(int hFile, uint32 dwInfoType);
typedef int (WINAPI* Function_SFileSetArchivePriority)(int hMPQ, uint32 dwPriority);
typedef int (WINAPI* Function_SFileFindMpqHeader)(int hFile);
typedef int (WINAPI* Function_SFileListFiles)(int hMPQ, char* lpFileLists, FILELISTENTRY* lpListBuffer, uint32 dwFlags);
typedef int (WINAPI* Function_MpqOpenArchiveForUpdate)(char* lpFileName, uint32 dwFlags, uint32 dwMaximumFilesInArchive);
typedef uint32 (WINAPI* Function_MpqCloseUpdatedArchive)(int hMpq, uint32 dwUnknown2);
typedef void (WINAPI* Function_MpqAddFileToArchive)(int hMPQ, char* lpSourceFileName, char* lpDestFileName, uint32 dwFlags);
typedef int (WINAPI* Function_MpqAddWaveToArchive)(int hMPQ, char* lpSourceFileName, char* lpDestFileName, uint32 dwFlags, uint32 dwQuality);
typedef int (WINAPI* Function_MpqRenameFile)(int hMPQ, char* lpcOldFileName, char* lpcNewFileName);
typedef int (WINAPI* Function_MpqDeleteFile)(int hMPQ, char* lpFileName);
typedef int (WINAPI* Function_MpqCompactArchive)(int hMPQ);
typedef int (WINAPI* Function_MpqAddFileToArchiveEx)(int hMPQ, char* lpSourceFileName, char* lpDestFileName, uint32 dwFlags, uint32 dwCompressionType, uint32 dwCompressLevel);
typedef int (WINAPI* Function_MpqAddFileFromBufferEx)(int hMPQ, byte* lpBuffer, uint32 dwLength, char* lpFileName, uint32 dwFlags, uint32 dwCompressionType, uint32 dwCompressLevel);
typedef int (WINAPI* Function_MpqAddFileFromBuffer)(int hMPQ, byte* lpBuffer, uint32 dwLength, char* lpFileName, uint32 dwFlags);
typedef int (WINAPI* Function_MpqAddWaveFromBuffer)(int hMPQ, byte* lpBuffer, uint32 dwLength, char* lpFileName, uint32 dwFlags, uint32 dwQuality);
typedef int (WINAPI* Function_MpqSetFileLocale)(int hMPQ, char* lpFileName, mpqLCID nOldLocale, mpqLCID nNewLocale);

struct AddressTable
{
// char* MpqGetVersionString();
	Function_MpqGetVersionString pMpqGetVersionString;
// float MpqGetVersion();
	Function_MpqGetVersion pMpqGetVersion;
// char* SFMpqGetVersionString();
	Function_SFMpqGetVersionString pSFMpqGetVersionString;
// SFMpqGetVersionString2's return value is the required length of the buffer plus
// the terminating null, so use SFMpqGetVersionString2(0, 0); to get the length.
// uint32 SFMpqGetVersionString2(char* lpBuffer, uint32 dwBufferLength);
	Function_SFMpqGetVersionString2 pSFMpqGetVersionString2;
// SFMPQVERSION SFMpqGetVersion();
	Function_SFMpqGetVersion pSFMpqGetVersion;

// Storm functions implemented by this library
// int SFileOpenArchive(char* lpFileName, uint32 dwPriority, uint32 dwFlags, int* hMPQ);
	Function_SFileOpenArchive pSFileOpenArchive;
// int SFileCloseArchive(int hMPQ);
	Function_SFileCloseArchive pSFileCloseArchive;
// int SFileGetArchiveName(int hMPQ, char* lpBuffer, uint32 dwBufferLength);
	Function_SFileGetArchiveName pSFileGetArchiveName;
// int SFileOpenFile(char* lpFileName, int* hFile);
	Function_SFileOpenFile pSFileOpenFile;
// int SFileOpenFileEx(int hMPQ, char* lpFileName, uint32 dwSearchScope, int* hFile);
	Function_SFileOpenFileEx pSFileOpenFileEx;
// int SFileCloseFile(int hFile);
	Function_SFileCloseFile pSFileCloseFile;
// int SFileGetFileSize(int hFile, int* lpFileSizeHigh);
	Function_SFileGetFileSize pSFileGetFileSize;
// int SFileGetFileArchive(int hFile, int* hMPQ);
	Function_SFileGetFileArchive pSFileGetFileArchive;
// int SFileGetFileName(int hFile, char* lpBuffer, uint32 dwBufferLength);
	Function_SFileGetFileName pSFileGetFileName;
// int SFileSetFilePointer(int hFile, int lDistanceToMove, int* lplDistanceToMoveHigh, uint32 dwMoveMethod);
	Function_SFileSetFilePointer pSFileSetFilePointer;
// int SFileReadFile(int hFile, byte* lpBuffer,uint32 nNumberOfBytesToRead, int* lpNumberOfBytesRead, int* lpOverlapped);
	Function_SFileReadFile pSFileReadFile;
// mpqLCID SFileSetLocale(mpqLCID nNewLocale);
	Function_SFileSetLocale pSFileSetLocale;
// int SFileGetBasePath(char* lpBuffer, uint32 dwBufferLength);
	Function_SFileGetBasePath pSFileGetBasePath;
// int SFileSetBasePath(char* lpNewBasePath);
	Function_SFileSetBasePath pSFileSetBasePath;

// Extra storm-related functions
// int SFileGetFileInfo(int hFile, uint32 dwInfoType);
	Function_SFileGetFileInfo pSFileGetFileInfo;
// int SFileSetArchivePriority(int hMPQ, uint32 dwPriority);
	Function_SFileSetArchivePriority pSFileSetArchivePriority;
// int SFileFindMpqHeader(int hFile);
	Function_SFileFindMpqHeader pSFileFindMpqHeader;
// int SFileListFiles(int hMPQ, char* lpFileLists, FILELISTENTRY* lpListBuffer, uint32 dwFlags);
	Function_SFileListFiles pSFileListFiles;

// Archive editing functions implemented by this library
// int MpqOpenArchiveForUpdate(char* lpFileName, uint32 dwFlags, uint32 dwMaximumFilesInArchive);
	Function_MpqOpenArchiveForUpdate pMpqOpenArchiveForUpdate;
// uint32 MpqCloseUpdatedArchive(int hMPQ, uint32 dwUnknown2);
	Function_MpqCloseUpdatedArchive pMpqCloseUpdatedArchive;
// MpqAddFileToArchive(int hMPQ, char* lpSourceFileName, char* lpDestFileName, uint32 dwFlags);
	Function_MpqAddFileToArchive pMpqAddFileToArchive;
// int MpqAddWaveToArchive(int hMPQ, char* lpSourceFileName, char* lpDestFileName, uint32 dwFlags, uint32 dwQuality);
	Function_MpqAddWaveToArchive pMpqAddWaveToArchive;
// int MpqRenameFile(int hMPQ, char* lpcOldFileName, char* lpcNewFileName);
	Function_MpqRenameFile pMpqRenameFile;
// int MpqDeleteFile(int hMPQ, char* lpFileName);
	Function_MpqDeleteFile pMpqDeleteFile;
// int MpqCompactArchive(int hMPQ);
	Function_MpqCompactArchive pMpqCompactArchive;

// Extra archive editing functions
// int MpqAddFileToArchiveEx(int hMPQ, char* lpSourceFileName, char* lpDestFileName, uint32 dwFlags, uint32 dwCompressionType, uint32 dwCompressLevel);
	Function_MpqAddFileToArchiveEx pMpqAddFileToArchiveEx;
// int MpqAddFileFromBufferEx(int hMPQ, byte* lpBuffer, uint32 dwLength, char* lpFileName, uint32 dwFlags, uint32 dwCompressionType, uint32 dwCompressLevel);
	Function_MpqAddFileFromBufferEx pMpqAddFileFromBufferEx;
// int MpqAddFileFromBuffer(int hMPQ, byte* lpBuffer, uint32 dwLength, char* lpFileName, uint32 dwFlags);
	Function_MpqAddFileFromBuffer pMpqAddFileFromBuffer;
// int MpqAddWaveFromBuffer(int hMPQ, byte* lpBuffer, uint32 dwLength, char* lpFileName, uint32 dwFlags, uint32 dwQuality);
	Function_MpqAddWaveFromBuffer pMpqAddWaveFromBuffer;
// int MpqSetFileLocale(int hMPQ, char* lpFileName, LCID nOldLocale, LCID nNewLocale);
	Function_MpqSetFileLocale pMpqSetFileLocale;
};

#define ERRBUF_SIZE 1024

BOOL LoadAddresses(AddressTable* at);
void ShowHelp(void);
void mpqExtract(const AddressTable* at, mpqeOptions* op, char* fileMPQ);
bool Match(char* pattern, char* s, bool caseSensitive);
char* bytes2text(int bytes);
char* GetErrorText(DWORD err);

#endif // _SFMPQ_H_
