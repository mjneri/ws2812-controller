# 1 "graphics/drivers/sh1106.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Users/MJ/.mchp_packs/Microchip/PIC18F-K_DFP/1.6.125/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "graphics/drivers/sh1106.c" 2
# 18 "graphics/drivers/sh1106.c"
# 1 "graphics/drivers/sh1106.h" 1
# 27 "graphics/drivers/sh1106.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\string.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\string.h" 2 3






# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\features.h" 1 3
# 10 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\string.h" 2 3
# 25 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\string.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 122 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned size_t;
# 168 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef __int24 int24_t;
# 204 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef __uint24 uint24_t;
# 411 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef struct __locale_struct * locale_t;
# 25 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\string.h" 2 3


void *memcpy (void *restrict, const void *restrict, size_t);
void *memmove (void *, const void *, size_t);
void *memset (void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void *memchr (const void *, int, size_t);

char *strcpy (char *restrict, const char *restrict);
char *strncpy (char *restrict, const char *restrict, size_t);

char *strcat (char *restrict, const char *restrict);
char *strncat (char *restrict, const char *restrict, size_t);

int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);

int strcoll (const char *, const char *);
size_t strxfrm (char *restrict, const char *restrict, size_t);

char *strchr (const char *, int);
char *strrchr (const char *, int);

size_t strcspn (const char *, const char *);
size_t strspn (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok (char *restrict, const char *restrict);

size_t strlen (const char *);

char *strerror (int);
# 65 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\string.h" 3
char *strtok_r (char *restrict, const char *restrict, char **restrict);
int strerror_r (int, char *, size_t);
char *stpcpy(char *restrict, const char *restrict);
char *stpncpy(char *restrict, const char *restrict, size_t);
size_t strnlen (const char *, size_t);
char *strdup (const char *);
char *strndup (const char *, size_t);
char *strsignal(int);
char *strerror_l (int, locale_t);
int strcoll_l (const char *, const char *, locale_t);
size_t strxfrm_l (char *restrict, const char *restrict, size_t, locale_t);




void *memccpy (void *restrict, const void *restrict, int, size_t);
# 27 "graphics/drivers/sh1106.h" 2

# 1 "graphics/drivers/../../mcc_generated_files/i2c1_master.h" 1
# 54 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdio.h" 1 3
# 24 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdio.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 1 3





typedef void * va_list[1];




typedef void * __isoc_va_list[1];
# 137 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long ssize_t;
# 246 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long off_t;
# 399 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef struct _IO_FILE FILE;
# 24 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdio.h" 2 3
# 52 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdio.h" 3
typedef union _G_fpos64_t {
 char __opaque[16];
 double __align;
} fpos_t;

extern FILE *const stdin;
extern FILE *const stdout;
extern FILE *const stderr;





FILE *fopen(const char *restrict, const char *restrict);
FILE *freopen(const char *restrict, const char *restrict, FILE *restrict);
int fclose(FILE *);

int remove(const char *);
int rename(const char *, const char *);

int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
void clearerr(FILE *);

int fseek(FILE *, long, int);
long ftell(FILE *);
void rewind(FILE *);

int fgetpos(FILE *restrict, fpos_t *restrict);
int fsetpos(FILE *, const fpos_t *);

size_t fread(void *restrict, size_t, size_t, FILE *restrict);
size_t fwrite(const void *restrict, size_t, size_t, FILE *restrict);

int fgetc(FILE *);
int getc(FILE *);
int getchar(void);
int ungetc(int, FILE *);

int fputc(int, FILE *);
int putc(int, FILE *);
int putchar(int);

char *fgets(char *restrict, int, FILE *restrict);

char *gets(char *);


int fputs(const char *restrict, FILE *restrict);
int puts(const char *);

#pragma printf_check(printf) const
#pragma printf_check(vprintf) const
#pragma printf_check(sprintf) const
#pragma printf_check(snprintf) const
#pragma printf_check(vsprintf) const
#pragma printf_check(vsnprintf) const

int printf(const char *restrict, ...);
int fprintf(FILE *restrict, const char *restrict, ...);
int sprintf(char *restrict, const char *restrict, ...);
int snprintf(char *restrict, size_t, const char *restrict, ...);

int vprintf(const char *restrict, __isoc_va_list);
int vfprintf(FILE *restrict, const char *restrict, __isoc_va_list);
int vsprintf(char *restrict, const char *restrict, __isoc_va_list);
int vsnprintf(char *restrict, size_t, const char *restrict, __isoc_va_list);

int scanf(const char *restrict, ...);
int fscanf(FILE *restrict, const char *restrict, ...);
int sscanf(const char *restrict, const char *restrict, ...);
int vscanf(const char *restrict, __isoc_va_list);
int vfscanf(FILE *restrict, const char *restrict, __isoc_va_list);
int vsscanf(const char *restrict, const char *restrict, __isoc_va_list);

void perror(const char *);

int setvbuf(FILE *restrict, char *restrict, int, size_t);
void setbuf(FILE *restrict, char *restrict);

char *tmpnam(char *);
FILE *tmpfile(void);




FILE *fmemopen(void *restrict, size_t, const char *restrict);
FILE *open_memstream(char **, size_t *);
FILE *fdopen(int, const char *);
FILE *popen(const char *, const char *);
int pclose(FILE *);
int fileno(FILE *);
int fseeko(FILE *, off_t, int);
off_t ftello(FILE *);
int dprintf(int, const char *restrict, ...);
int vdprintf(int, const char *restrict, __isoc_va_list);
void flockfile(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);
ssize_t getdelim(char **restrict, size_t *restrict, int, FILE *restrict);
ssize_t getline(char **restrict, size_t *restrict, FILE *restrict);
int renameat(int, const char *, int, const char *);
char *ctermid(char *);







char *tempnam(const char *, const char *);
# 54 "graphics/drivers/../../mcc_generated_files/i2c1_master.h" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdint.h" 1 3
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 127 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 142 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long intptr_t;
# 158 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;
# 173 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long int32_t;





typedef long long int64_t;
# 188 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;
# 209 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 229 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdint.h" 2 3


typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdint.h" 2 3
# 55 "graphics/drivers/../../mcc_generated_files/i2c1_master.h" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.32\\pic\\include\\c99\\stdbool.h" 1 3
# 56 "graphics/drivers/../../mcc_generated_files/i2c1_master.h" 2


typedef enum {
    I2C1_NOERR,
    I2C1_BUSY,
    I2C1_FAIL


} i2c1_error_t;

typedef enum
{
    I2C1_STOP=1,
    I2C1_RESTART_READ,
    I2C1_RESTART_WRITE,
    I2C1_CONTINUE,
    I2C1_RESET_LINK
} i2c1_operations_t;

typedef uint8_t i2c1_address_t;
typedef i2c1_operations_t (*i2c1_callback_t)(void *funPtr);


i2c1_operations_t I2C1_CallbackReturnStop(void *funPtr);
i2c1_operations_t I2C1_CallbackReturnReset(void *funPtr);
i2c1_operations_t I2C1_CallbackRestartWrite(void *funPtr);
i2c1_operations_t I2C1_CallbackRestartRead(void *funPtr);






void I2C1_Initialize(void);
# 101 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
i2c1_error_t I2C1_Open(i2c1_address_t address);
# 111 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
i2c1_error_t I2C1_Close(void);
# 123 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
i2c1_error_t I2C1_MasterOperation(_Bool read);




i2c1_error_t I2C1_MasterWrite(void);




i2c1_error_t I2C1_MasterRead(void);
# 142 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetTimeout(uint8_t timeOut);
# 152 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetBuffer(void *buffer, size_t bufferSize);
# 164 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetDataCompleteCallback(i2c1_callback_t cb, void *ptr);
# 174 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetWriteCollisionCallback(i2c1_callback_t cb, void *ptr);
# 184 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetAddressNackCallback(i2c1_callback_t cb, void *ptr);
# 194 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetDataNackCallback(i2c1_callback_t cb, void *ptr);
# 204 "graphics/drivers/../../mcc_generated_files/i2c1_master.h"
void I2C1_SetTimeoutCallback(i2c1_callback_t cb, void *ptr);
# 28 "graphics/drivers/sh1106.h" 2


# 1 "graphics/drivers/../canvas/fonts.h" 1
# 31 "graphics/drivers/../canvas/fonts.h"
extern const uint8_t font5x7[];
extern const uint8_t font8x16[];
# 30 "graphics/drivers/sh1106.h" 2

# 1 "graphics/drivers/../canvas/bitmaps.h" 1
# 23 "graphics/drivers/../canvas/bitmaps.h"
extern const uint8_t Owl[];
# 31 "graphics/drivers/sh1106.h" 2
# 72 "graphics/drivers/sh1106.h"
typedef enum
{
    SETCONTRAST = 0x81,
    DISPLAYALLON_RESUME = 0xA4,
    DISPLAYALLON = 0xA5,
    NORMALDISPLAY = 0xA6,
    INVERTDISPLAY = 0xA7,

    DISPLAYOFF = 0xAE,
    DISPLAYON = 0xAF,

    SETDISPLAYOFFSET = 0xD3,
    SETCOMPINS = 0xDA,
    SETVCOMDETECT = 0xDB,
    SETDISPLAYCLOCKDIV = 0xD5,
    SETPRECHARGE = 0xD9,
    SETMULTIPLEX = 0xA8,

    SETCOLUMNADDRLOW = 0x00,
    SETCOLUMNADDRHIGH = 0x10,

    SETSTARTLINE = 0x40,
    PAGEADDR = 0xB0,

    COMSCANINC = 0xC0,
    COMSCANDEC = 0xC8,

    SEGREMAP = 0xA0,
    CHARGEPUMP = 0x8D,
    EXTERNALVCC = 0x1,
    SWITCHCAPVCC = 0x2,

    MESSAGETYPE_COMMAND = 0x80,
    MESSAGETYPE_DATA = 0x40,

    RMWSTART = 0xE0,
    RMWEND = 0xEE,

} SH1106_CMD;
# 119 "graphics/drivers/sh1106.h"
void OLED_Initialize(void);
void OLED_Send(uint8_t *buffer, size_t bufSize);
void OLED_Recv(uint8_t *buffer, size_t bufSize);

void OLED_ClearDisplay(void);
void OLED_SetCursor(uint8_t row, uint8_t col);
void OLED_DrawBitmap(void);
void OLED_DrawFont(uint8_t row, uint8_t col, char c, uint8_t *font, _Bool invertBG);
void OLED_DrawString(uint8_t row, uint8_t col, char *str, uint8_t *font, _Bool invertBG);
void OLED_SetPixel(uint8_t row, uint8_t col, uint8_t pixStatus);
# 18 "graphics/drivers/sh1106.c" 2





static const SH1106_CMD OLED_InitCommands[] =
{
    0x00,

    DISPLAYOFF,
    SETDISPLAYCLOCKDIV,
    0x80,
    SETMULTIPLEX,
    0x3F,
    SETDISPLAYOFFSET,
    0x00,

    (SETSTARTLINE | 0x0),
    CHARGEPUMP,
    0x10,
    (SEGREMAP | 0x01),
    COMSCANDEC,
    SETCOMPINS,
    0x12,
    SETCONTRAST,
    0x9F,
    SETPRECHARGE,
    0x22,
    SETVCOMDETECT,
    0x40,
    DISPLAYALLON_RESUME,
    NORMALDISPLAY,
    DISPLAYON,

    (SETCOLUMNADDRLOW | 0x00),
    (SETCOLUMNADDRHIGH | 0x00),
    (SETSTARTLINE | 0x00)
};


static size_t gBufSize = 0;



const uint8_t OLED_BlankDisplay[132] = {0};





static i2c1_operations_t WriteCompleteHandler(void *data);


void OLED_Initialize(void)
{

    I2C1_SetDataCompleteCallback(((void*)0), ((void*)0));
    I2C1_SetAddressNackCallback(((void*)0), ((void*)0));
    I2C1_SetDataNackCallback(((void*)0), ((void*)0));
    I2C1_SetTimeoutCallback(((void*)0), ((void*)0));

    OLED_Send(OLED_InitCommands, sizeof(OLED_InitCommands));
    return;
}

void OLED_Send(uint8_t *buffer, size_t bufSize)
{

    I2C1_Open(0x3C);

    I2C1_SetBuffer(buffer, bufSize);
    I2C1_SetDataCompleteCallback(((void*)0), ((void*)0));
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close());
}

void OLED_Recv(uint8_t *buffer, size_t bufSize)
{

    I2C1_Open(0x3C);


    I2C1_SetBuffer(buffer, bufSize);

    I2C1_MasterRead();
    while(I2C1_BUSY == I2C1_Close());
    return;
}

void OLED_ClearDisplay(void)
{

    for(uint8_t i = 0; i < 8; i++)
    {

        I2C1_Open(0x3C);


        I2C1_SetBuffer((uint8_t []) {0x80, (PAGEADDR | i), 0x80, (SETCOLUMNADDRLOW | 0), 0x80, (SETCOLUMNADDRHIGH | 0), 0x40}, 7);





        I2C1_SetDataCompleteCallback(WriteCompleteHandler, OLED_BlankDisplay);
        gBufSize = sizeof(OLED_BlankDisplay);

        I2C1_MasterWrite();
        while(I2C1_BUSY == I2C1_Close());
    }
}

void OLED_SetCursor(uint8_t row, uint8_t col)
{

    I2C1_Open(0x3C);


    I2C1_SetBuffer((uint8_t[])
        {0x00, (PAGEADDR | (row >> 3)), (SETCOLUMNADDRLOW | ((col + 2) & 0x0F)), (SETCOLUMNADDRHIGH | (col + 2) >> 4)}, 4);




    I2C1_SetDataCompleteCallback(((void*)0), ((void*)0));
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close());
}

void OLED_DrawBitmap(void)
{
    for(uint8_t i = 0; i < 8; i++)
    {

        OLED_SetCursor((i << 3), 0);


        I2C1_Open(0x3C);


        I2C1_SetBuffer((uint8_t []){0x40}, 1);
        I2C1_SetDataCompleteCallback(WriteCompleteHandler, &Owl[128*i]);
        I2C1_MasterWrite();
        while(I2C1_BUSY == I2C1_Close());
    }
}

void OLED_DrawFont(uint8_t row, uint8_t col, char c, uint8_t *font, _Bool invertBG)
{

    uint16_t chIndex = 0;
# 177 "graphics/drivers/sh1106.c"
    uint8_t fontWidth = font[1];
    uint8_t fontHeight = font[2];
    uint8_t firstChar = font[3];


    uint8_t fontBytes = (fontWidth*(((fontHeight - 1) >> 3) + 1));







    switch(c)
    {
        case 0x0A:
        case 0x0D:
            chIndex = 4;
            break;

        default:
        {
            chIndex = 4 + (c - firstChar)*fontBytes;
            break;
        }
    }
# 238 "graphics/drivers/sh1106.c"
    for(uint8_t i = 0; i < fontWidth; i++)
    {
        if(invertBG)
        {
            OLED_Send((uint8_t []){0x40, ~(font[chIndex + i])}, 2);
        }
        else
        {
            OLED_Send((uint8_t []){0x40, font[chIndex + i]}, 2);
        }
    }


    if(invertBG)
    {
        OLED_Send((uint8_t[]){0x40, 0xFF}, 2);
    }
    else
    {
        OLED_Send((uint8_t[]){0x40, 0x00}, 2);
    }
}

void OLED_DrawString(uint8_t row, uint8_t col, char *str, uint8_t *font, _Bool invertBG)
{





    OLED_SetCursor(row, col);

    for(uint8_t i = 0; i < strlen(str); i++)
    {
        OLED_DrawFont(row, col, str[i], font, invertBG);
    }
}

void OLED_SetPixel(uint8_t row, uint8_t col, uint8_t pixStatus)
{

    uint8_t readByte[2] = {0, 0};


    I2C1_Open(0x3C);


    I2C1_SetBuffer((uint8_t[])
        {0x80, (PAGEADDR | (row >> 3)), 0x80, (SETCOLUMNADDRLOW | ((col + 2) & 0x0F)), 0x80, (SETCOLUMNADDRHIGH | (col + 2) >> 4), 0x80, RMWSTART, 0xC0}, 9);




    I2C1_SetDataCompleteCallback(((void*)0), ((void*)0));
    I2C1_MasterWrite();
    while(I2C1_BUSY == I2C1_Close());


    OLED_Recv(readByte, 2);


    if(pixStatus)
    {
        readByte[1] = readByte[1] | (1 << (row & 0x7));
    }
    else
    {
        readByte[1] = readByte[1] & ~(1 << (row & 0x7));
    }


    OLED_Send((uint8_t[]){0x40, readByte[1]}, 2);


    OLED_Send((uint8_t[]){0x00, RMWEND}, 2);

    return;
}


static i2c1_operations_t WriteCompleteHandler(void *data)
{

    I2C1_SetBuffer(data, gBufSize);


    I2C1_SetDataCompleteCallback(((void*)0), ((void*)0));


    return I2C1_CONTINUE;
}
