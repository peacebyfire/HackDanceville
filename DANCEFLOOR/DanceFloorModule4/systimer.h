/*
 * systimer.h
 *
 * Created: 7/14/2012 5:38:36 PM
 *  Author: ben
 */ 


#ifndef SYSTIMER_H_
#define SYSTIMER_H_

//The maximum number of timers
#define MAX_TIMERS	16

//#define TIMER_TYPE	uint32_t  //Allow for 49 days    max delay (in ms)
#define TIMER_TYPE	uint16_t      //Allow for 60 seconds max delay (in ms)

#define TIMER_UNUSED	-1
#define TIMER_RUNNING	0
#define TIMER_EXPIRED	1

/*! \fn void init_timer()
 *  \brief Initializes the timer system.
	This function sets all timers to unused state and initializes TIMER2 to interrupt every millisecond.
 */
void init_timer();

/*! \fn int8_t timer_get()
 *  \brief Allocates a timer.
	\return The number of a free timer. Returns -1 if no timers are available.
 */
int8_t timer_get();

/*! \fn void timer_release(int8_t timer)
 *  \brief Frees a timer.
	\param timer The timer to free.
	This function sets the specified timer to unused state.
 */
void timer_release(int8_t timer);

/*! \fn void timer_set(uint8_t timer, TIMER_TYPE ms_delay)
 *  \brief Sets a timer.
	\param timer The timer to set.
	\param ms_delay The desired time in milliseconds before the timer expires.
 */
void timer_set(uint8_t timer, TIMER_TYPE ms_delay);

/*! \fn void timer_check(int8_t timer)
 *  \brief Checks if a timer has expired.
	\param timer The timer to check.
	\return TIMER_RUNNING if the timer is running, TIMER_EXPIRED if the timer has expired.
 */
uint8_t timer_check(uint8_t timer);

//TIMER_TYPE millis();

#endif /* SYSTIMER_H_ */