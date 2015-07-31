#include "clc/data/Buffer.h"
#include "clc/storage/Path.h"

#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <new>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <dirent>
#define clc_mkdir(path, mode) ::_mkdir(path, mode)
#else
#define clc_statstruct stat
#define clc_fstat(fd, statbuf) ::fstat(fd, statbuf)
#define clc_stat(path, statbuf) ::stat(path, statbuf)
#define clc_mkdir(path, mode) ::mkdir(path, mode)
#define clc_rmdir(path) ::rmdir(path)
#define clc_seek(fd, offset, whence) ::fseeko(fd, offset, whence)
#define clc_tell(fd) ::ftello(fd)
#define clc_unlink(pathname) ::unlink(pathname)
#endif

#define PATH_SEP '/'

namespace clc {

#if 0
static bool isFile(const Buffer &pathname)
{
    struct stat stat_info;

    return ::lstat(pathname.c_str(), &stat_info) == 0 && S_ISREG(stat_info.st_mode);
}
#endif

bool Path::isAbsolute(const char *path)
{
#ifdef _WIN32
    return (*path == '\\') ||
           (((*path >= 'A' && *path <= 'Z') ||
             (*path >= 'a' && *path <= 'z')) &&
            *(path + 1) == ':');
#else
    return *path == '/';
#endif
}

Buffer Path::join(const char *base, const char *leaf)
{
    Buffer path;

    if (!leaf) {
        path = base;
    } else if (isAbsolute(leaf)) {
        path = leaf;
    } else {
        path = base;
        if (path.length() && path[path.length() - 1] != PATH_SEP)
            path += PATH_SEP;
        path += leaf;
    }
    return path;
}

void Path::split(const char *path, Buffer &base, Buffer &file)
{
    Buffer pattern(path);
    int32_t pos = pattern.FindLast(PATH_SEP);

    if (pos != -1) {
        pattern.CopyInto(base, 0, pos > 0 ? pos : 1);
        pos += 1;
    } else {
        base = ".";
        pos = 0;
    }
    pattern.CopyInto(file, pos, pattern.length());
}

Buffer &Path::join(Buffer &base, const char *leaf)
{
    if (!leaf) {
        // no change
    } else if (isAbsolute(leaf)) {
        base = leaf;
    } else {
        if (base.length() && base[base.length() - 1] != PATH_SEP)
            base += PATH_SEP;
        base += leaf;
    }
    return base;
}

int Path::list(const char *dir, const char *glob, std::list<Buffer> &files)
{
    int r = -1;

    try {
#ifdef _WIN32
        Buffer pattern(dir);
        Path::join(pattern, glob ? glob : "*");
        HANDLE h;
        WIN32_FIND_DATA dp;
        if ((h = FindFirstFile(pattern.c_str(), &dp)) != INVALID_HANDLE_VALUE) {
            r = 0;
            do {
                files.push_back(Buffer(dp.cFileName));
            } while (FindNextFile(h, &dp));
            FindClose(h);
        }
#else
        DIR *dp;
        if ((dp = opendir(dir)) == NULL) {
            r = errno;
        } else {
            r = 0;
            struct dirent *dir_entry;
            while ((dir_entry = readdir(dp)) != NULL) {
                const char *name = dir_entry->d_name;
                if (!glob || fnmatch(glob, name, 0) == 0) {
                    files.push_back(Buffer(name));
                }
            }
            closedir(dp);
        }
#endif
    } catch (std::bad_alloc &) {
        r = ENOMEM;
    }
    return r;
}

Buffer Path::getDirectory(Buffer &path)
{
    Buffer dir(path);
    size_t pos = path.FindLast(PATH_SEP);

    if (pos != Buffer::NotFound)
        dir.truncate(pos);
    return dir;
}

bool Path::exists(const char *path)
{
    struct clc_statstruct statbuf;
    int r = clc_stat(path, &statbuf);

    return r == 0;
}

}
