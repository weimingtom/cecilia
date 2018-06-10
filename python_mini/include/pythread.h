//20180324
#pragma once

#define NO_EXIT_PROG		

typedef void *PyThread_type_lock;
typedef void *PyThread_type_sema;

DL_IMPORT(void) PyThread_init_thread();
DL_IMPORT(long) PyThread_start_new_thread(void (*)(void *), void *);
DL_IMPORT(void) PyThread_exit_thread();
DL_IMPORT(void) PyThread__PyThread_exit_thread();
DL_IMPORT(long) PyThread_get_thread_ident();

DL_IMPORT(PyThread_type_lock) PyThread_allocate_lock();
DL_IMPORT(void) PyThread_free_lock(PyThread_type_lock);
DL_IMPORT(int) PyThread_acquire_lock(PyThread_type_lock, int);
#define WAIT_LOCK	1
#define NOWAIT_LOCK	0
DL_IMPORT(void) PyThread_release_lock(PyThread_type_lock);

DL_IMPORT(PyThread_type_sema) PyThread_allocate_sema(int);
DL_IMPORT(void) PyThread_free_sema(PyThread_type_sema);
DL_IMPORT(int) PyThread_down_sema(PyThread_type_sema, int);
#define WAIT_SEMA	1
#define NOWAIT_SEMA	0
DL_IMPORT(void) PyThread_up_sema(PyThread_type_sema);

DL_IMPORT(int) PyThread_create_key();
DL_IMPORT(void) PyThread_delete_key(int);
DL_IMPORT(int) PyThread_set_key_value(int, void *);
DL_IMPORT(void *) PyThread_get_key_value(int);
