/*
 * MIT License
 * Copyright (c) 2021 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the follo18wing conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "lv_anim_timeline.h"

static void lv_anim_timeline_add(const lv_anim_timeline_t* anim_timeline, bool playback, uint32_t playtime)
{
    lv_obj_t* obj = anim_timeline->obj;
    uint32_t delay = anim_timeline->start_time;

    lv_anim_exec_xcb_t exec_cb = anim_timeline->exec_cb;
    uint16_t time = anim_timeline->duration;

    int32_t start = anim_timeline->start;
    int32_t end = anim_timeline->end;
    if (playback)
    {
        int32_t temp = start;
        start = end;
        end = temp;

        delay = playtime - (delay + time);
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_delay(&a, delay);
    lv_anim_set_exec_cb(&a, exec_cb);
    lv_anim_set_time(&a, time);
    lv_anim_set_values(&a, start, end);
    lv_anim_set_path_cb(&a, anim_timeline->path_cb);

    lv_anim_start(&a);
}

uint32_t lv_anim_timeline_start(const lv_anim_timeline_t* anim_timeline, uint32_t len, bool playback)
{
    uint32_t playtime = lv_anim_timeline_get_playtime(anim_timeline, len);
    for (uint32_t i = 0; i < len; i++)
    {
        lv_anim_timeline_add(&(anim_timeline[i]), playback, playtime);
    }
    return playtime;
}

void lv_anim_timeline_del(const lv_anim_timeline_t* anim_timeline, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++)
    {
        lv_obj_t* obj = anim_timeline[i].obj;
        lv_anim_exec_xcb_t exec_xcb = anim_timeline[i].exec_cb;
        lv_anim_del(obj, exec_xcb);
    }
}

uint32_t lv_anim_timeline_get_playtime(const lv_anim_timeline_t* anim_timeline, uint32_t len)
{
    uint32_t playtime = 0;
    for (uint32_t i = 0; i < len; i++)
    {
        uint32_t end = anim_timeline[i].start_time + anim_timeline[i].duration;
        if (end > playtime)
        {
            playtime = end;
        }
    }
    return playtime;
}
