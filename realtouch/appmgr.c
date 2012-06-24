#include <rtthread.h>
#include <rtgui/rtgui_application.h>

static rt_bool_t appmgr_event_handler(struct rtgui_object *object,
                                      struct rtgui_event *event)
{
    if (event->type == RTGUI_EVENT_APP_CREATE)
    {
        rt_kprintf("recieved a app creation event from %s(role:%d)\n",
                   ((struct rtgui_event_app*)event)->app->name,
                   ((struct rtgui_event_app*)event)->app->role);
        rtgui_application_ack(event, RTGUI_STATUS_OK);
        return RT_TRUE;
    }
}

void appmgr_entry(void* parameter)
{
	struct rtgui_application* app;

	app = rtgui_application_create_with_role(rt_thread_self(),
                                             "AppMgr",
                                             RTGUI_APPLICATION_ROLE_APPMGR);
	if (app != RT_NULL)
	{
        rtgui_object_set_event_handler(RTGUI_OBJECT(app),
                                       appmgr_event_handler);
		rtgui_application_run(app);
		rtgui_application_destroy(app);
	}
}

void appmgr_init(void)
{
	rt_thread_t tid;

	tid = rt_thread_create("appmgr", appmgr_entry, RT_NULL, 4096, 20, 20);
	if (tid != RT_NULL)
		rt_thread_startup(tid);
}
