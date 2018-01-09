/*
 * Copyright (C) 2017 AngeloGioacchino Del Regno <kholk11@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <expat.h>
#include "power.h"

#include <cutils/properties.h>
#include <utils/Log.h>

#define UNUSED __attribute__((unused))

#define LOG_TAG "RQBalance-PowerHAL-XML"

static short xml_depth = 0;
static short parse = -1;
static char* main_node;

struct rqbalance_params trqb;

void parseElm(const char *elm, const char **attr)
{
    int i;

    if (strcmp("cpuquiet", elm) == 0) {
        for (i = 0; attr[i]; i += 2) {
            if (strcmp("min_cpus", attr[i]) == 0)
                strcpy(trqb.min_cpus, attr[i+1]);
            else if (strcmp("max_cpus", attr[i]) == 0)
                strcpy(trqb.max_cpus, attr[i+1]);
        }
    } else if (strcmp("rqbalance", elm) == 0) {
        for (i = 0; attr[i]; i +=2) {
            if (strcmp("balance_level", attr[i]) == 0)
                strcpy(trqb.balance_level, attr[i+1]);
            else if (strcmp("up_thresholds", attr[i]) == 0)
                strcpy(trqb.up_thresholds, attr[i+1]);
            else if (strcmp("down_thresholds", attr[i]) == 0)
                strcpy(trqb.down_thresholds, attr[i+1]);
        }
    }

end:
    return;
}

void startElm(void *data UNUSED, const char *elm, const char **attr)
{
    xml_depth++;

    if (strncmp(main_node, elm, strlen(main_node)) == 0)
        parse = xml_depth;

    if (parse > 0)
        parseElm(elm, attr);
}

void endElm(void *data UNUSED, const char *elm UNUSED)
{
    if ((parse > 0) && (parse == xml_depth))
        parse = -1;

    xml_depth--;
}

void str_handler(void *data, const char *str, int len)
{
    char *buf = malloc(len+1);

    strncpy(buf, str, len);
    buf[len+1] = '\0';

    data = (void*)buf;
}

int parse_xml_data(char* filepath,
            char* node, struct rqbalance_params *therqb)
{
    int ret, fd, count, sz;
    char *buf;
    struct stat st;
    XML_Parser pa;

    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        ALOGE("Cannot open configuration file!!!");
        return -ENOENT;
    }

    pa = XML_ParserCreate(NULL);

    stat(filepath, &st);
    sz = st.st_size;

    /* Security check: do NOT parse too big files */
    if (sz > 51200) {
        ALOGE("File is huge. Preventing parse as a security measure.");
	ret = -E2BIG;
        goto secfail;
    }

    buf = malloc(sizeof(char)*sz);

    memset(buf, 0, sz);
    count = read(fd, buf, (sz - 1));
    if (count < 0) {
        ALOGE("Cannot read configuration file!!!");
        ret = -EIO;
        goto end;
    }

    XML_SetElementHandler(pa, startElm, endElm);
    XML_SetCharacterDataHandler(pa, str_handler);

    main_node = node;

    if (XML_Parse(pa, buf, strlen(buf), XML_TRUE) == XML_STATUS_ERROR) {
        ALOGE("XML Parse error: %s\n", XML_ErrorString(XML_GetErrorCode(pa)));
        return -EINVAL;
    }
    ret = 0;

    /* All ok, copy the struct */
    memcpy(therqb, &trqb, sizeof(struct rqbalance_params));

end:
    free(buf);
secfail:
    close(fd);
    XML_ParserFree(pa);

    return ret;
}
