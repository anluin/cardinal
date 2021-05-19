#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "file.h"


__attribute__((overloadable))
result_t(file_t, file_error_t) File(const char* pathname, file_access_mode_t access_mode) {
    #define OK    Ok   (file_t, file_error_t)
    #define ERROR Error(file_t, file_error_t)

    int open_flags = 0;

    switch (access_mode) {
        case FileAccessMode_NONE: {
            char* message;
            
            asprintf(&message, "File access mode cannot be none!");

            return ERROR(FileError(message));
        }
        case FileAccessMode_READ:
            open_flags = O_RDONLY;
            break;
        case FileAccessMode_WRITE:
            open_flags = O_WRONLY;
            break;
        case FileAccessMode_READ_AND_WRITE:
            open_flags = O_RDWR;
            break;
    }

    int file_descriptor = open(pathname, open_flags | O_NONBLOCK);

    if (file_descriptor < 0) {
        return ERROR(FileError(strdup(strerror(errno))));
    }

    int flags = fcntl(file_descriptor, F_GETFL, 0);
    fcntl(file_descriptor, F_SETFL, flags | O_NONBLOCK);

    return OK((file_t) {
        .handle = file_descriptor,
    });
}

__attribute__((overloadable))
void cleanup(file_t* self_ptr) {
    close(self_ptr->handle);
}

__attribute__((overloadable))
size_t get_size(file_t* self_ptr) {
    off_t cursor = lseek(self_ptr->handle, 0, SEEK_CUR);
    off_t size   = lseek(self_ptr->handle, 0, SEEK_END);

    lseek(self_ptr->handle, cursor, SEEK_SET);

    return size;
}
