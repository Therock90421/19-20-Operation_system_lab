#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_bootblock(FILE *image, FILE *bbfile, Elf32_Phdr *Phdr);
Elf32_Phdr *read_exec_file(FILE *opfile);
uint8_t count_kernel_sectors(Elf32_Phdr *Phdr);
void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz);

Elf32_Phdr *read_exec_file(FILE *opfile)               //读取elf格式的文件中的程序头
{
    Elf32_Phdr *phdr;
    Elf32_Ehdr *ehdr;
    uint8_t buffer[512];                               //分配一个扇区大小的缓冲区
    phdr = (Elf32_Phdr *)calloc(1,sizeof(Elf32_Phdr));   //分配一个程序头大小的区域，并将数据初始化为0
    fread(buffer, 1, 512, opfile);      //size_t   fread(void *buffer, size_t   size, size_t  count, FILE  *stream );读取512个字节到buffer
    ehdr = (Elf32_Ehdr *)buffer;                       //把缓冲区文件转换为文件头的格式，以读取程序头的偏移量
    memcpy(phdr,(void *)ehdr + ehdr->e_phoff,32 );      //程序头的大小为32个字节       e_phoff为程序头到文件首地址的偏移量
    return phdr;
    
}

uint8_t count_kernel_sectors(Elf32_Phdr *Phdr)         //计算kernel有多少扇区
{
    uint8_t number = (Phdr->p_filesz + 511) / 512;    //p_filesz表示segment段的大小，但是其可能不是512的整数倍，故向上取整
    return number;
}

void write_bootblock(FILE *image, FILE *file, Elf32_Phdr *phdr) //把bootblock写入到image
{
    uint8_t buffer[512];
    int counter;                         //p_offset为segment到文件首地址的偏移量
    fseek(file, phdr->p_offset, SEEK_SET); //  例：fseek(fp,50L,SEEK_SET);    其作用是将位置指针移到离文件首地址50个字节处
                                           //SEEK_SET 文件首地址  SEEK_CUR 当前地址  SEEK_END 文件尾地址
    for( counter = 1; counter*512 <= phdr->p_filesz; counter++)
    {
         memset(buffer, 0, sizeof(buffer));
         fread(buffer, 1, 512, file);
         fwrite(buffer, 1, 512, image);
    }                                      
    if((phdr->p_filesz)%512)                        //如果segment段大小不为整数扇区,就多写一次（包括大小不到一个扇区的情况）
    {
         memset(buffer, 0, sizeof(buffer));
         fread(buffer, 1, 512, file);
         fwrite(buffer, 1, 512, image);
    }

}
//　(1)写操作fwrite()后必须关闭流fclose()。

//   (2)不关闭流的情况下，每次读或写数据后，文件指针都会指向下一个待写或者读数据位置的指针。
void write_kernel(FILE *image, FILE *knfile, Elf32_Phdr *Phdr, int kernelsz) //把kernel写入到image
{
     uint8_t buffer[512];
     int counter;
     counter = kernelsz;
     fseek(knfile, Phdr->p_offset, SEEK_SET);
     while(counter--)
     {
         memset(buffer, 0, sizeof(buffer));
         fread(buffer, 1, 512, knfile);
         fwrite(buffer, 1, 512, image);
     }
}

void record_kernel_sectors(FILE *image, uint8_t kernelsz)           //把扇区个数写道bootblock的os_size位
{
     uint8_t *buffer;
     buffer = &kernelsz;                                            //指针
     fseek(image, 511, SEEK_SET);                                   //汇编源码
     fwrite(buffer, 1, 1, image);
}

void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz) //打印信息
{
     printf("Bootblock message: \n");
     printf("size   %d byte \n", Phdr_bb->p_filesz);
     printf("memory size  0x%x \n",Phdr_bb->p_memsz);
     printf("memory offset  0x%x \n", Phdr_bb->p_offset);
     printf("virtual address  0x%x \n", Phdr_bb->p_vaddr);
     printf("padding up to 0x%x \n\n", 0x200*((Phdr_bb->p_filesz+511)/512));


     printf("Kernel message: \n");
     printf("sector size   %d \n", kernelsz);
     printf("memory size  0x%x \n",Phdr_k->p_memsz);
     printf("memory offset  0x%x \n", Phdr_k->p_offset);
     printf("virtual address  0x%x \n", Phdr_bb->p_vaddr);
     printf("padding up to 0x%x \n\n", 0x200*((Phdr_bb->p_filesz+511)/512)+0x200*kernelsz);
}

int main()
{
     int kernelsz;
     FILE *bfile = fopen("bootblock","rb");              //rb只读打开二进制文件
     FILE *kfile = fopen("kernel", "rb");                //打开流
     FILE *image = fopen("image", "wb");                 //wb只写打开二进制文件
     Elf32_Phdr *Phdr_bootblock = read_exec_file(bfile);  //读取bootblock程序头
     Elf32_Phdr *Phdr_kernel = read_exec_file(kfile);     //读取kernel程序头
     
     kernelsz = (int)count_kernel_sectors(Phdr_kernel);
     write_bootblock(image, bfile, Phdr_bootblock);       //bootblock的segment写入image
       
     write_kernel(image, kfile, Phdr_kernel, kernelsz);

     record_kernel_sectors(image, kernelsz);

     extent_opt(Phdr_bootblock, Phdr_kernel, kernelsz);
     fclose(bfile);
     fclose(kfile);
     fclose(image);




     
}
