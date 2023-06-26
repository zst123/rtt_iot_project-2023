#include <rtthread.h>
#include <rtdevice.h>
#include <stdio.h>
#include <string.h>
#include "webclient.h"

extern float TRMS;

#define THREAD_PRIORITY         20
#define THREAD_STACK_SIZE       2048
#define THREAD_TIMESLICE        5

// Wifi Credentials
const char * ssid = "YOUR_SSID";
const char * password = "YOUR_PASSWORD";

// Adafruit.io
#define IO_USERNAME "YOUR_USERNAME"
#define IO_KEY "YOUR_KEY"

/* Entry Function for Thread 1 */
static rt_thread_t uploader_tid = RT_NULL;

static void uploader_entry(void *parameter) {
    // Wait for Wifi to become stable
    rt_thread_mdelay(1000 * 5);

    // Connect to Wifi

    while(rt_wlan_connect(ssid, password) != RT_EOK) {
        rt_kprintf("Could not connect to Wifi. Retrying... \r\n");
        rt_thread_mdelay(1000 * 3);
    }
    rt_kprintf("Wifi successfully connected \r\n");
    
    // Start uploading to cloud
    int resp_status;
    while(1) {
        rt_thread_delay(1000);

        // GET request
        struct webclient_session *session = NULL;
        session = webclient_session_create(2048);


        char * URI = "http://io.adafruit.com/api/v2/" IO_USERNAME "/feeds/current-sensor/data?X-AIO-Key=" IO_KEY;
        char post_data[100];
        size_t data_len = snprintf(post_data, 100, "value=%d\r\n", (int) (TRMS*1000));
        rt_kprintf("Sending to cloud: %s\r\n", post_data);

        webclient_header_fields_add(session, "Content-Length: %d\r\n", data_len);
        webclient_header_fields_add(session, "Content-Type: application/x-www-form-urlencoded\r\n");
        if ((resp_status = webclient_post(session, URI, post_data, data_len)) != 200) {
            rt_kprintf("webclient POST request failed, response(%d) error.\n", resp_status);
        }
        webclient_close(session);

    }
}

int uploader_begin(void) {
    uploader_tid = rt_thread_create("uploader_thread",
                                    uploader_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY,
                                    THREAD_TIMESLICE);
    if (uploader_tid != RT_NULL) {
        rt_thread_startup(uploader_tid);
    }
}

// Application initialization
// https://github.com/RT-Thread/rtthread-manual-doc/blob/master/basic/basic.md#rt-thread-automatic-initialization-mechanism
INIT_APP_EXPORT(uploader_begin);
