#ifndef REX_SYSTEM_H
#define REX_SYSTEM_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "../memoryengine.h"
#include "../register.h"
#include "../error.h"
#include "../cstring.h"
#include "../opcodes.h"
#include "../rexcall.h"

int execSyscall(uint32_t size, FILE** openFiles, int* filePointer) {
    switch (re_get(0xF))
    {
    case SC_EXIT:
        #ifdef DEBUG
        printf("\n");
        printf("Debug: ");
        me_heapDump("memory.dump");
        printf("Exited with exit code %d (0x%08x)\n", re_get(0), re_get(0));
        #endif
        {
            int ret = re_get(0);
            return ret;
        }

    case SC_READ_CON:
    {
        addr = re_get(0);
        int len = re_get(1);
        if (addr > HEAP_MAX || addr < 0)
        {
            heap_error("Attempted to write outside of memory space (Address: %02x)\n", addr);
            me_heapDump("memory.dump");
        }
        if (addr <= size)
        {
            heap_error("Attempted to write to reserved memory space (Address: %02x)\n", addr);
            me_heapDump("memory.dump");
        }

        for (int i = 0; i < len; i++)
        {
            me_writeByte(addr + i, getchar());
        }
    }
    break;

    case SC_WRITE_CON:
    {
        string str = me_readString(re_get(0));
        for (size_t i = 0; i < strlen(str); i++)
        {
            if (str[i] == 0)
                break;
            if (str[i] == '\\')
            {
                i++;
                switch (str[i])
                {
                case 'n':
                    printf("\n");
                    break;
                case 't':
                    printf("\t");
                    break;
                case 'r':
                    printf("\r");
                    break;
                case '\\':
                    printf("\\");
                    break;
                default:
                    printf("%c", str[i]);
                    break;
                }
            }
            else
            {
                printf("%c", str[i]);
            }
        }
        free(str);
    }
    break;

    case SC_SLEEP:
        sleep(re_get(0));
        break;

    case SC_OPEN:
    {
        string str = me_readString(re_get(0));
        int mode = re_get(1);
        FILE *f = fopen(str, mode == 0 ? "r" : "w");
        if (f == NULL)
        {
            system_error("IO: Could not open file \"%s\"\n", str);
        }
        openFiles[*filePointer++] = f;
        free(str);
        me_push(*filePointer - 1);
    }
    break;

    case SC_CLOSE:
        fclose(openFiles[re_get(0)]);
        break;

    case SC_READ:
    {
        string str = NULL;
        int fd = re_get(0);
        int len = re_get(1);
        addr = re_get(2);
        string buf = (string)malloc(len);

        read(fileno(openFiles[fd]), buf, len);
        for (int i = 0; i < len; i++)
        {
            me_writeByte(addr + i, buf[i]);
        }
        free(buf);
        free(str);
    }
    break;

    case SC_WRITE:
    {
        int fd = re_get(0);
        int len = re_get(1);
        addr = re_get(2);

        string buf = (string)malloc(len);
        for (int i = 0; i < len; i++)
        {
            buf[i] = me_readByte(addr + i);
        }

        for (size_t i = 0; i < strlen(buf); i++)
        {
            if (buf[i] == '\0')
                break;
            if (buf[i] == '\\')
            {
                i++;
                if (buf[i] == 'n')
                {
                    fprintf(openFiles[fd], "\n");
                }
                else if (buf[i] == 't')
                {
                    fprintf(openFiles[fd], "\t");
                }
                else if (buf[i] == 'r')
                {
                    fprintf(openFiles[fd], "\r");
                }
                else if (buf[i] == '\\')
                {
                    fprintf(openFiles[fd], "\\");
                }
                else
                {
                    fprintf(openFiles[fd], "%c", buf[i]);
                }
            }
            else
            {
                fprintf(openFiles[fd], "%c", buf[i]);
            }
        }
        free(buf);
        break;
    }
    break;

    case SC_EXEC:
    {
        string str = me_readString(re_get(0));
        int r = system(str);
        free(str);
        if (r < 0)
        {
            system_error("Failed to execute program\n");
        }
        me_push(r);
    }
    break;
    }
    return -1;
}

#endif
