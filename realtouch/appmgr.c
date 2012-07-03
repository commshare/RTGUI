#include <rtthread.h>
#include <rtgui/widgets/window.h>
#include <rtgui/rtgui_application.h>

// record one app
static struct rtgui_application *the_app;

static rt_bool_t appmgr_event_handler(struct rtgui_object *object,
                                      struct rtgui_event *event)
{
    struct rtgui_event_app *eapp = (struct rtgui_event_app*)event;

    if (event->type == RTGUI_EVENT_APP_CREATE)
    {
        rt_kprintf("recieved a app creation event from %s(role:%d)\n",
                eapp->app->name,
                eapp->app->role);
        the_app = eapp->app;
        rtgui_application_ack(event, RTGUI_STATUS_OK);
        return RT_TRUE;
    }
    return rtgui_application_event_handler(object, event);
}

static rt_bool_t appmgr_activate_the_app(struct rtgui_object *object,
		                                 struct rtgui_event *event)
{
    if (the_app)
    {
        struct rtgui_event_app eapp;
        RTGUI_EVENT_APP_ACTIVATE_INIT(&eapp);
        eapp.app = the_app;
        rtgui_application_send(the_app->tid, (struct rtgui_event*)&eapp, sizeof(eapp));
    }
    return RT_TRUE;
}

void appmgr_entry(void* parameter)
{
    struct rtgui_application* app;

    app = rtgui_application_create_with_role(rt_thread_self(),
            "AppMgr",
            RTGUI_APPLICATION_ROLE_APPMGR);
    if (app != RT_NULL)
    {
        struct rtgui_win *win;
        rtgui_rect_t rect = {120, 120, 300, 200};

        rtgui_object_set_event_handler(RTGUI_OBJECT(app),
                appmgr_event_handler);

        win = rtgui_win_create(RT_NULL, "MgrWindow", &rect, RTGUI_WIN_STYLE_DEFAULT);
        rtgui_win_set_onactivate(win, appmgr_activate_the_app);
        rtgui_win_show(win, RT_FALSE);

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
