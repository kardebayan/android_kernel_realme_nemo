#ifndef _SENSOR_CMD_H
#define _SENSOR_CMD_H

#include <SCP_sensorHub.h>
#include <hwmsensor.h>

//cmd for kernel to scp
int oplus_enable_to_hub(uint8_t handle, int enabledisable);
int oplus_set_delay_to_hub(uint8_t handle, unsigned int delayms);
int oplus_batch_to_hub(uint8_t handle,
    int flag, int64_t samplingPeriodNs, int64_t maxBatchReportLatencyNs);
int oplus_flush_to_hub(uint8_t handle);

#endif
