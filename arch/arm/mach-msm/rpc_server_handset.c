/* arch/arm/mach-msm/rpc_server_handset.c
 *
 * Copyright (c) 2008-2010,2012 Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/switch.h>
#include <linux/sec_jack.h>

#include <asm/mach-types.h>

#include <mach/msm_rpcrouter.h>
#include <mach/board.h>
#include <mach/rpc_server_handset.h>

#define DRIVER_NAME	"msm-handset"
#define FEATURE_HEADSET_AP_DETECT
#undef HS_DEBUG

#define HS_SERVER_PROG 0x30000062
#define HS_SERVER_VERS 0x00010001

#define HS_RPC_PROG 0x30000091

#define HS_PROCESS_CMD_PROC 0x02
#define HS_SUBSCRIBE_SRVC_PROC 0x03
#define HS_REPORT_EVNT_PROC    0x05
#define HS_EVENT_CB_PROC	1
#define HS_EVENT_DATA_VER	1

#define RPC_KEYPAD_NULL_PROC 0
#define RPC_KEYPAD_PASS_KEY_CODE_PROC 2
#define RPC_KEYPAD_SET_PWR_KEY_STATE_PROC 3

#define HS_PWR_K		0x6F	/* Power key */
#define HS_END_K		0x51	/* End key or Power key */
#define HS_HEADSET_K  		0x7E 	/* 4pole headset */
#define HS_STEREO_HEADSET_K	0x82
#define HS_HEADSET_SWITCH_K	0x84	/* sendend key */
#define HS_HEADSET_SWITCH_2_K	0xF0	/* Volume up key */
#define HS_HEADSET_SWITCH_3_K	0xF1	/* Volume down key */
#define HS_HEADSET_HEADPHONE_K	0xF6	/* 3pole headset key */
#define HS_HEADSET_MICROPHONE_K 0xF7
#if defined(CONFIG_MACH_ARUBASLIM_OPEN)
#define HS_NONE_K		0x00 /* key press */
#endif
#define HS_REL_K		0xFF	/* key release */

#define SW_HEADPHONE_INSERT_W_MIC 1 /* HS with mic */

#define KEY(hs_key, input_key) ((hs_key << 24) | input_key)

#if 0
enum hs_event {
	HS_EVNT_EXT_PWR = 0,	/* External Power status        */
	HS_EVNT_HSD,		/* Headset Detection            */
	HS_EVNT_HSTD,		/* Headset Type Detection       */
	HS_EVNT_HSSD,		/* Headset Switch Detection     */
	HS_EVNT_KPD,
	HS_EVNT_FLIP,		/* Flip / Clamshell status (open/close) */
	HS_EVNT_CHARGER,	/* Battery is being charged or not */
	HS_EVNT_ENV,		/* Events from runtime environment like DEM */
	HS_EVNT_REM,		/* Events received from HS counterpart on a
				remote processor*/
	HS_EVNT_DIAG,		/* Diag Events  */
	HS_EVNT_LAST,		 /* Should always be the last event type */
	HS_EVNT_MAX		/* Force enum to be an 32-bit number */
};

#else

enum hs_event {
	HS_EVNT_EXT_PWR = 0,
	HS_EVNT_HSD,
	HS_EVNT_HSTD,
	HS_EVNT_HSSD,
	HS_EVNT_KPD,
	HS_EVNT_PWR_END,
	HS_EVNT_FLIP,
	HS_EVNT_CHARGER,
	HS_EVNT_ENV,
	HS_EVNT_REM,
	HS_EVNT_DIAG,
	HS_EVNT_ACCESSORY_DETECT,
	HS_EVNT_HPD,
	HS_EVNT_MIC_DETECT,
	HS_EVNT_ACC_TYPE_DETECT,
	HS_EVNT_LAST,
	HS_EVNT_MAX = 0x7FFFFFFF,
};

#endif

enum hs_src_state {
	HS_SRC_STATE_UNKWN = 0,
	HS_SRC_STATE_LO,
	HS_SRC_STATE_HI,
#if defined(CONFIG_MACH_ARUBASLIM_OPEN)
	HS_SRC_NONE_K,
	HS_SRC_REL_K
#endif
};

struct hs_event_data {
	uint32_t	ver;		/* Version number */
	enum hs_event	event_type;     /* Event Type	*/
	enum hs_event	enum_disc;     /* discriminator */
	uint32_t	data_length;	/* length of the next field */
	enum hs_src_state	data;    /* Pointer to data */
	uint32_t	data_size;	/* Elements to be processed in data */
};

enum hs_return_value {
	HS_EKPDLOCKED     = -2,	/* Operation failed because keypad is locked */
	HS_ENOTSUPPORTED  = -1,	/* Functionality not supported */
	HS_FALSE          =  0, /* Inquired condition is not true */
	HS_FAILURE        =  0, /* Requested operation was not successful */
	HS_TRUE           =  1, /* Inquired condition is true */
	HS_SUCCESS        =  1, /* Requested operation was successful */
	HS_MAX_RETURN     =  0x7FFFFFFF/* Force enum to be a 32 bit number */
};

struct hs_key_data {
	uint32_t ver;        /* Version number to track sturcture changes */
	uint32_t code;       /* which key? */
	uint32_t parm;       /* key status. Up/down or pressed/released */
};

enum hs_subs_srvc {
	HS_SUBS_SEND_CMD = 0, /* Subscribe to send commands to HS */
	HS_SUBS_RCV_EVNT,     /* Subscribe to receive Events from HS */
	HS_SUBS_SRVC_MAX
};

enum hs_subs_req {
	HS_SUBS_REGISTER,    /* Subscribe   */
	HS_SUBS_CANCEL,      /* Unsubscribe */
	HS_SUB_STATUS_MAX
};

enum hs_event_class {
	HS_EVNT_CLASS_ALL = 0, /* All HS events */
	HS_EVNT_CLASS_LAST,    /* Should always be the last class type   */
	HS_EVNT_CLASS_MAX
};

enum hs_cmd_class {
	HS_CMD_CLASS_LCD = 0, /* Send LCD related commands              */
	HS_CMD_CLASS_KPD,     /* Send KPD related commands              */
	HS_CMD_CLASS_LAST,    /* Should always be the last class type   */
	HS_CMD_CLASS_MAX
};

/*
 * Receive events or send command
 */
union hs_subs_class {
	enum hs_event_class	evnt;
	enum hs_cmd_class	cmd;
};

struct hs_subs {
	uint32_t                ver;
	enum hs_subs_srvc	srvc;  /* commands or events */
	enum hs_subs_req	req;   /* subscribe or unsubscribe  */
	uint32_t		host_os;
	enum hs_subs_req	disc;  /* discriminator    */
	union hs_subs_class      id;
};

struct hs_event_cb_recv {
	uint32_t cb_id;
	uint32_t hs_key_data_ptr;
	struct hs_key_data key;
};
enum hs_ext_cmd_type {
	HS_EXT_CMD_KPD_SEND_KEY = 0, /* Send Key */
	HS_EXT_CMD_KPD_BKLT_CTRL, /* Keypad backlight intensity	*/
	HS_EXT_CMD_LCD_BKLT_CTRL, /* LCD Backlight intensity */
	HS_EXT_CMD_DIAG_KEYMAP, /* Emulating a Diag key sequence */
	HS_EXT_CMD_DIAG_LOCK, /* Device Lock/Unlock */
	HS_EXT_CMD_GET_EVNT_STATUS, /* Get the status for one of the drivers */
	HS_EXT_CMD_KPD_GET_KEYS_STATUS,/* Get a list of keys status */
	HS_EXT_CMD_KPD_SET_PWR_KEY_RST_THOLD, /* PWR Key HW Reset duration */
	HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD, /* Set pwr key threshold duration */
	HS_EXT_CMD_LAST, /* Should always be the last command type */
	HS_EXT_CMD_MAX = 0x7FFFFFFF /* Force enum to be an 32-bit number */
};

struct hs_cmd_data_type {
	uint32_t hs_cmd_data_type_ptr; /* hs_cmd_data_type ptr length */
	uint32_t ver; /* version */
	enum hs_ext_cmd_type id; /* command id */
	uint32_t handle; /* handle returned from subscribe proc */
	enum hs_ext_cmd_type disc_id1; /* discriminator id */
	uint32_t input_ptr; /* input ptr length */
	uint32_t input_val; /* command specific data */
	uint32_t input_len; /* length of command input */
	enum hs_ext_cmd_type disc_id2; /* discriminator id */
	uint32_t output_len; /* length of output data */
	uint32_t delayed; /* execution context for modem
				true - caller context
				false - hs task context*/
};

static const uint32_t hs_key_map[] = {
	KEY(HS_PWR_K, KEY_POWER),
	KEY(HS_END_K, KEY_POWER),
#if 0
	KEY(HS_STEREO_HEADSET_K, SW_HEADPHONE_INSERT_W_MIC),
	KEY(HS_HEADSET_MICROPHONE_K, SW_MICROPHONE_INSERT),
	KEY(HS_HEADSET_HEADPHONE_K, SW_HEADPHONE_INSERT), 			/* 3pole headset */
	KEY(HS_HEADSET_K, SW_HEADPHONE_INSERT_W_MIC), 				/* 4pole headset */	
#else
	KEY(HS_HEADSET_HEADPHONE_K, SW_HEADPHONE_INSERT), 	/* 3pole headset */
	KEY(HS_STEREO_HEADSET_K, SW_HEADPHONE_INSERT_W_MIC),    /* 4pole headset */	
#endif
	KEY(HS_HEADSET_SWITCH_K, KEY_MEDIA),
	KEY(HS_HEADSET_SWITCH_2_K, KEY_VOLUMEUP),
	KEY(HS_HEADSET_SWITCH_3_K, KEY_VOLUMEDOWN),
	0
};

int current_jack_type = SEC_JACK_NO_DEVICE;		/* report current jack type */
int current_key_state;		/* report current key state */

/* To support samsung factory test */
struct switch_dev switch_sendend = {
	.name = "send_end",
};

enum {
	NO_DEVICE	= 0,
	MSM_HEADSET	= 1,
};
/* Add newer versions at the top of array */
static const unsigned int rpc_vers[] = {
	0x00030001,
	0x00020001,
	0x00010001,
};
/* hs subscription request parameters */
struct hs_subs_rpc_req {
	uint32_t hs_subs_ptr;
	struct hs_subs hs_subs;
	uint32_t hs_cb_id;
	uint32_t hs_handle_ptr;
	uint32_t hs_handle_data;
};

static struct hs_subs_rpc_req *hs_subs_req;

struct msm_handset {
	struct input_dev *ipdev;
	struct switch_dev sdev;
	struct msm_handset_platform_data *hs_pdata;
	bool mic_on, hs_on;
};

static struct msm_rpc_client *rpc_client;
static struct msm_handset *hs;

extern struct class *sec_class;
struct device *pwr_dev;
static int key_count;

static ssize_t keyshort_test(struct device *dev, struct device_attribute *attr, char *buf)
{
	int count;
	if(key_count) {
		count = sprintf(buf, "PRESS\n");
	}
	else {
		count = sprintf(buf, "RELEASE\n");
	}

	return count;
}

static DEVICE_ATTR(key_pressed, 0664, keyshort_test, NULL);

static int hs_find_key(uint32_t hscode)
{
	int i, key;

	key = KEY(hscode, 0);

	for (i = 0; hs_key_map[i] != 0; i++) {
		if ((hs_key_map[i] & 0xff000000) == key)
			return hs_key_map[i] & 0x00ffffff;
	}
	return -1;
}

#ifndef FEATURE_HEADSET_AP_DETECT
static void update_state(void)
{
	int state;

	if (hs->mic_on && hs->hs_on)
		state = 1 << 0;
	else if (hs->hs_on)
		state = 1 << 1;
	else if (hs->mic_on)
		state = 1 << 2;
	else
		state = 0;

	switch_set_state(&hs->sdev, state);
}
#endif

/*
 * tuple format: (key_code, key_param)
 *
 * old-architecture:
 * key-press = (key_code, 0)
 * key-release = (0xff, key_code)
 *
 * new-architecutre:
 * key-press = (key_code, 0)
 * key-release = (key_code, 0xff)
 */
 
extern unsigned int sec_key_pressed;
int get_msm7x27a_det_jack_state(void);
static void report_hs_key(uint32_t key_code, uint32_t key_parm)
{
	int key, temp_key_code;

	if (key_code == HS_REL_K)
		key = hs_find_key(key_parm);
	else
		key = hs_find_key(key_code);

	temp_key_code = key_code;

	if (key_parm == HS_REL_K) {
		key_code = key_parm;
	}

	if ((key != KEY_POWER) && !get_msm7x27a_det_jack_state()) {
		pr_info("%s headset is not connected %d %d\n", __func__, key, (key_code != HS_REL_K));
		return;	
	}

#if 0
	if( key == KEY_END && !get_msm7x27a_det_jack_state() )
	key = KEY_POWER;
#endif
	switch (key) {
	case KEY_POWER:
#if 0 // msm8x25q patch,but not used
		if (hs->hs_pdata->ignore_end_key)
			input_report_key(hs->ipdev, KEY_POWER,
						(key_code != HS_REL_K));
		else
			input_report_key(hs->ipdev, key,
						(key_code != HS_REL_K));
#endif
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		if(key_code != HS_REL_K) sec_key_pressed=1;
		else sec_key_pressed=0;
		pr_info("%s KEY_POWER %d\n", __func__, (key_code != HS_REL_K));
		key_count = (key_code != HS_REL_K);
		break;

	case KEY_MEDIA:
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		pr_info("%s KEY_MEDIA %d\n", __func__, (key_code != HS_REL_K));
		switch_set_state(&switch_sendend, (key_code != HS_REL_K));
		/* inform current_key_state to sec_jack driver for factory test */
		current_key_state = (key_code != HS_REL_K);
		break;
		
	case KEY_VOLUMEUP:
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		pr_info("%s KEY_VOL+ %d\n", __func__, (key_code != HS_REL_K));
		break;
		
	case KEY_VOLUMEDOWN:
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		pr_info("%s KEY_VOL- %d\n", __func__, (key_code != HS_REL_K));
		break;
		
	case KEY_END:
		input_report_key(hs->ipdev, key, (key_code != HS_REL_K));
		pr_info("%s KEY_END %d\n", __func__, (key_code != HS_REL_K));
		break;
		
#ifdef FEATURE_HEADSET_AP_DETECT
	case SW_HEADPHONE_INSERT: 		/* 3pole headset */
		current_jack_type = SEC_HEADSET_3POLE;
		pr_info("%s 3pole headset %d\n", __func__, key);
		break;

	case SW_HEADPHONE_INSERT_W_MIC:	/* 4pole headset */
		current_jack_type = SEC_HEADSET_4POLE;
		pr_info("%s 4pole headset %d\n", __func__, key);
		break;
#else
	case SW_HEADPHONE_INSERT_W_MIC:
		hs->mic_on = hs->hs_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, SW_HEADPHONE_INSERT,
							hs->hs_on);
		input_report_switch(hs->ipdev, SW_MICROPHONE_INSERT,
							hs->mic_on);
		update_state();
		break;

	case SW_HEADPHONE_INSERT:
		hs->hs_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, key, hs->hs_on);
		update_state();
		break;
	case SW_MICROPHONE_INSERT:
		hs->mic_on = (key_code != HS_REL_K) ? 1 : 0;
		input_report_switch(hs->ipdev, key, hs->mic_on);
		update_state();
		break;
#endif
	case -1:
		printk(KERN_ERR "%s: No mapping for remote handset event %d\n",
				 __func__, temp_key_code);
		return;
	default:
		pr_err("%s another value %d\n", __func__, key);
	}
	input_sync(hs->ipdev);
}

static int handle_hs_rpc_call(struct msm_rpc_server *server,
			   struct rpc_request_hdr *req, unsigned len)
{
	struct rpc_keypad_pass_key_code_args {
		uint32_t key_code;
		uint32_t key_parm;
	};

	switch (req->procedure) {
	case RPC_KEYPAD_NULL_PROC:
		return 0;

	case RPC_KEYPAD_PASS_KEY_CODE_PROC: {
		struct rpc_keypad_pass_key_code_args *args;

		args = (struct rpc_keypad_pass_key_code_args *)(req + 1);
		args->key_code = be32_to_cpu(args->key_code);
		args->key_parm = be32_to_cpu(args->key_parm);

		report_hs_key(args->key_code, args->key_parm);

		return 0;
	}

	case RPC_KEYPAD_SET_PWR_KEY_STATE_PROC:
		/* This RPC function must be available for the ARM9
		 * to function properly.  This function is redundant
		 * when RPC_KEYPAD_PASS_KEY_CODE_PROC is handled. So
		 * input_report_key is not needed.
		 */
		return 0;
	default:
		return -ENODEV;
	}
}

static struct msm_rpc_server hs_rpc_server = {
	.prog		= HS_SERVER_PROG,
	.vers		= HS_SERVER_VERS,
	.rpc_call	= handle_hs_rpc_call,
};

static int process_subs_srvc_callback(struct hs_event_cb_recv *recv)
{
	if (!recv)
		return -ENODATA;

	report_hs_key(be32_to_cpu(recv->key.code), be32_to_cpu(recv->key.parm));

	return 0;
}

static void process_hs_rpc_request(uint32_t proc, void *data)
{
	if (proc == HS_EVENT_CB_PROC)
		process_subs_srvc_callback(data);
	else
		pr_err("%s: unknown rpc proc %d\n", __func__, proc);
}

static int hs_rpc_report_event_arg(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	struct hs_event_rpc_req {
		uint32_t hs_event_data_ptr;
		struct hs_event_data data;
	};

	struct hs_event_rpc_req *req = buffer;

	req->hs_event_data_ptr	= cpu_to_be32(0x1);
	req->data.ver		= cpu_to_be32(HS_EVENT_DATA_VER);
#if 0
	req->data.event_type	= cpu_to_be32(HS_EVNT_HSD);
	req->data.enum_disc	= cpu_to_be32(HS_EVNT_HSD);
#else

	req->data.event_type = cpu_to_be32(HS_EVNT_ACCESSORY_DETECT);
	req->data.enum_disc = cpu_to_be32(HS_EVNT_ACCESSORY_DETECT);
#endif
	req->data.data_length	= cpu_to_be32(0x1);
	req->data.data		= cpu_to_be32(*(enum hs_src_state *)data);
	req->data.data_size	= cpu_to_be32(sizeof(enum hs_src_state));

	return sizeof(*req);
}

static int hs_rpc_report_event_res(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	enum hs_return_value result;

	result = be32_to_cpu(*(enum hs_return_value *)buffer);
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	if (result == HS_SUCCESS)
		return 0;

	return 1;
}

void report_headset_status(bool connected)
{
	int rc = -1;
	enum hs_src_state status;

	pr_info("%s %d\n", __func__, connected);

	if (connected == true)
		status = HS_SRC_STATE_HI;
	else
		status = HS_SRC_STATE_LO;

	rc = msm_rpc_client_req(rpc_client, HS_REPORT_EVNT_PROC,
				hs_rpc_report_event_arg, &status,
				hs_rpc_report_event_res, NULL, -1);

	if (rc)
		pr_err("%s: couldn't send rpc client request\n", __func__);
}
EXPORT_SYMBOL(report_headset_status);

#if defined(CONFIG_MACH_ARUBASLIM_OPEN)
static int hs_rpc_report_key_event_arg(struct msm_rpc_client *client,
					void *buffer, void *data)
{
	struct hs_event_rpc_req {
		uint32_t hs_event_data_ptr;
		struct hs_event_data data;
	};

	struct hs_event_rpc_req *req = buffer;

	req->hs_event_data_ptr	= cpu_to_be32(0x1);
	req->data.ver		= cpu_to_be32(HS_EVENT_DATA_VER);

	req->data.event_type = cpu_to_be32(HS_EVNT_ACCESSORY_DETECT);
	req->data.enum_disc = cpu_to_be32(HS_EVNT_ACCESSORY_DETECT);

	req->data.data_length	= cpu_to_be32(0x1);
	req->data.data		= cpu_to_be32((*(enum hs_src_state *)data));
	req->data.data_size	= cpu_to_be32(sizeof(enum hs_src_state));

	return sizeof(*req);
}

void report_headset_key_status(bool connected)
{
	int rc = -1;
	enum hs_src_state status;

	pr_info("%s %d\n", __func__, connected);

	if (connected == false)
		status = HS_SRC_NONE_K;
	else
		status = HS_SRC_REL_K;

	rc = msm_rpc_client_req(rpc_client, HS_REPORT_EVNT_PROC,
				hs_rpc_report_key_event_arg, &status,
				hs_rpc_report_event_res, NULL, -1);

	if (rc)
		pr_err("%s: couldn't send rpc client request\n", __func__);
}
EXPORT_SYMBOL(report_headset_key_status);
#endif

static int hs_rpc_pwr_cmd_arg(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	struct hs_cmd_data_type *hs_pwr_cmd = buffer;

	hs_pwr_cmd->hs_cmd_data_type_ptr = cpu_to_be32(0x01);

	hs_pwr_cmd->ver = cpu_to_be32(0x03);
	hs_pwr_cmd->id = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->handle = cpu_to_be32(hs_subs_req->hs_handle_data);
	hs_pwr_cmd->disc_id1 = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->input_ptr = cpu_to_be32(0x01);
	hs_pwr_cmd->input_val = cpu_to_be32(hs->hs_pdata->pwr_key_delay_ms);
	hs_pwr_cmd->input_len = cpu_to_be32(0x01);
	hs_pwr_cmd->disc_id2 = cpu_to_be32(HS_EXT_CMD_KPD_SET_PWR_KEY_THOLD);
	hs_pwr_cmd->output_len = cpu_to_be32(0x00);
	hs_pwr_cmd->delayed = cpu_to_be32(0x00);

	return sizeof(*hs_pwr_cmd);
}

static int hs_rpc_pwr_cmd_res(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	uint32_t result;

	result = be32_to_cpu(*((uint32_t *)buffer));
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	return 0;
}

static int hs_rpc_register_subs_arg(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	hs_subs_req = buffer;

	hs_subs_req->hs_subs_ptr	= cpu_to_be32(0x1);
	hs_subs_req->hs_subs.ver	= cpu_to_be32(0x1);
	hs_subs_req->hs_subs.srvc	= cpu_to_be32(HS_SUBS_RCV_EVNT);
	hs_subs_req->hs_subs.req	= cpu_to_be32(HS_SUBS_REGISTER);
	hs_subs_req->hs_subs.host_os	= cpu_to_be32(0x4); /* linux */
	hs_subs_req->hs_subs.disc	= cpu_to_be32(HS_SUBS_RCV_EVNT);
	hs_subs_req->hs_subs.id.evnt	= cpu_to_be32(HS_EVNT_CLASS_ALL);

	hs_subs_req->hs_cb_id		= cpu_to_be32(0x1);

	hs_subs_req->hs_handle_ptr	= cpu_to_be32(0x1);
	hs_subs_req->hs_handle_data	= cpu_to_be32(0x0);

	return sizeof(*hs_subs_req);
}

static int hs_rpc_register_subs_res(struct msm_rpc_client *client,
				    void *buffer, void *data)
{
	uint32_t result;

	result = be32_to_cpu(*((uint32_t *)buffer));
	pr_debug("%s: request completed: 0x%x\n", __func__, result);

	return 0;
}

static int hs_cb_func(struct msm_rpc_client *client, void *buffer, int in_size)
{
	int rc = -1;

	struct rpc_request_hdr *hdr = buffer;

	hdr->type = be32_to_cpu(hdr->type);
	hdr->xid = be32_to_cpu(hdr->xid);
	hdr->rpc_vers = be32_to_cpu(hdr->rpc_vers);
	hdr->prog = be32_to_cpu(hdr->prog);
	hdr->vers = be32_to_cpu(hdr->vers);
	hdr->procedure = be32_to_cpu(hdr->procedure);

	process_hs_rpc_request(hdr->procedure,
			    (void *) (hdr + 1));

	msm_rpc_start_accepted_reply(client, hdr->xid,
				     RPC_ACCEPTSTAT_SUCCESS);
	rc = msm_rpc_send_accepted_reply(client, 0);
	if (rc) {
		pr_err("%s: sending reply failed: %d\n", __func__, rc);
		return rc;
	}

	return 0;
}

static int __devinit hs_rpc_cb_init(void)
{
	int rc = 0, i, num_vers;

	num_vers = ARRAY_SIZE(rpc_vers);

	for (i = 0; i < num_vers; i++) {
		rpc_client = msm_rpc_register_client("hs",
			HS_RPC_PROG, rpc_vers[i], 0, hs_cb_func);

		if (IS_ERR(rpc_client))
			pr_debug("%s: RPC Client version %d failed, fallback\n",
				 __func__, rpc_vers[i]);
		else
			break;
	}

	if (IS_ERR(rpc_client)) {
		pr_err("%s: Incompatible RPC version error %ld\n",
			 __func__, PTR_ERR(rpc_client));
		return PTR_ERR(rpc_client);
	}

	rc = msm_rpc_client_req(rpc_client, HS_SUBSCRIBE_SRVC_PROC,
				hs_rpc_register_subs_arg, NULL,
				hs_rpc_register_subs_res, NULL, -1);
	if (rc) {
		pr_err("%s: RPC client request failed for subscribe services\n",
						__func__);
		goto err_client_req;
	}

	rc = msm_rpc_client_req(rpc_client, HS_PROCESS_CMD_PROC,
			hs_rpc_pwr_cmd_arg, NULL,
			hs_rpc_pwr_cmd_res, NULL, -1);
	if (rc)
		pr_err("%s: RPC client request failed for pwr key"
			" delay cmd, using normal mode\n", __func__);
	return 0;
err_client_req:
	msm_rpc_unregister_client(rpc_client);
	return rc;
}

static int __devinit hs_rpc_init(void)
{
	int rc;

	rc = hs_rpc_cb_init();
	if (rc) {
		pr_err("%s: failed to initialize rpc client, try server...\n",
						__func__);

		rc = msm_rpc_create_server(&hs_rpc_server);
		if (rc) {
			pr_err("%s: failed to create rpc server\n", __func__);
			return rc;
		}
	}

	return rc;
}

static void __devexit hs_rpc_deinit(void)
{
	if (rpc_client)
		msm_rpc_unregister_client(rpc_client);
}

#ifndef FEATURE_HEADSET_AP_DETECT
static ssize_t msm_headset_print_name(struct switch_dev *sdev, char *buf)
{
	switch (switch_get_state(&hs->sdev)) {
	case NO_DEVICE:
		return sprintf(buf, "No Device\n");
	case MSM_HEADSET:
		return sprintf(buf, "Headset\n");
	}
	return -EINVAL;
}
#endif

static int __devinit hs_probe(struct platform_device *pdev)
{
	int rc = 0;
	struct input_dev *ipdev;

	hs = kzalloc(sizeof(struct msm_handset), GFP_KERNEL);
	if (!hs)
		return -ENOMEM;

#ifndef FEATURE_HEADSET_AP_DETECT
	hs->sdev.name	= "h2w";
	hs->sdev.print_name = msm_headset_print_name;

	rc = switch_dev_register(&hs->sdev);
	if (rc)
		goto err_switch_dev_register;
#endif

	rc = switch_dev_register(&switch_sendend);
	if (rc < 0) {
		pr_err("%s : Failed to register switch device\n", __func__);
		goto err_switch_dev_register;
	}

	ipdev = input_allocate_device();
	if (!ipdev) {
		rc = -ENOMEM;
		goto err_alloc_input_dev;
	}
	input_set_drvdata(ipdev, hs);

	hs->ipdev = ipdev;

	if (pdev->dev.platform_data)
		hs->hs_pdata = pdev->dev.platform_data;

	if (hs->hs_pdata->hs_name)
		ipdev->name = hs->hs_pdata->hs_name;
	else
		ipdev->name	= DRIVER_NAME;

	ipdev->id.vendor	= 0x0001;
	ipdev->id.product	= 1;
	ipdev->id.version	= 1;

	input_set_capability(ipdev, EV_KEY, KEY_MEDIA);
	input_set_capability(ipdev, EV_KEY, KEY_VOLUMEUP);
	input_set_capability(ipdev, EV_KEY, KEY_VOLUMEDOWN);
	//input_set_capability(ipdev, EV_SW, SW_HEADPHONE_INSERT);
	//input_set_capability(ipdev, EV_SW, SW_MICROPHONE_INSERT);
	input_set_capability(ipdev, EV_KEY, KEY_POWER);
	input_set_capability(ipdev, EV_KEY, KEY_END);
	rc = input_register_device(ipdev);
	if (rc) {
		dev_err(&ipdev->dev,
				"hs_probe: input_register_device rc=%d\n", rc);
		goto err_reg_input_dev;
	}

	platform_set_drvdata(pdev, hs);

	rc = hs_rpc_init();
	if (rc) {
		dev_err(&ipdev->dev, "rpc init failure\n");
		goto err_hs_rpc_init;
	}

	pwr_dev = device_create(sec_class, NULL, 0, NULL, "pwr_key");
	if (!pwr_dev)
		printk("Failed to create device(pwr_key)!\n");
	
	if(device_create_file(pwr_dev, &dev_attr_key_pressed) < 0)
		printk("Failed to create file(%s)!\n", dev_attr_key_pressed.attr.name);

	return 0;

err_hs_rpc_init:
	input_unregister_device(ipdev);
err_reg_input_dev:
	input_free_device(ipdev);
	ipdev = NULL;	
err_alloc_input_dev:
#ifndef FEATURE_HEADSET_AP_DETECT
	switch_dev_unregister(&hs->sdev);
#endif
err_switch_dev_register:
	kfree(hs);
	return rc;
}

static int __devexit hs_remove(struct platform_device *pdev)
{
	struct msm_handset *hs = platform_get_drvdata(pdev);

	input_unregister_device(hs->ipdev);
#ifndef FEATURE_HEADSET_AP_DETECT
	switch_dev_unregister(&hs->sdev);
#endif
	switch_dev_unregister(&switch_sendend);

	kfree(hs);
	hs_rpc_deinit();
	return 0;
}

static struct platform_driver hs_driver = {
	.probe		= hs_probe,
	.remove		= __devexit_p(hs_remove),
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init hs_init(void)
{
	return platform_driver_register(&hs_driver);
}
late_initcall(hs_init);

static void __exit hs_exit(void)
{
	platform_driver_unregister(&hs_driver);
}
module_exit(hs_exit);

MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:msm-handset");
