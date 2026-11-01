subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/sensorHub/inc_v1
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/oppo_virtual_sensor
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/oppo_virtual_sensor/platform/sensor_cmd
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/oppo_virtual_sensor/platform/sensor_device
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/oppo_virtual_sensor/platform/sensor_event

obj-y += platform/
obj-y += virtual_sensor.o
obj-$(CONFIG_OPLUS_FEATURE_CAMERA_PROTECT)	+= camera_protecthub/
obj-$(CONFIG_OPLUS_FEATURE_FREE_FALL)		+= free_fallhub/
obj-$(CONFIG_OPLUS_FEATURE_PICKUP_DETECT)	+= pickup_detecthub/
obj-$(CONFIG_OPLUS_FEATURE_FP_DISPLAY)		+= fp_displayhub/
obj-$(CONFIG_OPLUS_FEATURE_LUX_AOD)		+= lux_aodhub/
obj-$(CONFIG_OPLUS_FEATURE_PEDO_MINUTE)		+= pedo_minutehub/
obj-$(CONFIG_OPLUS_FEATURE_TP_GESTURE)		+= tp_gesturehub/
