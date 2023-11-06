#include "../src/latest.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#ifdef TESTING

int main() {

    // Create a test case
    const char *ignore_list[] = {"ignored_dir1", "ignored_dir2", "ignored_dir3"};
    int num_ignore = sizeof(ignore_list) / sizeof(ignore_list[0]);

    // Test the is_ignored function
    assert(is_ignored("ignored_dir3", (char**)ignore_list, num_ignore) == true); 
    printf("Assert Passed: Directory is ignored.\n");

    if (is_ignored("ignored_dir3", (char**)ignore_list, num_ignore)) {
        printf("Test Passed: Directory is ignored.\n");
    } else {
        printf("Test Failed: Directory is not ignored.\n");
    }

    return 0;
}

#endif
