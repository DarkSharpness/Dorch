#include "basic/storage.h"

signed main() {
    dark::Storage storage;
    storage = dark::Storage(new int[4], 4 * sizeof(int), [](void *ptr) {
        delete[] static_cast<int *>(ptr);
    });
}
