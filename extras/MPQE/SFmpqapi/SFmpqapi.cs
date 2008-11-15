/**
 * The author of this file is unknown, it is not public domain.
 */
using System;
using System.Runtime.InteropServices;

namespace SFmpqapi
{
	public class SFmpq 
	{
		// General error codes
		const uint MPQ_ERROR_MPQ_INVALID = 0x85200065;
		const uint MPQ_ERROR_FILE_NOT_FOUND = 0x85200066;
		const uint MPQ_ERROR_DISK_FULL = 0x85200068; //Physical write file to MPQ failed. Not sure of exact meaning
		const uint MPQ_ERROR_HASH_TABLE_FULL = 0x85200069;
		const uint MPQ_ERROR_ALREADY_EXISTS = 0x8520006A;
		const uint MPQ_ERROR_BAD_OPEN_MODE = 0x8520006C; //When MOAU_READ_ONLY is used without MOAU_OPEN_EXISTING

		const uint MPQ_ERROR_COMPACT_ERROR = 0x85300001;

		// MpqOpenArchiveForUpdate flags
		const uint MOAU_CREATE_NEW = 0x00;
		const uint MOAU_CREATE_ALWAYS = 0x08; //Was wrongly named MOAU_CREATE_NEW
		const uint MOAU_OPEN_EXISTING = 0x04;
		const uint MOAU_OPEN_ALWAYS = 0x20;
		const uint MOAU_READ_ONLY = 0x10; //Must be used with MOAU_OPEN_EXISTING
		const uint MOAU_MAINTAIN_LISTFILE = 0x01;

		// MpqAddFileToArchive flags
		const uint MAFA_EXISTS = 0x80000000; //Will be added if not present
		const uint MAFA_UNKNOWN40000000 = 0x40000000;
		const uint MAFA_MODCRYPTKEY = 0x00020000;
		const uint MAFA_ENCRYPT = 0x00010000;
		const uint MAFA_COMPRESS = 0x00000200;
		const uint MAFA_COMPRESS2 = 0x00000100;
		const uint MAFA_REPLACE_EXISTING = 0x00000001;

		// MpqAddFileToArchiveEx compression flags
		const uint MAFA_COMPRESS_STANDARD = 0x08; //Standard PKWare DCL compression
		const uint MAFA_COMPRESS_DEFLATE = 0x02; //ZLib's deflate compression
		const uint MAFA_COMPRESS_WAVE = 0x81; //Standard wave compression
		const uint MAFA_COMPRESS_WAVE2 = 0x41; //Unused wave compression

		// Flags for individual compression types used for wave compression
		const uint MAFA_COMPRESS_WAVECOMP1 = 0x80; //Main compressor for standard wave compression
		const uint MAFA_COMPRESS_WAVECOMP2 = 0x40; //Main compressor for unused wave compression
		const uint MAFA_COMPRESS_WAVECOMP3 = 0x01; //Secondary compressor for wave compression

		// ZLib deflate compression level constants (used with MpqAddFileToArchiveEx and MpqAddFileFromBufferEx)
		const uint Z_NO_COMPRESSION = 0;
		const uint Z_BEST_SPEED = 1;
		const uint Z_BEST_COMPRESSION = 9;
		const int Z_DEFAULT_COMPRESSION = (-1);

		// MpqAddWaveToArchive quality flags
		const uint MAWA_QUALITY_HIGH = 1;
		const uint MAWA_QUALITY_MEDIUM = 0;
		const uint MAWA_QUALITY_LOW = 2;

		// SFileGetFileInfo flags
		const uint SFILE_INFO_BLOCK_SIZE = 0x01; //Block size in MPQ
		const uint SFILE_INFO_HASH_TABLE_SIZE = 0x02; //Hash table size in MPQ
		const uint SFILE_INFO_NUM_FILES = 0x03; //Number of files in MPQ
		const uint SFILE_INFO_TYPE = 0x04; //Is int a file or an MPQ?
		const uint SFILE_INFO_SIZE = 0x05; //Size of MPQ or uncompressed file
		const uint SFILE_INFO_COMPRESSED_SIZE = 0x06; //Size of compressed file
		const uint SFILE_INFO_FLAGS = 0x07; //File flags (compressed, etc.), file attributes if a file not in an archive
		const uint SFILE_INFO_PARENT = 0x08; //int of MPQ that file is in
		const uint SFILE_INFO_POSITION = 0x09; //Position of file pointer in files
		const uint SFILE_INFO_LOCALEID = 0x0A; //Locale ID of file in MPQ
		const uint SFILE_INFO_PRIORITY = 0x0B; //Priority of open MPQ
		const uint SFILE_INFO_HASH_INDEX = 0x0C; //Hash index of file in MPQ

		// SFileListFiles flags
		const uint SFILE_LIST_MEMORY_LIST = 0x01; // Specifies that lpFilelists is a file list from memory, rather than being a list of file lists
		const uint SFILE_LIST_ONLY_KNOWN = 0x02; // Only list files that the function finds a name for
		const uint SFILE_LIST_ONLY_UNKNOWN = 0x04; // Only list files that the function does not find a name for

		const uint SFILE_TYPE_MPQ = 0x01;
		const uint SFILE_TYPE_FILE = 0x02;

		const uint SFILE_OPEN_HARD_DISK_FILE = 0x0000; //Open archive without regard to the drive type it resides on
		const uint SFILE_OPEN_CD_ROM_FILE = 0x0001; //Open the archive only if it is on a CD-ROM
		const uint SFILE_OPEN_ALLOW_WRITE = 0x8000; //Open file with write access

		const uint SFILE_SEARCH_CURRENT_ONLY = 0x00; //Used with SFileOpenFileEx; only the archive with the int specified will be searched for the file
		const uint SFILE_SEARCH_ALL_OPEN = 0x01; //SFileOpenFileEx will look through all open archives for the file

		[StructLayout( LayoutKind.Sequential)]
		public struct LCID
		{
			[MarshalAs(UnmanagedType.LPArray, SizeConst=4)]
			public char[] lcLocale;
		}

		[StructLayout( LayoutKind.Sequential)]
		public struct SFMPQVERSION
		{
			public ushort Major;
			public ushort Minor;
			public ushort Revision;
			public ushort Subrevision;
		}

		[StructLayout( LayoutKind.Sequential)]
		public struct FILELISTENTRY 
		{
			public uint dwFileExists; // Nonzero if this entry is used
			
			[MarshalAs(UnmanagedType.LPArray, SizeConst=4)]
			public char[] lcLocale; // Locale ID of file

			public uint dwCompressedSize; // Compressed size of file
			public uint dwFullSize; // Uncompressed size of file
			public uint dwFlags; // Flags for file
			
			[MarshalAs(UnmanagedType.LPArray, SizeConst=260)]
			public char[] szFileName;
		}

		[StructLayout( LayoutKind.Sequential)]
		public struct MPQHEADER 
		{
			public uint dwMPQID; //"MPQ\x1A" for mpq's, "BN3\x1A" for bncache.dat
			public uint dwHeaderSize; // Size of this header
			public uint dwMPQSize; //The size of the mpq archive
			public ushort wUnused0C; // Seems to always be 0
			public ushort wBlockSize; // Size of blocks in files equals 512 << wBlockSize
			public uint dwHashTableOffset; // Offset to hash table
			public uint dwBlockTableOffset; // Offset to block table
			public uint dwHashTableSize; // Number of entries in hash table
			public uint dwBlockTableSize; // Number of entries in block table
		}

		//Archive ints may be typecasted to this struct so you can access
		//some of the archive's properties and the decrypted hash table and
		//block table directly.
		[StructLayout( LayoutKind.Sequential)]
		public struct MPQARCHIVE 
		{
			// Arranged according to priority with lowest priority first
			public IntPtr lpNextArc; // Pointer to the next ARCHIVEREC struct. Pointer to addresses of first and last archives if last archive
			public IntPtr lpPrevArc; // Pointer to the previous ARCHIVEREC struct. 0xEAFC5E23 if first archive

			[MarshalAs(UnmanagedType.LPArray, SizeConst=260)]
			char[] szFileName; // Filename of the archive

			public uint hFile; // The archive's file int
			public uint dwFlags1; // Some flags, bit 1 (0 based) seems to be set when opening an archive from a CD
			public uint dwPriority; // Priority of the archive set when calling SFileOpenArchive
			public IntPtr lpLastReadFile; // Pointer to the last read file's FILEREC struct. Only used for incomplete reads of blocks
			public uint dwUnk; // Seems to always be 0
			public uint dwBlockSize; // Size of file blocks in bytes
			public IntPtr lpLastReadBlock; // Pointer to the read buffer for archive. Only used for incomplete reads of blocks
			public uint dwBufferSize; // Size of the read buffer for archive. Only used for incomplete reads of blocks
			public uint dwMPQStart; // The starting offset of the archive
			public IntPtr lpMPQHeader; // Pointer to the archive header
			public IntPtr lpBlockTable; // Pointer to the start of the block table
			public IntPtr lpHashTable; // Pointer to the start of the hash table
			public uint dwFileSize; // The size of the file in which the archive is contained
			public uint dwOpenFiles; // Count of files open in archive + 1
			public MPQHEADER MpqHeader;
			public uint dwFlags; //The only flag that should be changed is MOAU_MAINTAIN_LISTFILE
			public String lpFileName;
		}

		//ints to files in the archive may be typecasted to this struct
		//so you can access some of the file's properties directly.
		[StructLayout( LayoutKind.Sequential)]
		public struct MPQFILE 
		{
			public IntPtr lpNextFile; // Pointer to the next FILEREC struct. Pointer to addresses of first and last files if last file
			public IntPtr lpPrevFile; // Pointer to the previous FILEREC struct. 0xEAFC5E13 if first file

			[MarshalAs(UnmanagedType.LPArray, SizeConst=260)]
			char[] szFileName; // Filename of the archive
			
			public uint hPlaceHolder; // Always 0xFFFFFFFF
			public IntPtr lpParentArc; // Pointer to the ARCHIVEREC struct of the archive in which the file is contained
			public IntPtr lpBlockEntry; // Pointer to the file's block table entry
			public uint dwCryptKey; // Decryption key for the file
			public uint dwFilePointer; // Position of file pointer in the file
			public uint dwUnk1; // Seems to always be 0
			public uint dwBlockCount; // Number of blocks in file
			public IntPtr lpdwBlockOffsets; // Offsets to blocks in file. There are 1 more of these than the number of blocks
			public uint dwReadStarted; // Set to 1 after first read
			public uint dwUnk2; // Seems to always be 0
			public IntPtr lpLastReadBlock; // Pointer to the read buffer for file. Only used for incomplete reads of blocks
			public uint dwBytesRead; // Total bytes read from open file
			public uint dwBufferSize; // Size of the read buffer for file. Only used for incomplete reads of blocks
			public uint dwConstant; // Seems to always be 1
			public IntPtr lpHashEntry;
			public String lpFileName;
		}

		[StructLayout( LayoutKind.Sequential)]
		public struct BLOCKTABLEENTRY 
		{
			public uint dwFileOffset; // Offset to file
			public uint dwCompressedSize; // Compressed size of file
			public uint dwFullSize; // Uncompressed size of file
			public uint dwFlags; // Flags for file
		}

		[StructLayout( LayoutKind.Sequential)]
		public struct HASHTABLEENTRY 
		{
			public uint dwNameHashA; // First name hash of file
			public uint dwNameHashB; // Second name hash of file
			[MarshalAs(UnmanagedType.LPArray, SizeConst=4)]
			public char[] lcLocale; // Locale ID of file
			public uint dwBlockTableIndex; // Index to the block table entry for the file
		}

		[DllImport("SFmpq.dll")]
		public static extern String MpqGetVersionString();

		[DllImport("SFmpq.dll")]
		public static extern float MpqGetVersion();

		[DllImport("SFmpq.dll")]
		public static extern String SFMpqGetVersionString();

		// SFMpqGetVersionString2's return value is the required length of the buffer plus
		// the terminating null, so use SFMpqGetVersionString2(0, 0); to get the length.
		[DllImport("SFmpq.dll")]
		public static extern uint SFMpqGetVersionString2(IntPtr lpBuffer, uint dwBufferLength);

		[DllImport("SFmpq.dll")]
		public static extern SFMPQVERSION SFMpqGetVersion();

		// Storm functions implemented by this library
		[DllImport("SFmpq.dll")]
		public static extern int SFileOpenArchive(String lpFileName, uint dwPriority, uint dwFlags, ref int hMPQ);
		[DllImport("SFmpq.dll")]
		public static extern int SFileCloseArchive(int hMPQ);
		[DllImport("SFmpq.dll")]
		public static extern int SFileGetArchiveName(int hMPQ, String lpBuffer, uint dwBufferLength);
		[DllImport("SFmpq.dll")]
		public static extern int SFileOpenFile(String lpFileName, ref int hFile);
		[DllImport("SFmpq.dll")]
		public static extern int SFileOpenFileEx(int hMPQ, String lpFileName, uint dwSearchScope, ref int hFile);
		[DllImport("SFmpq.dll")]
		public static extern int SFileCloseFile(int hFile);
		[DllImport("SFmpq.dll")]
		public static extern int SFileGetFileSize(int hFile, ref int lpFileSizeHigh);
		[DllImport("SFmpq.dll")]
		public static extern int SFileGetFileArchive(int hFile, ref int hMPQ);
		[DllImport("SFmpq.dll")]
		public static extern int SFileGetFileName(int hFile, String lpBuffer, uint dwBufferLength);
		[DllImport("SFmpq.dll")]
		public static extern int SFileSetFilePointer(int hFile, int lDistanceToMove, ref int lplDistanceToMoveHigh, uint dwMoveMethod);
		[DllImport("SFmpq.dll")]
		public static extern int SFileReadFile(int hFile, byte[] lpBuffer,uint nNumberOfBytesToRead, ref int lpNumberOfBytesRead, IntPtr lpOverlapped);
		[DllImport("SFmpq.dll")]
		public static extern LCID SFileSetLocale(LCID nNewLocale);
		[DllImport("SFmpq.dll")]
		public static extern int SFileGetBasePath(String lpBuffer, uint dwBufferLength);
		[DllImport("SFmpq.dll")]
		public static extern int SFileSetBasePath(String lpNewBasePath);

		// Extra storm-related functions
		[DllImport("SFmpq.dll")]
		public static extern int SFileGetFileInfo(int hFile, uint dwInfoType);
		[DllImport("SFmpq.dll")]
		public static extern int SFileSetArchivePriority(int hMPQ, uint dwPriority);
		[DllImport("SFmpq.dll")]
		public static extern int SFileFindMpqHeader(int hFile);
		[DllImport("SFmpq.dll")]
		public static extern int SFileListFiles(int hMPQ, String lpFileLists, ref FILELISTENTRY lpListBuffer, uint dwFlags);

		// Archive editing functions implemented by this library
		[DllImport("SFmpq.dll")]
		public static extern int MpqOpenArchiveForUpdate(String lpFileName, uint dwFlags, uint dwMaximumFilesInArchive);
		[DllImport("SFmpq.dll")]
		public static extern uint MpqCloseUpdatedArchive(int hMPQ, uint dwUnknown2);
		[DllImport("SFmpq.dll")]
		public static extern int MpqAddFileToArchive(int hMPQ, String lpSourceFileName, String lpDestFileName, uint dwFlags);
		[DllImport("SFmpq.dll")]
		public static extern int MpqAddWaveToArchive(int hMPQ, String lpSourceFileName, String lpDestFileName, uint dwFlags, uint dwQuality);
		[DllImport("SFmpq.dll")]
		public static extern int MpqRenameFile(int hMPQ, String lpcOldFileName, String lpcNewFileName);
		[DllImport("SFmpq.dll")]
		public static extern int MpqDeleteFile(int hMPQ, String lpFileName);
		[DllImport("SFmpq.dll")]
		public static extern int MpqCompactArchive(int hMPQ);

		// Extra archive editing functions
		[DllImport("SFmpq.dll")]
		public static extern int MpqAddFileToArchiveEx(int hMPQ, String lpSourceFileName, String lpDestFileName, uint dwFlags, uint dwCompressionType, uint dwCompressLevel);
		[DllImport("SFmpq.dll")]
		public static extern int MpqAddFileFromBufferEx(int hMPQ, byte[] lpBuffer, uint dwLength, String lpFileName, uint dwFlags, uint dwCompressionType, uint dwCompressLevel);
		[DllImport("SFmpq.dll")]
		public static extern int MpqAddFileFromBuffer(int hMPQ, byte[] lpBuffer, uint dwLength, String lpFileName, uint dwFlags);
		[DllImport("SFmpq.dll")]
		public static extern int MpqAddWaveFromBuffer(int hMPQ, byte[] lpBuffer, uint dwLength, String lpFileName, uint dwFlags, uint dwQuality);
		[DllImport("SFmpq.dll")]
		public static extern int MpqSetFileLocale(int hMPQ, String lpFileName, LCID nOldLocale, LCID nNewLocale);
	}
}
