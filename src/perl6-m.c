#include <moar.h>
#include <stdio.h>
#include "perl6-m.h"

#define NQP_LIB_DIR PREFIX "/languages/nqp/lib"
#define PL6_LIB_DIR PREFIX "/languages/perl6/lib"
#define PL6_RUN_DIR PREFIX "/languages/perl6/runtime"

/* This callback is passed to the interpreter code. It takes care of making
 * the initial invocation. */
static void toplevel_initial_invoke(MVMThreadContext *tc, void *data) {
    /* Create initial frame, which sets up all of the interpreter state also. */
    MVM_frame_invoke(tc, (MVMStaticFrame *)data, MVM_callsite_get_common(tc, MVM_CALLSITE_ID_NULL_ARGS), NULL, NULL, NULL, -1);
}


static void run_bytes(MVMInstance *instance, char *filename, unsigned char *bytes, unsigned int size) {
    MVMThreadContext *tc = instance->main_thread;
    MVMCompUnit *cu = MVM_cu_from_bytes(tc, bytes, size);
    MVMStaticFrame *start_frame;
    MVMROOT(tc, cu, {
        MVMString *const str = MVM_string_utf8_decode(tc, instance->VMString, filename, strlen(filename));
        cu->body.filename = str;

        /* Run deserialization frame, if there is one. */
        if (cu->body.deserialize_frame) {
            MVM_interp_run(tc, &toplevel_initial_invoke, cu->body.deserialize_frame);
        }
    });
    start_frame = cu->body.main_frame ? cu->body.main_frame : cu->body.frames[0];
    MVM_interp_run(tc, &toplevel_initial_invoke, start_frame);
}


int main(int argc, char **argv) {
    const char *lib_path[] = { NQP_LIB_DIR, PL6_LIB_DIR, PL6_RUN_DIR };
    const char *filename = PL6_RUN_DIR "/perl6.moarvm";
    MVMInstance *instance = MVM_vm_create_instance();
    instance->num_clargs = argc - 1;
    instance->raw_clargs = argv + 1;
    instance->exec_name = argv[0];
    instance->prog_name = filename;
    memcpy(instance->lib_path, lib_path, sizeof(lib_path));
    //MVM_vm_run_file(instance, filename);
    run_bytes(instance, INPUT_FILES_NAME[0], INPUT_FILES_DATA, INPUT_FILES_SIZE[0]);
    MVM_vm_exit(instance);
}
