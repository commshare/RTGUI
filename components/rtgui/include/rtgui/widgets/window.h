/*
 * File      : window.h
 * This file is part of RTGUI in RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-10-04     Bernard      first version
 * 2010-05-03     Bernard      add win close function
 */
#ifndef __RTGUI_WINDOW_H__
#define __RTGUI_WINDOW_H__

#include <rtgui/rtgui.h>
#include <rtgui/list.h>
#include <rtgui/widgets/widget.h>
#include <rtgui/widgets/toplevel.h>
#include <rtgui/widgets/box.h>

DECLARE_CLASS_TYPE(win);
/** Gets the type of a win */
#define RTGUI_WIN_TYPE       (RTGUI_TYPE(win))
/** Casts the object to an rtgui_win */
#define RTGUI_WIN(obj)       (RTGUI_OBJECT_CAST((obj), RTGUI_WIN_TYPE, rtgui_win_t))
/** Checks if the object is an rtgui_win */
#define RTGUI_IS_WIN(obj)    (RTGUI_OBJECT_CHECK_TYPE((obj), RTGUI_WIN_TYPE))

#define RTGUI_WIN_STYLE_NO_FOCUS	        0x001	/* non-focused window			*/

#define RTGUI_WIN_STYLE_NO_TITLE	        0x002	/* no title window				*/
#define RTGUI_WIN_STYLE_NO_BORDER	        0x004	/* no border window				*/
#define RTGUI_WIN_STYLE_CLOSEBOX	        0x008	/* window has the close button	*/
#define RTGUI_WIN_STYLE_MINIBOX		        0x010	/* window has the mini button	*/

#define RTGUI_WIN_STYLE_DESTROY_ON_CLOSE	0x020   /* window is destroyed when closed */

#define RTGUI_WIN_STYLE_DEFAULT		(RTGUI_WIN_STYLE_CLOSEBOX | RTGUI_WIN_STYLE_MINIBOX)

enum rtgui_win_flag
{
	RTGUI_WIN_FLAG_INIT           = 0x00,   /* init state              */
	RTGUI_WIN_FLAG_MODAL		  = 0x01,	/* modal mode window	   */
	RTGUI_WIN_FLAG_CLOSED		  = 0x02,	/* window is closed		   */
	RTGUI_WIN_FLAG_ACTIVATE	      = 0x04,	/* window is activated	   */
	RTGUI_WIN_FLAG_UNDER_MODAL	  = 0x08,   /* window is under modal
											   show(modaled by other)  */
	RTGUI_WIN_FLAG_CONNECTED      = 0x10    /* connected to server */
};

struct rtgui_win_title;
struct rtgui_win_area;

struct rtgui_win
{
	/* inherit from toplevel */
	struct rtgui_toplevel parent;

    /* parent window. RT_NULL if the window is a top level window */
	struct rtgui_win *parent_toplevel;

	/* top window style */
	rt_uint16_t style;

	/* window state flag */
	enum rtgui_win_flag flag;

	rtgui_modal_code_t modal_code;
	rt_bool_t in_modal;

	/* last mouse event handled widget */
	rtgui_widget_t* last_mevent_widget;

	/* window title */
	char* title;

	/* call back */
	rt_bool_t (*on_activate)	(struct rtgui_object* widget, struct rtgui_event* event);
	rt_bool_t (*on_deactivate)	(struct rtgui_object* widget, struct rtgui_event* event);
	rt_bool_t (*on_close)		(struct rtgui_object* widget, struct rtgui_event* event);

	/* reserved user data */
	rt_uint32_t user_data;
};

rtgui_win_t* rtgui_win_create(struct rtgui_win *parent_toplevel, const char* title,
							  rtgui_rect_t *rect, rt_uint16_t style);
void rtgui_win_destroy(rtgui_win_t* win);

/** Close window.
 *
 * @param win the window you want to close
 *
 * @return RT_TRUE if the window is closed. RT_FALSE if not. If the onclose
 * callback returns RT_FALSE, the window won't be closed.
 *
 * \sa rtgui_win_set_onclose .
 */
rt_bool_t rtgui_win_close(struct rtgui_win* win);

rt_base_t rtgui_win_show(struct rtgui_win *win, rt_bool_t is_modal);
void rtgui_win_hiden(rtgui_win_t* win);
void rtgui_win_end_modal(rtgui_win_t* win, rtgui_modal_code_t modal_code);

rt_bool_t rtgui_win_is_activated(struct rtgui_win* win);

void rtgui_win_move(struct rtgui_win* win, int x, int y);

/* reset extent of window */
void rtgui_win_set_rect(rtgui_win_t* win, rtgui_rect_t* rect);

#ifndef RTGUI_USING_SMALL_SIZE
void rtgui_win_set_box(rtgui_win_t* win, rtgui_box_t* box);
#endif

void rtgui_win_set_onactivate(rtgui_win_t* win, rtgui_event_handler_ptr handler);
void rtgui_win_set_ondeactivate(rtgui_win_t* win, rtgui_event_handler_ptr handler);
void rtgui_win_set_onclose(rtgui_win_t* win, rtgui_event_handler_ptr handler);

rt_bool_t rtgui_win_event_handler(struct rtgui_object* win, struct rtgui_event* event);

void rtgui_win_event_loop(rtgui_win_t* wnd);

void rtgui_win_set_title(rtgui_win_t* win, const char *title);
char* rtgui_win_get_title(rtgui_win_t* win);

#endif
