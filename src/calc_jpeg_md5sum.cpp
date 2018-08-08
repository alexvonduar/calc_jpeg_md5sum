#include <cstdio>
#include <cstdlib>

#include "md5_utils.hpp"
#include "time_utils.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }

    unsigned char result[MD5_DIGEST_LENGTH];

    TimeLapse t;
    t.start();
    bool r = gen_jpeg_md5sum(string(argv[1]), result);
    t.stop();
    //printf("take %lu ns\n", t.interval());
    if (r) {
        print_md5_sum(result);
        //printf("\n");
        return 0;
    } else {
        return 1;
    }
}
