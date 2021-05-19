#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/event.h>
#include <unistd.h>

#include "waker.h"

#include "reactor.h"


__attribute__((overloadable))
reactor_t Reactor(void) {
    int handle = kqueue();

    if (handle < 0) {
        fprintf(stderr, "[ERROR] Something went wrong!\n");
        exit(EXIT_FAILURE);
    }

    return (reactor_t) {
        .handle             = handle,
        .num_pending_events = 0,
    };
}

__attribute__((overloadable))
void cleanup(reactor_t* self_ptr) {
    close(self_ptr->handle);
}

__attribute__((overloadable))
void add_timeout(reactor_t* self_ptr, unsigned long milliseconds, waker_t* waker_ptr) {
    struct kevent change;

    static unsigned int timer_id = 0;

    EV_SET(&change, ++timer_id, EVFILT_TIMER, EV_ADD | EV_CLEAR | EV_ONESHOT, 0, milliseconds, waker_ptr);

    int num_events = kevent(self_ptr->handle, &change, 1, NULL, 0, NULL);

    if (num_events < 0) {
        fprintf(stderr, "[ERROR] Something went wrong: %d; %s\n", num_events, strerror(errno));
        exit(EXIT_FAILURE);
    }

    self_ptr->num_pending_events += 1;
}

__attribute__((overloadable))
void add_read(reactor_t* self_ptr, int file_descriptor, waker_t* waker_ptr) {
    struct kevent change;

    EV_SET(&change, file_descriptor, EVFILT_READ, EV_ADD | EV_CLEAR | EV_ONESHOT, 0, 0, waker_ptr);

    int num_events = kevent(self_ptr->handle, &change, 1, NULL, 0, NULL);

    if (num_events < 0) {
        fprintf(stderr, "[ERROR] Something went wrong: %d; %s\n", num_events, strerror(errno));
        exit(EXIT_FAILURE);
    }

    self_ptr->num_pending_events += 1;
}

__attribute__((overloadable))
void ignite(reactor_t* self_ptr) {
    struct kevent events[32];

    int num_events = kevent(self_ptr->handle, NULL, 0, &events[0], 32, NULL);

    printf("num_events: %d\n", num_events);

    if (num_events < 0) {
        fprintf(stderr, "[ERROR] Something went wrong!\n");
        exit(EXIT_FAILURE);
    }

    for (int index = 0; index < num_events; ++index) {
        struct kevent* event_ptr = &events[index];
        waker_t* waker_ptr       = (waker_t*)event_ptr->udata;

        wake(waker_ptr);
    }

    self_ptr->num_pending_events -= num_events;
}
