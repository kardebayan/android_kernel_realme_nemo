/*
 * Copyright (C) 2017 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef __IMGSENSOR_HWCFG_CUSTOM_CTRL_H__
#define __IMGSENSOR_HWCFG_CUSTOM_CTRL_H__

#include "imgsensor.h"
#include "imgsensor_sensor_list.h"
#include "kd_imgsensor.h"
#include "kd_imgsensor_errcode.h"
#include "kd_camera_typedef.h"
#include <soc/oplus/system/oplus_project.h>
#include <soc/oppo/device_info.h>

#define DEVICE_MANUFACUTRE_NA           "None"
#define DEVICE_MANUFACUTRE_SUNNY        "Sunny"
#define DEVICE_MANUFACUTRE_TRULY        "Truly"
#define DEVICE_MANUFACUTRE_SEMCO        "Semco"
#define DEVICE_MANUFACUTRE_LITEON       "Liteon"
#define DEVICE_MANUFACUTRE_QTECH        "Qtech"
#define DEVICE_MANUFACUTRE_OFILM        "Ofilm"
#define DEVICE_MANUFACUTRE_SHINE        "Shine"
#define DEVICE_MANUFACUTRE_HOLITECH     "Holitech"

#define IMGSENSOR_MODULE_ID_SUNNY       0x01
#define IMGSENSOR_MODULE_ID_TRULY       0x02
#define IMGSENSOR_MODULE_ID_SEMCO       0x03
#define IMGSENSOR_MODULE_ID_LITEON      0x04
#define IMGSENSOR_MODULE_ID_QTECH       0x05
#define IMGSENSOR_MODULE_ID_OFILM       0x06
#define IMGSENSOR_MODULE_ID_SHINE       0x07
#define IMGSENSOR_MODULE_ID_HOLITECH    0x09

enum IMGSENSOR_POWER_ACTION_INDEX {
	IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX = 81,
	IMGSENSOR_POWER_MATCHSENSOR_HWCFG_INDEX,
};

struct IMGSENSOR_HW_POWER_SEQ *Oplusimgsensor_matchhwcfg_power(
		enum IMGSENSOR_POWER_ACTION_INDEX pwr_actidx);
struct IMGSENSOR_SENSOR_LIST *Oplusimgsensor_Sensorlist(void);
struct IMGSENSOR_HW_CFG *Oplusimgsensor_Custom_Config(void);
void Oplusimgsensor_Registdeviceinfo(char *name, char *version,
		kal_uint8 module_id);

extern struct IMGSENSOR_HW_POWER_SEQ *oplus_sensor_power_sequence;
extern struct IMGSENSOR_HW_POWER_SEQ *oplus_platform_power_sequence;
extern struct IMGSENSOR_HW_CFG *oplus_imgsensor_custom_config;
extern struct IMGSENSOR_SENSOR_LIST *oplus_gimgsensor_sensor_list;
void oplus_imgsensor_hwcfg(void);

/* sensor drivers in the per-project lists */
UINT32 IMX682_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 S5KGM1SP_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 SALAA_QTECH_MAIN_S5KGM1SP_SensorInit(
		struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 SARTER_QTECH_MAIN_S5KGM1SP_SensorInit(
		struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 IMX471_MIPI_RAW1_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 OV8856_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 SALA_WIDE_OV8856_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 OV02B10_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 OV02B1B_MIPI_MONO_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 S5KGW1_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 S5K3P9SP_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 GC2375H_MIPI_RAW_19661_SensorInit(
		struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 GC02M0_MIPI_MONO_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);
UINT32 GC02K0_MIPI_RAW_SensorInit(struct SENSOR_FUNCTION_STRUCT **pfFunc);

#endif
