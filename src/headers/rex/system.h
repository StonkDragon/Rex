#ifndef REX_SYSTEM_H
#define REX_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../memoryengine.h"
#include "../register.h"
#include "../error.h"
#include "../cstring.h"
#include "../opcodes.h"
#include "../rexcall.h"
#include "../cutil.h"

int execSyscall(uint64_t size, FILE** openFiles, int* filePouinter) {
    switch (re_get(0xF))
    {
    case SC_EXIT:
        #ifdef DEBUG
        printf("\n");
        printf("Debug: ");
        me_heapDump("memory.dump");
        printf("Exited with exit code %llu (0x%08llx)\n", re_get(0), re_get(0));
        #endif
        {
            int ret = re_get(0);
            return ret;
        }

    case SC_READ:
    {
        int fd = re_get(0);
        uint64_t addr = re_get(1);
        if (addr > HEAP_MAX || addr < 0)
        {
            heap_error("Attempted to write outside of memory space (Address: 0x%02llx)\n", addr);
            me_heapDump("memory.dump");
        }
        if (addr <= size)
        {
            heap_error("Attempted to write to reserved memory space (Address: 0x%02llx)\n", addr);
            me_heapDump("memory.dump");
        }

        size_t len;
        string buffer = NULL;

        getline(&buffer, &len, openFiles[fd]);
        if (len > 0) {
            buffer[len - 1] = '\0';
        }

        me_writeString(addr, buffer);
    }
    break;

    case SC_WRITE:
    {
        int fd = re_get(0);
        uint64_t addr = re_get(1);

        char c;
        int i = 0;

        while ((c = me_readByte(addr + i)) != 0)
        {
            if (c == '\\')
            {
                i++;
                c = me_readByte(addr + i);
                if (c == 'n')
                {
                    fprintf(openFiles[fd], "\n");
                }
                else if (c == 't')
                {
                    fprintf(openFiles[fd], "\t");
                }
                else if (c == 'r')
                {
                    fprintf(openFiles[fd], "\r");
                }
                else if (c == '\\')
                {
                    fprintf(openFiles[fd], "\\");
                }
                else
                {
                    fprintf(openFiles[fd], "%c", c);
                }
            }
            else
            {
                fprintf(openFiles[fd], "%c", c);
            }
            i++;
        }
        break;
    }
    break;

    case SC_SLEEP:
        sleep_ms(re_get(0));
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
        openFiles[*filePouinter++] = f;
        freeOrErr(str);
        me_push(*filePouinter - 1);
    }
    break;

    case SC_CLOSE:
        fclose(openFiles[re_get(0)]);
        break;

    case SC_EXEC:
    {
        string str = me_readString(re_get(0));
        int r = system(str);
        freeOrErr(str);
        if (r < 0)
        {
            system_error("Failed to execute program\n");
        }
        me_push(r);
    }
    break;

    case SC_STORE:
    {
        uint64_t addr = re_get(0);
        uint64_t val = re_get(1);
        string str = (string) mallocOrErr(MAX_STRING_LENGTH);
        sprintf(str, "%llu", val);
        me_writeString(addr, str);
    }
    break;
    }
    return -1;
}

#endif
