/* AUTHOR: TIAN_SHENG, DATE: 2022/7/11 */
#include "vmarchcmd.h"
#include <stdio.h>

#define VMARCH_ERROR_OUT_OF_INDEX(cmd, size, outsize) \
    fprintf(stderr, "-ERROR 参数超出命令 %s 固定长度：%d, 参数长度：%d\n", (cmd), (size), (outsize))

/* 设置string值 */
#define VMARCH_SET_VAL(opt, arg, asize, val)                                        \
    if ((asize) > 0) {                                                              \
        if ((asize) <= sizeof((val))) {                                             \
            strcpy((val), (arg));                                                   \
        } else {                                                                    \
            VMARCH_ERROR_OUT_OF_INDEX(opt, sizeof((val)), (asize));                 \
            exit(-1);                                                               \
        }                                                                           \
    }

VMARCHCMD has_cmd(char **argv)
{
    char *ch_cmd = argv[1];

    if (ch_cmd == NULL)
        goto NO_CMD;

    if (strcmp(ch_cmd, "start") == 0) return VMARCHCMD_START;
    if (strcmp(ch_cmd, "stop") == 0) return VMARCHCMD_STOP;
    if (strcmp(ch_cmd, "restart") == 0) return VMARCHCMD_RESTART;
    if (strcmp(ch_cmd, "ps") == 0) return VMARCHCMD_PS;
    if (strcmp(ch_cmd, "exec") == 0) return VMARCHCMD_EXEC;
    if (strcmp(ch_cmd, "pack") == 0) return VMARCHCMD_PACK;
    if (strcmp(ch_cmd, "dump") == 0) return VMARCHCMD_DUMP;

NO_CMD:
    return VMARCHCMD_NULL;
}

void vmarch_make_cmdline(int argc, char **argv, struct vmarch_option_flags *flags)
{
    int         opt;
    VMARCHCMD   cmd;

    if ((cmd = has_cmd(argv)) != VMARCHCMD_NULL)
        printf("-CMD: %d\n", cmd);

    flags->cmd |= cmd;

    while (getopts(argc, argv, VMARCH_OPTIONS, ARRAY_SIZE(VMARCH_OPTIONS), &opt) != -1) {
        size_t argsize = optarg != NULL ? strlen(optarg) : 0;

        switch (opt) {
            case OPT_NSD:
                VMARCH_SET_VAL(optopt, optarg, argsize, flags->nsd);
                break;
            case OPT_CP:
                VMARCH_SET_VAL(optopt, optarg, argsize, flags->cp);
                break;
            case OPT_PORT:
                flags->port = atoi(optarg);
                break;
            case OPT_DEBUG_PORT:
                flags->dp = atoi(optarg);
                break;
            case OPT_MONITOR:
                flags->mon = TRUE;
                break;
        }
    }
}