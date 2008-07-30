#include "mpq_libmpq.h"
#include <vector>
#include "libmpq/common.h"

//typedef std::vector<mpq_archive*> ArchiveSet;
typedef std::vector<MPQArchive*> ArchiveSet;
ArchiveSet gOpenArchives;

MPQArchive::MPQArchive(const char* filename)
{
	int result = libmpq_archive_open(&mpq_a, (unsigned char*)filename);
    printf("libmpq: Opening MPQ Archive: %s\n", filename);
	if(result) {
		printf("Error opening archive %s\n", filename);
		return;
	}
	gOpenArchives.push_back(this); //&mpq_a);
}

void MPQArchive::close()
{
	//gOpenArchives.erase(erase(&mpq_a);
	libmpq_archive_close(&mpq_a);
}

bool mpq_file_exists(const char* filename)
{
    for(ArchiveSet::iterator i=gOpenArchives.begin(); i!=gOpenArchives.end();++i)
    {
        mpq_archive &mpq_a = (*i)->mpq_a;

        mpq_hash hash = (*i)->GetHashEntry(filename);
        uint32 blockindex = hash.blockindex;

        if ((blockindex == 0xFFFFFFFF) || (blockindex == 0)) {
            continue; //file not found
        }
        return true;
    }
    return false;
}

MPQFile::MPQFile(const char* filename):
	eof(false),
	buffer(0),
	pointer(0),
	size(0)
{
	for(ArchiveSet::reverse_iterator i=gOpenArchives.rbegin(); i!=gOpenArchives.rend();++i)
	{
		mpq_archive &mpq_a = (*i)->mpq_a;
		
        mpq_hash hash = (*i)->GetHashEntry(filename);
		uint32 blockindex = hash.blockindex;

        if ((blockindex == 0xFFFFFFFF) || (blockindex == 0)) {
            continue; //file not found
        }
        
        int fileno = blockindex;

        //int fileno = libmpq_file_number(&mpq_a, filename);
		//if(fileno == LIBMPQ_EFILE_NOT_FOUND)
		//	continue;

        // Found!
		size = libmpq_file_info(&mpq_a, LIBMPQ_FILE_UNCOMPRESSED_SIZE, fileno);
		// HACK: in patch.mpq some files don't want to open and give 1 for filesize
		if (size<=1) {
			eof = true;
			buffer = 0;
			return;
		}
		buffer = new char[size];

		//libmpq_file_getdata
        int result = libmpq_file_getdata(&mpq_a, hash, fileno, (unsigned char*)buffer);
		return;

	}
	eof = true;
	buffer = 0;
}

MPQFile::~MPQFile()
{
	close();
}


size_t MPQFile::read(void* dest, size_t bytes)
{
	if (eof) return 0;

	size_t rpos = pointer + bytes;
	if (rpos > size) {
		bytes = size - pointer;
		eof = true;
	}

	memcpy(dest, &(buffer[pointer]), bytes);

	pointer = rpos;

	return bytes;
}

bool MPQFile::isEof()
{
    return eof;
}

void MPQFile::seek(int offset)
{
	pointer = offset;
	eof = (pointer >= size);
}

void MPQFile::seekRelative(int offset)
{
	pointer += offset;
	eof = (pointer >= size);
}

void MPQFile::close()
{
	if (buffer) delete[] buffer;
	buffer = 0;
	eof = true;
}

size_t MPQFile::getSize()
{
	return size;
}

size_t MPQFile::getPos()
{
	return pointer;
}

char* MPQFile::getBuffer()
{
	return buffer;
}

char* MPQFile::getPointer()
{
	return buffer + pointer;
}
