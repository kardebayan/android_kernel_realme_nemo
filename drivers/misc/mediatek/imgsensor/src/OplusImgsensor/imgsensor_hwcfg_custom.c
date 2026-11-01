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
#include "imgsensor_hwcfg_custom.h"

struct IMGSENSOR_SENSOR_LIST *Oplusimgsensor_Sensorlist(void)
{
    pr_info("oplus_gimgsensor_sensor_list Selected\n");

    return oplus_gimgsensor_sensor_list;
}

struct IMGSENSOR_HW_CFG *Oplusimgsensor_Custom_Config(void)
{
    pr_info("oplus_imgsensor_custom_config Selected\n");

    return oplus_imgsensor_custom_config;
}

struct IMGSENSOR_HW_POWER_SEQ *Oplusimgsensor_matchhwcfg_power(
        enum  IMGSENSOR_POWER_ACTION_INDEX  pwr_actidx)
{
    struct IMGSENSOR_HW_POWER_SEQ *ppwr_seq = NULL;
    pr_debug("[%s] pwr_actidx:%d\n", __func__, pwr_actidx);
    if ((pwr_actidx != IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX)
        && (pwr_actidx != IMGSENSOR_POWER_MATCHSENSOR_HWCFG_INDEX)) {
        return NULL;
    }

    if (pwr_actidx == IMGSENSOR_POWER_MATCHSENSOR_HWCFG_INDEX) {
        ppwr_seq = oplus_sensor_power_sequence;
        pr_info("[%s] match oplus_sensor_power_sequence\n", __func__);
    } else if (pwr_actidx == IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX){
            if (is_project(20682) || is_project(19661)) {
                ppwr_seq = oplus_platform_power_sequence;
                pr_info("[%s] match 20682 19661 IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX \n", __func__);
            }
    } else {
        pr_info("[%s] NOT Support MIPISWITCH\n", __func__);
    }

    return ppwr_seq;
}

void Oplusimgsensor_Registdeviceinfo(char *name, char *version, kal_uint8 module_id)
{
    char *manufacture;
    if (name == NULL || version == NULL)
    {
        pr_info("name or version is NULL");
        return;
    }
    switch (module_id)
    {
        case IMGSENSOR_MODULE_ID_SUNNY:  /* Sunny */
            manufacture = DEVICE_MANUFACUTRE_SUNNY;
            break;
        case IMGSENSOR_MODULE_ID_TRULY:  /* Truly */
            manufacture = DEVICE_MANUFACUTRE_TRULY;
            break;
        case IMGSENSOR_MODULE_ID_SEMCO:  /* Semco */
            manufacture = DEVICE_MANUFACUTRE_SEMCO;
            break;
        case IMGSENSOR_MODULE_ID_LITEON:  /* Lite-ON */
            manufacture = DEVICE_MANUFACUTRE_LITEON;
            break;
        case IMGSENSOR_MODULE_ID_QTECH:  /* Q-Tech */
            manufacture = DEVICE_MANUFACUTRE_QTECH;
            break;
        case IMGSENSOR_MODULE_ID_OFILM:  /* O-Film */
            manufacture = DEVICE_MANUFACUTRE_OFILM;
            break;
        case IMGSENSOR_MODULE_ID_SHINE:  /* Shine */
            manufacture = DEVICE_MANUFACUTRE_SHINE;
            break;
        case IMGSENSOR_MODULE_ID_HOLITECH:  /* Holitech */
            manufacture = DEVICE_MANUFACUTRE_HOLITECH;
            break;
        default:
            manufacture = DEVICE_MANUFACUTRE_NA;
    }
    register_device_proc(name, version, manufacture);
}
