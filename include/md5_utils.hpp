#ifndef _MD5_UTILS_HPP_
#define _MD5_UTILS_HPP_

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

//#include <opencv2/opencv.hpp>
#include <openssl/md5.h>

//unsigned char result[MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.
static inline void print_md5_sum(unsigned char *md)
{
    int i;
    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        printf("%02x", md[i]);
    }
}

// Get the size of the file by its file descriptor
static inline off_t get_size_by_fd(int fd)
{
    struct stat statbuf;
    off_t size = -1;
    if (fstat(fd, &statbuf) < 0)
    {
        size = -1;
    }
    else
    {
        size = statbuf.st_size;
    }
    return size;
}

static const unsigned char JFIF_MARKER_START = 0xFF;
static const unsigned char JFIF_MARKER_SOS = 0xDA; // start of scan
static const unsigned char JFIF_MARKER_EOI = 0xD9; // end of image
static const unsigned char JFIF_MARKER_APP = 0xE0; // APP 0
static const unsigned char JFIF_MARKER_APP_MASK = 0xF0;

static inline bool find_jpeg_coeffs(unsigned char *buff, const off_t &size, off_t &start, off_t &end)
{
    start = -1;
    end = -1;
    bool find_sos = false;
    for (off_t i = 0; i < (size - 1); ++i)
    {
        if (buff[i] == JFIF_MARKER_START)
        {
            if ((buff[i + 1] & JFIF_MARKER_APP_MASK) == JFIF_MARKER_APP)
            {
                // find one app section
                unsigned char s1 = buff[i + 2];
                unsigned char s2 = buff[i + 3];
                size_t s = 256 * s1 + s2 - 1;
                i += s;
            }
            else if (buff[i + 1] == 0xDA)
            {
                start = i + 2;
                end = start;
                find_sos = true;
            }
            else if (buff[i + 1] == JFIF_MARKER_EOI && find_sos)
            {
                end = i;
                break;
            }
            i += 1;
        }
    }
    return (start < end);
}

static inline bool gen_jpeg_md5sum(std::string name, unsigned char *md5, const bool &debug = false)
{
    int fd;
    off_t file_size;
    unsigned char *file_buffer;
    bool result = true;

    fd = open(name.c_str(), O_RDONLY);
    if (fd < 0)
    {
        result = false;
    }

    file_size = get_size_by_fd(fd);

    file_buffer = (unsigned char *)mmap(0, file_size, PROT_READ, MAP_SHARED, fd, 0);
    off_t start = -1;
    off_t end = -1;
    if (find_jpeg_coeffs(file_buffer, file_size, start, end))
    {
        unsigned char *p = file_buffer + start;
        off_t coeffs_size = end - start;
        MD5(p, coeffs_size, md5);
    }
    munmap(file_buffer, file_size);

    if (debug)
    {
        printf("file size:\t%ld start:\t%ld end:\t%ld\n", file_size, start, end);
        print_md5_sum(md5);
        printf("  %s\n", name.c_str());
    }
    close(fd);

    return result;
}

#endif //_MD5_UTILS_HPP_
