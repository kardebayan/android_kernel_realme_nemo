/*
 * Copyright (C) 2015 MediaTek Inc.
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

#define pr_fmt(fmt) "["KBUILD_MODNAME"] " fmt
#include <linux/atomic.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <mt-plat/mtk_boot_common.h>

enum {
	BM_UNINIT = 0,
	BM_INITIALIZING = 1,
	BM_INITIALIZED = 2,
} BM_INIT_STATE;

enum boot_mode_t g_boot_mode = UNKNOWN_BOOT;
static int g_boot_type = 0xFF;
static atomic_t g_boot_init = ATOMIC_INIT(BM_UNINIT);
static atomic_t g_boot_errcnt = ATOMIC_INIT(0);
static atomic_t g_boot_status = ATOMIC_INIT(0);

#ifdef CONFIG_OF
struct tag_bootmode {
	u32 size;
	u32 tag;
	u32 bootmode;
	u32 boottype;
};
static int __init dt_get_boot_common(unsigned long node, const char *uname,
	int depth, void *data)
{
	struct tag_bootmode *tags = NULL;

	if (depth != 1 || (strcmp(uname, "chosen") != 0 &&
		strcmp(uname, "chosen@0") != 0))
		return 0;

	tags = (struct tag_bootmode *)of_get_flat_dt_prop(node,
		"atag,boot", NULL);

	if (tags) {
		g_boot_mode = tags->bootmode;
		g_boot_type = tags->boottype;
		/* Add default value if no lk pass storage type */
		if ((g_boot_type > 2) || (g_boot_type < 0))
			g_boot_type = BOOTDEV_SDMMC;
		atomic_set(&g_boot_status, 1);
	} else {
		pr_warn("'atag,boot' is not found\n");
	}

	/* break now */
	return 1;
}
#endif


static void __init init_boot_common(unsigned int line)
{
#ifdef CONFIG_OF
	int rc;

	if (atomic_read(&g_boot_init) == BM_INITIALIZING) {
		pr_notice("%s (%d) state(%d,%d)\n", __func__, line,
			atomic_read(&g_boot_init), g_boot_mode);
		atomic_inc(&g_boot_errcnt);
		return;
	}

	if (atomic_read(&g_boot_init) == BM_UNINIT)
		atomic_set(&g_boot_init, BM_INITIALIZING);
	else
		return;

	if (g_boot_mode != UNKNOWN_BOOT) {
		atomic_set(&g_boot_init, BM_INITIALIZED);
		pr_notice("%s (%d) boot_mode = %d\n",
			__func__, line, g_boot_mode);
		return;
	}

	pr_debug("%s %d %d %d\n", __func__, line, g_boot_mode,
		atomic_read(&g_boot_init));
	rc = of_scan_flat_dt(dt_get_boot_common, NULL);
	if (rc != 0)
		atomic_set(&g_boot_init, BM_INITIALIZED);
	else
		pr_warn("fail, of_scan_flat_dt() = %d", rc);
	pr_debug("%s %d %d %d\n", __func__, line, g_boot_mode,
		atomic_read(&g_boot_init));
#endif
}

/* return boot mode */
enum boot_mode_t get_boot_mode(void)
{
	if (atomic_read(&g_boot_init) != BM_INITIALIZED) {
		pr_warn("fail, %s (%d) state(%d,%d)\n", __func__, __LINE__,
			atomic_read(&g_boot_init), g_boot_mode);
	}
	return g_boot_mode;
}
EXPORT_SYMBOL(get_boot_mode);

unsigned int get_boot_type(void)
{
	if (atomic_read(&g_boot_init) != BM_INITIALIZED) {
		pr_debug("fail, %s (%d) state(%d,%d)\n", __func__, __LINE__,
			atomic_read(&g_boot_init), g_boot_mode);
	}
	return g_boot_type;
}
EXPORT_SYMBOL(get_boot_type);

/* for convenience, simply check is meta mode or not */
bool is_meta_mode(void)
{
	if (atomic_read(&g_boot_init) != BM_INITIALIZED) {
		pr_warn("fail, %s (%d) state(%d,%d)\n", __func__, __LINE__,
			atomic_read(&g_boot_init), g_boot_mode);
	}

	if (g_boot_mode == META_BOOT)
		return true;
	else
		return false;
}
EXPORT_SYMBOL(is_meta_mode);

bool is_advanced_meta_mode(void)
{
	if (atomic_read(&g_boot_init) != BM_INITIALIZED) {
		pr_warn("fail, %s (%d) state(%d,%d)\n", __func__, __LINE__,
			atomic_read(&g_boot_init), g_boot_mode);
	}

	if (g_boot_mode == ADVMETA_BOOT)
		return true;
	else
		return false;
}
EXPORT_SYMBOL(is_advanced_meta_mode);

static int __init boot_common_core(void)
{
	init_boot_common(__LINE__);
	return 0;
}

pure_initcall(boot_common_core);
MODULE_DESCRIPTION("MTK Boot Information Common Driver");
MODULE_LICENSE("GPL");
