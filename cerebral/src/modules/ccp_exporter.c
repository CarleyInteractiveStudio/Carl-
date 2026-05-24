#include "ccp_exporter.h"
#include <stdlib.h>
#include <string.h>

CCPExporter* ccp_exporter_open(const char *filename) {
    CCPExporter *exporter = malloc(sizeof(CCPExporter));
    if (!exporter) return NULL;

    exporter->file = fopen(filename, "w");
    if (!exporter->file) {
        free(exporter);
        return NULL;
    }

    exporter->frame_count = 0;
    // CCP Header
    fprintf(exporter->file, "{\"format\": \"CCP\", \"version\": \"1.0\", \"frames\": [\n");
    return exporter;
}

void ccp_exporter_record_frame(CCPExporter *exporter, CerebralNetwork *net, const char *inner_monologue) {
    if (!exporter || !net) return;

    if (exporter->frame_count > 0) {
        fprintf(exporter->file, ",\n");
    }

    fprintf(exporter->file, "  {\n");
    fprintf(exporter->file, "    \"timestamp\": %.2f,\n", net->current_time);
    fprintf(exporter->file, "    \"inner_monologue\": \"%s\",\n", inner_monologue ? inner_monologue : "");
    fprintf(exporter->file, "    \"levels\": {\n");
    fprintf(exporter->file, "      \"dopamine\": %.3f,\n", net->global_dopamine);
    fprintf(exporter->file, "      \"noradrenaline\": %.3f,\n", net->global_noradrenaline);
    fprintf(exporter->file, "      \"serotonin\": %.3f,\n", net->global_serotonin);
    fprintf(exporter->file, "      \"fear\": %.3f,\n", net->fear_level);
    fprintf(exporter->file, "      \"pain\": %.3f,\n", net->pain_level);
    fprintf(exporter->file, "      \"hunger\": %.3f,\n", net->hunger_level);
    fprintf(exporter->file, "      \"curiosity\": %.3f\n", net->curiosity_drive);
    fprintf(exporter->file, "    },\n");

    // Record only spiked neurons to save space
    fprintf(exporter->file, "    \"active_neurons\": [");
    bool first = true;
    for (uint32_t i = 0; i < net->total_neurons; i++) {
        if (net->neurons[i].has_spiked) {
            if (!first) fprintf(exporter->file, ", ");
            fprintf(exporter->file, "%u", i);
            first = false;
        }
    }
    fprintf(exporter->file, "]\n");
    fprintf(exporter->file, "  }");

    exporter->frame_count++;
    fflush(exporter->file);
}

void ccp_exporter_close(CCPExporter *exporter) {
    if (!exporter) return;
    fprintf(exporter->file, "\n]}\n");
    fclose(exporter->file);
    free(exporter);
}
