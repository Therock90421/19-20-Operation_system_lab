#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512
#define OS_SIZE_LOC 2
#define BOOT_LOADER_SIG_OFFSET 0x1fe
#define BOOT_LOADER_SIG_1 0x55
#define BOOT_LOADER_SIG_2 0xaa
#define BOOT_MEM_LOC 0x7c00
#define OS_MEM_LOC 0x1000

/* structure to store command line options */
static struct
{
    int vm;
    int extended;
} options;

/* prototypes of local functions */
static void create_image(int nfiles, char *files[]);
static void error(char *fmt, ...);
static void read_ehdr(Elf32_Ehdr *ehdr, FILE *fp);
static void read_phdr(Elf32_Phdr *phdr, FILE *fp, int ph,
                      Elf32_Ehdr ehdr);
static void write_segment(Elf32_Ehdr ehdr, Elf32_Phdr phdr, FILE *fp,
                          FILE *img, int *nbytes, int *first);
static void write_os_size(int nbytes, FILE *img);

int main(int argc, char **argv)
{
}

static void create_image(int nfiles, char *files[])
{
}

static void read_ehdr(Elf32_Ehdr *ehdr, FILE *fp)
{
}

static void read_phdr(Elf32_Phdr *phdr, FILE *fp, int ph,
                      Elf32_Ehdr ehdr)
{
}

static void write_segment(Elf32_Ehdr ehdr, Elf32_Phdr phdr, FILE *fp,
                          FILE *img, int *nbytes, int *first)
{
}

static void write_os_size(int nbytes, FILE *img)
{
}

/* print an error message and exit */
static void error(char *fmt, ...)
{
}
