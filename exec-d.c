#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include <lzss.h>

int
main(int argc, char * const argv[])
{
    FILE *	fin;
    FILE *	fout;
    char		tmp[20];
    mode_t	mode;

    fin = fopen(argv[1], "r");
    if(fin == NULL)
    {
        perror(argv[1]);
        return 1;
    }

    {
        struct stat st;

        if(fstat(fileno(fin), &st) < 0)
        {
            perror(argv[1]);
            return 1;
        }

        mode = st.st_mode;
    }

    {
        int c;

        do
        {
            c = fgetc(fin);
            if (c == EOF)
                return 1;
        }
        while(c != '\n');
    }

    sprintf(tmp, "/tmp/lzss-exec.%d", getpid());
    fout = fopen(tmp, "w");
    if (fout == NULL)
    {
        perror(tmp);
        return 1;
    }

    if(lzss_decode(lzss_get(fgetc), fin, lzss_put(fputc), fout) < 0)
    {
        fputs("Cannot decode.\n", stderr);
        return 1;
    }

    if(fchmod(fileno(fout), mode) < 0)
    {
        perror(tmp);
        return -1;
    }

    fclose(fout);
    fclose(fin);

    execv(tmp, argv + 1);
    fputs("Cannot execute.\n", stderr);
    return 1;
}
