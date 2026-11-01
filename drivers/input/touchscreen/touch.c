/***************************************************
 * File:touch.c
 * VENDOR_EDIT
 * Copyright (c)  2008- 2030  Oppo Mobile communication Corp.ltd.
 * Description:
 *             tp dev
 * Version:1.0:
 * Date created:2016/09/02
 * Author: hao.wang@Bsp.Driver
 * TAG: BSP.TP.Init
*/

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/serio.h>
#include "oppo_touchscreen/tp_devices.h"
#include "oppo_touchscreen/touchpanel_common.h"
#include <soc/oplus/system/oplus_project.h>
#include <soc/oppo/device_info.h>
#include "touch.h"

#define MAX_LIMIT_DATA_LENGTH         100
extern char *saved_command_line;
int g_tp_dev_vendor = TP_UNKNOWN;
int g_tp_prj_id = 0;
struct hw_resource *g_hw_res;
static bool is_tp_type_got_in_match = false;    /*indicate whether the tp type is specified in the dts*/
/*if can not compile success, please update vendor/oppo_touchsreen*/
struct tp_dev_name tp_dev_names[] = {
	{TP_OFILM, "OFILM"},
	{TP_BIEL, "BIEL"},
	{TP_TRULY, "TRULY"},
	{TP_BOE, "BOE"},
	{TP_G2Y, "G2Y"},
	{TP_TPK, "TPK"},
	{TP_JDI, "JDI"},
	{TP_TIANMA, "TIANMA"},
	{TP_SAMSUNG, "SAMSUNG"},
	{TP_DSJM, "DSJM"},
	{TP_BOE_B8, "BOEB8"},
	{TP_INNOLUX, "INNOLUX"},
	{TP_HIMAX_DPT, "DPT"},
	{TP_AUO, "AUO"},
	{TP_DEPUTE, "DEPUTE"},
	{TP_HUAXING, "HUAXING"},
	{TP_HLT, "HLT"},
	{TP_DJN, "DJN"},
	{TP_TXD, "TXD"},
	{TP_UNKNOWN, "UNKNOWN"},
};

typedef enum {
	TP_INDEX_NULL,
	nt36672c,
} TP_USED_INDEX;
TP_USED_INDEX tp_used_index  = TP_INDEX_NULL;

#define GET_TP_DEV_NAME(tp_type) ((tp_dev_names[tp_type].type == (tp_type))?tp_dev_names[tp_type].name:"UNMATCH")

bool __init tp_judge_ic_match(char *tp_ic_name)
{
	pr_err("[TP] tp_ic_name = %s \n", tp_ic_name);
	return true;
}

bool  tp_judge_ic_match_commandline(struct panel_info *panel_data)
{
	int prj_id = 0;
	int i = 0;
	bool ic_matched = false;
	prj_id = get_project();
	pr_err("[TP] get_project() = %d \n", prj_id);
	pr_err("[TP] boot_command_line = %s \n", saved_command_line);

	for (i = 0; i < panel_data->project_num; i++) {
		if (prj_id == panel_data->platform_support_project[i]) {
			g_tp_prj_id = panel_data->platform_support_project_dir[i];

			if (strstr(saved_command_line, panel_data->platform_support_commandline[i])
					|| strstr("default_commandline", panel_data->platform_support_commandline[i])) {
				pr_err("[TP] Driver match the project\n");
				ic_matched = true;
			}
		}
	}

	if (!ic_matched) {
		pr_err("[TP] Driver does not match the project\n");
		pr_err("Lcd module not found\n");
		return false;
	}

	switch (prj_id) {
	case 19661:
	case 19662:
	case 19663:
	case 19665:
	case 19666:
	case 19667:
	case 19668:
		pr_info("[TP] case 19661\n");
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "nt36672c_tianma")) {
			pr_err("[TP] touch ic = nt36672c_tianma \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_TIANMA;
		}

		if (strstr(saved_command_line, "nt36672c_jdi")) {
			pr_err("[TP] touch ic = nt36672c_jdi \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_JDI;
		}
		break;
	case 20682:
		pr_info("[TP] case 20682\n");
		is_tp_type_got_in_match = true;
		tp_used_index = nt36672c;

		if (strstr(saved_command_line, "nt36672c_tianma")) {
			g_tp_dev_vendor = TP_TIANMA;

		} else if (strstr(saved_command_line, "nt36672c_jdi")) {
			g_tp_dev_vendor = TP_JDI;

		} else if (strstr(saved_command_line, "nt36672c_boe")) {
			g_tp_dev_vendor = TP_BOE;
		} else {
			g_tp_dev_vendor = TP_UNKNOWN;
		}

		pr_err("[TP] g_tp_dev_vendor: %s\n", tp_dev_names[g_tp_dev_vendor].name);
		break;
	default:
		pr_info("other project, no need process here!\n");
		break;
	}

	pr_info("[TP]ic:%d, vendor:%d\n", tp_used_index, g_tp_dev_vendor);
	return true;
}

static void tp_util_set_nt36672c_paths(struct panel_info *panel_data,
				       int dir, const char *vendor)
{
	snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
		 "tp/%d/FW_%s_%s.bin", dir, "NT36672C_NF", vendor);

	if (panel_data->test_limit_name) {
		snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
			 "tp/%d/LIMIT_%s_%s.img", dir, "NT36672C_NF", vendor);
	}

	if (panel_data->extra) {
		snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
			 "tp/%d/BOOT_FW_%s_%s.ihex", dir, "NT36672C_NF", vendor);
	}

	panel_data->manufacture_info.fw_path = panel_data->fw_name;
}

int tp_util_get_vendor(struct hw_resource *hw_res,
		       struct panel_info *panel_data)
{
	char *vendor;
	int prj_id = 0;
	prj_id = get_project();
	g_hw_res = hw_res;

	panel_data->test_limit_name = kzalloc(MAX_LIMIT_DATA_LENGTH, GFP_KERNEL);

	if (panel_data->test_limit_name == NULL) {
		pr_err("[TP]panel_data.test_limit_name kzalloc error\n");
	}

	panel_data->extra = kzalloc(MAX_LIMIT_DATA_LENGTH, GFP_KERNEL);

	if (panel_data->extra == NULL) {
		pr_err("[TP]panel_data.extra kzalloc error\n");
	}

	if (is_tp_type_got_in_match) {
		panel_data->tp_type = g_tp_dev_vendor;
	}

	if (panel_data->tp_type == TP_UNKNOWN) {
		pr_err("[TP]%s type is unknown\n", __func__);
		return 0;
	}

	vendor = GET_TP_DEV_NAME(panel_data->tp_type);

	if (strstr(saved_command_line, "nt36672c_boe_sala")) {
		hw_res->TX_NUM = 18;
		hw_res->RX_NUM = 36;
	}

	strcpy(panel_data->manufacture_info.manufacture, vendor);

	switch (prj_id) {
	case 19661:
	case 19662:
	case 19663:
	case 19665:
	case 19666:
	case 19667:
	case 19668:
		pr_info("[TP] enter case OPPO_19661\n");

		if (tp_used_index != nt36672c)
			break;

		tp_util_set_nt36672c_paths(panel_data, 19661, vendor);

		if (g_tp_dev_vendor == TP_JDI) {
			pr_info("[TP]: firmware_headfile = FW_19661_NT36672C_JDI\n");
			memcpy(panel_data->manufacture_info.version, "NT72C_JDI", 9);
			panel_data->firmware_headfile.firmware_data = FW_19661_NT36672C_JDI;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_19661_NT36672C_JDI);
		} else if (g_tp_dev_vendor == TP_TIANMA) {
			pr_info("[TP]: firmware_headfile = FW_19661_NT36672C_TIANMA\n");
			memcpy(panel_data->manufacture_info.version, "NT72C_TM_", 9);
			panel_data->firmware_headfile.firmware_data = FW_19661_NT36672C_TIANMA;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_19661_NT36672C_TIANMA);
		}
		break;
	case 20682:
		pr_info("[TP] enter case 20682\n");

		if (tp_used_index != nt36672c)
			break;

		if (g_tp_dev_vendor == TP_TIANMA) {
			pr_info("[TP] 20682 tp vendor tianma\n");
			tp_util_set_nt36672c_paths(panel_data, 20682, vendor);
			memcpy(panel_data->manufacture_info.version, "NT72C_TM_", 9);
			panel_data->firmware_headfile.firmware_data = FW_20682_NT36672C_TIANMA;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_20682_NT36672C_TIANMA);
		} else if (g_tp_dev_vendor == TP_JDI) {
			pr_info("[TP] 20682 tp vendor jdi\n");
			tp_util_set_nt36672c_paths(panel_data, 20682, vendor);
			memcpy(panel_data->manufacture_info.version, "NT72C_JDI", 9);
			panel_data->firmware_headfile.firmware_data = FW_20682_NT36672C_JDI;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_20682_NT36672C_JDI);
		} else if (g_tp_dev_vendor == TP_BOE) {
			pr_info("[TP] 20682 tp vendor boe\n");
			tp_util_set_nt36672c_paths(panel_data, 20682, vendor);
			memcpy(panel_data->manufacture_info.version, "NT72C_BOE", 9);
			panel_data->firmware_headfile.firmware_data = FW_20682_NT36672C_BOE;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_20682_NT36672C_BOE);
		} else {
			pr_info("[TP] 20682 tp ic not found\n");
		}
		break;
	default:
		snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
			 "tp/%d/FW_%s_%s.img",
			 g_tp_prj_id, panel_data->chip_name, vendor);

		if (panel_data->test_limit_name) {
			snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				 "tp/%d/LIMIT_%s_%s.img",
				 g_tp_prj_id, panel_data->chip_name, vendor);
		}

		if (panel_data->extra) {
			snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
				 "tp/%d/BOOT_FW_%s_%s.ihex",
				 prj_id, panel_data->chip_name, vendor);
		}

		panel_data->manufacture_info.fw_path = panel_data->fw_name;
		break;
	}

	pr_info("[TP]vendor:%s fw:%s limit:%s\n",
		vendor,
		panel_data->fw_name,
		panel_data->test_limit_name == NULL ? "NO Limit" : panel_data->test_limit_name);

	return 0;
}

/**
 * Description:
 * pulldown spi7 cs to avoid current leakage
 * because of current sourcing from cs (pullup state) flowing into display module
 **/
void switch_spi7cs_state(bool normal)
{
    if(normal){
        if( !IS_ERR_OR_NULL(g_hw_res->pin_set_high) ) {
            pr_info("%s: going to set spi7 cs to spi mode .\n", __func__);
            pinctrl_select_state(g_hw_res->pinctrl, g_hw_res->pin_set_high);
        }else{
            pr_info("%s: cannot to set spi7 cs to spi mode .\n", __func__);
        }
    } else {
        if( !IS_ERR_OR_NULL(g_hw_res->pin_set_low) ) {
            pr_info("%s: going to set spi7 cs to pulldown .\n", __func__);
            pinctrl_select_state(g_hw_res->pinctrl, g_hw_res->pin_set_low);
        }else{
            pr_info("%s: cannot to set spi7 cs to pulldown .\n", __func__);
        }
    }
}
EXPORT_SYMBOL(switch_spi7cs_state);
