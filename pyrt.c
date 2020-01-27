//
// Created by Oren Bell on 1/24/20.
//

#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <sched.h>
#include <stdio.h>

static PyObject *PyRtError;


static PyObject* pyrt_request_priority(PyObject* self, PyObject* args) {
    int rt_priority;
    int scheduler = SCHED_RR;
    struct sched_param priority;

    if (!PyArg_ParseTuple(args, "i|i", &rt_priority, &scheduler)) {
        PyErr_SetString(PyRtError, "Parsing tuple failed. Argument should be integer in range 0-99");
        return NULL;
    }
    // TODO: sched_get_priority_min(SCHED_RR) || rt_priority > sched_get_priority_max(SCHED_RR)
    if (rt_priority < 0 || rt_priority > 99) {
        PyErr_SetString(PyRtError, "Priority not in range 0-99");
        return NULL;
    }

    printf("Priority requested: %d\n", rt_priority);
    if (scheduler == SCHED_RR) {
        printf("Scheduler requested is round robin\n");
    } else if (scheduler == SCHED_FIFO) {
        printf("Scheduler requested is FIFO\n");
    } else {
        printf("Scheduler requested is other\n");
    }

    priority.sched_priority = rt_priority;
    int ret = sched_setscheduler(getpid(), SCHED_RR, &priority);
    if (ret != 0) {
        fprintf(stderr, "Error setting priority");
        exit(EXIT_FAILURE);
    }

    Py_RETURN_NONE;
}

static char pyrt_docs[] =
        "request_priority(): Sets a RT priority the python script is testing";

static PyMethodDef PyRtMethods[] = {
        {"request_priority", pyrt_request_priority, METH_VARARGS,
         pyrt_docs},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pyrtmodule = {
        PyModuleDef_HEAD_INIT,
        "pyrt",
        pyrt_docs,
        -1,
        PyRtMethods
};

PyMODINIT_FUNC PyInit_pyrt(void) {
    PyObject* m;

    m = PyModule_Create(&pyrtmodule);
    if (m == NULL)
        return NULL;

    PyRtError = PyErr_NewException("pyrt.error", NULL, NULL);
    Py_XINCREF(PyRtError);
    if (PyModule_AddObject(m, "error", PyRtError) < 0) {
        Py_XDECREF(PyRtError);
        Py_CLEAR(PyRtError);
        Py_DECREF(m);
        return NULL;
    }

    PyModule_AddIntConstant(m, "SCHED_FIFO", SCHED_FIFO);
    PyModule_AddIntConstant(m, "SCHED_RR", SCHED_RR);
    PyModule_AddIntConstant(m, "SCHED_OTHER", SCHED_OTHER);

    return m;
}