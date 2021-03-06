#include <stdlib.h>
#include <rtgui/dc.h>
#include <rtgui/dc_hw.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/container.h>
#include "demo_view.h"

#define RAND(x1, x2) ((rand() % (x2 - x1)) + x1)

static struct rtgui_container *container = RT_NULL;
static int running = 0;
static rt_tick_t tick_bak=0;
static rt_uint32_t frame_rate=0;

void _onidle(struct rtgui_object *object, rtgui_event_t *event)
{
	rtgui_color_t color;
	rtgui_rect_t rect, draw_rect;
	struct rtgui_dc *dc;

	if(rt_tick_get()-tick_bak >= RT_TICK_PER_SECOND)
	{
		rt_kprintf("frame_rate=%d\n",frame_rate);
		frame_rate=0;
		tick_bak = rt_tick_get();
	}
	else frame_rate ++;
	
	dc = rtgui_dc_begin_drawing(RTGUI_WIDGET(container));
	if (dc == RT_NULL)
		return;

	demo_view_get_logic_rect(RTGUI_CONTAINER(container), &rect);
	draw_rect.x1 = rect.x1;
	draw_rect.y1 = rect.y1;
	draw_rect.x2 = draw_rect.x1+320;
	draw_rect.y2 = draw_rect.y1+240;

	color = RTGUI_RGB(rand() % 255, rand() % 255, rand() % 255);
	RTGUI_WIDGET_BACKGROUND(RTGUI_WIDGET(container)) = color;

#if (0)
	rtgui_dc_fill_rect(dc, &draw_rect);
#else
	{
		int x,y;
		for(x=draw_rect.x1; x<draw_rect.x2; x++)
			for(y=draw_rect.y1; y<draw_rect.y2; y++)
				rtgui_dc_draw_color_point(dc, x, y, color);
	}
#endif
	/* 绘图完成 */
	rtgui_dc_end_drawing(dc);
}

void _draw_default(struct rtgui_object *object, rtgui_event_t* event)
{
	struct rtgui_widget *widget = RTGUI_WIDGET(object);
	struct rtgui_dc* dc;
	rtgui_rect_t rect;

	/* 因为用的是demo container，上面本身有一部分控件，所以在绘图时先要让demo container先绘图 */
	rtgui_container_event_handler(object, event);

	/* 获得控件所属的DC */
	dc = rtgui_dc_begin_drawing(widget);
	if (dc == RT_NULL) /* 如果不能正常获得DC，返回（如果控件或父控件是隐藏状态，DC是获取不成功的） */
		return;

	/* 获得demo container允许绘图的区域 */
	demo_view_get_logic_rect(RTGUI_CONTAINER(widget), &rect);

	/* 擦除所有 */
	RTGUI_WIDGET_BACKGROUND(widget) = default_background;
	rtgui_dc_fill_rect(dc, &rect);

	/* 显示提示 */
	rtgui_dc_draw_text(dc, "按任意键开始/停止测试...", &rect);

	/* 绘图完成 */
	rtgui_dc_end_drawing(dc);
}

rt_bool_t benchmark_event_handler(struct rtgui_object *object, rtgui_event_t *event)
{
	if (event->type == RTGUI_EVENT_PAINT)
	{
		_draw_default(object, event);
	}
	else if (event->type == RTGUI_EVENT_MOUSE_BUTTON)
	{
		struct rtgui_event_mouse *emouse = (struct rtgui_event_mouse*)event;

		if (emouse->button & RTGUI_MOUSE_BUTTON_DOWN)
		{
			if (running)
			{
				/* stop */
				rtgui_application_set_onidle(RT_NULL);
				_draw_default(object, event);
			}
			else
			{
				/* run */
				rtgui_application_set_onidle(_onidle);
			}

			running = !running;
		}
		return RT_TRUE;
	}
	else
	{
		/* 调用默认的事件处理函数 */
		return rtgui_container_event_handler(object, event);
	}

	return RT_FALSE;
}

rtgui_container_t* demo_view_benchmark(void)
{
	srand(100);
	container = demo_view("绘图测试");
	rtgui_object_set_event_handler(RTGUI_OBJECT(container), benchmark_event_handler);

	return container;
}
