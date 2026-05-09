/*
    ============================================================
        SANNE SCRIPT COMPILER
        Author: Dr Sanne Karibo
    ============================================================

    DESCRIPTION
    ------------------------------------------------------------
    Converts script files into native launchable executables.

    SUPPORTED:
        Windows:
            .bat
            .cmd
            .ps1

        Linux:
            .sh

    FEATURES
    ------------------------------------------------------------
    - Auto scans current folder
    - Single file compile mode
    - Windows uses TinyCC (libtcc)
    - Linux uses GCC
    - Cross-platform
    - Single source file

    USAGE
    ------------------------------------------------------------

    AUTO MODE:
        compile.exe
        ./compile

    SINGLE FILE:
        compile.exe script.bat
        ./compile script.sh

    BUILD
    ------------------------------------------------------------

    LINUX:
        gcc compile.c -o compile

    WINDOWS:
        gcc compile.c -ltcc -o compile.exe

    NOTE
    ------------------------------------------------------------
    Linux compilation uses GCC directly because
    most Linux systems already include GCC.

    Windows uses TinyCC library internally.

    ============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <windows.h>
#include <libtcc.h>

#else

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#endif

#define MAX_CONTENT 65536
#define MAX_PATH_LEN 1024

/* ============================================================ */

int ends_with(
    const char *str,
    const char *suffix
)
{
    if (!str || !suffix)
        return 0;

    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);

    if (lensuffix > lenstr)
        return 0;

    return strcmp(
        str + lenstr - lensuffix,
        suffix
    ) == 0;
}

/* ============================================================ */

void remove_extension(char *file)
{
    char *dot = strrchr(file, '.');

    if (dot)
        *dot = '\0';
}

/* ============================================================ */

int read_file(
    const char *path,
    char *buffer
)
{
    FILE *f = fopen(path, "rb");

    if (!f)
        return 0;

    size_t len = fread(
        buffer,
        1,
        MAX_CONTENT - 1,
        f
    );

    buffer[len] = '\0';

    fclose(f);

    return 1;
}

/* ============================================================ */

void escape_string(
    const char *src,
    char *dst
)
{
    while (*src)
    {
        if (*src == '\\')
        {
            *dst++ = '\\';
            *dst++ = '\\';
        }
        else if (*src == '"')
        {
            *dst++ = '\\';
            *dst++ = '"';
        }
        else if (*src == '\n')
        {
            *dst++ = '\\';
            *dst++ = 'n';
        }
        else if (*src == '\r')
        {
        }
        else
        {
            *dst++ = *src;
        }

        src++;
    }

    *dst = '\0';
}

/* ============================================================ */

void compile_linux_script(
    const char *script_path
)
{
    char content[MAX_CONTENT];
    char escaped[MAX_CONTENT * 2];

    if (!read_file(script_path, content))
    {
        printf("FAILED: %s\n", script_path);
        return;
    }

    escape_string(content, escaped);

    char output[MAX_PATH_LEN];

    strncpy(output,
            script_path,
            sizeof(output));

    remove_extension(output);

    FILE *f = fopen("temp_wrapper.c", "w");

    if (!f)
    {
        printf("Cannot create wrapper\n");
        return;
    }

    fprintf(f,
        "#include <stdlib.h>\n"
        "int main(){\n"
        "system(\"bash -c \\\"%s\\\"\");\n"
        "return 0;\n"
        "}\n",
        escaped
    );

    fclose(f);

    char cmd[4096];

    snprintf(
        cmd,
        sizeof(cmd),
        "gcc temp_wrapper.c -static -o \"%s\"",
        output
    );

    printf("COMPILING: %s\n", script_path);

    int result = system(cmd);

    if (result == 0)
    {
        chmod(output, 0755);

        printf("SUCCESS: %s\n", output);
    }
    else
    {
        printf("FAILED: %s\n", script_path);
    }

    remove("temp_wrapper.c");
}

/* ============================================================ */

#ifdef _WIN32

void compile_windows_script(
    const char *script_path
)
{
    char content[MAX_CONTENT];
    char escaped[MAX_CONTENT * 2];

    if (!read_file(script_path, content))
    {
        printf("FAILED: %s\n", script_path);
        return;
    }

    escape_string(content, escaped);

    char output[MAX_PATH_LEN];

    strncpy(output,
            script_path,
            sizeof(output));

    remove_extension(output);

    strcat(output, ".exe");

    char code[MAX_CONTENT * 3];

    memset(code, 0, sizeof(code));

    strcat(code,
        "#include <stdlib.h>\n"
        "int main(){\n"
        "system(\"");

    if (ends_with(script_path, ".ps1"))
    {
        strcat(code,
            "powershell -Command \\\"");
    }
    else
    {
        strcat(code,
            "cmd /c \\\"");
    }

    strcat(code, escaped);

    strcat(code,
        "\\\"\");"
        "return 0;"
        "}\n");

    TCCState *s;

    s = tcc_new();

    if (!s)
    {
        printf("Cannot initialize TinyCC\n");
        return;
    }

    tcc_set_output_type(
        s,
        TCC_OUTPUT_EXE
    );

    if (tcc_compile_string(s, code) == -1)
    {
        printf("Compile failed: %s\n",
               script_path);

        tcc_delete(s);

        return;
    }

    if (tcc_output_file(s, output) == -1)
    {
        printf("Output failed: %s\n",
               output);

        tcc_delete(s);

        return;
    }

    printf("SUCCESS: %s\n", output);

    tcc_delete(s);
}

#endif

/* ============================================================ */

void process_file(
    const char *name
)
{
#ifdef _WIN32

    if (
        ends_with(name, ".bat") ||
        ends_with(name, ".cmd") ||
        ends_with(name, ".ps1")
    )
    {
        compile_windows_script(name);
    }

#else

    if (ends_with(name, ".sh"))
    {
        compile_linux_script(name);
    }

#endif
}

/* ============================================================ */

void scan_directory()
{

#ifdef _WIN32

    WIN32_FIND_DATA fd;

    HANDLE h = FindFirstFile("*.*", &fd);

    if (h == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (!(fd.dwFileAttributes &
              FILE_ATTRIBUTE_DIRECTORY))
        {
            process_file(fd.cFileName);
        }

    } while (FindNextFile(h, &fd));

    FindClose(h);

#else

    DIR *d = opendir(".");

    if (!d)
        return;

    struct dirent *dir;

    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_type == DT_REG)
        {
            process_file(dir->d_name);
        }
    }

    closedir(d);

#endif
}

/* ============================================================ */

int main(
    int argc,
    char *argv[]
)
{
    printf("\n");
    printf("================================================\n");
    printf("      SANNE SCRIPT COMPILER\n");
    printf("      Author: Dr Sanne Karibo\n");
    printf("================================================\n\n");

#ifdef _WIN32

    printf("OS DETECTED: WINDOWS\n");
    printf("Compiler Backend: TinyCC\n\n");

#else

    printf("OS DETECTED: LINUX\n");
    printf("Compiler Backend: GCC\n\n");

#endif

    if (argc > 1)
    {
        process_file(argv[1]);
    }
    else
    {
        scan_directory();
    }

    printf("\nDONE\n");

    return 0;
}
