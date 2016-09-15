/*
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program;if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  *  Copyright (C) 2009-2015 Broadcom Corporation
 */


/******************************************************************************
 *
 *  Filename:      utils.h
 *
 *  Description:   Utility functions declaration
 *
 ******************************************************************************/

#ifndef UTILS_H
#define UTILS_H



/******************************************************************************
**  Type definitions
******************************************************************************/

typedef struct
{
    void        *p_first;
    void        *p_last;
    uint16_t    count;
} BUFFER_Q;

typedef struct _hc_buffer_hdr
{
    struct _hc_buffer_hdr *p_next;   /* next buffer in the queue */
} HC_BUFFER_HDR_T;

#define BT_HC_BUFFER_HDR_SIZE (sizeof(HC_BUFFER_HDR_T))

#define MAX_ACL_PKT_SIZE 4096


/******************************************************************************
**  Extern variables and functions
******************************************************************************/

/******************************************************************************
**  Functions
******************************************************************************/

/*******************************************************************************
**
** Function        utils_init
**
** Description     Utils initialization
**
** Returns         None
**
*******************************************************************************/
void utils_init ();

/*******************************************************************************
**
** Function        utils_cleanup
**
** Description     Utils cleanup
**
** Returns         None
**
*******************************************************************************/
void utils_cleanup ();

/*******************************************************************************
**
** Function        utils_queue_init
**
** Description     Initialize the given buffer queue
**
** Returns         None
**
*******************************************************************************/
static void utils_queue_init (BUFFER_Q *p_q);

/*******************************************************************************
**
** Function        utils_enqueue
**
** Description     Enqueue a buffer at the tail of the given queue
**
** Returns         None
**
*******************************************************************************/
void utils_enqueue (void *p_buf);

/*******************************************************************************
**
** Function        utils_dequeue
**
** Description     Dequeues a buffer from the head of the given queue
**
** Returns         NULL if queue is empty, else buffer
**
*******************************************************************************/
void *utils_dequeue (BUFFER_Q *p_q);

/*******************************************************************************
**
** Function        utils_dequeue_unlocked
**
** Description     Dequeues a buffer from the head of the given queue without lock
**
** Returns         NULL if queue is empty, else buffer
**
*******************************************************************************/
void *utils_dequeue_unlocked (BUFFER_Q *p_q);

/*******************************************************************************
**
** Function        utils_getnext
**
** Description     Return a pointer to the next buffer linked to the given buffer
**
** Returns         NULL if the given buffer does not point to any next buffer,
**                 else next buffer address
**
*******************************************************************************/
void *utils_getnext (void *p_buf);

/*******************************************************************************
**
** Function        utils_remove_from_queue
**
** Description     Dequeue the given buffer from the middle of the given queue
**
** Returns         NULL if the given queue is empty, else the given buffer
**
*******************************************************************************/
void *utils_remove_from_queue (void *p_buf);

/*******************************************************************************
**
** Function        utils_remove_from_queue_unlocked
**
** Description     Dequeue the given buffer from the middle of the given queue without lock
**
** Returns         NULL if the given queue is empty, else the given buffer
**
*******************************************************************************/
void *utils_remove_from_queue_unlocked (BUFFER_Q *p_q, void *p_buf);


/*******************************************************************************
**
** Function        utils_delay
**
** Description     sleep unconditionally for timeout milliseconds
**
** Returns         None
**
*******************************************************************************/
void utils_delay (uint32_t timeout);

/*******************************************************************************
**
** Function        utils_lock
**
** Description     application calls this function before entering critical
**                 section
**
** Returns         None
**
*******************************************************************************/
void utils_lock (void);

/*******************************************************************************
**
** Function        utils_unlock
**
** Description     application calls this function when leaving critical
**                 section
**
** Returns         None
**
*******************************************************************************/
void utils_unlock (void);

/*******************************************************************************
**
** Function        utils_alloc
**
** Description     allocate memory for buffer
**
** Returns         None
**
*******************************************************************************/
uint8_t* utils_alloc (int size);

/*******************************************************************************
**
** Function        utils_lelease
**
** Description     release memory for buffer
**
** Returns         None+**
*******************************************************************************/
void utils_release(uint8_t* ptr);

/*******************************************************************************
**
** Function        utils_get_count
**
** Description     Get number of allocated buffers
**
** Returns         None
**
*******************************************************************************/
uint16_t utils_get_count();

/*******************************************************************************
**
** Function        utils_get_count
**
** Description     Get number of allocated buffers
**
** Returns         None
**
*******************************************************************************/
void* utils_get_first ();

#endif /* UTILS_H */

