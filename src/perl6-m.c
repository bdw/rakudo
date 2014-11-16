#include <moar.h>
#include <stdio.h>
#define NQP_LIB_DIR PREFIX "/languages/nqp/lib"
#define PL6_LIB_DIR PREFIX "/languages/perl6/lib"
#define PL6_RUN_DIR PREFIX "/languages/perl6/runtime"

int main(int argc, char **argv) {
    const char *lib_path[] = { NQP_LIB_DIR, PL6_LIB_DIR, PL6_RUN_DIR };
    const char *filename = PL6_RUN_DIR "/perl6.moarvm";
    MVMInstance *instance = MVM_vm_create_instance();
    int i;
    instance->num_clargs = argc - 1;
    instance->raw_clargs = argv + 1;
    instance->exec_name = argv[0];
    instance->prog_name = filename;
    memcpy(instance->lib_path, lib_path, sizeof(lib_path));
    MVM_vm_run_file(instance, filename);
    MVM_vm_exit(instance);
}
