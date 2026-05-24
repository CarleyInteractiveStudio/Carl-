#ifndef CCP_EXPORTER_H
#define CCP_EXPORTER_H

#include "cerebral.h"
#include <stdio.h>

typedef struct {
    FILE *file;
    uint32_t frame_count;
} CCPExporter;

CCPExporter* ccp_exporter_open(const char *filename);
void ccp_exporter_record_frame(CCPExporter *exporter, CerebralNetwork *net, const char *inner_monologue);
void ccp_exporter_close(CCPExporter *exporter);

#endif
