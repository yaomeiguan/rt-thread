#include <rtgui/rtgui.h>
#include <rtgui/rtgui_system.h>

#include <rtgui/widgets/view.h>
#include <rtgui/widgets/workbench.h>

static rt_bool_t demo_workbench_event_handler(struct rtgui_widget* widget, struct rtgui_event* event)
{
	/* 我们目前只对按键事件感兴趣 */
	if (event->type == RTGUI_EVENT_KBD)
	{
		struct rtgui_event_kbd* ekbd = (struct rtgui_event_kbd*)event;

		if (ekbd->type == RTGUI_KEYDOWN)
		{
			if (ekbd->key == RTGUIK_RIGHT)
			{
				demo_view_next(RT_NULL, RT_NULL);
				return RT_TRUE;
			}
			else if (ekbd->key == RTGUIK_LEFT)
			{
				demo_view_prev(RT_NULL, RT_NULL);
				return RT_TRUE;
			}
		}
	}

	/* 如果不是绘制事件，使用view原来的事件处理函数处理 */
	return rtgui_workbench_event_handler(widget, event);
}

static void workbench_entry(void* parameter)
{
	rt_mq_t mq;
	struct rtgui_view* view;
	struct rtgui_workbench* workbench;

	/* 创建GUI应用需要的消息队列 */
	mq = rt_mq_create("workbench", 256, 4, RT_IPC_FLAG_FIFO);
	/* 注册当前线程为GUI线程 */
	rtgui_thread_register(rt_thread_self(), mq);

	/* 创建一个工作台 */
	workbench = rtgui_workbench_create("main", "workbench");
	if (workbench == RT_NULL) return;

	rtgui_widget_set_event_handler(RTGUI_WIDGET(workbench), demo_workbench_event_handler);

	/* 初始化各个例子的视图 */
	demo_view_dc(workbench);
	demo_view_window(workbench);
	demo_view_label(workbench);
	demo_view_button(workbench);
	demo_view_checkbox(workbench);
	demo_view_progressbar(workbench);
	demo_view_radiobox(workbench);
	demo_view_textbox(workbench);
	demo_view_slider(workbench);
	demo_view_mywidget(workbench);
	demo_view_image(workbench);
	demo_listview_view(workbench);
	demo_fn_view(workbench);

	/* 显示视图 */
	demo_view_show();

	/* 执行工作台事件循环 */
	rtgui_workbench_event_loop(workbench);

	/* 去注册GUI线程 */
	rtgui_thread_deregister(rt_thread_self());
	rt_mq_delete(mq);
}

void workbench_init()
{
	static rt_bool_t inited = RT_FALSE;

	if (inited == RT_FALSE) /* 避免重复初始化而做的保护 */
	{
		rt_thread_t tid;

		tid = rt_thread_create("wb",
			workbench_entry, RT_NULL,
			2048, 25, 10);

		if (tid != RT_NULL) rt_thread_startup(tid);

		inited = RT_TRUE;
	}
}

#ifdef RT_USING_RTGUI
#include <finsh.h>
void workbench()
{
	workbench_init();
}
/* finsh的命令输出，可以直接执行workbench()函数以执行上面的函数 */
FINSH_FUNCTION_EXPORT(workbench, workbench demo)
#endif