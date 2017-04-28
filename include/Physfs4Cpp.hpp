#ifndef PHYSFS4CPP_HPP
#define PHYSFS4CPP_HPP
#include <vector>
#include <list>
#include <memory>

extern "C" {
#include <physfs.h>
}

namespace PhysFs
{

class FileHandle : public std::enable_shared_from_this<FileHandle>
{
public:
	~FileHandle();
	typedef std::shared_ptr<FileHandle> SharedHandle;
	typedef std::vector<SharedHandle> HandleContainer;
	typedef HandleContainer::iterator HandleIterator;
	typedef std::list<std::string> StringList;
	enum Mode : uint8_t
	{
		WRITE,
		APPEND,
		READ
	};
	FileHandle(const char* path,Mode mode);
	FileHandle(std::string path,Mode mode);
private:
	//static bool s_initialized; // Determines whether the system is initialized or not
	static bool s_unsafe_mode;  // Determines whether initializer and deinitializer checks are enabled or not, and some other safety features
	static HandleContainer s_container;
	std::string m_name;
	Mode m_mode; // True = Write, False = Read
	PHYSFS_File* m_chandle;
public:
	// Static functions

	static bool ReturnGlobalStat();
	static bool ReturnGlobalUnsafety();
	static void SetUnsafety(bool setto);

	static void Initialize(const char *argv0=0);
	static void Initialize(std::string argv0);
	static void Deinitialize();
	static void PermitSymbolicLinks(bool allow);

	static const PHYSFS_ArchiveInfo** o_supportedArchiveTypes(); // Please don't use this - or do so with extreme caution.
	static std::list<PHYSFS_ArchiveInfo*> SupportedArchiveTypes();

	static const char* o_GetLastError(); // Original getLastError
	static std::string GetLastError();
	static const char* o_GetDirSeparator(); // Original getDirSeparator
	static std::string GetDirSeparator();
	static const char* o_GetBaseDir(); // Original getBaseDir
	static std::string GetBaseDir();
	static const char* o_GetUserDir(); // Original getUserDir
	static std::string GetUserDir();
	static const char* o_GetWriteDir(); // Original getWriteDir
	static std::string GetWriteDir();
	static char** o_GetCdRomDirs(); // Original getCdRomDirs. Please never use it, or use it with extreme precaution. Remember to PHYSFS_freeList(cds) afterwards.
	static StringList GetCdRomDirs();
	static char** o_GetSearchPath(); // Original getSearchPath. Please never use it, or use it with extreme precaution. Remember to PHYSFS_freeList(cds) afterwards.
	static StringList GetSearchPath();

	static void SetWriteDir(const char* newdir); // The original setWriteDir... with exceptions.
	static void SetWriteDir(std::string newdir);
	static void Mount(const char *newDir, const char *mountPoint=0, bool appendToPath=false); // The original Mount... with exceptions.
	static void Mount(std::string newDir, std::string mountPoint="", bool appendToPath=false);
	static void Unmount(const char* oldDir); // The original Unmount... with exceptions.
	static void Unmount(std::string oldDir);

	static const char* o_GetRealDir(const char* filename); // Original getRealDir
	static const char* o_GetRealDir(std::string filename);
	static std::string s_GetRealDir(const char* filename); // Same, just converted to string
	static std::string s_GetRealDir(std::string filename);

	static void Mkdir(const char* newdir);
	static void Mkdir(std::string newdir);
	static void Delete(const char* newdir);
	static void Delete(std::string newdir);

	static char** o_enumerateFiles(const char* dir); // Please never use it, or do so with extreme caution.
	static char** o_enumerateFiles(std::string dir); // Please never use it, or do so with extreme caution.
	static StringList EnumerateFiles(const char* dir); // Use this one.
	static StringList EnumerateFiles(std::string dir); // Use this one.

	static bool Exists(const char* fname);
	static bool Exists(std::string fname);
	static bool IsDirectory(const char* fname);
	static bool IsDirectory(std::string fname);
	static bool IsSymlink(const char* fname);
	static bool IsSymlink(std::string fname);

		// These 6 are largerly convenience functions mainly intended for testing.
		// They are completely useless if you intend to sublcass FileHandle.
	static SharedHandle openWrite(const char* filename);
	static SharedHandle openRead(const char* filename);
	static SharedHandle openAppend(const char* filename);
	static SharedHandle openWrite(std::string filename);
	static SharedHandle openRead(std::string filename);
	static SharedHandle openAppend(std::string filename);

	// Non-satic functions
	HandleIterator FindSelf();
	Mode GetMode();
	std::string GetName();

	PHYSFS_sint64 pRead(void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount);
	PHYSFS_sint64 pWrite(const void *buffer, PHYSFS_uint32 objSize, PHYSFS_uint32 objCount);
	bool pEof();
	PHYSFS_sint64 pTell();
	void pSeek(PHYSFS_uint64 pos);
	PHYSFS_sint64 pFileLength();
	bool pFlush();
	bool pSetBuffer(PHYSFS_uint64 bufsize);
};

}


#endif // PHYSFS4CPP_HPP
