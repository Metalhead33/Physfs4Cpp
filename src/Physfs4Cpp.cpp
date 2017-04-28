#include <exception>
#include "Physfs4Cpp.hpp"

namespace PhysFs
{

bool FileHandle::s_unsafe_mode = false;  // Determines whether initializer and deinitializer checks are enabled or not, and some other safety features
FileHandle::HandleContainer FileHandle::s_container;

FileHandle::FileHandle(const char* path,Mode mode)
{
	auto wptr = std::shared_ptr<FileHandle>( this, [](FileHandle*){} );
	// ^ Needed to allow the constructor to use shared_from_this().
	// It's basically sorcery.
	m_name = std::string(path);
	m_mode = mode;
	switch(mode)
	{
	case WRITE:
		m_chandle = PHYSFS_openWrite(path);
		break;
	case READ:
		m_chandle = PHYSFS_openRead(path);
		break;
	case APPEND:
		m_chandle = PHYSFS_openAppend(path);
		break;
	default:
		m_chandle = PHYSFS_openRead(path);
		break;
	}
	if(!m_chandle) throw std::runtime_error(PHYSFS_getLastError());
	s_container.push_back(shared_from_this());
}
FileHandle::FileHandle(std::string path,Mode mode)
{
	auto wptr = std::shared_ptr<FileHandle>( this, [](FileHandle*){} );
	// ^ Needed to allow the constructor to use shared_from_this().
	// It's basically sorcery.
	m_name = path;
	m_mode = mode;
	switch(mode)
	{
	case WRITE:
		m_chandle = PHYSFS_openWrite(path.c_str());
		break;
	case READ:
		m_chandle = PHYSFS_openRead(path.c_str());
		break;
	case APPEND:
		m_chandle = PHYSFS_openAppend(path.c_str());
		break;
	default:
		m_chandle = PHYSFS_openRead(path.c_str());
		break;
	}
	if(!m_chandle) throw std::runtime_error(PHYSFS_getLastError());
	s_container.push_back(shared_from_this());
}

FileHandle::~FileHandle()
{
	if(m_chandle) PHYSFS_close(m_chandle);
	try {
	s_container.erase(FindSelf());
	} catch(bool e) // In case it is already removed
	{ } // Do nothing.
}

// Here come the static functions


const PHYSFS_ArchiveInfo** FileHandle::o_supportedArchiveTypes()
{
	return o_supportedArchiveTypes();
} // Please don't use this - or do so with extreme caution.
std::list<PHYSFS_ArchiveInfo*> FileHandle::SupportedArchiveTypes()
{
	std::list<PHYSFS_ArchiveInfo*> tmp;
	PHYSFS_ArchiveInfo **i;
	for (i = (PHYSFS_ArchiveInfo**)PHYSFS_supportedArchiveTypes(); *i != NULL; i++)
	{
		tmp.push_back(*i);
	}
	return tmp;
}

void FileHandle::Initialize(const char *argv0)
{
	//if(!PHYSFS_isInit()) s_unsafe_mode = false;
	int check;
	if(s_unsafe_mode)
	{
		check = PHYSFS_init(argv0);
		//s_initialized = true;
	}
	else {
		if(!PHYSFS_isInit())
		{
			check = PHYSFS_init(argv0);
			//s_initialized = true;
		}
	}
	if(!check)
	{
		//s_initialized = false;
		throw std::runtime_error(PHYSFS_getLastError());
	}
}
void FileHandle::Initialize(std::string argv0)
{
	Initialize(argv0.c_str());
}

void FileHandle::Deinitialize()
{
	int check;
	if(!s_unsafe_mode)
	{
		for(HandleIterator it = s_container.begin(); it != s_container.end();++it) (*it).reset();
	}
	check = PHYSFS_deinit();
	//s_initialized = false;
	if(!check)
	{
		//s_initialized = true;
		throw std::runtime_error(PHYSFS_getLastError());
	}
}

const char* FileHandle::o_GetLastError()
{
	return PHYSFS_getLastError();
}
std::string FileHandle::GetLastError()
{
	return std::string(PHYSFS_getLastError());
}

const char* FileHandle::o_GetDirSeparator()
{
	return PHYSFS_getDirSeparator();
} // Original getDirSeparator
std::string FileHandle::GetDirSeparator()
{
	return std::string(PHYSFS_getDirSeparator());
}
const char* FileHandle::o_GetBaseDir()
{
	return PHYSFS_getBaseDir();
} // Original getBaseDir
std::string FileHandle::GetBaseDir()
{
	return std::string(PHYSFS_getBaseDir());
}
const char* FileHandle::o_GetUserDir()
{
	return PHYSFS_getUserDir();
} // Original getUserDir
std::string FileHandle::GetUserDir()
{
	return std::string(PHYSFS_getUserDir());
}
const char* FileHandle::o_GetWriteDir()
{
	return PHYSFS_getWriteDir();
} // Original getWriteDir
std::string FileHandle::GetWriteDir()
{
	return std::string(PHYSFS_getWriteDir());
}
char** FileHandle::o_GetCdRomDirs()
{
	return PHYSFS_getCdRomDirs();
}// Original getCdRomDirs
FileHandle::StringList FileHandle::GetCdRomDirs()
{
	FileHandle::StringList tmp;
	char **cds = PHYSFS_getCdRomDirs();
	char **i;
	for (i = cds; *i != NULL; i++) tmp.push_back(std::string(*i));
	PHYSFS_freeList(cds);
	return tmp;
}
char** FileHandle::o_GetSearchPath()
{
	return PHYSFS_getSearchPath();
}// Original getCdRomDirs
FileHandle::StringList FileHandle::GetSearchPath()
{
	FileHandle::StringList tmp;
	char **cds = PHYSFS_getSearchPath();
	char **i;
	for (i = cds; *i != NULL; i++) tmp.push_back(std::string(*i));
	PHYSFS_freeList(cds);
	return tmp;
}


void FileHandle::SetWriteDir(const char* newdir)
{
	if (!PHYSFS_setWriteDir(newdir)) throw std::runtime_error(PHYSFS_getLastError());
}// The original setWriteDir... with exceptions.
void FileHandle::SetWriteDir(std::string newdir)
{
	SetWriteDir(newdir.c_str());
}

void FileHandle::Mount(const char *newDir, const char *mountPoint, bool appendToPath)
{
	if(!PHYSFS_mount(newDir,mountPoint,appendToPath)) throw std::runtime_error(PHYSFS_getLastError());
} // The original Mount... with exceptions.
void FileHandle::Mount(std::string newDir, std::string mountPoint, bool appendToPath)
{
	Mount(newDir.c_str(),mountPoint.c_str(),appendToPath);
}

void FileHandle::Unmount(const char* oldDir)
{
	if(!PHYSFS_removeFromSearchPath(oldDir)) throw std::runtime_error(PHYSFS_getLastError());
} // The original Unmount... with exceptions.

void FileHandle::Unmount(std::string oldDir)
{
	Unmount(oldDir.c_str());
}

// Non-satic functions

FileHandle::HandleIterator FileHandle::FindSelf()
{
	for(HandleIterator it = s_container.begin(); it != s_container.end();++it)
	{
		if((*it).get() == this) return it;
	}
	throw false;
}
FileHandle::Mode FileHandle::GetMode()
{
	return m_mode;
}
std::string FileHandle::GetName()
{
	return m_name;
}

void FileHandle::Mkdir(const char* newdir)
{
	if(!PHYSFS_mkdir(newdir)) throw std::runtime_error(PHYSFS_getLastError());
}
void FileHandle::Mkdir(std::string newdir)
{
	Mkdir(newdir.c_str());
}
void FileHandle::Delete(const char* newdir)
{
	if(!PHYSFS_delete(newdir)) throw std::runtime_error(PHYSFS_getLastError());
}
void FileHandle::Delete(std::string newdir)
{
	Delete(newdir.c_str());
}

const char* FileHandle::o_GetRealDir(const char* filename)
{
	return PHYSFS_getRealDir(filename);
} // Original getRealDir
const char* FileHandle::o_GetRealDir(std::string filename)
{
	o_GetRealDir(filename.c_str());
}
std::string FileHandle::s_GetRealDir(const char* filename)
{
	return std::string(o_GetRealDir(filename));
} // Same, just converted to string
std::string FileHandle::s_GetRealDir(std::string filename)
{
	return std::string(o_GetRealDir(filename));
}
char** FileHandle::o_enumerateFiles(const char* dir)
{
	return PHYSFS_enumerateFiles(dir);
} // Please never use it, or do so with extreme caution.
char** FileHandle::o_enumerateFiles(std::string dir)
{
	return PHYSFS_enumerateFiles(dir.c_str());
}// Please never use it, or do so with extreme caution.

FileHandle::StringList FileHandle::EnumerateFiles(const char* dir)
{
	FileHandle::StringList tmp;
	char **rc = PHYSFS_enumerateFiles(dir);
	char **i;

	for (i = rc; *i != NULL; i++) tmp.push_back(std::string(*i));

	PHYSFS_freeList(rc);
	return tmp;
}
FileHandle::StringList FileHandle::EnumerateFiles(std::string dir)
{
	return EnumerateFiles(dir.c_str());
}
bool FileHandle::Exists(const char* fname)
{
	return PHYSFS_exists(fname);
}
bool FileHandle::Exists(std::string fname)
{
	return PHYSFS_exists(fname.c_str());
}
bool FileHandle::IsDirectory(const char* fname)
{
	return PHYSFS_isDirectory(fname);
}
bool FileHandle::IsDirectory(std::string fname)
{
	return PHYSFS_isDirectory(fname.c_str());
}
bool FileHandle::IsSymlink(const char* fname)
{
	return PHYSFS_isSymbolicLink(fname);
}
bool FileHandle::IsSymlink(std::string fname)
{
	return PHYSFS_isSymbolicLink(fname.c_str());
}
FileHandle::SharedHandle FileHandle::openWrite(const char* filename)
{
	return SharedHandle(new FileHandle(filename,WRITE));
}
FileHandle::SharedHandle FileHandle::openRead(const char* filename)
{
	return SharedHandle(new FileHandle(filename,READ));
}
FileHandle::SharedHandle FileHandle::openAppend(const char* filename)
{
	return SharedHandle(new FileHandle(filename,APPEND));
}
FileHandle::SharedHandle FileHandle::openWrite(std::string filename)
{
	return openWrite(filename.c_str());
}
FileHandle::SharedHandle FileHandle::openRead(std::string filename)
{
	return openRead(filename.c_str());
}
FileHandle::SharedHandle FileHandle::openAppend(std::string filename)
{
	return openAppend(filename.c_str());
}
PHYSFS_sint64 FileHandle::pRead(void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount)
{
	return PHYSFS_read(m_chandle,buffer,objSize,objCount);
}
PHYSFS_sint64 FileHandle::pWrite(const void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount)
{
	return PHYSFS_write(m_chandle,buffer,objSize,objCount);
}

bool FileHandle::pEof()
{
	return PHYSFS_eof(m_chandle);
}
PHYSFS_sint64 FileHandle::pTell()
{
	return PHYSFS_tell(m_chandle);
}
void FileHandle::pSeek(PHYSFS_uint64 pos)
{
	if(!PHYSFS_seek(m_chandle,pos)) throw std::runtime_error(PHYSFS_getLastError());
}
PHYSFS_sint64 FileHandle::pFileLength()
{
	return PHYSFS_fileLength(m_chandle);
}
bool FileHandle::pFlush()
{
	return PHYSFS_flush(m_chandle);
}
bool FileHandle::pSetBuffer(PHYSFS_uint64 bufsize)
{
	return PHYSFS_setBuffer(m_chandle,bufsize);
}

}
